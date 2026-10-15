#include "EstatFacil.h"
#include "Config.h"

float EstatFacil::interval() const {
	return Config::INTERVAL_FACIL;
}

float EstatFacil::velocitat() const {
	return Config::VELOCITAT_FACIL;
}

DificultatEstat* EstatFacil::seguent(int punts) const {
	/*if (punts >= Config::PUNTS_PER_PUJAR_NIVELL)
		return new EstatMitja(); */
	(void)punts;   // de moment no s'usa (fins que existeixi EstatMitja)
	return nullptr;
}
