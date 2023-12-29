# Get Positive Dataset
This step is very simple. Just edit the "GPDS.sh" and run it. (Change lines 3 to 9)
This tool can run with just 1 CPU and files can not be bigger than 1 GB of RAM. and run roughly 1-2 seconds.
If you require this to change please contact me at maaikejacobine.bierenbroodspot@uni-goettingen.de

This tool basically interprets the *.outfmt6 file and obtains the corresponding matching fasta blocks (1 Fasta block = 1 Header + 1 sequence).
It will create different fasta files based on the header file.
An example can be found with [Headerfile.GPDS.txt](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/08_GetPositiveDataSet_GPDS/Headerfile_GPDS.txt)

It will create different Fasta files (objects) based on each line within the Headerfile. And copy the fasta block object pointers into the fasta file object. Which are then used to create the output files (1 Fasta file object = 1 Fasta file)

At the end, it will then create a summary file, giving you, the user, an overview of how contaminated the original file was.
Though read this with a grain of salt, considering it's very, very strict, and actual decontamination values can be much lower.
It just considers the best matching to be the correct one (needed for getting a dataset that's 100% matching to our positive dataset).
However, it is a really good indication of how contaminated a file is, and it saved me some headaches when one of the samples was actually contaminated with another non-klebsormidiophyceae sample.

## GPDS (Get Positive Data Set)
This tool is created with the boost library (v1.78). And is developed in Visual Studio 2019 with the GCC compiler (For remote Linux).  I've compiled 3 different executables (they are portable!) under Debian 10 but also work and tested on Debian 11 ([GPDS_Debian.out](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Executables/GPDS)), Scientific Linux 7, which is based on Red Hat ([GPDS_HPC.out](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Executables/GPDS)), and Arch Linux which is still in beta ([GPDS_Arch.out](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Executables/GPDS)).

Either make your own executable with the corresponding source files or use one of the already existing executables.

### Command Line commands
To run GPDS, follow this command guideline
```
GPDS.out -i <HeaderFile> -f <Fasta> -b <Blast> -c <EvalueColumn> -s <StrainName> -r <OutputPath>
```

Program options:
```

-i	<HeaderFile>	Basically an overview that the program uses to interpret if a header is a contaminant or not. E.g.
				HeaderName	+-	Full Name
				
				plast_neg	-	Plastid
				kleb_pos	+	Klebsormidiophyceae
```					
This means that everything with plast_neg is interpreted as a contaminant, and the fasta file name will be StrainName_Plastid.fa . 
The 2nd will be interpreted as a non-contaminant, and the fasta file name will be StrainName_Klebsormidiophyceae.

```
-f	<FastaFile>		The Transdecoder Output File (.pep)
-b	<Blast>			The output file of MMseqs2 in blast .outfmt6 format.
-c	<ColumnNumber>		The column of the e-value. (Normally, this is 11, but can change due to settings)
-s	<StrainName>		The Strain Name of the sample
-r	<OutputPath>		The full path to the output folder.
-t				Unset the strict decontamination, ignore the positive set if the result is weaker (default is strict is on, best
				e-value is the one taken)
```
Alternatively you can edit [GPDS.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/08_GetPositiveDataSet_GPDS/GPDS.sh) and run the script
