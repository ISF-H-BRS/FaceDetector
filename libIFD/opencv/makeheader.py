#!/usr/bin/env python3

import os
import re
import sys

if len(sys.argv) != 4:
    print("Usage: %s infile outfile strname" % sys.argv[0])
    sys.exit(2)
    
if os.path.isfile(sys.argv[2]) and os.path.getmtime(sys.argv[2]) >= os.path.getmtime(sys.argv[1]):
    print("OpenCV model header is up to date.")
    sys.exit(0)

infile = open(sys.argv[1], 'r')
data = infile.read()
infile.close()

outfile = open(sys.argv[2], 'w')

outfile.write("// Automatically generated by makeheader.py. Do not edit, it will be overwritten!\n")
outfile.write("#pragma once\n")
outfile.write("\n")
outfile.write("#include <string>\n")
outfile.write("\n")
outfile.write("const std::string %s = \n" % sys.argv[3])

lines = data.splitlines()

for line in lines:
    line = line.replace('\"', '\\\"')
    outfile.write("    \"%s\"\n" % line)

outfile.write(";\n")
outfile.close()

print("New OpenCV model header generated.")
