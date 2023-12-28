#Final Tree
This is the last step done for creating the final tree.

We first use Modelfinder with IQtree V1.6.12 to find the best fitting tree model, which resulted in the model LG+C60.
(See script Final_IQ_Tree_ModelFinder.sh)

Afterwards we run the intitial tree with this model with the script "Final_IQ_Tree_LG_C60.sh"
Finally a posterior mean site frequency (PMSF) method is used on the original tree with the script:
Final_IQ_Tree_PMSF.sh


To learn more about IQTree simply follow the link to https://www.iqtree.org/