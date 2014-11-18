
using namespace std;

#include <iostream>
#include <stdlib.h>

#include <snnlib/util/optionparser/opt.h>
#include <snnlib/config/constants.h>

#include <snnlib/sim/sim.h>

#include <snnlib/util/time_series.h>

enum  optionIndex { ARG_UNKNOWN, ARG_HELP, ARG_CONSTANTS, ARG_INPUT_TS, ARG_OUT_STAT, ARG_OUT_SPIKES };
const option::Descriptor usage[] =
{
 {ARG_UNKNOWN, 0, "", "",Arg::None, "USAGE: example [options]\n\n"
                                        "Options:" },
 {ARG_HELP, 0,"h", "help",Arg::None, "  --help  \tPrint usage and exit." },
 {ARG_CONSTANTS, 0,"c","constants",Arg::NonEmpty, "  --constants, -c  \tConstants filename." },
 {ARG_OUT_STAT, 0,"","stat",Arg::NonEmpty, "  --stat  \tFile name with detailed statistics." },
 {ARG_OUT_SPIKES, 0,"o","--output",Arg::NonEmpty, "  --output, -o  \tOutput file with serialized spikes list" },
 {ARG_INPUT_TS, 0,"i","input-ts",Arg::NonEmpty, "  --input-ts, -i  \tInput time series protobuf file." },
 {ARG_UNKNOWN, 0, "", "",Arg::None, "\nExamples:\n"
                               "% Need to fill %" },
 {0,0,0,0,0,0}
};

void parseOptions(option::Option* options, option::Stats &stats,  int argc, char **argv) {
    option::Option* buffer  = new option::Option[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);
    if (parse.error())
        exit(1);

    if (options[ARG_HELP]) { // || argc == 0) {
        option::printUsage(cout, usage);
        exit(0);
    }
    if(options[ARG_CONSTANTS].count() != 1) {
        cerr << "Inappropriate constants filename argument\n";
        exit(1);
    }
    if(options[ARG_INPUT_TS].count() != 1) {
        cerr << "Inappropriate input time series argument\n";
        exit(1);
    }
    if(options[ARG_OUT_SPIKES].count() != 1) {
        cerr << "Inappropriate output spikes argument\n";
        exit(1);
    }

    for (option::Option* opt = options[ARG_UNKNOWN]; opt; opt = opt->next()) {
      cerr << "Unknown option: " << string(opt->name,opt->namelen) << "\n";
    }
    if(options[ARG_UNKNOWN].count() > 0) exit(1);

    for (int i = 0; i < parse.nonOptionsCount(); ++i) {
      cerr << "Non-options arguments are not supported (" << i << ": " << parse.nonOption(i) << ")\n";
    }
    if(parse.nonOptionsCount()>0) exit(1);

    delete[] buffer;
}


int main(int argc, char **argv) {
    argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
    option::Stats  stats(usage, argc, argv);
    option::Option* options = new option::Option[stats.options_max];
    parseOptions(options, stats, argc, argv);

    Constants c = Constants(options[ARG_CONSTANTS].arg);
    cout << c;

    Sim s(c);
    cout << s;

    s.setOutputSpikesFile(options[ARG_OUT_SPIKES].arg);

    {
        LabeledTimeSeriesList lts_list(options[ARG_INPUT_TS].arg);
        s.setInputTimeSeries(lts_list);
    }
    if(options[ARG_OUT_STAT].count()>0) {
        s.monitorStat(options[ARG_OUT_STAT].arg);
    }

    s.run();

    delete[] options;
}
