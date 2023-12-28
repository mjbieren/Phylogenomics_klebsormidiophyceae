#!/bin/bash

IQ_TREEPATH= #iqtree2 full path
THREADS=50 #change this to your needs
FILE_INPUT= #Output file from clipkit step 18
MSUB=nuclear #What type of AA models, can be nuclear, mitochondrial, chloroplast or viral.
BRANCH_TEST_REPLICATES=1000
INPUT_ALIGNMENT_FILE_TYPE=FASTA #Can be PHYLIP, FASTA, NEXUS, CLUSTAL or MSF
AUTOMATIC_MODEL_SELECTION=LG+C60+F+G
OUTPUT_PREFIX= #Change this
GUIDE_TREE= #Output from the Other IQTree Script. (Final_IQ_Tree_LG_C60) Run that one first!

$IQ_TREEPATH -nt $THREADS -m $AUTOMATIC_MODEL_SELECTION -msub $MSUB -s $FILE_INPUT -bb $BRANCH_TEST_REPLICATES -alrt $BRANCH_TEST_REPLICATES -pre $OUTPUT_PREFIX -ft $GUIDE_TREE

