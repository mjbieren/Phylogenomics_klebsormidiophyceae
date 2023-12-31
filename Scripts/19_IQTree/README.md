# Final Tree
This is the last step for creating the final tree.

We first used Modelfinder with IQtree V1.6.12 to find the best-fitting tree model, which resulted in the model LG+C60.<br/>
See script [Final_IQ_Tree_ModelFinder.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/19_IQTree/Final_IQ_Tree_ModelFinder.sh) <br/>

Afterward, we run the initial tree with this model with the script [Final_IQ_Tree_LG_C60.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/19_IQTree/Final_IQ_Tree_LG_C60.sh)<br/>

Finally, a posterior mean site frequency (PMSF) method is used on the original tree with the script: [Final_IQ_Tree_PMSF.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/19_IQTree/Final_IQ_Tree_PMSF.sh). <br/>

Look at the [Examples](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/tree/main/Scripts/19_IQTree/Examples) for a better overview

To learn more about IQTree, simply follow the link to https://www.iqtree.org/
