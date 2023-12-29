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
[FastQC](https://github.com/s-andrews/FastQC) was used as a quality control, as very bad samples (Either from our in-house or downloaded from the SRA set) were then identified to be removed. Even if we removed the bad reads, some of them had such a low read count after trimming that we decided not to include them in our sample list and, therefore not included within the project. For more information on how FastQC was used within the project, go [here](Scripts/02_Trinity). For more information on FastQC please go to their site [FastQC](http://www.bioinformatics.babraham.ac.uk/projects/fastqc/)

## 3. Trinity *de novo* Assembly
After FastQC quality control, all samples were then assembled with the Trinity pipeline. First, the adapters were trimmed with [Trimmomatic](https://github.com/usadellab/Trimmomatic) with the settings 
```
-trimmomatic “novogene_adapter_sequences.fa:2:30:10:2:keepBothReads LEADING:3 TRAILING:3 MINLEN:36”
```

Followed by running [Trinity](https://github.com/trinityrnaseq/trinityrnaseq) with the command 
```
Trinity --seqType fq --left [LEFT_READS] --right [RIGHT_READS] --output [OUTPUT_FOLDER] --SS_lib_type RF --CPU 48 --trimmomatic --full_cleanup --max_memory 350G --quality_trimming_params  "ILLUMINACLIP:novogene_adapter_sequences.fa:2:30:10:2:keepBothReads LEADING:3 TRAILING:3 MINLEN:36"
```
I highly recommend reading Trinity's [Wiki site](https://github.com/trinityrnaseq/trinityrnaseq/wiki), which explains everything pretty well.<br/>

See [02_Trinity](Scripts/02_Trinity) for a more in-depth overview of what we did.

## 4. SuperTranscripts
[SuperTranscripts](https://github.com/trinityrnaseq/trinityrnaseq/wiki/SuperTranscripts) ([Davidson](https://genomebiology.biomedcentral.com/articles/10.1186/s13059-017-1284-1) et al. was inferred by collapsing splicing isoforms using the Trinity implementation. <br/>See [SuperTranscripts](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/03_SuperTranscript) for a more in-depth overview of what we did.

## 5. BUSCO I
To determine the quality (completeness) of our assemblies we followed up by running [BUSCO](https://busco.ezlab.org/) ([Seppey *et al* 2019](https://link.springer.com/protocol/10.1007/978-1-4939-9173-0_14)). using the ‘eukaryota_odb10’ reference set. <br/>See [BUSCO_I](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/04_BUSCO_I) for a more in-depth overview of what we did.

## 6. TransDecoder
After getting the SuperTranscript and confirming they are above the completeness threshold (70%) we continued with obtaining the proteins with [TransDecoder](https://github.com/TransDecoder/TransDecoder/wiki) ([Haas, BJ](https://github.com/TransDecoder/TransDecoder))
1. Extracting the open reading frames with TransDecoder.LongOrfs
2. Identify ORFs with homology via Blast
3. Predict the likely coding region, but retain only blastp hits (also use  the --single_Best_only option)

See [TransDecoder](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/05_Transdecoder) for a more in-depth overview of what we did.

## 7. BUSCO II
Another quality control is to determine if we still are above the completeness threshold (70%). This is because we used the single best option, and some of the values dropped a bit. <br/>
See [BUSCO_II](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/06_BUSCO_II/README.md) for a more in-depth overview of what we did.

## 8. Decontamination
### Setting up the Decontamination
To remove potential contaminants, we conducted sequence similarity searches against a comprehensive database that included proteins from various sources. Which were a positive set [*Klebsormidium nitens* NIES-2285](https://www.nature.com/articles/ncomms4978) and 4 potential contaminants through the RefSeq (downloaded on 17 Augustus 2020) representative bacterial genomes (11,318 genomes), fungi (2,397), all available viruses, archaea (1,833), and plastid genes (78,2087) (downloaded on 3 April 2023). We use this database to employ [MMseqs2](https://github.com/soedinglab/MMseqs2) ([Steinegger and Söding](https://www.nature.com/articles/nbt.3988)) for the search, using an iterative approach with increasing sensitivities and maintaining a maximum of 10 hits (--start-sens 1 --sens-steps 3 -s 7 --alignment-mode 3 --max-seqs 10). 
<br/>
This will give a blast output file in the .outfmt6 format. <br/>
See [Decontamination](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/07_Decontamination) for a more in-depth overview of what we did.

### Get Positive Data Set (GPDS)
To obtain the actual positive set. I've created a tool that automatically obtains the hits against the positive set and writes an FASTA output file. It also does this for all the contaminants to give you the user information about the proteins that were removed. Furthermore, it gives you an overview of the percentage of contaminants.<br/>

See [GPDS](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/08_GetPositiveDataSet_GPDS/README.md) for a more in-depth overview of what we did.

## 9 OrthoFinder
For the next step, we have to run [OrthoFinder](https://github.com/davidemms/OrthoFinder) ([DM Emms & S Kelly](https://genomebiology.biomedcentral.com/articles/10.1186/s13059-019-1832-y)) to obtain the OrtoGroups. For this we use a guide tree and all the positive samples we have, and included also our outgroups. <br/>
See [OrthoFinder](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/09_OrthoFinder) for a more in-depth overview of what we did.

## 10. OrthoGroup Sequence Grabber
This is another tool I've created to obtain all the Fasta Blocks (fasta block = fasta header + sequence) for each OrthoGroup and create a Fasta output file. Furthermore, it can take into consideration how many taxonomic groups you want to have as a minimum for each Fasta File as a filter. <br/>
See [OSG](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/10_OrthogroupSequenceGrabber_OSG) for a more in-depth overview of what we did.

In the pipeline, you will see two different sets as an output for OSG
1. Old Set
2. New Set

We later combine them (see COGS) to get a good representation of the IN and OUT groups.

### Old Set
This was the original dataset we used within the preprint (on archive), however after our Reviewer pointed out that the In-Groups (Klebsormidiophyceae) had no great branch support but had great branch support for the out-group.
For this set we used the Taxonomic Group file: [Klebsormidiophyceae_TaxonomicGroupFile_14_Taxa_420_set.txt](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/10_OrthogroupSequenceGrabber_OSG/TaxonomicGroupFiles/Klebsormidiophyceae_TaxonomicGroupFile_14_Taxa_420_set.txt) and used a threshold value of 10 (10/14 Taxonomic Groups had to be present).

### New Set
We basically started this set to define a good set that represents the In-Groups (Klebsormidiophyceae). Aka, have good branch support for them. For this set, we used the Taxonomic Group file: [Klebsormidiophyceae_TaxonomicGroupFile_4_Taxa.txt](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/10_OrthogroupSequenceGrabber_OSG/TaxonomicGroupFiles/Klebsormidiophyceae_TaxonomicGroupFile_4_Taxa.txt) and used a threshold value of 3 (3/4 Taxanomic Groups had to be present).


## 11. MAFFT/IQTree
As preparation for [PhyloPyPruner](https://pypi.org/project/phylopypruner/) to remove all the paralogs, we have to align all the sequences (Output from OSG) with [MAFFT](https://mafft.cbrc.jp/alignment/software/) ([K. Katoh and D.M. Standley](https://academic.oup.com/mbe/article/30/4/772/1073398)) and then create trees out of the MSAs with [IQTree](http://www.iqtree.org/) ([Bui Quang Minh *et al*](https://academic.oup.com/mbe/article/37/8/2461/5859215)) <br/>
See [MAFFT/IQTree](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/11_MAFFT_IQtree) for a more in-depth overview of what we did.


## 12. Apply PhyloPyPruner Format
This is the 3rd tool I've created. It basically reformats the Newick tree files that IQtree gives as an output (by default) and makes it in a format that PhyloPyPruner (PPP) can use.
See [APPPFilter](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/12_APPPFilter) for a more in-depth overview of what we did.


## 13. PhyloPyPruner


# Notes for future development
I plan to develop a fully functional C++ tool pipeline in the future. This project was meant as a simple in and output project and it became so much more than that.
The goal is to enter only the RNA-seq data (and related information) and a guide tree and let the program handle the rest so that people who are not skilled in bioinformatics can run it themselves.
Either with the help of a High Performance Cluster, or a single High Performachine machine.


# Citing
If you use anything within this repository, please cite
```
Maaike J. Bierenbroodspot, Tatyana Darienko, Sophie de Vries, Janine M.R. Fürst-Jansen, Henrik Buschmann, Thomas Pröschold, Iker Irisarri, Jan de Vries. Phylogenomic insights into the first multicellular streptophyte. bioRxiv [Preprint. 2023 Nov 1: 2023.11.01.564981. doi: 10.1101/2023.11.01.564981
```
