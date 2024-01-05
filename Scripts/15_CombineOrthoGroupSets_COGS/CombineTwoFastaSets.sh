#!/bin/bash

PROGRAM_PATH= #COGS.out
FASTA_SET_BASE= #Input fasta folder from OrthoFinder.
FASTA_SET_ONE= #FastaFilesFirstSet. E.g. the old 420 set
FASTA_SET_TWO= #FastaFilesSecondSet. E.g. the new set 21 Tax set
ORTHOFINDER_OUTPUT= #Orthofinder Output folder "Phylogenetic_Hierarchical_Orthogroups".
OUTPUT_DIR= #Output Folder


#-f <FastaFilesBase> Set the Path to the directory containing your fasta files: REQUIRED
#-s <FastaFileFirstSet> Set the Path to the first orthogroup set in fasta file formats: REQUIRED
#-t <FastaFileSecondSet> Set the Path to the second orthogroup set in fasta file formats: REQUIRED
#-o <OrthoGroupFilesPath> Set the Path to the directory containing the Orthogroups in TSV format: REQUIRED
#-r <OutputDirPath> Set the Path to directory where the output files have to be written to: REQUIRED
#-p Changes all fasta headers into the format needed for PhyloPyPruner: NOT REQUIRED.

#<ProgramPath> -f <SourceFileBase> -s <PathToFirstSet> -t <PathToSecondSet> -o <OrthoFinderOutput> -r <PathOutputDirectory> -p

${PROGRAM_PATH} -f ${FASTA_SET_BASE} -s ${FASTA_SET_ONE} -t ${FASTA_SET_TWO} -o ${ORTHOFINDER_OUTPUT} -r ${OUTPUT_DIR}