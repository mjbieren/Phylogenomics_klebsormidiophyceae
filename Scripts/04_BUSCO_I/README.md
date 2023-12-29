# BUSCO I
The 4th step is running BUSCO to determine how complete the alignments are (which is a good indication if we can use the dataset or not)

In my case BUSCO was a module already installed on our High Performance Cluster system. But you can also run it through a conda environment.

```
conda create -n busco -c conda-forge -c bioconda busco
```

Afterward simply edit the [Busco_I.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/04_BUSCO_I/Busco_I.sh) script and run it.


For more information on BUSCO please go to their website [BUSCO](https://busco.ezlab.org/)
