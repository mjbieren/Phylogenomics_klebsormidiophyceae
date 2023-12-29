#!/bin/bash

#SBATCH -p medium
#SBATCH -t 5:00:00
#SBATCH -o Busco_k_flaccidum_%J.out
#SBATCH -C scratch2
#SBATCH --exclusive
#SBATCH --mem 10G

#Clear everything to create a clean environment 
module purge
#Load busco
module load busco


#Input
IN= #Transdecoder Output
#Mode
#MODE=genome
MODE=proteins
#MODE=transcriptome
#Output folder
OUT=Busco_Out_Clean

mkdir -p "$OUT" || exit -1 #Make the directory and if already exist error will be ignored, if no permission is available it will stop the program

#Busco Run
#Auto lineage with only eukaryotic
#busco -m $MODE -i $IN -o $OUT -f --auto-lineage-euk

#Busco with specified lineage selection (eukaryota_odb10)
busco -m $MODE -i $IN -o $OUT -f -l eukaryota_odb10
#busco -m $MODE -i $IN -o $OUT -f -l chlorophyta_odb10

module purge
