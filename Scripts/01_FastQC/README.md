# FastQC
The first step within the pipeline is doing quality control of the RNA-seq Reads<br/>
We do this with FastQC.
The script [fastqc_Dir.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/01_FastQC/fastqc_Dir.sh) will execute fastqc on each directory (1 layer deep). But you can also just run it within the directory with the command

```
fastqc *.fq.gz
```


For more information on FastQC follow the link [FastQC](http://www.bioinformatics.babraham.ac.uk/projects/fastqc/)
