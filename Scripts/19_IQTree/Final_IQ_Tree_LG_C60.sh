#!/bin/bash

IQ_TREEPATH= #iqtree2 full path
THREADS=20
FILE_INPUT= #Output file from clipkit step 18
MSUB=nuclear #What type of AA models, can be nuclear, mitochondrial, chloroplast or viral.
BRANCH_TEST_REPLICATES=1000
INPUT_ALIGNMENT_FILE_TYPE=FASTA #Can be PHYLIP, FASTA, NEXUS, CLUSTAL or MSF
AUTOMATIC_MODEL_SELECTION=LG+C60
OUTPUT_PREFIX= #Change this to your own.

$IQ_TREEPATH -nt $THREADS -m $AUTOMATIC_MODEL_SELECTION -msub $MSUB -s $FILE_INPUT -bb $BRANCH_TEST_REPLICATES -alrt $BRANCH_TEST_REPLICATES -pre $OUTPUT_PREFIX
