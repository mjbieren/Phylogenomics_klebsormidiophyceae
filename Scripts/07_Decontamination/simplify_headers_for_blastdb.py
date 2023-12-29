#!/usr/bin/env python3

import sys


'''
# Iker Irisarri 17.08.2020 University of Goettingen (Germany)

script to prepare fasta for makeblastdb

extreme simplification of headers, composed of a custom tag (append) + consecutive numbers:
bact-1, bact-2, etc.

it also prints an outfile with header corespondences

'''

input_file = sys.argv[1]
append = sys.argv[2]

seq_count = int("0")

output_file = input_file + ".headers_map.out"
fo=open(output_file, 'w')

with open(input_file, 'r') as infile:
    for line in infile:
    
        # process headers
        if line.startswith('>'):
            seq_count = seq_count + 1
            line_new= ">" + append + "-" + str(seq_count)
            print(line_new)
            
            if ( len(line_new)> 50 ):
                    print("ERROR: header longer than 50 chars: ", line_new,  file=sys.stderr)
                    break
            
            # print header correspondences to headers_map.out
            write_out = line_new + '\t' + line
            fo.write(write_out)
        
        # print sequences
        else:
            print(line, end='')

