#!/usr/bin/env python

import argparse
import os
import sys
import md5
import shutil
import multiprocessing
from contextlib import contextmanager
import json
import traceback

from config import RUNS_DIR
from config import SNN_SIM
from config import SNN_PROC
from config import CONST_JSON

import subprocess as sub
import time

this_file = os.path.realpath(__file__)


@contextmanager
def pushd(newDir):
    previousDir = os.getcwd()
    os.chdir(newDir)
    yield
    os.chdir(previousDir)


def runProcess(bin, args, log_stdout=None, verbose=False):
    if type(bin) is list:
        cmd = list(bin)
    else:
        cmd = [
            bin,
        ]            
    for k, v in [ (k, args[k]) for k in sorted(args) ]:
        if type(v) is bool:
            cmd += [k]
        else:
            cmd += [k, str(v)]
    start_time = time.time()
    if log_stdout:
        sp = sub.Popen(cmd, stdout=open(log_stdout, 'w'), stderr=sub.STDOUT, shell=False)
    else:        
        sp = sub.Popen(cmd, stdout=sub.PIPE, stderr=sub.STDOUT, shell=False)

    if verbose:
        print 
        print " ".join(cmd)
        print 
    try:
        stdout, stderr = sp.communicate()
    except KeyboardInterrupt:
        print "Bye"
    end_time = time.time()
    if verbose:
        print "time run: %s sec" % str(end_time-start_time)
    if sp.returncode != 0:
        print "Run failed: "
        if log_stdout:
            with open(log_stdout) as lf:
                for l in lf:
                    print l.strip()
        else:
            for l in stdout.split("\n"):
                print l.strip()
        sys.exit(1)
    if not log_stdout:
        return stdout
    
def evalClusteringPStat(args, wd, ep, p_stat_file, spikes, stat):
    print "Evaluation through clustering ...",
    if args.verbose:
        print 
    proc_args = {}
    proc_args['--p-stat'] = p_stat_file
    proc_args['--spikes'] = spikes
    proc_args['--jobs'] = args.jobs
    json_proc = os.path.join(wd, "%s_proc_output.json" % ep)
    proc_args['--output'] = json_proc
    proc_output = os.path.join(wd, "%s_proc_stdout.log" % ep)
    runProcess([args.snn_proc_bin, "p_stat_dist"], proc_args, proc_output, verbose=args.verbose)
    if os.path.exists(proc_output) and os.stat(proc_output).st_size == 0:
        os.remove(proc_output)
    
    j = json.load(open(json_proc))
    j['epoch'] = ep 
    json.dump(j, open(json_proc, 'w'), indent=4)

    eval_output = os.path.join(wd, "%s_eval_pstat.log" % ep)
    eval_r_script = os.path.join(os.path.dirname(this_file), "eval_dist_matrix.R")
    with pushd(wd):
        eval = runProcess("Rscript",  { eval_r_script : json_proc }, verbose=args.verbose)

    stat['eval'] = float(eval.strip())
    stat['mean_rate'] = float(j['mean_rate'])
    if args.verbose:
        for k in sorted(stat.keys()):
            print k, ":", stat[k],
        print

def main(args):
    const_hex = md5.new(args.const).hexdigest()
    i=0
    wd = args.working_dir

    start_ep = 0
    if not wd:
        while i<1000:
            new_wd = os.path.join(args.runs_dir, const_hex + "_%04d" % i)
            if not os.path.exists(new_wd):
                if not args.old_dir or not wd:
                    wd = new_wd
                break
            wd = new_wd
            i+=1
        
        if os.path.exists(wd):
            max_ep = 0
            for f in os.listdir(wd):
                f_spl = f.split("_")
                if len(f_spl) > 1 and "model" in f:
                    max_ep = max(max_ep, int(f_spl[0]))
            if max_ep>=0:
                while True:
                    ans = raw_input("%s already exists and %s epochs was done here. Continue learning? (y/n): " % (os.path.basename(wd), max_ep))
                    if ans in ["Y","y"]:
                        start_ep = max_ep + 1
                        break
                    elif ans in ["N", "n"]:
                        print "Cleaning %s ... " % wd
                        for f in os.listdir(wd):
                            os.remove(os.path.join(wd, f))
                        break                        
                    else:
                        print "incomprehensible answer"
        else:
            os.mkdir(wd)

    const = os.path.join(wd, os.path.basename(args.const))
    if not os.path.exists(const):
        shutil.copy(args.const, wd)

    def wd_file(s):
        return os.path.join(wd, s)
    
    input = None
    if args.input:
        input = args.input
    elif args.spikes:
        input = args.spikes

    common_sim_args = {
        '--jobs' : str(args.jobs),
        '--input' : input,
        '--constants' : const
    }            
    if args.T_max != 0:
        common_sim_args['--T-max'] = args.T_max
    
    stat = {'eval' : None, 'mean_rate' : 0}

    for ep in xrange(start_ep, start_ep + args.epochs + 1):
        sim_args = common_sim_args.copy()
        if args.eval_clustering_p_stat: 
            sim_args['--p-stat'] = wd_file("%s_p_stat.pb" % ep) 
        sim_args['--save'] = wd_file("%s_model.pb" % ep)
        sim_args['--output'] = wd_file("%s_output_spikes.pb" % ep)
        if args.stat:
            sim_args['--stat'] = wd_file("%s_stat.pb" % ep)
        elif args.p_stat and '--p-stat' not in sim_args:
            sim_args['--p-stat'] = wd_file("%s_p_stat.pb" % ep)

        if ep>1:
            model_load = wd_file("%s_model.pb" % str(ep-1))
            if not os.path.exists(model_load):
                raise Exception("Can't find model for previous epoch number %s" % str(ep-1))
            sim_args['--load'] = model_load

        if ep == 1 and args.collect_statistics:
            sim_args['--T-max'] = args.collect_statistics
            sim_args['--no-learning'] = True

        if args.input:
            if ep == 0:
                sim_args['--precalc'] = True
                sim_args['--output'] = wd_file("input_spikes.pb")
                del sim_args['--save']
            else:
                if not os.path.exists( wd_file("input_spikes.pb") ):
                    raise Exception("Can't find input_spikes.pb")
                sim_args['--input'] = wd_file("input_spikes.pb")
        elif ep == 0:
            continue
        print "Epoch %d" % ep
        runProcess(args.snn_sim_bin, sim_args, wd_file("%s_output.log" % ep), verbose = args.verbose)
        if args.eval_clustering_p_stat and ep > 0: 
            evalClusteringPStat(args, wd, ep, sim_args['--p-stat'], sim_args['--output'], stat)
    return stat


class RunSimArgs(object):
    input = None
    epochs = 1
    eval_clustering_p_stat = False
    const = os.path.join(os.path.dirname(this_file), "../", CONST_JSON)
    snn_sim_bin = os.path.join(os.path.dirname(this_file), "../build/bin", SNN_SIM)
    snn_proc_bin = os.path.join(os.path.dirname(this_file), "../build/bin", SNN_PROC)
    working_dir = None
    jobs = multiprocessing.cpu_count()
    T_max = 0
    stat = None
    verbose = True
    old_dir = False
    p_stat = None
    spikes = None
    runs_dir = RUNS_DIR
    collect_statistics = None

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Tool for simulating snn')

    parser.add_argument('-i', 
                        '--input', 
                        required=False,
                        help='Input time series protobuf')
    parser.add_argument('-sp', 
                        '--spikes', 
                        required=False,
                        help='Input labeled spikes list protobuf')
    parser.add_argument('-e', 
                        '--epochs', 
                        required=False,
                        help='Number of epochs to run', default=RunSimArgs.epochs,type=int)
    parser.add_argument('-j', 
                        '--jobs', 
                        required=False,
                        help='Number of parallell jobs (default: %(default)s)', default=RunSimArgs.jobs)
    parser.add_argument('-T', 
                        '--T-max', 
                        required=False,
                        help='Run only specific amount of simulation time (ms)', default=RunSimArgs.T_max)
    parser.add_argument('-o', 
                        '--old-dir', 
                        action='store_true',
                        help='Do not create new dir for that simulation')
    parser.add_argument('-s', 
                        '--stat',
                        action='store_true',
                        help='Save statistics')
    parser.add_argument('-ps', 
                        '--p-stat',
                        action='store_true',
                        help='Save probabilites of neurons')
    parser.add_argument('-v', 
                        '--verbose', 
                        action='store_true',
                        help='Verbose running snn_sim commands')
    parser.add_argument('-r', 
                        '--runs-dir', 
                        required=False,
                        help='Runs dir (default: %(default)s)', default=RunSimArgs.runs_dir)
    parser.add_argument('-w', 
                        '--working-dir',
                        required=False,
                        help='Working dir (default: %%runs_dir%%/%%md5_of_const%%_%%number_of_experiment%%)')
    parser.add_argument('-c', 
                        '--const', 
                        required=False,
                        help='Path to const.json file (default: $SCRIPT_DIR/../%s)' % CONST_JSON, default=RunSimArgs.const)
    parser.add_argument('--snn-sim-bin', 
                        required=False,
                        help='Path to snn sim bin (default: $SCRIPT_DIR/../build/bin/%s)' % SNN_SIM, default=RunSimArgs.snn_sim_bin)
    parser.add_argument('--snn-proc-bin', 
                        required=False,
                        help='Path to snn proc bin (default: $SCRIPT_DIR/../build/bin/%s)' % SNN_PROC, default=RunSimArgs.snn_proc_bin)
    parser.add_argument('--eval-clustering-p-stat',
                        action='store_true',
                        help='Run evaluation of unsupervised classification with clustering of model intensities')
    parser.add_argument('--collect-statistics',
                        required=False,
                        help='Run simulation in first epoch without learning to collect statistics')
    args = parser.parse_args(sys.argv[1:])    
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)

    if (not args.spikes and not args.input) or (args.spikes and args.input):
        raise Exception("Need input time series or input spikes")
    if args.stat and args.p_stat:
        raise Exception("Can't collect Full and Partial statistics. Choose one")
    sim_opts = RunSimArgs()
    sim_opts.__dict__.update(args.__dict__)

    with open(sim_opts.const) as const_ptr:
        c_json = "\n".join([ l.split("//")[0].split("#")[0] for l in const_ptr.readlines() ])
    try:
        const = json.loads(c_json)
    except:
        print "Error while reading const.json:"
        print traceback.format_exc()
        sys.exit(1) 
    main(sim_opts)
