#include "TaxonomicGroup.h"

long CTaxonomicSpecies::SetFilterNumber(size_t siFilterNumber)
{
	long lReturn = ERR_NOERROR;

	m_siFilterNumber = siFilterNumber;

	return lReturn;
}

long CTaxonomicSpecies::GetFilterNumber(size_t * psiFilterNumber)
{
	long lReturn = ERR_NOERROR;
		
	*psiFilterNumber = m_siFilterNumber;
	return lReturn;
}

long CTaxonomicSpecies::SetSpeciesName(const char* pSpeciesName, size_t sizeToCopy)
{
	if (pSpeciesName == nullptr)
		return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME;

	if (!pSpeciesName[sizeToCopy - 1])
		return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_SPECIESNAME_SIZETOSMALL;

	if (m_szSpeciesName != nullptr)
		delete m_szSpeciesName;

	m_szSpeciesName = new char[sizeToCopy + 10]; //Set a buffer
	memset(m_szSpeciesName, '\0', sizeToCopy + 10);
	memcpy(m_szSpeciesName, pSpeciesName, sizeToCopy);

	return ERR_NOERROR;
}

long CTaxonomicSpecies::GetSpeciesName(char** ppSpeciesName)
{
	if (m_szSpeciesName == nullptr)
		return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_SPECIESNAME;

	*ppSpeciesName = m_szSpeciesName;

	return ERR_NOERROR;
}

char * CTaxonomicSpecies::GetSpeciesName()
{
	return m_szSpeciesName;
}

long CTaxonomicSpecies::SetHeaderName(const char* pHeaderName, size_t sizeToCopy)
{
	if (pHeaderName == nullptr)
		return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME;

	if (!pHeaderName[sizeToCopy - 1])
		return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_SET_HEADERNAME_SIZETOSMALL;

	if (m_szHeaderName != nullptr)
		delete m_szHeaderName;

	m_szHeaderName = new char[sizeToCopy + 10]; //Set a buffer
	memset(m_szHeaderName, '\0', sizeToCopy + 10);
	memcpy(m_szHeaderName, pHeaderName, sizeToCopy);

	return ERR_NOERROR;
}

long CTaxonomicSpecies::GetHeaderName(char** ppHeaderName)
{
	if (m_szHeaderName == nullptr)
		return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICSPECIES_GET_HEADERNAME;

	*ppHeaderName = m_szHeaderName;

	return ERR_NOERROR;
}

char * CTaxonomicSpecies::GetHeaderName()
{
	return m_szHeaderName;
}






long CTaxonomicGroup::PushBack(CTaxonomicSpecies* pTaxonomicSpecies)
{
	if (pTaxonomicSpecies == nullptr)
		return PARSE_INPUT_FILE_TAXONOMICGROUPFILE_TAXONOMICGROUP_PUSHBACK_TAXONOMIC_SPECIES;

	m_vecTaxonomicSpecies.push_back(pTaxonomicSpecies);
	return ERR_NOERROR;
}

void CTaxonomicGroup::SetDeleteTaxonomicSpecies(bool bDelete)
{
	m_bDeleteItemsInVector = bDelete;
}

bool CTaxonomicGroup::GetDeleteTaxonomicSpecies()
{
	return m_bDeleteItemsInVector;
}

CTaxonomicSpecies* CTaxonomicGroup::GetSpeciesVectorItem(size_t idx)
{
	return m_vecTaxonomicSpecies.at(idx);
}

size_t CTaxonomicGroup::GetVectorSize()
{
	return m_vecTaxonomicSpecies.size();
}

std::string CTaxonomicGroup::StringToUpper(std::string strInput)
{
	std::transform(strInput.begin(), strInput.end(), strInput.begin(), ::toupper);
	return strInput;
}




long CTaxonomicGroup::SortTaxonomicGroupAlphabeticallySpecies()
{
	long lReturn = ERR_NOERROR;

	std::sort(m_vecTaxonomicSpecies.begin(), m_vecTaxonomicSpecies.end(), [](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			int iRes = strcasecmp(a->GetSpeciesName(), b->GetSpeciesName());
			return iRes < 0;
		});

	return lReturn;

}

long CTaxonomicGroup::SortTaxonomicGroupAlphabeticallyHeaders()
{
	long lReturn = ERR_NOERROR;

	std::sort(m_vecTaxonomicSpecies.begin(), m_vecTaxonomicSpecies.end(), [](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			int iRes = strcasecmp(a->GetHeaderName(), b->GetHeaderName());
			return iRes < 0;
		});

	return lReturn;
}


long  CTaxonomicGroup::FindTaxonomicSpeciesFromSpeciesName(const char* szToMatch, CTaxonomicSpecies** ppTaxonomicSpecies)
{

	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(szToMatch);


#ifdef _DEBUGX
	printf_s("Working on TaxonomicSpecies block %s %s", szToMatch, EOL);
#endif 

	std::vector<CTaxonomicSpecies*>::iterator iterLowPos;
	std::vector<CTaxonomicSpecies*>::iterator iterHighPos;
	std::vector<CTaxonomicSpecies*>::iterator iterBegin = m_vecTaxonomicSpecies.begin();
	std::vector<CTaxonomicSpecies*>::iterator iterEnd = m_vecTaxonomicSpecies.end() -1;

	bool bFoundTaxonomicSpecies = false;

	CTaxonomicSpecies TaxonomicSpeciesToFind;
	TaxonomicSpeciesToFind.SetSpeciesName(szToMatch,sizeToMatch);

	iterLowPos = std::lower_bound(iterBegin, iterEnd, &TaxonomicSpeciesToFind, [](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			int iRes = strcasecmp(a->GetSpeciesName(), b->GetSpeciesName());
			return iRes < 0;
			
		});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &TaxonomicSpeciesToFind, [sizeToMatch](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			int iRes = strcasecmp(a->GetSpeciesName(), b->GetSpeciesName());
			return iRes < 0;
			
		});

	CTaxonomicSpecies* pTaxonmicSpeciesMatching = nullptr;

	if (*iterLowPos)
	{
		if (iterLowPos != iterHighPos)
		{
			while (iterLowPos <= iterHighPos)
			{
				pTaxonmicSpeciesMatching = *iterLowPos;
				if (memcmp(szToMatch, pTaxonmicSpeciesMatching->GetSpeciesName(), sizeToMatch) == 0)
				{
					*ppTaxonomicSpecies = pTaxonmicSpeciesMatching;
					bFoundTaxonomicSpecies = true;
					break;
				}
				iterLowPos++;
			}
		}
		else
		{
			pTaxonmicSpeciesMatching = *iterLowPos;
			if (memcmp(szToMatch, pTaxonmicSpeciesMatching->GetSpeciesName(), sizeToMatch) == 0)
			{
				*ppTaxonomicSpecies = pTaxonmicSpeciesMatching;
				bFoundTaxonomicSpecies = true;	

			}
		}
	}
	if (bFoundTaxonomicSpecies == false)
	{
		printf("Couldn't find taxonomic group species: %s %s", szToMatch, EOL);
		return TAXONOMIC_GROUP_FIND_SPECIES_SPECIESNAME;
	}


	return lReturn;
}

long CTaxonomicGroup::FindTaxonomicSpeciesFromHeaderNameSource(const char * szToMatch, CTaxonomicSpecies** ppTaxonomicSpecies)
{
	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(szToMatch);


#ifdef _DEBUGX
	printf_s("Working on TaxonomicSpecies block %s %s", szToMatch, EOL);
#endif 

#ifdef _DEBUG
	size_t sizeVectorTaxonomic = m_vecTaxonomicSpecies.size();
	sizeVectorTaxonomic = sizeVectorTaxonomic; //Remove warning  var set but not used

#endif
	std::vector<CTaxonomicSpecies*>::iterator iterLowPos;
	std::vector<CTaxonomicSpecies*>::iterator iterHighPos;
	std::vector<CTaxonomicSpecies*>::iterator iterBegin = m_vecTaxonomicSpecies.begin();
	std::vector<CTaxonomicSpecies*>::iterator iterEnd = m_vecTaxonomicSpecies.end()-1;

	bool bFoundTaxonomicSpecies = false;

	CTaxonomicSpecies TaxonomicSpeciesToFind;
	TaxonomicSpeciesToFind.SetHeaderName(szToMatch, sizeToMatch);

	//Input can be a complete Header so we want to resize it to what we have in our pTaxonomicGroupSorted.

	iterLowPos = std::lower_bound(iterBegin, iterEnd, &TaxonomicSpeciesToFind, [](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			std::string a_name = a->GetHeaderName();
			std::string b_name = b->GetHeaderName();
			b_name.resize(a_name.length());
			int iRes = strcasecmp(a_name.c_str(), b_name.c_str());
			return iRes < 0;
			
		});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &TaxonomicSpeciesToFind, [](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			std::string a_name = a->GetHeaderName();
			std::string b_name = b->GetHeaderName();
			b_name.resize(a_name.length());
			int iRes = strcasecmp(a_name.c_str(), b_name.c_str());
			return iRes < 0;
			
		});

#ifdef _DEBUG
	CTaxonomicSpecies*pTaxonomicSpeciesLow = *iterLowPos;
	pTaxonomicSpeciesLow = pTaxonomicSpeciesLow;	//Remove warning  var set but not used

	CTaxonomicSpecies* pTaxonomicSpeciesHigh = *iterHighPos;
	pTaxonomicSpeciesHigh = pTaxonomicSpeciesHigh; //Remove warning  var set but not used
#endif

	CTaxonomicSpecies* pTaxonmicSpeciesMatching = nullptr;

	if (*iterLowPos)
	{
		if (iterLowPos != iterHighPos)
		{
			while (iterLowPos <= iterHighPos)
			{
				pTaxonmicSpeciesMatching = *iterLowPos;
				if (memcmp(szToMatch, pTaxonmicSpeciesMatching->GetHeaderName(), strlen(pTaxonmicSpeciesMatching->GetHeaderName())) == 0)
				{
					*ppTaxonomicSpecies = pTaxonmicSpeciesMatching;
					bFoundTaxonomicSpecies = true;
					break;
				}
				iterLowPos++;
			}
		}
		else
		{
			pTaxonmicSpeciesMatching = *iterLowPos;
			if (memcmp(szToMatch, pTaxonmicSpeciesMatching->GetHeaderName(), strlen(pTaxonmicSpeciesMatching->GetHeaderName())) == 0)
			{
				*ppTaxonomicSpecies = pTaxonmicSpeciesMatching;
				bFoundTaxonomicSpecies = true;	

			}
		}
	}
	if (bFoundTaxonomicSpecies == false)
	{
	#ifdef _DEBUG
		printf_s("Could not find species %s in the Taxonomic group file.%s", szToMatch, EOL);
	#endif
		return TAXONOMIC_GROUP_FIND_SPECIES_HEADERNAME;
	}


	return lReturn;
}

long  CTaxonomicGroup::FindTaxonomicSpeciesFromHeaderName(const char * szToMatch, CTaxonomicSpecies** ppTaxonomicSpecies)
{
	long lReturn = ERR_NOERROR;

	size_t sizeToMatch = strlen(szToMatch);


#ifdef _DEBUGX
	printf_s("Working on TaxonomicSpecies block %s %s", szToMatch, EOL);
#endif 

#ifdef _DEBUG
	size_t sizeVectorTaxonomic = m_vecTaxonomicSpecies.size();
	sizeVectorTaxonomic = sizeVectorTaxonomic; //Remove warning var set but not used.
#endif
	std::vector<CTaxonomicSpecies*>::iterator iterLowPos;
	std::vector<CTaxonomicSpecies*>::iterator iterHighPos;
	std::vector<CTaxonomicSpecies*>::iterator iterBegin = m_vecTaxonomicSpecies.begin();
	std::vector<CTaxonomicSpecies*>::iterator iterEnd = m_vecTaxonomicSpecies.end()-1;

	bool bFoundTaxonomicSpecies = false;

	CTaxonomicSpecies TaxonomicSpeciesToFind;
	TaxonomicSpeciesToFind.SetHeaderName(szToMatch, sizeToMatch);

	iterLowPos = std::lower_bound(iterBegin, iterEnd, &TaxonomicSpeciesToFind, [](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			int iRes = strcasecmp(a->GetHeaderName(), b->GetHeaderName());
			return iRes < 0;
			
		});
	iterHighPos = std::upper_bound(iterBegin, iterEnd, &TaxonomicSpeciesToFind, [](CTaxonomicSpecies* a, CTaxonomicSpecies* b)
		{
			int iRes = strcasecmp(a->GetHeaderName(), b->GetHeaderName());
			return iRes < 0;
			
		});

#ifdef _DEBUG
	CTaxonomicSpecies* pTaxonomicSpeciesLow = *iterLowPos;
	pTaxonomicSpeciesLow = pTaxonomicSpeciesLow; //Remove Var set but not used
	CTaxonomicSpecies* pTaxonomicSpeciesHigh = *iterHighPos;
	pTaxonomicSpeciesHigh = pTaxonomicSpeciesHigh; //Remove var set but not used.
#endif

	CTaxonomicSpecies* pTaxonmicSpeciesMatching = nullptr;



	if (*iterLowPos)
	{
		if (iterLowPos != iterHighPos)
		{
			while (iterLowPos <= iterHighPos)
			{
				pTaxonmicSpeciesMatching = *iterLowPos;
				if (memcmp(szToMatch, pTaxonmicSpeciesMatching->GetHeaderName(), sizeToMatch) == 0)
				{
					*ppTaxonomicSpecies = pTaxonmicSpeciesMatching;
					bFoundTaxonomicSpecies = true;
					break;
				}
				iterLowPos++;
			}
		}
		else
		{
			pTaxonmicSpeciesMatching = *iterLowPos;
			if (memcmp(szToMatch, pTaxonmicSpeciesMatching->GetHeaderName(), sizeToMatch) == 0)
			{
				*ppTaxonomicSpecies = pTaxonmicSpeciesMatching;
				bFoundTaxonomicSpecies = true;	

			}
		}
	}
	if (bFoundTaxonomicSpecies == false)
	{
		printf_s("Could not find species %s in the Taxonomic group file.%s", szToMatch, EOL);
		return TAXONOMIC_GROUP_FIND_SPECIES_HEADERNAME;
	}


	return lReturn;
}