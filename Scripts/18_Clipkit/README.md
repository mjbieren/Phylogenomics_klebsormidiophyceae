#ClipKIT
We use this step to remove the non-informative sides out of the concatenated alignment.

I run ClipKIT using the same conda environment in step 16 (Prequal). However you can create a separate conda environment with:

```
conda create -n clipkit -c conda-forge -c bioconda clipkit
```

Then run clipkit with the following command.
```
clipkit -m gappy -g 0.65 -o [OUTPUTFILE] [INPUTFILE]
```

Example:
```
clipkit -m gappy -g 0.65 -o New1_OldVsNewTax21_PPP3_Concatenated_Alignment_Phyx_g065.fas New1_OldVsNewTax21_PPP3_Concatenated_Alignment_Phyx.fas
```

To get more information about ClipKIT follow this link https://jlsteenwyk.com/ClipKIT/
