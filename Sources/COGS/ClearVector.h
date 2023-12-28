#pragma once
#include <vector>
#define ERR_NOERROR 0

//Template Functions
template<class CClearVector>
long ClearVector(std::vector<CClearVector*>* pVecToClear)
{
	std::size_t sizeVector = pVecToClear->size();

	CClearVector* pTarget = nullptr;

	for (std::size_t idxVector = 0; idxVector < sizeVector; idxVector++)
	{
		pTarget = pVecToClear->at(idxVector);
		delete pTarget;
	}
	pVecToClear->clear();

	return ERR_NOERROR;
}