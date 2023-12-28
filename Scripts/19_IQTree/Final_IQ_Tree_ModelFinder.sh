#!/bin/bash

IQ_TREEPATH= #iqtree2 see executables
THREADS=20
FILE_INPUT= #Alignment file output form step 18
MSUB=nuclear #What type of AA models, can be nuclear, mitochondrial, chloroplast or viral.
BRANCH_TEST_REPLICATES=1000
INPUT_ALIGNMENT_FILE_TYPE=FASTA #Can be PHYLIP, FASTA, NEXUS, CLUSTAL or MSF
AUTOMATIC_MODEL_SELECTION=TESTONLY
OUTPUT_PREFIX= #Change this to your own

$IQ_TREEPATH -nt $THREADS -m $AUTOMATIC_MODEL_SELECTION -madd LG+C60 -msub $MSUB -s $FILE_INPUT 
