# OrthoFinder
For the 9th step, we have to run OrthoFinder to get all the orthogroups from the complete dataset.
Keep in mind that besides your samples, you should also add enough outgroups.
Before you add the OutGroups, please reformat the headers with [simplify_headers_for_blastdb.py](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/07_Decontamination/simplify_headers_for_blastdb.py). 
simply use the script by the command:

```
python [script] [inputFile] [RenameHeaders]>>[FileOutput]
```


For a better OrthoFinder result, we have added a tree file as guidance (See [Complete_Klebsotree.txt](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/09_OrthoFinder/Complete_Klebsotree.txt))

I've used my collogue's (Armin Dadras) as a guide to install OrthoFinder on an HPC cluster (but it can also be done on a high-performance machine)<br/>
https://github.com/dadrasarmin/orthofinder_hpc_gwdg

Afterwards, simply edit the file [Orthofinder.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/09_OrthoFinder/Orthofinder.sh) and run it
