#pragma once
#include <cstdio>

#ifndef ERR_NOERROR
#define ERR_NOERROR 0
#endif // !ERR_NOERROR

enum MANAGEMENT_ERRORS
{
	ERR_ERROR = 1,
	PARSE_COMMAND_LINE,
	PARSE_COMMAND_LINE_MAN,
	DIRLIST_FAIL,
	ERR_FILE_OF_INTEREST_DOES_NOT_EXIST_SET_FILE_BUFFER, //
	ERR_FOPEN_SET_FILE_BUFFER_COULD_NOT_OPEN_FILE, //
	ERR_FILE_OF_INTEREST_DOES_NOT_EXIST_GET_BUFFER_SIZE, //
	ERR_FILE_OF_INTEREST_BUFFER_SIZE_BUFFER_TOO_SMALL, //

	PARSECOMMANDLINE_COLUMNNUMBER = 20, //Command Line GPDS
	PARSECOMMANDLINE_BLASTEXTENSION,
	PARSECOMMANDLINE_FASTAQUERY,
	PARSECOMMANDLINE_GENESPATH,
	PARSECOMMANDLINE_BLASTFILE,
	PARSECOMMANDLINE_OUTPUTPATH,
	PARSECOMMANDLINE_STRAINNAME,
	PARSECOMMANDLINE_DECONTAMINATIONLIST,
	PARSECOMMANDLINE_STRICTDECONTAMINATION,

	PARSECOMMANDLINE_FASTADIR_OSG = 30, //Command Line OSG
	PARSECOMMANDLINE_ORTHOGROUPDIR_OSG,
	PARSECOMMANDLINE_OUTPUTDIR_OSG,
	PARSECOMMANDLINE_TAXONOMICGROUPFILE_OSG,
	PARSECOMMANDLINE_TAXONOMICTHRESHOLD_OSG,
	PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_OSG,
	PARSECOMMANDLINE_PHYLOPYPRUNERFORMAT_OSG,
	PARSECOMMANDLINE_SUMMARYPATH_OSG,
	PARSECOMMANDLINE_BOOLSUMMARY_OSG,
	PARSECOMMANDLINE_BOOLSTRAINONLY_SCPT,

	PARSECOMMANDLINE_TREEFORMAT_APPPF = 40, //Command Line APPPF
	PARSECOMMANDLINE_TREEDIR_APPPF,
	PARSECOMMANDLINE_TAXONOMICGROUPFILE_APPPF,
	PARSECOMMANDLINE_OUTPUTFOLDER_APPPF,
	PARSECOMMANDLINE_BOOLMOVEMAFFTFILE_APPPF,
	PARSECOMMANDLINE_MOVEMAFFTFIELPATH_APPPF,

	PARSECOMMANDLINE_FASTADIR_FPPPR = 50, //Command Line FPPPR
	PARSECOMMANDLINE_OUTPUTDIR_FPPPR,
	PARSECOMMANDLINE_TAXONOMICGROUPFILE_FPPPR,
	PARSECOMMANDLINE_TAXONOMICTHRESHOLD_FPPPR,
	PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_FPPPR,
	PARSECOMMANDLINE_SUMMARYPATH_FPPPR,
	PARSECOMMANDLINE_BOOLSUMMARY_FPPPR,
	PARSECOMMANDLINE_BOOLNOHEADER_FPPPR,
	PARSECOMMANDLINE_BOOLNOGAP_FPPPR,

	PARSECOMMANDLINE_FASTADIR_RFBS = 60, //Command line RFBS
	PARSECOMMANDLINE_HEADERFILE_RFBS,
	PARSECOMMANDLINE_OUTPUTDIR_RFBS,
	PARSECOMMANDLINE_BOOLPHYLOPYPRUNERFORMAT_RFBS,

	PARSECOMMANDLINE_INPUTFOLDER_RASFAF = 70, //Command line RASFAF
	PARSECOMMANDLINE_OUTPUTFOLDER_RASFAF,
	PARSECOMMANDLINE_JOBNUMBER_TOO_LOW_RASFAF,
	PARSECOMMANDLINE_JOBNUMBER_INCORRECT_RASFAF,
	PARSECOMMANDLINE_JOBWAITINGTIME_TOO_LOW_RASFAF,
	PARSECOMMANDLINE_JOBWAITINGTIME_INCORRECT_RASFAF,
	PARSECOMMANDLINE_BOOLISSLURM_RASFAF,
	PARSECOMMANDLINE_BOOLISSINGLECORE_RASFAF,
	PARSECOMMANDLINE_JOBLIMIT_RASFAF,

	PARSECOMMANDLINE_FASTAFILE_ONE_COGS = 80, //Command line COGS
	PARSECOMMANDLINE_FASTAFILE_TWO_COGS,
	PARSECOMMANDLINE_FASTAFILE_BASE_COGS,
	PARSECOMMANDLINE_ORTHOGROUPDIR_COGS,
	PARSECOMMANDLINE_OUTPUTDIR_COGS,
	PARSECOMMANDLINE_PHYLOPYPRUNER_FORMAT_COGS,


	PROPERTYBAG_GETCOLUMNNUMBER = 100, //PropertyBag GPDS
	PROPERTYBAG_GETBLASTEXTENSION,
	PROPERTYBAG_GETFASTAQUERY,
	PROPERTYBAG_GETGENESPATH,
	PROPERTYBAG_GETBLASTFILE,
	PROPERTYBAG_GETOUTPUTPATH,
	PROPERTYBAG_GETSTRICTDECONTAMINATION,

	PROPERTYBAG_GETFASTADIR_OSG = 130, // PropertyBag OSG
	PROPERTYBAG_ORTHOGROUPDIR_OSG,
	PROPERTYBAG_OUTPUTDIR_OSG,
	PROPERTYBAG_TAXONOMICGROUPFILE_OSG,
	PROPERTYBAG_TAXONOMICTHRESHOLD_OSG,
	PROPERTYBAG_PHYLOPYPRUNERFORMAT_OSG,
	PROPERTYBAG_SETTAXONOMICGROUPSIZE_VECTOR_OSG,
	PROPERTYBAG_GETTAXONOMICGROUPSIZE_VECTOR_OSG,
	PROPERTYBAG_GET_SUMMARYPATH_OSG,
	PROPERTYBAG_GET_BOOLSUMMARY_OSG,
	PROPERTYBAG_GETBOOLSTRAINONLY_SCPT,

	PROPERTYBAG_GETTREEFORMAT_APPPF = 150, //PropertyBag APPPF
	PROPERTYBAG_SETTREEFORMAT_APPPF,
	PROPERTYBAG_GETTREEDIRPATH_APPPF,
	PROPERTYBAG_GETTAXONOMICGROUPFILE_APPPF,
	PROPERTYBAG_GETOUTPUTDIR_APPPF,
	PROPERTYBAG_GET_BOOLMOVEMAFFTFILE_APPPF,
	PROPERTYBAG_GETMOVEMAFFTFILEPATH_APPPF,

	PROPERTYBAG_GETFASTADIR_FPPPR = 170, // PropertyBag FPPPR
	PROPERTYBAG_OUTPUTDIR_FPPPR,
	PROPERTYBAG_TAXONOMICGROUPFILE_FPPPR,
	PROPERTYBAG_TAXONOMICTHRESHOLD_FPPPR,
	PROPERTYBAG_SETTAXONOMICGROUPSIZE_VECTOR_FPPPR,
	PROPERTYBAG_GETTAXONOMICGROUPSIZE_VECTOR_FPPPR,
	PROPERTYBAG_GET_SUMMARYPATH_FPPPR,
	PROPERTYBAG_GET_BOOLSUMMARY_FPPPR,
	PROPERTYBAG_GET_BOOLNOHEADER_FPPPR,
	PROPERTYBAG_GET_BOOLNOGAPS_FPPPR,

	PROPERTYBAG_GETFASTADIR_RFBS = 180, //PropertyBag RFBS
	PROPERTYBAG_GETHEADERFILE_RFBS,
	PROPERTYBAG_GETOUTPUTDIR_RFBS,
	PROPERTYBAG_GETBOOLPHYLOPYPRUNERFORMAT_RFBS,

	PROPERTYBAG_GETINPUTFOLDERSCRIPT_RASFAF = 190, //Propertybag RASFAF
	PROPERTYBAG_GETOUTPUTFOLDERSCRIPT_RASFAF,
	PROPERTYBAG_GETJOBLIMITNUMBER_RASFAF,
	PROPERTYBAG_GETJOBWAITING_TIME_RASFAF,
	PROPERTYBAG_GETBOOLSLURM_RASFAF,
	PROPERTYBAG_GETBOOLSINGLECORE_RASFAF,
	PROPERTYBAG_GETJOBTIMELIMIT_RASFAF,

	PROPERTYBAG_FASTAFILE_ONE_COGS = 200, //Propertybag COGS
	PROPERTYBAG_FASTAFILE_TWO_COGS,
	PROPERTYBAG_FASTAFILE_BASE_COGS,
	PROPERTYBAG_ORTHOGROUPDIR_COGS,
	PROPERTYBAG_OUTPUTDIR_COGS,
	PROPERTYBAG_PHYLOPYPRUNER_FORMAT_COGS,

	FASTA_PUSBACKBLOCK_ADD_VECTOR = 500, //Fasta Block
	FASTABLOCK_SETBLOCK_NOFASTAFORMAT,
	FASTABLOCK_SETFASTAHEADER,
	FASTABLOCK_SETFASTAHEADER_POINTERDIFFERENCE,
	FASTABLOCK_SETFASTAHEADERSTRAIN_POINTERDIFFERENCE,
	FASTABLOCK_SETHEADERSTRAIN_NODATA,
	FASTABLOCK_SETFASTAHREST_POITNERDIFFERENCE, 
	FASTABLOCK_SETHEADERREST_NODATA,
	FASTABLOCK_GETFASTAHEADER,
	FASTABLOCK_SETFASTASEQUENCE,
	FASTABLOCK_SETFASTASEQUENCE_POINTERDIFFERENCE,
	FASTABLOCK_GETFASTASEQUENCE,
	FASTABLOCK_GETHEADERNUMBER,
	FASTABLOCK_GETHEADERSTRAIN,
	FASTABLOCK_GETHEADERREST, ////
	FASTABLOCK_SETHEADERNUMBER,
	FASTABLOCK_GETWRITEFASTABLOCKBOOL,
	FASTABLOCK_SETWRITEFASTABLOCKBOOL,

	FASTAFILE_GETFASTAFILEBUFFER = 600, //Fasta File
	FASTAFILE_WRONGFORMAT,
	FASTAFILE_PARSEFASTABLOCK_WRONGFORMAT,
	FASTAFILE_SETFASTAFILENAME_POINTERDIFFERENCE,
	FASTAFILE_GETFASTAFILENAME,
	FASTAFILE_EMPTY_STRING_FASTAFILENAME, 
	FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND,
	FASTAFILE_MATCHING_FASTABLOCK_NOT_FOUND_BASED_ON_HEADERNUMBER,
	FASTAFILE_SETMATCHINGNAME_POINTERDIFFERENCE,
	FASTAFILE_EMPTY_STRING_MATCHINGNAME,
	FASTAFILE_GETMATCHINGNAME,

	TABLEROW_SETCOLUMNNAME_HEADER = 900, //Table Row
	TABLEROW_SETCOLUMNNAMENEW_HEADER,
	TABLEROW_SETCOLUMNNAMENEW_HEADER_POINTER,
	TABLEROW_GETCOLUMNNAME_HEADER,
	TABLEROW_GETCOLUMNNAMENEW_HEADER,

	TABLE_NOTMATCHING_COLUMNNUMBER = 1000, //Whole Table
	TABLE_NOMATCH_HEADERCOLUMN,
	TABLE_NEWNAME_SIZE_FORMAT_WRONG,
	TABLE_PARSE_ROW_SIZE_TOO_SMALL,
	TABLE_PARSE_ROW_SIZE_TOO_BIG,
	TABLE_NEWCOLUMN_SIZE_FORMAT_WRONG,
	TABLE_PARSE_ROW_SIZE_FORMAT_WRONG,
	TABLE_SIZE_HEADER_COLUMN_WRONG,
	TABLE_FIND_MATCHING_ROW_NAME_DOES_NOT_EXIST,
	TABLE_ROW_MATCHING_NOT_FOUND,
	TABLE_PARSE_HEADER_FORMAT_WRONG,
	TABLE_HEADER_VECTOR_AND_COLUMN_COUNT_NOT_MATCHING,
	TABLE_PARSE_TABLE_FORMAT_WRONG_NO_EOL,
	TABLE_CREATE_FILE_BUFFER,
	TABLE_SET_TABLE_ROW_HEADER, 
	TABLE_GET_TABLE_ROW_HEADER,
	TABLE_NO_TABLE_CONTENT,
	TABLE_GET_TABLE_NAME,
	TABLE_PUSHBACK_ROW_SAFE,
	TABLE_PUSHBACK_NO_ROWS,
	TABLE_POPBACK_NO_ROWS,


	TAXONOMIC_GROUP_FIND_SPECIES_SPECIESNAME = 1050,
	TAXONOMIC_GROUP_FIND_SPECIES_HEADERNAME,

	JOB_SETINPUTFILE_STRING_RASFAF = 1100, // JOBS class in RASFAF
	JOB_SETINPUTFILE_POSITION_RASFAF,
	JOB_GETINPUTFILE_RASFAF,
	JOB_SETOUTPUTFILE_STRING_RASFAF,
	JOB_SETOUTPUTFILE_POSITION_RASFAF,
	JOB_GETOUTPUTFILE_RASFAF,
	JOB_SETSYSTEMCOMMAND_STRING_RASFAF,
	JOB_SETSYSTEMCOMMAND_POSITION_RASFAF,
	JOB_GETSYSTEMCOMMAND_RASFAF,
	JOB_FINDEMPTYSPOT_FORNEWJOB_RASFAF,

	PARSEINPUT_FILE_BLAST_NO_ROWS = 1500, //own ones - Input function/ GPDS
	PARSEINPUT_FILE_BLAST_NO_TABLE,
	PARSEINPUT_FILE_BLAST_NO_TABLENAME,
	PARSEINPUT_FILE_SPLIT_FASTA_FILES_NO_FASTABLOCKS, 

	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME = 1550, //Own ones - input function / OSG
	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME_SIZETOSMALL,
	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_SPECIESNAME,
	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME,
	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME_SIZETOSMALL,
	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_HEADERNAME,
	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICGROUP_PUSHBACK_TAXONOMIC_SPECIES,
	PARSE_INPUT_FILE_TAXONOMICGROUPFILE_WRONG_FORMAT,

	PARSE_INPUT_FILE_HEADERFILE_SETFILEBUFFER_RFBS = 1600, //Own ones - Input Functon / RFBS
	PARSE_INPUT_FILE_HEADERFILE_WRONG_FORMAT_RFBS,

	MATCHING_FASTAFILE_NOT_FOUND = 2000, //Matching Function
	MATCHING_SPLIT_HEADERNAME_AND_NUMBER_TOO_SMALL,

	MATCHING_FASTAFILE_FILENAME_NOT_FOUND = 2050,
	MATCHING_FASTAFILE_MATCHINGNAME_NOT_FOUND,

	MATCHING_FASTAFILE_DOES_NOT_EXIST_RFBS = 2100,
	MATCHING_HEADERNAME_DOES_NOT_EXIST_RFBS,
	MATCHING_HEADERNAME_HEADERORSTRAIN_DOES_NOT_EXIST_RFBS,
	MATCHING_HEADERNAME_WRONG_FORMAT,

	JOB_ALREADYFINISHED_RASFAF = 2300,
	JOB_EMPTYJOBOBJECT,
	JOB_NOTFINISHEDYET_RASFAF,
	JOB_NOEMPTYSPOTFOUND_RASFAF,

	WRITE_OUTPUT_FILE_ERROR_FOPEN = 2500, //Write output function
	WRITE_OUTPUT_FILE_SUMMARY_DECONTAMINATION,

	WRITE_OUTPUT_ORTHOGROUP_FASTAOBJECT = 2550, //Write Output Function no fasta file object
	WRITE_OUTPUT_FILE_SUMMARY_OSG,

	FILTERED_DIR_LIST = 3000, //Failed Dir list
	FILTERED_DIR_LIST_NO_ITEMS, //No items in dir list.





};

class CManagement
{
public:
	CManagement() {};
	~CManagement() {};
	virtual const char* GetErrorDescription(long lError) const;
	long ErrorChecker(long lError) const;
};