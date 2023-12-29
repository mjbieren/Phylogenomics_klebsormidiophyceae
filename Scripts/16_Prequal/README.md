# Create Prequal, ginsi, IQtree, clipkit file
This step creates a script per fasta file (Output Filter PPP Result).<br/>
Basically, for each Fasta file, prequal, ginsi, IQtree and clipkit are run.<br/>
To run it, edit line 3,4,5 in file [Create_PrequalScripts.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/16_Prequal/Create_PrequalScripts.sh)<br/>

Additionally, you have to create a conda environment OR have the programs installed by default.

For the conda environment, I used the command:

```
conda create -n Prequal -c conda-forge -c bioconda python mafft clipkit prequal
```

Either edit or remove the line (if you have the programs installed by default) at line 26.

The scripts then can be run with [RunAllScripts_FromAnyFolder.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/16_Prequal/RunAllScripts_FromAnyFolder.sh)

>[!TIP]
>Keep in mind that most job queue systems (like, in this case, SLURM) have a limitation on how many jobs can be run simultaneously. So keep that in mind when you run the scripts. I recommend creating a tool that does this automatically for you. Or run it in a SnakeMake setup.
