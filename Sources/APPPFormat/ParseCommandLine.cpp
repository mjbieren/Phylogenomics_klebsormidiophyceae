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
			case 'i': //Input Tree Path
				lRet = glb.propertyBag.SetTreeFilesPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_TREEDIR_APPPF;
				aiRequirements[0] = 1;
				break;
			case 't': //Input Tree format
				lRet = glb.propertyBag.SetTreeFormat(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return 	PARSECOMMANDLINE_TREEFORMAT_APPPF;
				aiRequirements[1] = 1;
				break;
			case 'r': //Output dir Path
				lRet = glb.propertyBag.SetOutputFolder(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_OUTPUTFOLDER_APPPF;
				aiRequirements[2] = 1;
				break;
			case 'g': //Taxonomic Group File Path
				lRet = glb.propertyBag.SetTaxonomicGroupFilePath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_TAXONOMICGROUPFILE_APPPF;
				aiRequirements[3] = 1;
				break;
			case 'm': //Mafft File path (optional)
				lRet = glb.propertyBag.SetMoveMafftFilesPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_MOVEMAFFTFIELPATH_APPPF;
				lRet = glb.propertyBag.SetMoveMafftFiles();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BOOLMOVEMAFFTFILE_APPPF;
				aiRequirements[4] = 1;
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
				"-i <TreeFilesPath>\t\t Set the Path to the directory containing your Tree files: REQUIRED %s"
				"-t <TreeFormat> \t\t Set the format of the tree files, e.g.: tre, treefile. Keep in mind it should be in newick format: REQUIRED%s"
				"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
				"-g <TaxonomicGroupFile>\t\t Set the Taxonomic Group File, used to filter your result: REQUIRED.%s"
				"-m <MoveMafftFilePath>\t\t Also move the MSA (Mafft) files into the output directory, so you have all the requirements for PhyloPyPruner: NOT REQUIRED. Not setting this value results not adding the MSA files in the designed output folder. You have to add them yourself!%s"
				"%s"
				, EOL, EOL, EOL, EOL,EOL, EOL,EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return lRet;
	}

	//Check if -t, -n and -p are set if not give a messsage to the user

	if (aiRequirements[4] == 0)
	{
		printf_s("The option to move the MSA (.mafft) files is disabled. This means that the MSA files will not be moved together with the tree files. You will have to do this manually %s", EOL);
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
				"-i <TreeFilesPath>\t\t Set the Path to the directory containing your Tree files: REQUIRED %s"
				"-t <TreeFormat> \t\t Set the format of the tree files, e.g.: tre, treefile. Keep in mind it should be in newick format: REQUIRED%s"
				"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
				"-g <TaxonomicGroupFile>\t\t Set the Taxonomic Group File, used to filter your result: REQUIRED.%s"
				"-m <MoveMafftFilePath>\t\t Also move the MSA (Mafft) files into the output directory, so you have all the requirements for PhyloPyPruner: NOT REQUIRED. Not setting this value results not adding the MSA files in the designed output folder. You have to add them yourself!%s"
				"%s"
				, EOL, EOL, EOL, EOL,EOL, EOL,EOL);
		return PARSE_COMMAND_LINE_MAN;
	}
	
	return lRet;
}