# Phylopypruner
This step is neccessary to remove all the paralogs from the orthogroup so that we only have one species per orthogroup.

Throughout the phylogenomic klebsormidiophyceae project we used different settings. Depending if we want more out or in groups.

1) [PhyloPruner_I_Conda_Gandalf_Tax10.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/13_Phylopypruner/PhyloPruner_I_Conda_Gandalf_Tax10.sh) was used for the original 420 loci set (or "old" set). Which was a set good for obtaining high aLRT branch support for out groups.
2) [PhyloPruner_I_Conda_Gandalf_CombinedSetTax21_New1.sh](https://github.com/mjbieren/Phylogenomics_klebsormidiophyceae/blob/main/Scripts/13_Phylopypruner/PhyloPruner_I_Conda_Gandalf_CombinedSetTax21_New1.sh) was used for the new set (Tax21, filtering happens after PPP) which was a good set for obtaining high aLRT branch support for in roups. Furthermore the same settings were used for the combined OldVsTax21Set (final Tree).

I run phylopypruner within a conda environment and a tmux session.

## Phylopypruner
PhyloPyPruner is a Python package for phylogenetic tree-based orthology inference, using the species overlap method and is developed by Dr. Felix Thalén.

To install and use Phylopypruner please follow the link https://github.com/fethalen/phylopypruner
