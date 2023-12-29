#!/bin/bash

#SBATCH -p medium
#SBATCH -t 48:00:00
#SBATCH -o %J_KlebsTrinity.out
#SBATCH -c 48
#SBATCH -C scratch
#SBATCH --exclusive
#SBATCH --mem 350G


module purge
module load trinity
#module load python/3.8.2 # python3 required by TRINITY (old version), not newer "trinity" 


IN1= #FullPath_x1_1.fg.qz,FullPath_x2_1fq.gz
IN2= #x1_2.fg.qz,x2_2fq.gz
OUT= #OutputPathFull

# TRUSEQ3 adapters
#Trinity --seqType fq --left $IN1 --right $IN2 --output $OUT --SS_lib_type RF --CPU 48 --trimmomatic --full_cleanup --max_memory 128G --quality_trimming_params "ILLUMINACLIP:TruSeq3-PE.fa:2:30:10:2:keepBothReads LEADING:3 TRAILING:3 MINLEN:36"


# TRUSEQ2 adapters
#Trinity --seqType fq --left $IN1 --right $IN2 --output $OUT --SS_lib_type RF --CPU 48 --trimmomatic --full_cleanup --max_memory 128G --quality_trimming_params  "ILLUMINACLIP:TruSeq2-PE.fa:2:30:10:2:keepBothReads LEADING:3 TRAILING:3 MINLEN:36"

#Novogene adapter
Trinity --seqType fq --left $IN1 --right $IN2 --output $OUT --SS_lib_type RF --CPU 48 --trimmomatic --full_cleanup --max_memory 350G --quality_trimming_params  "ILLUMINACLIP:novogene_adapter_sequences.fa:2:30:10:2:keepBothReads LEADING:3 TRAILING:3 MINLEN:36"

module purge
