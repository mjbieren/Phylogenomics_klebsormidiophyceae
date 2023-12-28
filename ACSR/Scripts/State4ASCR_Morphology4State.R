library("phytools")

#Set your working directory
setwd("[ENTER_FULL_PATH_TO_WORKING_DIR]")

tree<-read.newick("New_TimeTree_For_ACSR_18Dec2023.tre")
plotTree(tree,fsize=0.9,ftype="i")

# read data (saving first column as row names) aka the table with your setup
x<-read.table("4States_Morphology.txt", row.names = 1)
# change this into a vector
x<-as.matrix(x)


cols<-setNames(palette()[1:length(unique(x))],sort(unique(x))) # set automatic colors

cols<-c("#FFBE0B", "#FB5607", "#8338EC", "#FF006E")
# match tips with states
tiplabels(pie=to.matrix(x,sort(unique(x))),piecol=cols,cex=0.2)

#Equal Rates
transitions <- matrix(c(0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0), nrow=4)
fitORDERED <- ace(x, tree, type="discrete", model=transitions)

#It is fairly straightforward to overlay these posterior probabilities on the tree:
plotTree(tree,fsize=0.8,ftype="i")

nodelabels(node=1:tree$Nnode+Ntip(tree),
           pie=fitORDERED$lik.anc,piecol=cols,cex=0.5)
tiplabels(pie=to.matrix(x,sort(unique(x))),piecol=cols,cex=0.2)

