# Concatenate alignments
We use this step to concatenate the different alignments into one big alignment file.
For that, we use phyx: phylogenetic tools for unix. 
Edit the [ConcatAlignments_FromAnyFolder_Phyx.sh](Scripts/17_ConcatenateSequences/ConcatAlignments_FromAnyFolder_Phyx.sh) script and just run it (either through a conda environment if phyx is not installed locally, or just locally).


I've created a conda environment to run phyx within a tmux session (for me it only took a minute)

```
conda create -n phyx -c conda-forge -c bioconda phyx
```

