#pragma once
#include "PlayableCharacter.h"

// ============================================================
//  Enemic.h - classe ABSTRACTA base de tots els enemics
//  Ara hereta de PlayableCharacter (punt 5): el sprite, la
//  velocitat, draw() i getHitbox() venen de la classe base.
//  update(dt) continua sent virtual pura: cada fill es mou diferent.
// ============================================================
class Enemic : public PlayableCharacter {
public:
    Enemic(sf::Vector2f posicio, float velocitat, const std::string& textura);

    void update(float dt) override = 0;           // polimorfisme (punt 5)
    sf::FloatRect getHitbox() const override;     // una mica mes petita que el dibuix

    // Ha sortit per l'esquerra de la camera?
    bool foraDePantalla(float limitEsquerra) const;
};
