#include "EstatFacil.h"
#include "EstatMitja.h"
#include "Config.h"

float EstatFacil::interval() const {
	return Config::INTERVAL_FACIL;
}

float EstatFacil::velocitat() const {
	return Config::VELOCITAT_FACIL;
}

std::string EstatFacil::nom() const {
	return "Facil";
}

DificultatEstat* EstatFacil::seguent(int punts) const {
	if (punts >= Config::PUNTS_PER_PUJAR_NIVELL)
		return new EstatMitja();
	return nullptr;
}
