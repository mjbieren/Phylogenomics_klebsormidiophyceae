#!/bin/bash

APPPF_PATH=/home/uni08/applbioinfdevries/MAAIKE/Programs/APPPFormat/APPPFormat_HPC.out
TREE_FILES=/home/uni08/applbioinfdevries/DATA_ILLUMINA/Klebsormidium_SPP/SubTrees/NewFullTree/New_Tax_FixCurrentBiology/New_Tax3_Filtered_WithTrees/
TREEFORMAT=treefile
APPF_OUTPUT=/home/uni08/applbioinfdevries/DATA_ILLUMINA/Klebsormidium_SPP/SubTrees/NewFullTree/New_Tax_FixCurrentBiology/APPPFormat_Output/
TAXANOMIC_GROUP_FILE=/home/uni08/applbioinfdevries/DATA_ILLUMINA/Klebsormidium_SPP/SubTrees/EtcInformation/Reduced_Taxa_ForMoreAlignments_Klebso.txt

#Program format can be any order after program path
#[Program_Path] -i [PathToTrees] -t [FileExtensionOfTrees] -r [OutputFolderPath] -g [TaxonomicGroupFilePath] -m [MafftFilesPath:NOT REQUIRED]

#-i <PathToTrees> Set the path where the tree files are located. This is in newick format.[REQUIRED]
#-t <TreeFileExtension> Set the extension for the newick trees. For example .treefile.
#-g <TaxonomicGroupFile> Set the Taxonomic Group File, used to find the exact position to replace the _ with @ [REQUIRED]
#-r <OutputFolderPath> Set the Output Folder Path: [REQUIRED]
#-m <PathToMafftFiles> Set the path if you want to move your msa files (.mafft extension) and place them in your output folder. The extension automatically get changed from .mafft to .fa. [NOT REQUIRED]

$APPPF_PATH -i $TREE_FILES -t $TREEFORMAT -r $APPF_OUTPUT -g $TAXANOMIC_GROUP_FILE -m $TREE_FILES
