#!/bin/bash

APPPF_PATH= #APPPFormat.out
TREE_FILES= #Path to the treefiles aka output of step 11.
TREEFORMAT=treefile #Name of the file extension without a .
APPF_OUTPUT= #Output Folder for the program
TAXANOMIC_GROUP_FILE= #Path to your taxanomic group file

#Program format can be any order after program path
#[Program_Path] -i [PathToTrees] -t [FileExtensionOfTrees] -r [OutputFolderPath] -g [TaxonomicGroupFilePath] -m [MafftFilesPath:NOT REQUIRED]

#-i <PathToTrees> Set the path where the tree files are located. This is in newick format.[REQUIRED]
#-t <TreeFileExtension> Set the extension for the newick trees. For example .treefile.
#-g <TaxonomicGroupFile> Set the Taxonomic Group File, used to find the exact position to replace the _ with @ [REQUIRED]
#-r <OutputFolderPath> Set the Output Folder Path: [REQUIRED]
#-m <PathToMafftFiles> Set the path if you want to move your msa files (.mafft extension) and place them in your output folder. The extension automatically get changed from .mafft to .fa. [NOT REQUIRED]

$APPPF_PATH -i $TREE_FILES -t $TREEFORMAT -r $APPF_OUTPUT -g $TAXANOMIC_GROUP_FILE -m $TREE_FILES
