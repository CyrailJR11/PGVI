#include "EstatDificil.h"
#include "Config.h"

float EstatDificil::interval() const {
	return Config::INTERVAL_DIFICIL;
}

float EstatDificil::velocitat() const {
	return Config::VELOCITAT_DIFICIL;
}

std::string EstatDificil::nom() const {
	return "Dificil";
}

DificultatEstat* EstatDificil::seguent(int punts) const {
	(void)punts;      // es l'ultim nivell: no canvia mai
	return nullptr;
}
