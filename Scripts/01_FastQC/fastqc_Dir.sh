#!/bin/bash

#SBATCH -p medium
#SBATCH -t 48:00:00
#SBATCH -o %J_FastQC.out
#SBATCH -C scratch
#SBATCH -c 1
#SBATCH --mem 30G

module purge
module load fastqc

CURRENTDIR=`pwd`
DIR=${CURRENTDIR}/Raw_Sample_Data/ #Change this
OUT=${CURRENTDIR}/FASTQC_Result/

mkdir -p "$OUT" || exit -1
DIRECTORIES=`dir ${DIR}`

#Create output folder so that the work dir isn't overcrowded. the "exit -1" is to make sure that the script doesn't continue if the directory creation fails. The -p options is to make sure that no error occurs when a directory already exist.


for f in $DIRECTORIES
do
{
FILES=`ls ${DIR}${f}/*.fq.gz`
for x in $FILES
	do
	{
	
	fastqc ${x} -o ${OUT}
	}
	done
}
done
module purge
