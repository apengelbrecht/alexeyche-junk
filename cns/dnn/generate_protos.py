#!/usr/bin/env python


import re
import sys
import os
from os.path import join as pj
import argparse
from collections import defaultdict

known_types = {
    "double"    : "double",
    "int"       : "int32",
    "size_t"    : "uint32",
    "ui32"      : "uint32",
    "float"     : "float",
    "string"    : "string",
    "bool"      : "bool",
    "TString"   : "string",
    "complex<double>" : "TComplex",
}

vectors = ["TVector", "vector", "TActVector"]
vectors_re = re.compile("^(?:{})+<(.*)>.*".format("|".join(vectors)))

def build_field_re(known_types):
    all_types = {}
    all_types.update(known_types)
    for v in vectors:
        for l, r in known_types.iteritems():
            all_types["{}<{}>".format(v, l)] = r

    return re.compile("[ ]*({})[\W]+(?!__)([a-zA-Z]+)([ ]*=[ ]*[^ ]+)*;[ ]*$".format(
        "|".join(all_types)
    ))

def is_vector(t):
    return not vectors_re.match(t) is None


struct_re = re.compile(".*[ ]*struct[ ]*(T[a-zA-Z0-9_]+).*")
cpp_re = re.compile("(.*)(\.h|\.cpp)$")

instance_name_from_class = lambda t: re.sub("T([^ ]+)", "\\1", t)

ignore_re = re.compile(".*dnn/(contrib|util/thread|base|neuron/config|util/server).*")

class TStruct(object):
    def __init__(self, name):
        self.name = name
        self.fields = []

    def __repr__self(self):
        return str(self)

    def __str__(self):
        return "{}({})".format(self.name, ", ".join([ "{} {}".format(f[0], f[1]) for f in self.fields ]))

def GenerateProtos(structures_to_file, package, dst, imports):
    for dst_file, structures in structures_to_file.iteritems():
        with open(os.path.join(dst, dst_file), 'w') as f_ptr:
            f_ptr.write("package %s;\n" % package)
            f_ptr.write("\n")
            for imp in imports:
                f_ptr.write("import \"{}\";\n".format(imp))
                f_ptr.write("\n")
            for structure in structures.values():
                i = 1
                f_ptr.write("message %s {\n" % structure.name)
                for f in structure.fields:
                    m = vectors_re.match(f[0])
                    if m:
                        f_ptr.write("    repeated %s %s = %s;\n" % (known_types[ m.group(1) ], f[1], str(i)))
                    else:
                        f_ptr.write("    optional %s %s = %s;\n" % (known_types[ f[0] ], f[1], str(i)))
                    i += 1
                    
                f_ptr.write("}\n")
                f_ptr.write("\n")        

def ParseStructures(src_dir):
    structures_to_file = defaultdict(dict)
        
    for root, dirs, files in os.walk(src_dir):
        for f in files:
            if ignore_re.match(pj(root, f)):
                continue

            cpp_m = cpp_re.match(f)
            if cpp_m:
                dst_file = "{}.proto".format(f.split(".")[0])

                def parse(known_types):
                    open_brackets = 0
                    struct = None
                    line_num = 0
                    field_re = build_field_re(known_types)
                    for l in open(pj(root, f)):
                        l = l.split("//")[0]
                        line_num += 1
                        struct_m = struct_re.match(l)
                        if struct_m:
                            struct = TStruct(struct_m.group(1))
                            open_brackets = l.count("{")
                            continue

                        if struct:
                            open_brackets += l.count("{")
                            open_brackets -= l.count("}")
                            if open_brackets == 0:
                                # if len(struct.fields)>0:
                                structures_to_file[dst_file][struct.name] = struct
                                struct = None
                                continue
                            
                            m = field_re.match(l.strip())
                            if m:
                                struct.fields.append( (m.group(1), m.group(2)) )
                                continue
                parse(known_types)
                if dst_file in structures_to_file:
                    known_types.update(dict([ (s.name, s.name) for s in structures_to_file[dst_file].values() ]))
                    parse(known_types)
    for k, v in structures_to_file.iteritems():
        print k
        for vv in v:
            print "\t", vv
    return structures_to_file



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--source-path", help="Path to the sources",
                    type=str, required=True)
    parser.add_argument("-d", "--dest-path", help="Path where to store .proto",
                    type=str, required=True)
    parser.add_argument("-p", "--package", help="Package name, default : %(default)s",
                    type=str, required=False, default="NDnnProto")
    parser.add_argument("-i", "--imports", help="Put imports to all messages (separated by ;)",
                    type=str, required=False, default=None)
    args = parser.parse_args()
    structures = ParseStructures(args.source_path)
    imports = []
    if args.imports:
        imports = [ v.strip() for v in args.imports.split(";") if v.strip() ]
    GenerateProtos(structures, args.package, args.dest_path, imports)
