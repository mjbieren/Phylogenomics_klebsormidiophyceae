#include "Management.h"

long CManagement::ErrorChecker(long lError) const
{
	if (lError != ERR_NOERROR)
	{

#ifdef _WINDOWS
		printf_s("Error %i detected: %s \n", lError, GetErrorDescription(lError));
#elif _LINUX
		printf("Error %i detected: %s \r\n", lError, GetErrorDescription(lError));
#endif

		return lError;
	}
	else
		return ERR_NOERROR;

}

const char* CManagement::GetErrorDescription(long lError)const
{
	switch (lError)
	{
	case ERR_NOERROR:
		return "Error ERR_NOERROR, No Error Occured.";
	case PARSE_COMMAND_LINE:
		return "Error PARSE_COMMAND_LINE, could not parse the command line.";
	case PARSE_COMMAND_LINE_MAN:
		return "Please restart the program with the right parameters.";
	case DIRLIST_FAIL:
		return "Error DIRLIST_FAIL, failed to obtain files with the chosen directory.";
	case PARSECOMMANDLINE_COLUMNNUMBER:
		return "Error PARSECOMMANDLINE_COLUMNNUMBER, was not able to set the column number of the blast output file";
	case PARSECOMMANDLINE_BLASTEXTENSION:
		return "Error PARSECOMMANDLINE_BLASTEXTENSION, was not able to set the file extension of the blast output file";
	case PARSECOMMANDLINE_FASTAQUERY:
		return "Error PARSECOMMANDLINE_FASTAQUERY, was not able to set the path of the Query.fa file";
	case PARSECOMMANDLINE_GENESPATH:
		return "Error PARSECOMMANDLINE_GENESPATH, was not able to set the genes.txt file";
	case PARSECOMMANDLINE_BLASTFILE:
		return "Error PARSECOMMANDLINE_BLASTFILE, was not able to set the path of the blast file.";
	case PARSECOMMANDLINE_OUTPUTPATH:
		return "Error PARSECOMMANDLINE_OUTPUTPATH, Was not able to set the output folder.";
	case PARSECOMMANDLINE_STRAINNAME:
		return "Error PARSECOMMANDLINE_STRAINNAME, was not able to set the strain name.";
	case PARSECOMMANDLINE_DECONTAMINATIONLIST:
		return "Error PARSECOMMANDLINE_DECONTAMINATIONLIST, was not able to set the Decontamination list.";
	case PARSECOMMANDLINE_STRICTDECONTAMINATION:
		return "Error PARSECOMMANDLINE_STRICTDECONTAMINATION, was not able to set the decontamination filter to strict.";
	case PARSECOMMANDLINE_FASTADIR_OSG: //Command Line OSG
		return "Error PARSECOMMANDLINE_FASTADIR_OSG, was not able to set the path to the fasta files for OSG. Do you have permission?";
	case PARSECOMMANDLINE_ORTHOGROUPDIR_OSG:
		return "Error PARSECOMMANDLINE_ORTHOGROUPDIR_OSG, was not able to set the path to the Orthogroup files in tsv format, Do you have permission?";
	case PARSECOMMANDLINE_OUTPUTDIR_OSG:
		return "Error PARSECOMMANDLINE_OUTPUTDIR_OSG, was not able to set the path to the Output path.";
	case PARSECOMMANDLINE_TAXONOMICGROUPFILE_OSG:
		return "Error PARSECOMMANDLINE_TAXONOMICGROUPFILE_OSG. Was not able to set the full path to the Taxonomic Group file.";
	case PARSECOMMANDLINE_TAXONOMICTHRESHOLD_OSG:
		return "Error PARSECOMMANDLINE_TAXONOMICTHRESHOLD_OSG, was not able to set the threshold value for the taxonomic groups. Be sure it's a value lower than 9,223,372,036,854,775,807, without the commas";
	case PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_OSG:
		return "Error PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_OSG you can not do the filtering step with a negative number or 0. The minimum value is '1' for this program";
	case PARSECOMMANDLINE_PHYLOPYPRUNERFORMAT_OSG:
		return "Error PARSECOMMANDLINE_PHYLOPYPRUNERFORMAT_OSG, could not set the program to format the output to PhyloPyPruner aka SpeciesName@GeneID e.g. Umutabilis@1";
	case PARSECOMMANDLINE_SUMMARYPATH_OSG:
		return "Error PARSECOMMANDLINE_SUMMARYPATH_OSG, Could not set the path for the summary file.";
	case PARSECOMMANDLINE_BOOLSUMMARY_OSG:
		return "Error PARSECOMMANDLINE_BOOLSUMMARY_OSG, could not set if the user want a summary file or not.";
	case PARSECOMMANDLINE_BOOLSTRAINONLY_SCPT:
		return "Error PARSECOMMANDLINE_BOOLSTRAINONLY_SCPT, could not set if the user have only strains in their headers or not.";
	case PARSECOMMANDLINE_TREEFORMAT_APPPF:
		return "Error PARSECOMMANDLINE_TREEFORMAT_APPPF, could not set the file extension for the tree Files.";
	case PARSECOMMANDLINE_TREEDIR_APPPF:
		return "Error PARSECOMMANDLINE_TREEDIR_APPPF, could not set the path to the tree files.";
	case PARSECOMMANDLINE_TAXONOMICGROUPFILE_APPPF:
		return "Error PARSECOMMANDLINE_TAXONOMICGROUPFILE_APPPF, could not set the path to the taxonomic group file.";
	case PARSECOMMANDLINE_OUTPUTFOLDER_APPPF:
		return "Error PARSECOMMANDLINE_OUTPUTFOLDER_APPPF, could not set the path to the output folder.";
	case PARSECOMMANDLINE_BOOLMOVEMAFFTFILE_APPPF:
		return "Error PARSECOMMANDLINE_BOOLMOVEMAFFTFILE_APPPF, could not set the bool if Mafft files have to be moved.";
	case PARSECOMMANDLINE_MOVEMAFFTFIELPATH_APPPF:
		return "Error PARSECOMMANDLINE_MOVEMAFFTFIELPATH_APPPF, could not set the path to the directory where the mafft files are located.";
		case PARSECOMMANDLINE_FASTADIR_FPPPR: //Command Line FPPPR
		return "Error PARSECOMMANDLINE_FASTADIR_FPPPR, was not able to set the path to the fasta files for FPPPR. Do you have permission?";
	case PARSECOMMANDLINE_OUTPUTDIR_FPPPR:
		return "Error PARSECOMMANDLINE_OUTPUTDIR_FPPPR, was not able to set the path to the Output path.";
	case PARSECOMMANDLINE_TAXONOMICGROUPFILE_FPPPR:
		return "Error PARSECOMMANDLINE_TAXONOMICGROUPFILE_FPPPR. Was not able to set the full path to the Taxonomic Group file.";
	case PARSECOMMANDLINE_TAXONOMICTHRESHOLD_FPPPR:
		return "Error PARSECOMMANDLINE_TAXONOMICTHRESHOLD_FPPPR, was not able to set the threshold value for the taxonomic groups. Be sure it's a value lower than 9,223,372,036,854,775,807, without the commas";
	case PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_FPPPR:
		return "Error PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_FPPPR you can not do the filtering step with a negative number or 0. The minimum value is '1' for this program";
	case PARSECOMMANDLINE_SUMMARYPATH_FPPPR:
		return "Error PARSECOMMANDLINE_SUMMARYPATH_FPPPR, Could not set the path for the summary file.";
	case PARSECOMMANDLINE_BOOLSUMMARY_FPPPR:
		return "Error PARSECOMMANDLINE_BOOLSUMMARY_FPPPR, could not set if the user want a summary file or not.";
	case PARSECOMMANDLINE_BOOLNOGAP_FPPPR:
		return "Error PARSECOMMANDLINE_BOOLNOGAP_FPPPR, could not set if hte user wants the gaps removed out of the sequences or not.";
	case PARSECOMMANDLINE_FASTADIR_RFBS:
		return "Error PARSECOMMANDLINE_FASTADIR_RFBS, could not set the path to the Fasta files.";
	case PARSECOMMANDLINE_BOOLNOHEADER_FPPPR:
		return "Error PARSECOMMANDLINE_BOOLNOHEADER_FPPPR, could not set the if the user want to have full Gene IDS in the file or not.";
	case PARSECOMMANDLINE_HEADERFILE_RFBS:
		return "Error PARSECOMMANDLINE_HEADERFILE_RFBS, could not set the path to the header file.";
	case PARSECOMMANDLINE_OUTPUTDIR_RFBS:
		return "Error PARSECOMMANDLINE_OUTPUTDIR_RFBS, could not set the path to the output directory.";
	case PARSECOMMANDLINE_BOOLPHYLOPYPRUNERFORMAT_RFBS:
		return "Error PARSECOMMANDLINE_BOOLPHYLOPYPRUNERFORMAT_RFBS, could not set if the user want to reformat the fasta files to the PhylopyPrunerFormat or not.";
	case PARSECOMMANDLINE_INPUTFOLDER_RASFAF: //RASFAF
		return "Error PARSECOMMANDLINE_INPUTFOLDER_RASFAF, could not set the input folder where the scripts are located";
	case PARSECOMMANDLINE_OUTPUTFOLDER_RASFAF:
		return "Error PARSECOMMANDLINE_OUTPUTFOLDER_RASFAF, could not set the output folder where the scripts are located.";
	case PARSECOMMANDLINE_JOBNUMBER_TOO_LOW_RASFAF:
		return "Error PARSECOMMANDLINE_JOBNUMBER_TOO_LOW, the job limitation must be at least 1.";
	case PARSECOMMANDLINE_JOBNUMBER_INCORRECT_RASFAF:
		return "Error PARSECOMMANDLINE_JOBNUMBER_INCORRECT, the job limitation number is incorrect.";
	case PARSECOMMANDLINE_JOBWAITINGTIME_TOO_LOW_RASFAF:
		return "Error PARSECOMMANDLINE_JOBWAITINGTIME_TOO_LOW_RASFAF, the waiting time between jobs must be at least 1.";
	case PARSECOMMANDLINE_JOBWAITINGTIME_INCORRECT_RASFAF:
		return "Error PARSECOMMANDLINE_JOBWAITINGTIME_INCORRECT_RASFAF, the waiting time between jobs is invorrect.";
	case PARSECOMMANDLINE_BOOLISSLURM_RASFAF:
		return "Error PARSECOMMANDLINE_BOOLISSLURM_RASFAF, could not set if the scripts should be parsed as a slurm job or locally.";
	case PARSECOMMANDLINE_BOOLISSINGLECORE_RASFAF:
		return "Error PARSECOMMANDLINE_BOOLISSINGLECORE_RASFAF, could not set if the program should be running in single or multi core.";
	case PARSECOMMANDLINE_JOBLIMIT_RASFAF:
		return "Error PARSECOMMANDLINE_JOBLIMIT_RASFAF, could not set the time limitation for a job.";
	case PARSECOMMANDLINE_FASTAFILE_ONE_COGS:
		return "Error PARSECOMMANDLINE_FASTAFILE_ONE_COGS, could not set the First Fasta File set Path.";
	case PARSECOMMANDLINE_FASTAFILE_TWO_COGS:
		return "Error PARSECOMMANDLINE_FASTAFILE_TWO_COGS, could not set the Second Fasta File set Path.";
	case PARSECOMMANDLINE_FASTAFILE_BASE_COGS:
		return "Error PARSECOMMANDLINE_FASTAFILE_BASE_COGS, could not set the base fasta file set Path.";
	case PARSECOMMANDLINE_ORTHOGROUPDIR_COGS:
		return "Error PARSECOMMANDLINE_ORTHOGROUPDIR_COGS, could not set the OrthoGroup dir Path.";
	case PARSECOMMANDLINE_OUTPUTDIR_COGS:
		return "Error PARSECOMMANDLINE_OUTPUTDIR_COGS, could not set the output dir path.";
	case PARSECOMMANDLINE_PHYLOPYPRUNER_FORMAT_COGS:
		return "Error PARSECOMMANDLINE_PHYLOPYPRUNER_FORMAT_COGS, could not set if the user want to set the output in phylopypruner format.";
	case PROPERTYBAG_GETCOLUMNNUMBER:
		return "Error PROPERTYBAG_GETCOLUMNNUMBER, was not able to obtain the column number from the propertybag";
	case PROPERTYBAG_GETBLASTEXTENSION:
		return "Error PROPERTYBAG_GETBLASTEXTENSION, was not able to obtain the right file extension from the propertybag";
	case PROPERTYBAG_GETFASTAQUERY:
		return "Error PROPERTYBAG_GETFASTAQUERY, was not able to get the path of the Query.fa file.";
	case PROPERTYBAG_GETGENESPATH:
		return "Error PROPERTYBAG_GETGENESPATH, was not able to get the genes.txt path";
	case PROPERTYBAG_GETBLASTFILE:
		return "Error PROPERTYBAG_GETBLASTFILE, was not able to get the path of the blast output file";
	case PROPERTYBAG_GETOUTPUTPATH:
		return "Error PROPERTYBAG_GETOUTPUTPATH, was not able to get the path of the output folder";
	case PROPERTYBAG_GETSTRICTDECONTAMINATION:
		return "Error PROPERTYBAG_GETSTRICTDECONTAMINATION, was not able to get the boolean to set the decontamination filter to strict or not.";
	case PROPERTYBAG_GETFASTADIR_OSG: // PropertyBag OSG
		return "Error PROPERTYBAG_GETFASTADIR_OSG, could not obtain the path to the fasta files, are you sure it was set as a parameter?";
	case PROPERTYBAG_ORTHOGROUPDIR_OSG:
		return "Error PROPERTYBAG_ORTHOGROUPDIR_OSG, Could not set the path to the orthogroup files. Are you sure it was set?";
	case PROPERTYBAG_OUTPUTDIR_OSG:
		return "Error PROPERTYBAG_OUTPUTDIR_OSG, could not obtain the path to the output directory. Are you sure it was set?";
	case PROPERTYBAG_TAXONOMICGROUPFILE_OSG:
		return "Error PROPERTYBAG_TAXONOMICGROUPFILE_OSG, could not obtain the path to the Taxonomic Group file. Are you sure it was set?";
	case PROPERTYBAG_TAXONOMICTHRESHOLD_OSG:
		return "Error PROPERTYBAG_TAXONOMICTHRESHOLD, could not obtain the Taxonomic group threshold. Are you sure it was set?";
	case PROPERTYBAG_PHYLOPYPRUNERFORMAT_OSG:
		return "Error PROPERTYBAG_PHYLOPYPRUNERFORMAT_OSG, could not obtain if the user want to set the output files with or without the Phylopypruner format.";
	case PROPERTYBAG_SETTAXONOMICGROUPSIZE_VECTOR_OSG:
		return "Error PROPERTYBAG_SETTAXONOMICGROUPSIZE_VECTOR, could not set the size of the taxonomic group vector object. Size incorrect.";
	case PROPERTYBAG_GETTAXONOMICGROUPSIZE_VECTOR_OSG:
		return "Error PROPERTYBAG_GETTAXONOMICGROUPSIZE_VECTOR, could not obtain the size of the taxonomic group vector object. size incorrect.";
	case PROPERTYBAG_GET_SUMMARYPATH_OSG:
		return "Error PROPERTYBAG_GET_SUMMARYPATH_OSG, Could not obtain the path to the directory the user want to write the Summary file to.";
	case PROPERTYBAG_GET_BOOLSUMMARY_OSG:
		return "Error PROPERTYBAG_GET_BOOLSUMMARY, Could not obtain the boolean if the user want to write an output file or not.";
	case PROPERTYBAG_GETBOOLSTRAINONLY_SCPT:
		return "Error PROPERTYBAG_GET_BOOLSTRAINONLY_SCPT, could not obtain the if the user have strain only or not.";
	case PROPERTYBAG_GETTREEFORMAT_APPPF: //PropertyBag APPPF
		return "Error PROPERTYBAG_GETTREEFORMAT_APPPF, could not obtain the tree format from the propertybag.";
	case PROPERTYBAG_SETTREEFORMAT_APPPF:
		return "Error PROPERTYBAG_SETTREEFORMAT_APPPF, could not set the Tree format through the propertybag.";
	case PROPERTYBAG_GETTREEDIRPATH_APPPF:
		return "Error PROPERTYBAG_GETTREEDIRPATH_APPPF, could not obain the path from the propertybag to the directory where the trees are located.";
	case PROPERTYBAG_GETTAXONOMICGROUPFILE_APPPF:
		return "Error PROPERTYBAG_GETTAXONOMICGROUPFILE_APPPF, could not obtain the Taxonomic Group File path from the propertybag.";
	case PROPERTYBAG_GETOUTPUTDIR_APPPF:
		return "Error PROPERTYBAG_GETOUTPUTDIR_APPPF, could not obtain the output directory from the propertybag.";
	case PROPERTYBAG_GET_BOOLMOVEMAFFTFILE_APPPF:
		return "Error PROPERTYBAG_GET_BOOLMOVEMAFFTFILE_APPPF, could not obtain the boolean, if the user wants to move the MSA (Mafft) files.";
	case PROPERTYBAG_GETMOVEMAFFTFILEPATH_APPPF:
		return "ERror PROPERTYBAG_GETMOVEMAFFTFILEPATH_APPPF, could not obtain the path to the directory to the MSA (MAFFT) files.";
	case PROPERTYBAG_GETFASTADIR_FPPPR: // PropertyBag FPPPR
		return "Error PROPERTYBAG_GETFASTADIR_FPPPR, could not obtain the path to the fasta files, are you sure it was set as a parameter?";
	case PROPERTYBAG_OUTPUTDIR_FPPPR:
		return "Error PROPERTYBAG_OUTPUTDIR_FPPPR, could not obtain the path to the output directory. Are you sure it was set?";
	case PROPERTYBAG_TAXONOMICGROUPFILE_FPPPR:
		return "Error PROPERTYBAG_TAXONOMICGROUPFILE_FPPPR, could not obtain the path to the Taxonomic Group file. Are you sure it was set?";
	case PROPERTYBAG_TAXONOMICTHRESHOLD_FPPPR:
		return "Error PROPERTYBAG_TAXONOMICTHRESHOLD, could not obtain the Taxonomic group threshold. Are you sure it was set?";
	case PROPERTYBAG_SETTAXONOMICGROUPSIZE_VECTOR_FPPPR:
		return "Error PROPERTYBAG_SETTAXONOMICGROUPSIZE_VECTOR, could not set the size of the taxonomic group vector object. Size incorrect.";
	case PROPERTYBAG_GETTAXONOMICGROUPSIZE_VECTOR_FPPPR:
		return "Error PROPERTYBAG_GETTAXONOMICGROUPSIZE_VECTOR, could not obtain the size of the taxonomic group vector object. size incorrect.";
	case PROPERTYBAG_GET_SUMMARYPATH_FPPPR:
		return "Error PROPERTYBAG_GET_SUMMARYPATH_FPPPR, Could not obtain the path to the directory the user want to write the Summary file to.";
	case PROPERTYBAG_GET_BOOLSUMMARY_FPPPR:
		return "Error PROPERTYBAG_GET_BOOLSUMMARY_FPPPR, Could not obtain the boolean if the user want to write an output file or not.";
	case PROPERTYBAG_GET_BOOLNOHEADER_FPPPR:
		return "Error PROPERTYBAG_GET_BOOLNOHEADER_FPPPR, could not obtain the boolean if the user want to have gene ids in the fasta files or not.";
	case PROPERTYBAG_GET_BOOLNOGAPS_FPPPR:
		return "Error PROPERTYBAG_GET_BOOLNOGAPS_FPPPR, could not obtain the boolean if the user want to have the gaps removed out of the sequences or not.";
	case PROPERTYBAG_GETFASTADIR_RFBS:
		return "Error PROPERTYBAG_GETFASTADIR_RFBS, could not obtain the directory where the fasta files are located.";
	case PROPERTYBAG_GETHEADERFILE_RFBS:
		return "Error PROPERTYBAG_GETHEADERFILE_RFBS, could not obtain the path to the Header file.";
	case PROPERTYBAG_GETOUTPUTDIR_RFBS:
		return "Error PROPERTYBAG_GETOUTPUTDIR_RFBS, could not obtani the path to the output directory.";
	case PROPERTYBAG_GETBOOLPHYLOPYPRUNERFORMAT_RFBS:
		return "Error PROPERTYBAG_GETBOOLPHYLOPYPRUNERFORMAT_RFBS, could not obtain if the bool, if the user want to reformat the fasta files into the Phylopypruner format.";
	case PROPERTYBAG_GETINPUTFOLDERSCRIPT_RASFAF: //Propertybag RASFAF
		return "ERROR PROPERTYBAG_GETINPUTFOLDERSCRIPT_RASFAF, could not get the Input folder from the propertybag.";
	case PROPERTYBAG_GETOUTPUTFOLDERSCRIPT_RASFAF:
		return "ERROR PROPERTYBAG_GETOUTPUTFOLDERSCRIPT_RASFAF, could not get the output folder from the propertybag.";
	case PROPERTYBAG_GETJOBLIMITNUMBER_RASFAF:
		return "ERROR PROPERTYBAG_GETJOBLIMITNUMBER_RASFAF, could not get the job limit number from the propertybag.";
	case PROPERTYBAG_GETJOBWAITING_TIME_RASFAF:
		return "Error PROPERTYBAG_GETJOBWAITING_TIME, could not get the job waiting time from the propertybag.";
	case PROPERTYBAG_GETBOOLSLURM_RASFAF:
		return "Error PROPERTYBAG_GETBOOLSLURM_RASFAF, could not get if the system command should be on the slurm system or local.";
	case PROPERTYBAG_GETBOOLSINGLECORE_RASFAF:
		return "Error PROPERTYBAG_GETBOOLSINGLECORE_RASFAF, could not get if you want to run it in single or multi core use.";
	case PROPERTYBAG_GETJOBTIMELIMIT_RASFAF:
		return "Error PROPERTYBAG_GETJOBTIMELIMIT_RASFAF, could not get the time job limitation.";
	case PROPERTYBAG_FASTAFILE_ONE_COGS:
		return "Error PROPERTYBAG_FASTAFILE_ONE_COGS, could not get the path for the first fasta file set.";
	case PROPERTYBAG_FASTAFILE_TWO_COGS:
		return "Error PROPERTYBAG_FASTAFILE_TWO_COGS, could not get the path for the second fasta file set.";
	case PROPERTYBAG_FASTAFILE_BASE_COGS:
		return "Error PROPERTYBAG_FASTAFILE_BASE_COGS, could not get the path for the base fasta files.";
	case PROPERTYBAG_ORTHOGROUPDIR_COGS:
		return "Error PROPERTYBAG_ORTHOGROUPDIR_COGS, could not get the path for the orthogroups.";
	case PROPERTYBAG_OUTPUTDIR_COGS:
		return "Error PROPERTYBAG_OUTPUTDIR_COGS, could not get the output dir path.";
	case PROPERTYBAG_PHYLOPYPRUNER_FORMAT_COGS:
		return "Error PROPERTYBAG_PHYLOPYPRUNER_FORMAT_COGS, could not get if the user wants the output in phylopypruner format or not.";
	case FASTA_PUSBACKBLOCK_ADD_VECTOR:
		return "Error FASTA_PUSBACKBLOCK, could not add the Fasta Block to the FastaFile Vector, since the object is empty.";
	case FASTABLOCK_SETBLOCK_NOFASTAFORMAT:
		return "Error FASTABLOCK_SETBLOCK_NOFASTAFORMAT, this Fastablock does not have a fasta format, which should start with a '>', check your fasta file again.";
	case FASTABLOCK_SETFASTAHEADER:
		return "Error FASTABLOCK_SETFASTAHEADER, Could not set the fasta header in the fasta block.";
	case FASTABLOCK_SETFASTAHEADER_POINTERDIFFERENCE:
		return "Error FASTABLOCK_SETFASTAHEADER_POINTERDIFFERENCE, the difference between the pointers is not correct. The fasta file is not correctly formatted.";
	case FASTABLOCK_SETFASTAHEADERSTRAIN_POINTERDIFFERENCE:
		return "Error FASTABLOCK_SETFASTAHEADERSTRAIN_POINTERDIFFERENCE, the difference beween the pointers is not correct. THe fasta file is not correclty formatted.";
	case FASTABLOCK_SETHEADERSTRAIN_NODATA:
		return "Error FASTABLOCK_SETHEADERSTRAIN_NODATA, the input object to set the Header Strain is empty.";
	case FASTABLOCK_SETFASTAHREST_POITNERDIFFERENCE:
		return "Error FASTABLOCK_SETFASTAHREST_POITNERDIFFERENCE, the difference between the pointers is not the correct. the fasta file is not correctly formatted.";
	case FASTABLOCK_SETHEADERREST_NODATA:
		return "Error FASTABLOCK_SETHEADERREST_NODATA, the input object to set the Rest part of the header is empty.";
	case FASTABLOCK_GETFASTAHEADER:
		return "Error FASTABLOCK_GETFASTAHEADER, could not get the fasta sequence from the fasta block object.";
	case FASTABLOCK_SETFASTASEQUENCE:
		return "Error FASTABLOCK_SETFASTASEQUENCE, Could not set the fasta sequence in the fasta block.";
	case FASTABLOCK_SETFASTASEQUENCE_POINTERDIFFERENCE:
		return "Error FASTABLOCK_SETFASTASEQUENCE_POINTERDIFFERENCE, the difference between the pointers is not correct. The fasta file is not correctly formatted.";
	case FASTABLOCK_GETFASTASEQUENCE:
		return "Error FASTABLOCK_GETFASTASEQUENCE, could not get the fasta sequence from the fasta block object.";
	case FASTABLOCK_GETHEADERNUMBER:
		return "Error FASTABLOCK_GETHEADERNUMBER, could not get the header number, invalid number.";
	case FASTABLOCK_GETHEADERSTRAIN:
		return "Error FASTABLOCK_GETHEADERSTRAIN, could not get the header strain, invalid header strain object.";
	case FASTABLOCK_GETHEADERREST:
		return "Error FASTABLOCK_GETHEADERREST, could not get the rest of the header, invalid Header rest object.";
	case FASTABLOCK_SETHEADERNUMBER:
		return "Error FASTABLOCK_SETHEADERNUMBER, could not set the header number, invalid number.";
	case FASTABLOCK_GETWRITEFASTABLOCKBOOL:
		return "Error FASTABLOCK_GETWRITEFASTABLOCKBOOL, could not obtain if the FastaBlock has to be written or not.";
	case FASTABLOCK_SETWRITEFASTABLOCKBOOL:
		return "Error FASTABLOCK_SETWRITEFASTABLOCKBOOL, could not define if the setting value for m_bFastaBlockWrite has to be set to true or false.";
	case FASTAFILE_GETFASTAFILEBUFFER:
		return "Error FASTAFILE_GETFASTAFILEBUFFER, could not set file buffer.";
	case FASTAFILE_WRONGFORMAT:
		return "Error FASTAFILE_WRONGFORMAT, Fasta file has the wrong format, doesn't start with >.";
	case FASTAFILE_PARSEFASTABLOCK_WRONGFORMAT:
		return "Error FASTAFILE_PARSEFASTABLOCK_WRONGFORMAT, the fasta block does not have the right format. Are you sure it's a Fasta file starting with '>'.";
	case FASTAFILE_SETFASTAFILENAME_POINTERDIFFERENCE:
		return "Error FASTAFILE_SETFASTAFILENAME_POINTERDIFFERENCE, the difference between the pointers is not correct. The gene file is not correct.";
	case FASTAFILE_GETFASTAFILENAME:
		return "Error FASTAFILE_GETFASTAFILENAME,  could not get the fasta file name from the fasta file object.";
	case FASTAFILE_EMPTY_STRING_FASTAFILENAME:
		return "Error FASTAFILE_EMPTY_STRING_FASTAFILENAME, tried to set the fasta file name, but the string object is empty, does the blast file has entries on column 2?";
	case FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND:
		return "Error FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND, could not find the fasta block in the fasta file. Are you sure that the name is exactly matching and exist?";
	case FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND_BASED_ON_HEADERNUMBER:
		return "Error FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND_BASED_ON_HEADERNUMBER, could not find the fasta block in the fasta file. This is based on the header numbers in a simplified header format.";
	case FASTAFILE_SETMATCHINGNAME_POINTERDIFFERENCE:
		return "Error FASTAFILE_SETMATCHINGNAME_POINTERDIFFERENCE, the difference between the pointers is not correct. The matching name is not correct.";
	case FASTAFILE_EMPTY_STRING_MATCHINGNAME:
		return "Error FASTAFILE_EMPTY_STRING_MATCHINGNAME, tried to set the fasta file name, but the string object is empty, does the blast file has entries on column 2?";
	case FASTAFILE_GETMATCHINGNAME:
		return "Error FASTAFILE_GETMATCHINGNAME,  could not get the matching file name from the fasta file object.";
	case TABLEROW_SETCOLUMNNAME_HEADER:
		return "Error TABLEROW_SETCOLUMNNAME_HEADER, The header column name does not have the right format.";
	case TABLEROW_SETCOLUMNNAMENEW_HEADER:
		return "Error TABLEROW_SETCOLUMNNAMENEW_HEADER.The header column name to replace the old header name does not have the right format.";
	case TABLEROW_SETCOLUMNNAMENEW_HEADER_POINTER:
		return "Error TABLEROW_SETCOLUMNNAMENEW_HEADER_POINTER, The new header name object parameter does not have any content.";
	case TABLEROW_GETCOLUMNNAME_HEADER:
		return "Error TABLEROW_GETCOLUMNNAME_HEADER, the column name object does not have any content";
	case TABLEROW_GETCOLUMNNAMENEW_HEADER:
		return "Error TABLEROW_GETCOLUMNNAMENEW_HEADER, the column new name that repalces the old column does not have any content.";
	case TABLE_NOTMATCHING_COLUMNNUMBER:
		return "Error ERROR_NOTMATCHING_COLUMNNUMBER, the amount of columns are not matching to one another within this table.";
	case TABLE_NOMATCH_HEADERCOLUMN:
		return "Error TABLE_NOMATCH_HEADERCOLUMN, could not find the column within the table. ";
	case TABLE_NEWNAME_SIZE_FORMAT_WRONG:
		return "Error TABLE_NEWNAME_SIZE_FORMAT_WRONG, could not determine the table name, format wrong.";
	case TABLE_PARSE_ROW_SIZE_TOO_SMALL:
		return "Error TABLE_PARSE_ROW_SIZE_TOO_SMALL, the row for creating this buffer is too small";
	case TABLE_PARSE_ROW_SIZE_TOO_BIG:
		return "Error TABLE_PARSE_ROW_SIZE_TOO_BIG, the row for creating this buffer is too big. This table file probably doesn't have EOL's";
	case TABLE_NEWCOLUMN_SIZE_FORMAT_WRONG:
		return "Error TABLE_NEWCOLUMN_SIZE_FORMAT_WRONG, the file positions to set a new column into the table object is the wrong size.";
	case TABLE_PARSE_ROW_SIZE_FORMAT_WRONG:
		return "Error TABLE_PARSE_ROW_SIZE_FORMAT_WRONG, the row that's trying to be parsed has the wrong format (wrong size)";
	case TABLE_SIZE_HEADER_COLUMN_WRONG:
		return "Error TABLE_SIZE_HEADER_COLUMN_WRONG, This row does not have the right amount of columns, are you sure this table has the right format?";
	case TABLE_FIND_MATCHING_ROW_NAME_DOES_NOT_EXIST:
		return "Error TABLE_FIND_MATCHING_ROW_NAME_DOES_NOT_EXIST, the row you try to find does not exist";
	case TABLE_ROW_MATCHING_NOT_FOUND:
		return "Error TABLE_ROW_MATCHING_NOT_FOUND, Row is not found";
	case TABLE_PARSE_HEADER_FORMAT_WRONG:
		return "Error TABLE_PARSE_HEADER_FORMAT_WRONG, the header row you try to find does not exist";
	case TABLE_HEADER_VECTOR_AND_COLUMN_COUNT_NOT_MATCHING:
		return "Error TABLE_HEADER_VECTOR_AND_COLUMN_COUNT_NOT_MATCHING, the amount of columns does not match with the amount of header columns just added into the Table object. Is it the right format?";
	case TABLE_PARSE_TABLE_FORMAT_WRONG_NO_EOL:
		return "Error TABLE_PARSE_TABLE_FORMAT_WRONG_NO_EOL, the table you try to parse does not have EOLs";
	case TABLE_CREATE_FILE_BUFFER:
		return "Error TABLE_CREATE_FILE_BUFFER, the file buffer for this table could not be created are you sure the file has a content?";
	case TABLE_SET_TABLE_ROW_HEADER:
		return "Error TABLE_SET_TABLE_ROW_HEADER, could not set the table row header.";
	case TABLE_GET_TABLE_ROW_HEADER:
		return "Error TABLE_GET_TABLE_ROW_HEADER, could not get the table row header.";
	case TABLE_NO_TABLE_CONTENT:
		return "Error TABLE_NO_TABLE_CONTENT, this table does not have any content. Just the Header";
	case TABLE_GET_TABLE_NAME:
		return "Error TABLE_GET_TABLE_NAME, could not get the table name.";
	case TABLE_PUSHBACK_ROW_SAFE:
		return "Error TABLE_PUSHBACK_ROW_SAFE, could not push back the row into the table.";
	case TABLE_PUSHBACK_NO_ROWS:
		return "Error TABLE_PUSHBACK_NO_ROWS, there are no rows within this blast output file.";
	case TABLE_POPBACK_NO_ROWS:
		return "ERROR TABLE_POPBACK_NO_ROWS, there are no rows within this table to pop back.";
	case TAXONOMIC_GROUP_FIND_SPECIES_SPECIESNAME:
		return "Error TAXONOMIC_GROUP_FIND_SPECIES_SPECIESNAME, could not find the Taxonomic Species from the taxonomic group object. Does the Taxonomic Group file names matches the fasta file names?";
	case TAXONOMIC_GROUP_FIND_SPECIES_HEADERNAME:
		return "Error TAXONOMIC_GROUP_FIND_SPECIES_HEADERNAME, could not find the Taxonomic Species from the taxonomic group object. Does the Taxonomic group file contain the missing species?";
	case JOB_SETINPUTFILE_STRING_RASFAF:
		return "Error ERROR_SETINPUTFILE_STRING_RASFAF, could not set the input file path based on whole strings.";
	case JOB_SETINPUTFILE_POSITION_RASFAF:
		return "Error ERROR_SETINPUTFILE_POSITION_RASFAF, could not set hte input file path based on positions.";
	case JOB_GETINPUTFILE_RASFAF:
		return "Error ERROR_GETINPUTFILE_RASFAF, could not get the full input file path, probably not filled.";
	case JOB_SETOUTPUTFILE_STRING_RASFAF:
		return "Error ERROR_SETOUTPUTFILE_STRING_RASFAF, could not set the output file path based on whole strings.";
	case JOB_SETOUTPUTFILE_POSITION_RASFAF:
		return "Error ERROR_SETOUTPUTFILE_POSITION_RASFAF, could not set the output file path based on positions.";
	case JOB_GETOUTPUTFILE_RASFAF:
		return "Error ERROR_GETOUTPUTFILE_RASFAF, could not get the output file path.";
	case JOB_SETSYSTEMCOMMAND_STRING_RASFAF:
		return "Error ERROR_SETSYSTEMCOMMAND_STRING_RASFAF, could not set the command line command based on whole strings.";
	case JOB_SETSYSTEMCOMMAND_POSITION_RASFAF:
		return "Error ERROR_SETSYSTEMCOMMAND_POSITION_RASFAF, could not set the command line command based on positions.";
	case JOB_GETSYSTEMCOMMAND_RASFAF:
		return "Error ERROR_GETSYSTEMCOMMAND_RASFAF, could not get the system command.";
	case JOB_FINDEMPTYSPOT_FORNEWJOB_RASFAF:
		return "Error JOB_FINDEMPTYSPOT_FORNEWJOB_RASFAF, could not find the a spot in the Running job vector.";
	case PARSEINPUT_FILE_BLAST_NO_ROWS:
		return "Error PARSEINPUT_FILE_BLAST_NO_ROWS, the blast output file does not have any rows for content.";
	case PARSEINPUT_FILE_BLAST_NO_TABLE:
		return "Error PARSEINPUT_FILE_BLAST_NO_TABLE, there is no Blast file in the Blast Vector.";
	case PARSEINPUT_FILE_BLAST_NO_TABLENAME:
		return "Error PARSEINPUT_FILE_BLAST_NO_TABLENAME, there is no path in the blast file.";
	case PARSEINPUT_FILE_SPLIT_FASTA_FILES_NO_FASTABLOCKS:
		return "Error PARSEINPUT_FILE_SPLIT_FASTA_FILES_NO_FASTABLOCKS, the original fasta file does not have any fasta sequences (blocks) in it.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME: //Own ones - input function / OSG
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME, could not parse the species name in the Taxonomic Species object.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME_SIZETOSMALL:
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME_SIZETOSMALL, could not set the species in the taxonomic species object, because the size of the name is incorrect.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_SPECIESNAME:
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_SPECIESNAME, could not obtain the species name in the taxonomic species object, since the species name object does not exist within the taxonomic species object.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME:
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME, could not set the header name witin the taxonomic species object.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME_SIZETOSMALL:
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME_SIZETOSMALL, could not set the header name within the taxonimic species object, because the size of the name is incorrect.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_HEADERNAME:
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_HEADERNAME, could not obtain the header name in the taxonomic species object, since the Header Name object does not exist within the Taxonomic Species object.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICGROUP_PUSHBACK_TAXONOMIC_SPECIES:
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICGROUP_PUSHBACK_TAXONOMIC_SPECIES, could not push back a taxonimic species object within the taxonomic group object since there is no Taxonomic Species object to be pushed back.";
	case PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT:
		return "Error PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT, Taxonomic group file has the wrong format. The format should be 'SpeciesName1(HeaderName1),SpeciesName2(HeaderName2),etc,SpeciesNameN(HeaderNameN) ";
	case PARSE_INPUT_FILE_HEADERFILE_SETFILEBUFFER_RFBS: //Own ones - input function / RFBS
		return "Error PARSE_INPUT_FILE_HEADERFILE_SETFILEBUFFER, could not set the file buffer for the Header File.";
	case PARSE_INPUT_FILE_HEADERFILE_WRONG_FORMAT_RFBS:
		return "Error PARSE_INPUT_FILE_HEADERFILE_WRONG_FORMAT_RFBS, the header file does not have the right format. The right format is [HeaderToFind][TAB][+/-] e.g. + = Strain only, - is Full Header e.g. Aagrestis_BONN	+Aagrestis_OXF@17242	-";
	case MATCHING_FASTAFILE_NOT_FOUND:
		return "Error MATCHING_FASTAFILE_NOT_FOUND, could not find the matching Fasta File.";
	case MATCHING_SPLIT_HEADERNAME_AND_NUMBER_TOO_SMALL:
		return "Error MATCHING_SPLIT_HEADERNAME_AND_NUMBER_TOO_SMALL, the header of a fasta block is not properly formatted it should be [HeaderName]-[HeaderNumber]. Total size at least 3";
	case MATCHING_FASTAFILE_FILENAME_NOT_FOUND:
		return "Error MATCHING_FASTAFILE_FILENAME_NOT_FOUND, Fasta file was not found based on the fasta file name. Are you use the fasta file matches the orthofinder output?";
	case MATCHING_FASTAFILE_MATCHINGNAME_NOT_FOUND:
		return "Error MATCHING_FASTAFILE_MATCHINGNAME_NOT_FOUND, Fasta file was not found based on Matching file name. Are you sure the fasta file matches the orthofinder output?";
	case MATCHING_FASTAFILE_DOES_NOT_EXIST_RFBS:
		return "Error MATCHING_FASTAFILE_DOES_NOT_EXIST_RFBS, The fasta file withint he vector does not contain any data.";
	case MATCHING_HEADERNAME_DOES_NOT_EXIST_RFBS:
		return "Error MATCHING_HEADERNAME_DOES_NOT_EXIST_RFBS, the header object within the vector does not contain any information.";
	case MATCHING_HEADERNAME_HEADERORSTRAIN_DOES_NOT_EXIST_RFBS:
		return "Error MATCHING_HEADERNAME_HEADERORSTRAIN_DOES_NOT_EXIST_RFBS, the information if the header is a strain or complete header object does not contain any information.";
	case MATCHING_HEADERNAME_WRONG_FORMAT:
		return "Error MATCHING_HEADERNAME_WRONG_FORMAT, the header file is not in the right format. The right format is [HeaderToFind][TAB][+/-] e.g. + = Strain only, - is Full Header e.g. Aagrestis_BONN	+Aagrestis_OXF@17242	-";
	case JOB_ALREADYFINISHED_RASFAF:
		return "Error JOB_ALREADYFINISHED, the job is already finished, but not filled yet! This is no error.";
	case JOB_EMPTYJOBOBJECT:
		return "Error JOB_EMPTYJOBOBJECT, this job object has no data.";
	case JOB_NOTFINISHEDYET_RASFAF:
		return "Error JOB_NOTFINISHEDYET_RASFAF, Job not finished yet can not be replaced.";
	case JOB_NOEMPTYSPOTFOUND_RASFAF:
		return "Error JOB_NOEMPTYSPOTFOUND_RASFAF, Could not find an empty spot in the vector.";

	case WRITE_OUTPUT_FILE_ERROR_FOPEN:
		return "Error WRITE_OUTPUT_FILE_ERROR_FOPEN, could not write the output file. Be sure to have permissions in the output folder and/or to check if the same file does not exist already with locked permissions";
	case WRITE_OUTPUT_FILE_SUMMARY_DECONTAMINATION:
		return "Error WRITE_OUTPUT_FILE_SUMMARY_DECONTAMINATION, could not write the Summary Decontamination output file. Be sure to have permissions in the output folder and/or to check if the same file does not exist already with locked permissions";
	case WRITE_OUTPUT_ORTHOGROUP_FASTAOBJECT:
		return "Error WRITE_OUTPUT_ORTHOGROUP_FASTAOBJECT, The orthogroup fasta file object does not cotain any information.";
	case WRITE_OUTPUT_FILE_SUMMARY_OSG:
		return "Eror WRITE_OUTPUT_FILE_SUMMARY_OSG, could not write the summary file for OrthoGroup Sequence Grabber.";
	case FILTERED_DIR_LIST:
		return "Error FILTERED_DIR_LIST, failed to execute the function GetFilteredDirList";
	case FILTERED_DIR_LIST_NO_ITEMS:
		return "Error FILTERED_DIR_LIST_NO_ITEMS, there are no items within the filtered dir list";
	default:
		return "ERROR UNKOWN, no error number, unknown error.";
	}
	return "ERROR UNKNOWN ERROR occured";
}