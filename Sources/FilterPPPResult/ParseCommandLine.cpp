#include "GlobalHelper.h"

#define TOTALREQUIREMENTS 5

long ParseCommandLine(int argc, char* argv[], char* envp[])
{
	long lRet = ERR_NOERROR;

	int aiRequirements[TOTALREQUIREMENTS] = { 0,0,0,0,0};
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
					return PARSECOMMANDLINE_FASTADIR_FPPPR;
				aiRequirements[0] = 1;
				break;
			case 'r': //Output dir Path
				lRet = glb.propertyBag.SetOutputFolderPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_OUTPUTDIR_FPPPR;
				aiRequirements[1] = 1;
				break;
			case 't': //Taxonomic Group File Path
				lRet = glb.propertyBag.SetTaxonomicGroupFilePath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_TAXONOMICGROUPFILE_FPPPR;
				aiRequirements[2] = 1;
				break;
			case 'n': //Taxonomic Group Threshold
				lRet = glb.propertyBag.SetTaxonomicGroupThreshold(atol(&argv[i][3]));
				if (lRet != ERR_NOERROR)
				{
					if (lRet != PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_FPPPR)
						return PARSECOMMANDLINE_TAXONOMICTHRESHOLD_FPPPR;
					else
						return PARSECOMMANDLINE_TAXONOMICTHRESHOLD_TOOLOW_FPPPR;
				}
				aiRequirements[3] = 1;
				break;
			case 's': //SummaryFileOutput
				lRet = glb.propertyBag.SetOutputSummaryPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_SUMMARYPATH_FPPPR;
				lRet = glb.propertyBag.SetBoolSummary();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BOOLSUMMARY_FPPPR;
				aiRequirements[4] = 1;
				break;
			case 'h': //Remove Gene IDS or not (Just strains
				lRet = glb.propertyBag.SetBoolNoHeader();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BOOLNOHEADER_FPPPR;
				break;
			case 'a': //Remove Gaps/Alignments
				lRet = glb.propertyBag.SetBoolNoGaps();
				if(lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BOOLNOGAP_FPPPR;
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
				"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
				"-t <TaxonomicGroupFile>\t\t Set the Taxonomic Group File, used to filter your result: REQUIRED. %s"
				"-n <ThresholdNumber>\t\t Set the Threshold number of how many Taxonomic Groups need to be present: REQUIRED.%s"
				"-s <SummaryPath> \t\t Set the path to where the user want to write the Summary: NOT REQUIRED. Not setting it will result in no summary file.%s"
				"-h \t\t Set if you want only the strain names in the header (based on the @ in the fasta headers) %s"
				"-a \t\t Set if you want the gaps removed from the sequences (based on '-' within the sequence files) %s"
				"%s"
				, EOL, EOL, EOL, EOL, EOL,EOL, EOL,EOL, EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return lRet;
	}

	//Check if -t, -n and -p are set if not give a messsage to the user

	if (aiRequirements[4] == 0)
	{
		printf_s("No Path for the Summary file is set, this means that the program will not write a summary file.%s", EOL);
		aiRequirements[4] = 1;
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
			"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
			"-t <TaxonomicGroupFile>\t\t Set the Taxonomic Group File, used to filter your result: REQUIRED. %s"
			"-n <ThresholdNumber>\t\t Set the Threshold number of how many Taxonomic Groups need to be present: REQUIRED.%s"
			"-s <SummaryPath> \t\t Set the path to where the user want to write the Summary: NOT REQUIRED. Not setting it will result in no summary file.%s"
			"-h \t\t Set if you want only the strain names in the header (based on the @ in the fasta headers) %s"
			"-a \t\t Set if you want the gaps removed from the sequences (based on '-' within the sequence fiels) %s"
			"%s"
			, EOL, EOL, EOL, EOL, EOL,EOL, EOL,EOL, EOL);
		return PARSE_COMMAND_LINE_MAN;
	}
	
	return lRet;
}