#!/bin/bash

#SBATCH -p medium
#SBATCH -t 5:00:00
#SBATCH -o %J_TransDecoder.out
#SBATCH -C scratch2
#SBATCH --exclusive
#SBATCH --mem 20G
#SBATCH -c 24


#We purge priorly so that we are sure that we start with a clean state on the host
module purge
module load transdecoder
module load ncbi-blast/2.11.0

#Supertranscript fasta file
IN1=$PWD/trinity_genes.fasta #Change this
OUT=$IN1.transdecoder_dir #Change this
IN2=$OUT/longest_orfs.pep #Change this
OUT2=$OUT/.outfmt6 #Change this
REFERENCE_GENOME= #Change this

#Keep in mind you have to make a blast database first. E.g. below for creating a protein database from a reference fasta file
#makeblastdb -in NIES-2180_2285_SAG13.91_COMBINED.Protein.faa -parse_seqids -title "Combined_Klebso_DB" -dbtype prot -blastdb_version 4 #We work with version 4

#Create output folder so that the work dir isn't overcrowded. the "exit -1" is to make sure that the script doesn't continue if the directory creation fails. The -p options is to make sure that no error occurs when a directory already exist.
mkdir -p "$OUT" || exit -1

echo "Step 1 Extract the long open reading frames"
TransDecoder.LongOrfs -t $IN1 -O $OUT || exit -1  

echo "Step 2 Identify ORF's with homology via Blast"
blastp -query $IN2 -db $REFERENCE_GENOME -max_target_seqs 1 -outfmt 6 -evalue 1e-5 -num_threads 22 > $OUT2 || exit -1

echo "Step 3 predict the likely coding regions, but retain only the blastp hits"
#TransDecoder.Predict -t $IN1 -O $OUT/ --retain_blastp_hits $OUT2 || exit -1
TransDecoder.Predict -t $IN1 -O $OUT/ --single_best_only --retain_blastp_hits $OUT2 || exit -1 #--single_best_only

#Remove all modules
module purge