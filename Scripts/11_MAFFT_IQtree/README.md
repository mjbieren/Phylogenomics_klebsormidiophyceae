# Create Mafft and IQTree Scripts
This step creates mafft and iqtree scripts for each Orthogroup Fasta file with "create_mafft_iqtree_jobs_HPC.sh"
The scripts then can be run with "RunAllScripts_FromAnyFolder.sh"

>[!TIP]
>Keep in mind that most job queue systems (like in this case, slurm) have a limitation on how many jobs can be run simultaneously. So keep that in mind when you run the scripts. I recommend creating a tool that does this automatically for you. Or run it in a SnakeMake setup.
