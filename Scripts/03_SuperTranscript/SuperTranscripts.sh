#!/bin/bash

#SBATCH -p medium
#SBATCH -t 2:00:00
#SBATCH -o %J_SSuperTranscripts.out
#SBATCH -C scratch2
#SBATCH --exclusive
#SBATCH --mem 20G
#SBATCH --qos=short

module purge
module load trinity
#module load python/3.8.2 # python3 required by TRINITY (old version), not newer "trinity" 

/usr/product/bioinfo/TRINITY/2.11.0/Analysis/SuperTranscripts/Trinity_gene_splice_modeler.py --trinity_fasta Trinity_Output.Trinity.fasta

