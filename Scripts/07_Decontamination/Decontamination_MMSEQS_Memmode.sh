#!/bin/bash

module purge
module load mmseqs2

SAMPLE=${PWD##**/}

IN=*_db.db
OUT=Trinity_$SAMPLE.mmseqs2_decont
TMP=tmp_$SAMPLE                   
DB= #The decontamination database file with _db.db at the end


mkdir -p "$TMP" || exit -1 #Make the directory and if already exist error will be ignored, if no permission is available it will stop the program

# create query db
#for f in *pep; do mmseqs createdb $f $f.db; done
#output real seq identity: --alignment-mode 3
#iterative search in 3 steps, with sensitivity 1,4,7
# https://github.com/soedinglab/mmseqs2/wiki#how-to-find-the-best-hit-the-fastest-way
mmseqs search $IN $DB $OUT $TMP --start-sens 1 --sens-steps 3 -s 7 --alignment-mode 3 --max-seqs 10 --threads 8
# convert output format to BLAST+ TAB
mmseqs convertalis $IN $DB $OUT $OUT.outfmt6 --format-mode 2
