#include "EstatMitja.h"
#include "EstatDificil.h"
#include "Config.h"

float EstatMitja::interval() const {
	return Config::INTERVAL_MITJA;
}

float EstatMitja::velocitat() const {
	return Config::VELOCITAT_MITJA;
}

std::string EstatMitja::nom() const {
	return "Mitja";
}

DificultatEstat* EstatMitja::seguent(int punts) const {
	if (punts >= Config::PUNTS_PER_PUJAR_NIVELL * 2)
		return new EstatDificil();
	return nullptr;
}
