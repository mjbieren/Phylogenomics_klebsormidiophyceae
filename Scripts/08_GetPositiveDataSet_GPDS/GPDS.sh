#!/bin/bash

#$PROGRAM_PATH -i <Header> -f <Fasta> -b <Blast> -c <EvalueColumn> -s <StrainName> -r <OutputPath>

STRAIN=${PWD##**/} #Or just enter your strain name, my folders automatically were created with the strain name I wanted prior.
IN=*.outfmt6 #Match File MMseqs2 Output
HEADER= #HeaderFile
FASTA=*.pep #Fasta File source (Transdecoder Output)
WORKINGDIR=${PWD} #Current working dir where the script is run. I recommend to change it when the script is not in the same folder
COPYDIR= #Folder where you want your positive set to be copied into
PROGRAM_PATH= #The path to GPDS_[OS].out You have to compile this yourself or use on of the Executables /Executables/GPDS/

#Run the program
$PROGRAM_PATH -i $HEADER -f $FASTA -b $IN -c 11 -s $STRAIN -r $WORKINGDIR/

cd $WORKINGDIR

FILES=`ls *_Klebsormidiophyceae.fa | sed 's/_Klebsormidiophyceae.fa//g'`

for f in $FILES
do
(
	cp ${f}_Klebsormidiophyceae.fa ${COPYDIR}/${f}.fa #We want to remove the _Klebsormidiophyceae important for later
)
done