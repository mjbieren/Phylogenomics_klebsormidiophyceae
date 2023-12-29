# Phylogenomics klebsormidiophyceae
This is an overview of all novel tools and scripts used in the Klebsormidiophyceae Phylogenomics Project


# Workflow
![alt text](Sources/Images/Phylogenomic_Klebso_Pipeline_1500_815.png?raw=True "Pipeline")


# Introduction
Above, I've portrayed the pipeline in which the Klebsormidiophyceae Phylogenomics Project was executed.

## 1. RNA Seq Data
This was either obtained from the SRA database (see preprint under citing below). Or obtained in-house.
All in-house strains were cultivated in 3NBBM (medium 26a101) at 18°C under full-spectrum fluorescent lamps (25-35 µmol photons m−2 s−1; 14:10h light-dark cycle).
After 21 days, 50ml of the culture was centrifuged for 5 min at 20°C at 11000 rpm. The supernatant was removed, and the pellet was transferred into the Tenbroek tissue homogenizer and manually disrupted for 10 min on ice.
Further Extraction was then done with the RNA from the Spectrum™ Plant Total RNA Kit (Sigma-Aldrich Chemie GmbH, Germany) according to the manufacturer’s instructions. DNAse I treatment (Thermo Fisher, Waltham, MA, USA) was applied to the RNA samples, and their quality and quantity were assessed using a 1% agarose gel with an SDS stain, and nanodrop (Thermo Fisher), respectively. The RNA samples were shipped on dry ice to Novogene (Cambridge, UK).

At Novogene (Cambridge, UK), the samples underwent quality checks using a Bionanalyzer (Agilent Technologies Inc., Santa Clara, CA, USA), and library preparation was performed based on polyA enrichment and using directional mRNA library preparation. The libraries were quality checked and sequenced using the NovaSeq 6000 platform (Illumina) with Novogene dual adapters: 5’- AGATCGGAAGAGCGTCGTGTAGGGAAAGAGTGTAGATCTCGGTGGTCGCCGTATCATT-3’ for read 1 and 5’- GATCGGAAGAGCACACGTCTGAACTCCAGTCACGGATGACTATCTCGTATGCCGTCTTCTGCTTG-3’

## 2. FastQC
FastQC was used as a quality control, as very bad samples (Either from our in-house or downloaded from the SRA set) were then identified to be removed. Even if we removed the bad reads, some of them had such a low read count after trimming that we decided not to include them in our sample list and, therefore not included within the project. For more information on how FastQC was used within the project, go [here](Scripts/02_Trinity). For more information on FastQC please go to their site [FastQC](http://www.bioinformatics.babraham.ac.uk/projects/fastqc/)

## 3. Trinity *de novo* Assembly
After FastQC quality control, all samples were then assembled with the Trinity pipeline. First, the adapters were trimmed with [Trimmomatic](https://github.com/usadellab/Trimmomatic) with the settings 
```
-trimmomatic “novogene_adapter_sequences.fa:2:30:10:2:keepBothReads LEADING:3 TRAILING:3 MINLEN:36”
```

Followed by running [Trinity](https://github.com/trinityrnaseq/trinityrnaseq) with the command 
```
Trinity --seqType fq --left [LEFT_READS] --right [RIGHT_READS] --output [OUTPUT_FOLDER] --SS_lib_type RF --CPU 48 --trimmomatic --full_cleanup --max_memory 350G --quality_trimming_params  "ILLUMINACLIP:novogene_adapter_sequences.fa:2:30:10:2:keepBothReads LEADING:3 TRAILING:3 MINLEN:36"
```
I highly recommend reading Trinity's [Wiki site](https://github.com/trinityrnaseq/trinityrnaseq/wiki), which explains everything pretty well.

See [02_Trinity](Scripts/02_Trinity) for a more in-depth overview of what we did.

## 4. SuperTranscripts
[SuperTranscripts](https://github.com/trinityrnaseq/trinityrnaseq/wiki/SuperTranscripts) ([Davidson](https://genomebiology.biomedcentral.com/articles/10.1186/s13059-017-1284-1) et al. was inferred by collapsing splicing isoforms using the Trinity implementation. See [SuperTranscripts](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/03_SuperTranscript) for a more in-depth overview of what we did.

## 5. BUSCO I
To determine the quality (completeness) of our assemblies we followed up by running [BUSCO](https://busco.ezlab.org/)([Seppey *et al* 2019](https://link.springer.com/protocol/10.1007/978-1-4939-9173-0_14)). using the ‘eukaryota_odb10’ reference set. See [BUSCO_I](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/04_BUSCO_I) for a more in-depth overview of what we did.

## 6. TransDecoder
After getting the SuperTranscript and confirming they are above the completeness threshold (70%) we continued with obtaining the proteins with [TransDecoder](https://github.com/TransDecoder/TransDecoder/wiki) ([Haas, BJ](https://github.com/TransDecoder/TransDecoder))
1. Extracting the open reading frames with TransDecoder.LongOrfs
2. Identify ORFs with homology via Blast
3. Predict the likely coding region, but retain only blastp hits (also use  the --single_Best_only option)

See [TransDecoder](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/05_Transdecoder) for a more in-depth overview of what we did.

## 7. BUSCO II
Another quality control is to determine if we still are above the completeness threshold (70%). This is because we used the single best only option, and some of the values dropped a bit.
See [BUSCO_II](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/06_BUSCO_II/README.md) for a more in-dept overview of what we did.


## 8. Decontamination
To remove potential contaminatants we conducted sequence similarity searches against a comprehensive database that included proteins from various sources. Which are a positive set 


# Notes for future development
I plan to develop a fully functional C++ tool pipeline in the future. This project was meant as a simple in and output project and it became so much more than that.
The goal is to enter only the RNA-seq data (and related information) and a guide tree and let the program handle the rest so that people who are not skilled in bioinformatics can run it themselves.
Either with the help of a High Performance Cluster, or a single High Performachine machine.


# Citing
If you use anything within this repository, please cite
```
Maaike J. Bierenbroodspot, Tatyana Darienko, Sophie de Vries, Janine M.R. Fürst-Jansen, Henrik Buschmann, Thomas Pröschold, Iker Irisarri, Jan de Vries. Phylogenomic insights into the first multicellular streptophyte. bioRxiv [Preprint. 2023 Nov 1: 2023.11.01.564981. doi: 10.1101/2023.11.01.564981
```
