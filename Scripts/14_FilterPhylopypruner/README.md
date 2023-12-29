# Filter Phylopypruner Filter
To run this step, edit lines 3 to 8 within the [FilterPPP_Result_AndRemoveHeaders_AndRemoveGaps.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/14_FilterPhylopypruner/FilterPPP_Result_AndRemoveHeaders_AndRemoveGaps.sh). <br/>This tool can run with just 1 CPU, and output files can not be bigger than 1 GB of RAM. It can run on any 64-bit machine on the front end and is fast. 5% of the time is the calculations/parsing of the input files and 95% is writing them to the output folder. Which is heavily depending on the speed of your disks and/or your network speed.

If you require the size of the output files to change please contact me at maaikejacobine.bierenbroodspot@uni-goettingen.de

Basically, this tool parses the output alignment files of Phylopypruner, which are in FASTA format, and checks if they match the taxonomic threshold number. If not, they are ignored; if they are, they are copied into the output folder. Additionally you can remove the gene IDS so that only the strain names/species names are retained. You can also remove the alignments with the -a option. (needed because we will realign it with a different method later).


## FilterPPPResult (Filter Phylopypruner Result)
This tool is created with the boost library (v1.78). And is developed in Visual Studio 2019 with the GCC compiler (For remote Linux).  I've compiled 3 different executables (they are portable!) under Debian 10 but also work and tested on Debian 11 ([FilterPPPResult_Debian.out](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Executables/FilterPPPResult)), Scientific Linux 7, which is based on Red Hat ([FilterPPPResult_HPC.out](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Executables/FilterPPPResult)), and Arch Linux which is still in beta ([FilterPPPResult_Arch.out](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Executables/FilterPPPResult).

Either make your own executable with the corresponding source files or use one of the already existing executables.
### Command line options

Program format:
```
FilterPPPResult.out -f [FastaFilesPath] -r [OutputFolderPath] -t [TaxonomicGroupFile] -n [ThresholdNumber] -s [SummaryPath] -h -a
```

Program options:
```
-f <FastaFilesPath>			  Set the Path to the directory containing your fasta files: REQUIRED
-r <OutputFolderPath>	    Set the Output Folder Path: REQUIRED
-t <TaxonomicGroupFile>	  Set the Taxonomic Group File, used to filter your result: REQUIRED
-n <ThresholdNumber>		  Set the Threshold number of how many Taxonomic Groups need to be present: REQUIRED.
-s <SummaryPath>			    Set the path to where the user wants to write the Summary: NOT REQUIRED. Not setting it will result in no summary file.
-h							          Set if you want only the strain names in the header (based on the @ in the fasta headers)
-a							          Set if you want the gaps removed from the sequences (based on '-' within the sequence files)
```

### Summary File
You have the option to create a summary file that gives you an overview of what FilterPPPResult did. Example output:

```
There were a total of 1981 Fasta Files in the PhylopyPruner output directory, which was based 63 Strains. 
The Taxonomic Group file contained a total of 40 Taxonomic Groups
The User-defined 3 as a Threshold, meaning 3/40 Taxonomic Groups had to be present for each FastaFile.
From the total of 1981 FastaFiles, 212 Where Filtered out, resulting in 1769 Filtered Fasta Files, containing at least 3 Taxonomic Groups.

Each of these Filtered Fasta files was written into the output folder.
```
