# TransDecoder
Our 5th step is to obtain proteins from our SuperTranscripts since we are going to compare proteins with OrthoFinder.

In our case, TransDecoder was already installed on our High-Performance Cluster, so we could run this on the system there without the need to install it.
But you can simply install it with a conda environment:
```
conda create -n transdecoder -c conda-forge -c bioconda transdecoder
```

We use Blast in this case since the dataset is relatively small.
For this to work, you need to use a reference database first (from a reference genome/dataset). We work with version 4

```
makeblastdb -in [REFERENCE_GENOME_FASTAFILE] -parse_seqids -title "[NAME_OF_YOUR_DB]" -dbtype prot -blastdb_version 4
```

Then simply edit the [TransDecoder_Single_Best_HPC.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/05_Transdecoder/TransDecoder_Single_Best_HPC.sh) script and run it.

For more information on TransDecoder, go to their website: [Transdecoder](https://github.com/TransDecoder/TransDecoder/wiki)
