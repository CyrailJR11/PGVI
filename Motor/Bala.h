#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "GestorRecursos.h"

// ============================================================
//  Bala.h - el projectil del joc
//  Neix al cano del bazooka, vola recte cap on mira l'heroi
//  i desapareix quan surt de la camera o toca un enemic.
// ============================================================
class Bala {
public:
    Bala(sf::Vector2f posicio, float velocitat);   // velocitat < 0: cap a l'esquerra

    void update(float dt);
    void draw(sf::RenderTarget& target) const;
    bool foraDePantalla(float limitEsquerra, float limitDreta) const;
    const sf::Sprite& getSprite() const { return m_sprite; }

private:
    sf::Sprite m_sprite;
    float m_velocitat = 0.0f;
};
