#!/bin/bash
#SBATCH -p medium
#SBATCH -t 00:10:00
#SBATCH --qos=short
#SBATCH -o %J_Convert_to_MMSeqs.out
#SBATCH -C scratch2
#SBATCH --exclusive
#SBATCH --mem 20G
#SBATCH -c 24

#clean module list
module purge
#load module mmseqs2
module load mmseqs2

CURRENTDIR="$PWD"
IN=$CURRENTDIR/trinity_genes.fasta.transdecoder.pep #Change this into your source
OUT=${PWD##**/}_Klebsome_db.db #Change this into your target

#Create the mmseqs2 database, the exit -1 makes sure you get a error message when it fails and purges the modules on the node
mmseqs createdb $IN $OUT || module purge && exit -1
mmseqs createindex $OUT tmp || module purge && exit -1

#Clean up the node from your modules
module purge