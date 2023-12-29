#!/bin/bash

#SBATCH -p medium
#SBATCH -t 10:00:00
#SBATCH -o Busco_E_Fimbriata_%J.out
#SBATCH -C scratch
#SBATCH -c 3
#SBATCH --exclusive
#SBATCH --mem 20G


#Clear everything to create a clean environment 
module purge
#Load busco
module load busco


#Input
IN= #Input Fasta file
#Mode
#MODE=genome
#MODE=proteins
MODE=transcriptome
#Output folder
OUT=Busco_Out

#Busco Run determine lineage 
#Auto lineage with only eukaryotic
#busco -m $MODE -i $IN -o $OUT -f --auto-lineage-euk

#Busco with specified lineage selection (eukaryota_odb10)
busco -m $MODE -i $IN -o $OUT -f -l eukaryota_odb10
