#!/bin/bash

PROGRAMPATH= #FilterPPPResult_Debian.out Change the path
INPUT= #phylopypruner_output/output_alignments/ #Change This
OUTPUT= #Change This Output Path
TAXONOMIC_GROUPFILE= #Taxonomic Group file see (See /10_OrthogroupSequenceGrabber_OSG/TaxonomicGroupFiles/ for examples)
SUMMARY_FILE= #Output folder where the summary file ends up
NUMBER_OF_FILTER_GROUPS=3 #Change This. Threshold of for the amount of taxonomic groups

#Filter PPP result based on NUMBER_OF_FILTER_GROUPS
#with gene ids
#$PROGRAMPATH -f $INPUT -t $TAXONOMIC_GROUPFILE -r $OUTPUT -n $NUMBER_OF_FILTER_GROUPS -s $SUMMARY_FILE

#Without Gene IDs or alignments
$PROGRAMPATH -f $INPUT -t $TAXONOMIC_GROUPFILE_ADJUSTED -r $OUTPUT -n $NUMBER_OF_FILTER_GROUPS -s $SUMMARY_FILE -a -h

