#!/bin/bash

source activate orthofinder

IN= #Change this. This is the output folder of APPPFormat.
OUTPUT= #Change this. this is the output folder for Phylopypruner

mkdir -p "$OUTPUT" || exit -1
phylopypruner --dir $IN --output $OUTPUT --mask pdist --prune MI --min-taxa 10 --trim-lb 5 --min-support 0.75 --min-gene-occupancy 0.1 --min-otu-occupancy 0.1 --threads 40 --trim-divergent 1.25 --min-pdist 1e-8 --jackknife --no-plot  --overwrite

source deactivate
