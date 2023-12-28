#include "GlobalHelper.h"

#define TOTALREQUIREMENTS 6

long ParseCommandLine(int argc, char* argv[], char* envp[])
{
	long lRet = ERR_NOERROR;

	int aiRequirements[TOTALREQUIREMENTS] = { 0,0,0,0,0,0};
	for (int i = 1; i < argc; i++)
	{

		printf_s("arg %i, %s %s", i, argv[i], EOL);



		if (*argv[i] == '-') //switch detected
		{
			switch (argv[i][1])
			{
			case 'f': //Input fasta files dir path Base
				lRet = glb.propertyBag.SetFastaDirSetBase(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_FASTAFILE_BASE_COGS;
				aiRequirements[0] = 1;
				break;
			case 's': //Input OrthoGroup files dir path
				lRet = glb.propertyBag.SetFastaDirSetOne(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return 	PARSECOMMANDLINE_FASTAFILE_ONE_COGS;
				aiRequirements[1] = 1;
				break;
			case 't': //Output dir Path
				lRet = glb.propertyBag.SetFastaDirSetTwo(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_FASTAFILE_TWO_COGS;
				aiRequirements[2] = 1;
				break;
			case 'o': //Taxonomic Group File Path
				lRet = glb.propertyBag.SetOrthoGroupDir(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_ORTHOGROUPDIR_COGS;
				aiRequirements[3] = 1;
				break;
			case 'r': //Output File Path
				lRet = glb.propertyBag.SetOutputDir(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_OUTPUTDIR_COGS;
				aiRequirements[4] = 1;
				break;
			case 'p': //PhyloPyPrunerFormat
				lRet = glb.propertyBag.SetBoolPhyloPyPruner();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_PHYLOPYPRUNER_FORMAT_COGS;
				aiRequirements[5] = 1;
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
				"-f <FastaFilesBase>\t\t Set the Path to the directory containing your fasta files: REQUIRED %s"
				"-s <FastaFileFirstSet>\t\t Set the Path to the first orthogroup set in fasta file formats: REQUIRED %s"
				"-t <FastaFileSecondSet>\t\t Set the Path to the second orthogroup set in fasta file formats: REQUIRED %s"
				"-o <OrthoGroupFilesPath> \t\t Set the Path to the directory containing the Orthogroups in TSV format: REQUIRED%s"
				"-r <OutputDirPath> \t\t Set the Path to directory where the output files have to be written to: REQUIRED%s"
				"-p \t\t Changes all fasta headers into the format needed for PhyloPyPruner: NOT REQUIRED. Not setting it will result in the same fasta headers as the fasta files uses.%s"
				"%s"
				, EOL, EOL, EOL, EOL,EOL, EOL,EOL,EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return lRet;
	}

	//Check if -t, -n and -p are set if not give a messsage to the user


	if (aiRequirements[5] == 0)
	{
		printf_s("The PhyloPyPruner format is not set, this means that the program will now create a fasta file per orthogroup withou the phylopypruner format on the headers. E.g. Umutabilis-1 instead of Umutabilis@1 %s", EOL);
		aiRequirements[5] = 1;
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
			"-f <FastaFilesBase>\t\t Set the Path to the directory containing your fasta files: REQUIRED %s"
			"-s <FastaFileFirstSet>\t\t Set the Path to the first orthogroup set in fasta file formats: REQUIRED %s"
			"-t <FastaFileSecondSet>\t\t Set the Path to the second orthogroup set in fasta file formats: REQUIRED %s"
			"-o <OrthoGroupFilesPath> \t\t Set the Path to the directory containing the Orthogroups in TSV format: REQUIRED%s"
			"-r <OutputDirPath> \t\t Set the Path to directory where the output files have to be written to: REQUIRED%s"
			"-p \t\t Changes all fasta headers into the format needed for PhyloPyPruner: NOT REQUIRED. Not setting it will result in the same fasta headers as the fasta files uses.%s"
			"%s"
			, EOL, EOL, EOL, EOL,EOL, EOL,EOL,EOL);
		return PARSE_COMMAND_LINE_MAN;
	}
	
	return lRet;
}