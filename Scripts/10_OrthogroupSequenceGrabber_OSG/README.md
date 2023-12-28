#Orthogroup Sequence Grabber
This step is also very simple, again just edit the OSG.sh file and run it (Change line 4 till 15).
This tool can run with just 1 cpu and output files can not be bigger than 1gb of RAM. It can run on any 64-bit machine on the front end and is super fast. 5% of the time is the calculations/parsing of the input files and 95% is writing them to the output folder. Which is heavily depending on the speed of your disks and/or your network speed.

If you require the size of the output files to change please contact me under maaikejacobine.bierenbroodspot@uni-goettingen.de


Basically this tool interprets orthofinder's tsv files under "Phylogenetic_Hierarchical_Orthogroups" parses the Orthogroups in fasta objects, by obtaining the fasta blocks from the original fasta files (Orthofinder input folder). It then filters this result based on the users taxonomic group file and the threshold given by the user. This means that if you have a taxonomic group filter threshold of 2. You need at least 2 species of different taxonomic groups. Not two species of the same taxonomic group. See the folder TaxonomicGroupFiles for examples.

It will then create the fasta output files (which can be quite large depending on the filtering).
Finally a summary file is created if the user wishes so.

##OSG (Orthogroup Sequence Grabber)
This tool is created with the boost library (v1.78). And is developed in Visual Studio 2019 with the GCC compiler (For remote Linux).  I've compiled 3 different executables (they are portable!) under Debian 10 but also works and tested on Debian 11 (OSG_Debian.out), Scientific Linux 7 which is based on Red Hat (OSG_HPC.out) and Arch Linux which is still in beta (OSG_Arch.out).

Either make your own executable with the corresponding source files or use one of the already existing executables.


###Command line options
[Program Path] -i [Orthofinder folder] -f [Fasta Folder] -o [Output Folder] -h [Taxonomic group file] -s [Number of minimum taxonomic groups -r

-f <FastaFilesPath> 		Set the Path to the directory containing your fasta files: REQUIRED
-g <OrthoGroupFilesPath>	Set the Path to the directory containing the Orthogroups in TSV format: REQUIRED%
-r <OutputFolderPath>		Set the Output Folder Path: REQUIRED
-t <TaxonomicGroupFile>		Set the Taxonomic Group File, used to filter your result: NOT REQUIRED. 
							If not Set all Orthogroups are parsed
-n <ThresholdNumber>		Set the Threshold number of how many Taxonomic Groups need to be present: NOT REQUIRED.
							Not setting this value results in parsing all orthogroups!
-p							Changes all fasta headers into the format needed for PhyloPyPruner: NOT REQUIRED. 
							Not setting it will result in the same fasta headers as the fasta files uses.
-s <SummaryPath>			Set the path to where the user want to write the Summary: NOT REQUIRED. 
							Not setting it will result in no summary file.

###Summary File
You have the option to create a summary file which give you an overview of what OSG did. Example output:

```
There were a total of 1761660 OrthoGroups from the OrthoFinder Output, which was based on 63 taxa 
The Taxonomic Group file contained a total of 14 Taxonomic Groups
The User defined 14 as a Threshold, meaning 14/14 Taxonomic Groups had to be present for each orthogroup.
From the total off 1761660 Orthogroups, 1758040 Where Filtered out resulting in 3620 Orthogroups, containing at least 14 Taxonomic Groups.

Each of these Orthogroups was written into a Fasta File.
```