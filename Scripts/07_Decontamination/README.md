# Decontamination
Basically, this step requires MMseqs2, a decontamination database, and a conversion of your protein file to an MMseqs2 database.

## MMseqs2
MMSeqs2 is an ultra-fast and sensitive sequence search and clustering suite. It's a lot faster (100-10k times faster) than BLAST, and since we are looking at removing all decontaminants in a very strict matter, the slight loss in sensitivity can be ignored. Since performance is more important here, especially when comparing against a huge dataset (Against the whole: Archeae, Bacteria, Fungi, Viral, and Plastid dataset) we use MMseqs2 instead of BLAST.

MMSeqs2 is developed at the Söding's at the Max-Planck-Institute for Multidisciplinary Sciences, Göttingen.
To install MMseqs2 please follow the link https://github.com/soedinglab/MMseqs2


## Decontamination database
As a decontamination database, we downloaded on 17 August 2020 the full RefSeq database for Bacterial, Fungi, all available viruses, Archae genomes, and additionally, all plastids. For the Positive set, we used NIES 2285 (Klebsormidium nitens, otherwise known as strain SAG 335-1a). 

To make it easier to define what is a decontaminant and what is not I've reformatted all fasta headers with the script [simplify_headers_for_blastdb.py](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/07_Decontamination/simplify_headers_for_blastdb.py) made by Dr. Iker Irrisari.
simply use the script by the command:

```
python [script] [inputFile] [RenameHeaders]>>[FileOutput]
```
where the FileOutput is the concatenated file for all databases.
I used the following combinations:
```
"arch_neg" for the Archea set
"bact_neg" for the Bacteria set
"virus_neg" for the Viridae set
"fungi_neg" for te Fungi set
"plast_neg" for the Plastid set
"kleb_pos" for Klebsormidiaceae positive (NIES-2285)
```
This means, for example, that for every fasta header for bacteria, we now have bact_neg-n where n is the number. This is important since the step after the decontamination uses these as a reference (you can decide on your own what the names are, but they have to be consistent and preferable without any spaces since a lot of programs can not interpret a space properly).

Then simply use the commands:
```
mmseqs createdb Decontamination_Klebsome_DB.fa Decontamination_Klebsome_DB.db
mmseqs createindex Decontamination_Klebsome_DB.db tmp
```
Your decontamination database is ready to use

## Conversion of your protein file.
Similar to creating a database for decontamination, you have the same commands for creating an MMseqs2 database for your protein file.
```
mmseqs createdb trinity_genes.fasta.transdecoder.pep StrainName_Klebsome_db.db || module purge && exit -1
mmseqs createindex StrainName_Klebsome_db.db tmp || module purge && exit -1
```
You can also use the script [ConvertToMMseqsDatabase.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/07_Decontamination/ConvertToMMseqsDatabase.sh) and run it in the same folder as the TransDecoder output .pep file.


## Running the script
Now simply run the script [Decontamination_MMSEQS_Memmode.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/07_Decontamination/Decontamination_MMSEQS_Memmode.sh)
Don't forget to change the variables on lines: 8,9,10,11.

The output will be a simple outfmt6 output, identical to a blast output file with outfmt6 format.
