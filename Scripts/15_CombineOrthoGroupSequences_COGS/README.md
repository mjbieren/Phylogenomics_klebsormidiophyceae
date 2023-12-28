# Combine Orthogroup Sets
To run this step, edit [CombineTwoFastaSets.sh](Scripts/15_CombineOrthoGroupSequences_COGS/CombineTwoFastaSets.sh) lines 3 to 8.

This tool can run with just 1 CPU, and output files can not be bigger than 1 GB of RAM. It can run on any 64-bit machine on the front end and is super fast. 5% of the time is the calculations/parsing of the input files and 95% is writing them to the output folder. Which is heavily depending on the speed of your disks and/or your network speed.

If you require the size of the output files to change, please contact me at maaikejacobine.bierenbroodspot@uni-goettingen.de

Basically, this tool interprets the orthogroup names in the two fasta file set folders, parses this in an object, and removes the duplicates.
Then, it parses the Orthogroups based on the OrthoFinder Output and finds the fasta objects, and compares it to the combined set list. Then, write the fasta output files.

Additionally, you can tell the program to format the fasta headers into a Phylopypruner format.

> [!IMPORTANT]
>After running the script, you have to repeat Steps 11 to 14 before continuing with Step 16.

## COGS (Combine OrthoGroup Sets)
This tool is created with the boost library (v1.78). And is developed in Visual Studio 2019 with the GCC compiler (For remote Linux).  I've compiled 3 different executables (they are portable!) under Debian 10 but also works and tested on Debian 11 ([COGS_Debian.out](Executables/COGS)), Scientific Linux 7 which is based on Red Hat ([COGS_HPC.out](Executables/COGS)) and Arch Linux which is still in beta ([COGS_Arch.out](Executables/COGS)).

Either make your own executable with the corresponding source files or use one of the already existing executables.


### Command Line
Program format:
```
COGS.out -f <SourceFileBase> -s <PathToFirstSet> -t <PathToSecondSet> -o <OrthoFinderOutput> -r <PathOutputDirectory> -p
```

Program Options:
```
-f <FastaFilesBase>			  Set the Path to the directory containing your fasta files: REQUIRED
-s <FastaFileFirstSet>		Set the Path to the first orthogroup set in fasta file formats: REQUIRED
-t <FastaFileSecondSet>		Set the Path to the second orthogroup set in fasta file formats: REQUIRED
-o <OrthoGroupFilesPath>	Set the Path to the directory containing the Orthogroups in TSV format: REQUIRED
-r <OutputDirPath>		  	Set the Path to the directory where the output files have to be written to: REQUIRED
-p							          Changes all fasta headers into the format needed for PhyloPyPruner: NOT REQUIRED.
```
