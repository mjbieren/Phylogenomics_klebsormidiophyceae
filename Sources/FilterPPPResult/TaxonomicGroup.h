#pragma once
#include "ClearVector.h"
#include "Management.h"
#include "GlobalHelper.h"

class CTaxonomicSpecies
{
protected:
	char* m_szSpeciesName = nullptr;
	char* m_szHeaderName = nullptr;
	size_t m_siFilterNumber = -1;
	size_t m_sizeSequenceCounter = 0;
public:
	CTaxonomicSpecies() {};
	~CTaxonomicSpecies()
	{
		if (m_szSpeciesName != nullptr)
			delete m_szSpeciesName;

		if (m_szHeaderName != nullptr)
			delete m_szHeaderName;
	};

	void IncreaseSequenceCounter() { m_sizeSequenceCounter++; }
	void DecreaseSequenceCounter() { m_sizeSequenceCounter--; }

	size_t GetSequenceCounter() { return m_sizeSequenceCounter; }

	long SetFilterNumber(size_t siFilterNumber);

	long GetFilterNumber(size_t* psiFilterNumber);

	long SetSpeciesName(const char* pSpeciesName, size_t sizeToCopy);

	long GetSpeciesName(char** ppSpeciesName);

	char* GetSpeciesName();

	long SetHeaderName(const char* pHeaderName, size_t sizeToCopy);

	long GetHeaderName(char** ppHeaderName);

	char* GetHeaderName();

};

class CTaxonomicGroup
{
protected:
	std::vector<CTaxonomicSpecies*> m_vecTaxonomicSpecies;
	bool m_bDeleteItemsInVector = true;
public:
	CTaxonomicGroup() {};
	~CTaxonomicGroup() 
	{
		if(m_bDeleteItemsInVector == true)
			ClearVector<CTaxonomicSpecies>(&m_vecTaxonomicSpecies);
		m_vecTaxonomicSpecies.clear();
	};

	long PushBack(CTaxonomicSpecies* pTaxonomicSpecies);

	void SetDeleteTaxonomicSpecies(bool bDelete = true);

	bool GetDeleteTaxonomicSpecies();

	CTaxonomicSpecies* GetSpeciesVectorItem(size_t idx);

	size_t GetVectorSize();

	static std::string StringToUpper(std::string strInput);

	long SortTaxonomicGroupAlphabeticallySpecies();

	long SortTaxonomicGroupAlphabeticallyHeaders();

	long FindTaxonomicSpeciesFromSpeciesName(const char* szToMatch, CTaxonomicSpecies** ppTaxonomicSpecies);

	long FindTaxonomicSpeciesFromHeaderName(const char* szToMatch, CTaxonomicSpecies** ppTaxonomicSpecies);
	long FindTaxonomicSpeciesFromHeaderNameSource(const char* szToMatch, CTaxonomicSpecies** ppTaxonomicSpecies);
};

