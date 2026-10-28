#include "DificultatEstat.h"
#include "EstatFacil.h"
#include "EstatMitja.h"
#include "EstatDificil.h"

DificultatEstat* DificultatEstat::crea(int nivell) {
	switch (nivell) {
		case 1:  return new EstatMitja();
		case 2:  return new EstatDificil();
		default: return new EstatFacil();
	}
}

std::string DificultatEstat::nomNivell(int nivell) {
	switch (nivell) {
		case 1:  return "Mitja";
		case 2:  return "Dificil";
		default: return "Facil";
	}
}
