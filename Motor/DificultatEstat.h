#pragma once

// ============================================================
//  DificultatEstat - PATRO ESTATS
//  Cada estat diu cada quant surten enemics, a quina velocitat
//  i a quin estat s'ha de passar segons la puntuacio.
// ============================================================
class DificultatEstat {
public:
	virtual ~DificultatEstat() = default;
	virtual float interval() const = 0;
	virtual float velocitat() const = 0;

	// Retorna un estat NOU si cal canviar, o nullptr si ens quedem igual
	virtual DificultatEstat* seguent(int punts) const = 0;
};
