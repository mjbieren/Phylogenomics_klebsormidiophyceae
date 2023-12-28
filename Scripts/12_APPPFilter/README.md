# Apply Phylopypruner format (APPPFormat)
This step is necessary since Phylopypruner needs a certain format.
OSG added the right format to the alignment files, but IQTree removed this format in the tree output files. Apply Phylopypruner Format does this for you.
It then moves the tree files to the output folder and also copies the alignment files to the output files.

It needs a taxonomic group file as a guidance for the species/strain names (See [TaxonomicGroupFiles](/Scripts/10_OrthogroupSequenceGrabber_OSG/TaxonomicGroupFiles/) for examples)

To run it, edit the [APPPF.sh](/Scripts/12_APPPFilter/APPPF.sh) file and edit lines number 3 to 7
This tool can run with just 1 cpu and output files can not be bigger than 1gb of RAM. It can run on any 64-bit machine on the front end and is super fast. 5% of the time is the calculations. 95% of the time, it parses the input file (loading to memory) and writes the output files. Which is heavily depending on the speed of your disks and/or your network speed.

## APPPFormat (Apply Phylopypruner Format)
This tool is created with the boost library (v1.78). And is developed in Visual Studio 2019 with the GCC compiler (For remote Linux).  I've compiled 3 different executables (they are portable!) under Debian 10 but also work and tested on Debian 11 ([APPPFormat_Debian.out](/Executables/APPPFormat/)), Scientific Linux 7, which is based on Red Hat ([APPPFormat_HPC.out](/Executables/APPPFormat/)), and Arch Linux, which is still in beta ([APPPFormat_Arch.out](/Executables/APPPFormat/)).

Either make your own executable with the corresponding source files or use one of the already existing executables.

### Command line options
Program format:
```
APPPFormat.out -i [PathToTrees] -t [FileExtensionOfTrees] -r [OutputFolderPath] -g [TaxonomicGroupFilePath] -m [MafftFilesPath:NOT REQUIRED]
```
Options:
```
-i <PathToTrees>		Set the path where the tree files are located. 
				This is in newick format.[REQUIRED]
-t <TreeFileExtension>		Set the extension for the newick trees. For example, .treefile.
-g <TaxonomicGroupFile>		Set the Taxonomic Group File, used to find the exact position 
				to replace the _ with @ [REQUIRED]
-r <OutputFolderPath>		Set the Output Folder Path: [REQUIRED]
-m <PathToMafftFiles>		Set the path if you want to move your msa files (.mafft extension)
				and place them in your output folder. The extension automatically gets changed from .mafft to .fa [NOT REQUIRED]
```
