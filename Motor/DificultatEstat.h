#pragma once
#include <string>

// ============================================================
//  DificultatEstat - PATRO ESTATS
//  Cada estat diu cada quant surten enemics, a quina velocitat
//  i a quin estat s'ha de passar segons la puntuacio.
//
//     EstatFacil --(5 punts)--> EstatMitja --(10 punts)--> EstatDificil
// ============================================================
class DificultatEstat {
public:
	virtual ~DificultatEstat() = default;
	virtual float interval() const = 0;
	virtual float velocitat() const = 0;
	virtual std::string nom() const = 0;   // per al HUD i el menu

	// Retorna un estat NOU si cal canviar, o nullptr si ens quedem igual
	virtual DificultatEstat* seguent(int punts) const = 0;

	// Crea l'estat inicial que s'ha triat al menu (0 facil, 1 mitja, 2 dificil)
	static DificultatEstat* crea(int nivell);
	static std::string nomNivell(int nivell);
};
