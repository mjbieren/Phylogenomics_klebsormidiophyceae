#include "GlobalHelper.h"

#define TOTALREQUIREMENTS 7

long ParseCommandLine(int argc, char* argv[], char* envp[])
{
	long lRet = ERR_NOERROR;

	lRet = glb.propertyBag.SetBoolGeneFile(false);

	int aiRequirements[TOTALREQUIREMENTS] = { 0,0,0,0,0,0,0};
	for (int i = 1; i < argc; i++)
	{

		printf_s("arg %i, %s %s", i, argv[i], EOL);



		if (*argv[i] == '-') //switch detected
		{
			switch (argv[i][1])
			{
			case 'i': //Input decontamination file list
				lRet = glb.propertyBag.SetDecontaminationListPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_DECONTAMINATIONLIST;
				aiRequirements[0] = 1;
				break;
			case 'f': //Input Fasta Query File
				lRet = glb.propertyBag.SetFastaQuery(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_FASTAQUERY;
				aiRequirements[1] = 1;
				break;
			case 'b': //Blastfiles path
				lRet = glb.propertyBag.SetBlastFilePath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_BLASTFILE;
				aiRequirements[2] = 1;
				break;
			case 'c':
				lRet = glb.propertyBag.SetColumnNumber(atol(&argv[i][3]));
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_COLUMNNUMBER;
				aiRequirements[3] = 1;
				break;
			case 's': //Strain Name
				lRet = glb.propertyBag.SetStrainName(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_STRAINNAME;
				aiRequirements[4] = 1;
				break;
			case 'r': //OutputFile
				lRet = glb.propertyBag.SetOutputPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_OUTPUTPATH;
				aiRequirements[5] = 1;
				break;
			case 't': //Strict decontamination not needed
				lRet = glb.propertyBag.SetBoolStrictDecontamination();
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_STRICTDECONTAMINATION;
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
				"-i <input Decontamination file liste>\t\t Set your decontamination list file%s"
				"-f <Input Fasta file> \t\t Set Fasta Query File%s"
				"-b <BlastOutputFiles>\t\tThe Blast Output file, the output file from your decontamination%s"
				"-c <ColumnNumber>\t\tThe blast column on which the values should be filterd, e.g. max score, alignment size etc %s"
				"-s <StrainName>\t\tStrain Name needed for your output file.%s"
				"-r <OutputPath>\t\tOutput folder of the result. %s"
				"-t \t\t Set strict decontamination aka best e-value is moved to the right fasta file, ignore positive set if result is weaker%s"
				"%s"
				, EOL, EOL, EOL, EOL,EOL, EOL,EOL,EOL,EOL, EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return lRet;
	}

	//Add a default file extension to the blast output files

	//Just to be sure that the set value can be ignored
	aiRequirements[6] = 1; //Strict decontamination. not required to set.


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
		//print arguments: ToDO change it
		printf_s(
			"%s"
			"-i <input Decontamination file liste>\t\t Set your decontamination list file%s"
			"-f <Input Fasta file> \t\t Set Fasta Query File%s"
			"-b <BlastOutputFiles>\t\tThe Blast Output file, the output file from your decontamination%s"
			"-c <ColumnNumber>\t\tThe blast column on which the values should be filterd, e.g. max score, alignment size etc %s"
			"-s <StrainName>\t\tStrain Name needed for your output file.%s"
			"-r <OutputPath>\t\tOutput folder of the result. %s"
			"%s"
			, EOL, EOL, EOL, EOL,EOL, EOL,EOL,EOL,EOL);
		return PARSE_COMMAND_LINE_MAN;
	}
	
	return lRet;
}