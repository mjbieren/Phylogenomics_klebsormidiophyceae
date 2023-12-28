#Filter Phylopypruner Filter
To run this step edit line 3 till 8 within the FilterPPP_Result_AndRemoveHeaders_AndRemoveGaps.sh. This tool can run with just 1 cpu and output files can not be bigger than 1gb of RAM. It can run on any 64-bit machine on the front end and is fast. 5% of the time is the calculations/parsing of the input files and 95% is writing them to the output folder. Which is heavily depending on the speed of your disks and/or your network speed.

If you require the size of the output files to change please contact me under maaikejacobine.bierenbroodspot@uni-goettingen.de

Basically this tool parses the output alignment files of Phylopypruner, which are in fasta format and check if they matches the taxonomic threshold number. If not they are ignored if they are they are copied into the output folder. Additionally you can remove the gene IDS so that only the strain names/species names are retained. You can also remove the alignments with the -a option. (needed because we will realign it with a different method later).


##FilterPPPResult (Filter Phylopypruner Result)
This tool is created with the boost library (v1.78). And is developed in Visual Studio 2019 with the GCC compiler (For remote Linux).  I've compiled 3 different executables (they are portable!) under Debian 10 but also works and tested on Debian 11 (FilterPPPResult_Debian.out), Scientific Linux 7 which is based on Red Hat (FilterPPPResult_HPC.out) and Arch Linux which is still in beta (FilterPPPResult_Arch.out).

Either make your own executable with the corresponding source files or use one of the already existing executables.

###Command line options
-f <FastaFilesPath>			Set the Path to the directory containing your fasta files: REQUIRED %s
-r <OutputFolderPath>		Set the Output Folder Path: REQUIRED
-t <TaxonomicGroupFile>		Set the Taxonomic Group File, used to filter your result: REQUIRED
-n <ThresholdNumber>		Set the Threshold number of how many Taxonomic Groups need to be present: REQUIRED.
-s <SummaryPath>			Set the path to where the user want to write the Summary: NOT REQUIRED. Not setting it will result in no summary file.%s"
-h							Set if you want only the strain names in the header (based on the @ in the fasta headers)
-a							Set if you want the gaps removed from the sequences (based on '-' within the sequence files)

###Summary File
You have the option to create a summary file which give you an overview of what FilterPPPResult did. Example output:

```
There were a total of 1981 Fasta Files in the PhylopyPruner output directory, which was based 63 Strains 
The Taxonomic Group file contained a total of 40 Taxonomic Groups
The User defined 3 as a Threshold, meaning 3/40 Taxonomic Groups had to be present for each FastaFile.
From the total off 1981 FastaFiles, 212 Where Filtered out resulting in 1769 Filtered Fasta Files, containing at least 3 Taxonomic Groups.

Each of these Filtered Fasta files was written into the output folder.
```