# BUSCO II
The 6th step is running another BUSCO, but this time on the protein set based on the output of TransDecoder. We use this to determine the completeness of the set due to the fact we use the single best option.
Due to the setting, the completeness can be reduced, and this is just a quality check to determine if the sets are still usable for the following steps.

In my case, BUSCO was a module already installed on our High-Performance Cluster system. But you can also run it through a conda environment.

```
conda create -n busco -c conda-forge -c bioconda busco
```

Afterward, simply edit the [Busco_II_Proteins.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/06_BUSCO_II/Busco_II_Proteins.sh) script and run it.


For more information on BUSCO, please go to their website [BUSCO](https://busco.ezlab.org/)
