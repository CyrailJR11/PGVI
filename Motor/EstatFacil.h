#pragma once
#include "DificultatEstat.h"

class EstatFacil : public DificultatEstat {
public:
	float interval() const override;
	float velocitat() const override;
	std::string nom() const override;
	DificultatEstat* seguent(int punts) const override;
};
