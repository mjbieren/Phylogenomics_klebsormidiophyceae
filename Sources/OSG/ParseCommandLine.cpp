#include "GlobalHelper.h"

#define TOTALREQUIREMENTS 7

long ParseCommandLine(int argc, char* argv[], char* envp[])
{
	long lRet = ERR_NOERROR;

	int aiRequirements[TOTALREQUIREMENTS] = { 0,0,0,0,0,0,0};
	for (int i = 1; i < argc; i++)
	{

		printf_s("arg %i, %s %s", i, argv[i], EOL);



		if (*argv[i] == '-') //switch detected
		{
			switch (argv[i][1])
			{
			case 'f': //Input fasta files dir path
				lRet = glb.propertyBag.SetFastaDirPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_FASTADIR_OSG;
				aiRequirements[0] = 1;
				break;
			case 'g': //Input OrthoGroup files dir path
				lRet = glb.propertyBag.SetOrthoGroupDirPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return 	PARSECOMMANDLINE_ORTHOGROUPDIR_OSG;
				aiRequirements[1] = 1;
				break;
			case 'r': //Output dir Path
				lRet = glb.propertyBag.SetOutputFolderPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_OUTPUTDIR_OSG;
				aiRequirements[2] = 1;
				break;
			case 't': //Taxonomic Group File Path
				lRet = glb.propertyBag.SetTaxonomicGroupFilePath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_TAXONOMICGROUPFILE_OSG;
				aiRequirements[3] = 1;
				break;
			case 'n': //Taxonomic Group Threshold
				lRet = glb.propertyBag.SetTaxonomicGroupThreshold(atol(&argv[i][3]));
				if (lRet != ERR_NOERROR)
				{
					if (lRet != PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_OSG)
						return PARSECOMMANDLINE_TAXONOMICTHRESHOLD_OSG;
					else
						return PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_OSG;
				}
				aiRequirements[4] = 1;
				break;
			case 'p': //PhyloPyPrunerFormat
				lRet = glb.propertyBag.SetPhyloPyPrunerFormat();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_PHYLOPYPRUNERFORMAT_OSG;
				aiRequirements[5] = 1;
				break;
			case 's': //SummaryFileOutput
				lRet = glb.propertyBag.SetOutputSummaryPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_SUMMARYPATH_OSG;
				lRet = glb.propertyBag.SetBoolSummary();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BOOLSUMMARY_OSG;
				aiRequirements[6] = 1;
				break;
			default:
			{
				printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
				return PARSE_COMMAND_LINE;
			}
			}

			/*

E-value

*/

		}
		else if (*argv[i] == '?')
		{
			//print arguments: ToDO change it
			printf_s(
				"%s"
				"-f <FastaFilesPath>\t\t Set the Path to the directory containing your fasta files: REQUIRED %s"
				"-g <OrthoGroupFilesPath> \t\t Set the Path to the directory containing the Orthogroups in TSV format: REQUIRED%s"
				"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
				"-t <TaxonomicGroupFile>\t\t Set the Taxonomic Group File, used to filter your result: NOT REQUIRED. If not Set all Orthogroups are parsed%s"
				"-n <ThresholdNumber>\t\t Set the Threshold number of how many Taxonomic Groups need to be present: NOT REQUIRED. Not setting this value results in parsing all orthogroups!%s"
				"-p \t\t Changes all fasta headers into the format needed for PhyloPyPruner: NOT REQUIRED. Not setting it will result in the same fasta headers as the fasta files uses.%s"
				"-s <SummaryPath> \t\t Set the path to where the user want to write the Summary: NOT REQUIRED. Not setting it will result in no summary file.%s"
				"%s"
				, EOL, EOL, EOL, EOL,EOL, EOL,EOL,EOL,EOL,EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return lRet;
	}

	//Check if -t, -n and -p are set if not give a messsage to the user

	if (aiRequirements[3] == 0)
	{
		printf_s("The Path to the Taxonomic Group File is not set, this means that the program will now generate a fasta file for each Orthogroup without filtering the result. %s", EOL);
		aiRequirements[3] = 1;

	}
	if (aiRequirements[4] == 0)
	{
		printf_s("The Taxonomic group threshold is not set, this means that the program will now generate a fasta file for each Orthogroup without filtering the result. %s", EOL);
		aiRequirements[4] = 1;

	}
	if (aiRequirements[5] == 0)
	{
		printf_s("The PhyloPyPruner format is not set, this means that the program will now create a fasta file per orthogroup withou the phylopypruner format on the headers. E.g. Umutabilis-1 instead of Umutabilis@1 %s", EOL);
		aiRequirements[5] = 1;
	}

	if (aiRequirements[6] == 0)
	{
		printf_s("No Path for the Summary file is set, this means that the program will not write a summary file.%s", EOL);
		aiRequirements[6] = 1;
	}



	//Check if the other requiprements are met, if not sent a message to the user like if they would use ?. And sent back an error message.
	int iSum = 0;
	size_t idx = 0;
	while (idx < TOTALREQUIREMENTS)
	{
		iSum += aiRequirements[idx];
		idx++;
	}

	if (iSum == TOTALREQUIREMENTS)
		return lRet;
	else
	{
		//print arguments
		printf_s(
				"%s"
				"-f <FastaFilesPath>\t\t Set the Path to the directory containing your fasta files: REQUIRED %s"
				"-g <OrthoGroupFilesPath> \t\t Set the Path to the directory containing the Orthogroups in TSV format: REQUIRED%s"
				"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
				"-t <TaxonomicGroupFile>\t\t Set the Taxonomic Group File, used to filter your result: NOT REQUIRED. If not Set all Orthogroups are parsed%s"
				"-n <ThresholdNumber>\t\t Set the Threshold number of how many Taxonomic Groups need to be present: NOT REQUIRED. Not setting this value results in parsing all orthogroups!%s"
				"-p \t\t Changes all fasta headers into the format needed for PhyloPyPruner: NOT REQUIRED. Not setting it will result in the same fasta headers as the fasta files uses.%s"
				"-s <SummaryPath> \t\t Set the path to where the user want to write the Summary: NOT REQUIRED. Not setting it will result in no summary file.%s"
				"%s"
				, EOL, EOL, EOL, EOL,EOL, EOL,EOL,EOL,EOL,EOL);
		return PARSE_COMMAND_LINE_MAN;
	}
	
	return lRet;
}