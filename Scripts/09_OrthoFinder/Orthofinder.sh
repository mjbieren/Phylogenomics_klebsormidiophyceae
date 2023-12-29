#!/bin/bash

#Follow Armins guide on setting up orthofinder!
# https://github.com/dadrasarmin/orthofinder_hpc_gwdg

SPECIES_DIR= #change this, combination of all the positive sets from GPDS.sh
TREE_FILE= #example Complete_Klebsotree.txt #change this
OUTPUT_DIR= #change this Be sure not to create the directory orthofinder does this for you
OUTPUT_SUFFIX=Big_Tree_Klebso #change this
ORTHOFINDER_PATH= #Path to orthofinder.py

#Activate your orthofinder environment with conda, start it in a tmux!
#Just to make sure the directory doesn't already exist, because Orthofinder can not handle that...

python $ORTHOFINDER_PATH -f $SPECIES_DIR -S diamond -M msa -A mafft -T fasttree -t 100 -a 6 -y -n $OUTPUT_SUFFIX -o $OUTPUT_DIR &> orthofinder_${OUTPUT_SUFFIX}.log
