#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "GestorTextures.h"

// ============================================================
//  Bala.h - el projectil del joc (copia adaptada de l'Enemic)
//  Neix a la posicio de l'heroi, vola recte cap a la dreta
//  i desapareix quan surt de pantalla o toca un enemic.
// ============================================================
class Bala {
public:
    Bala(sf::Vector2f posicio, float velocitat);

    void update(float dt);
    void draw(sf::RenderWindow& finestra) const;
    bool foraDePantalla(float amplada) const;
    const sf::Sprite& getSprite() const { return m_sprite; }

private:
    sf::Sprite m_sprite;
    float m_velocitat = 0.0f;   // positiva = cap a la dreta
};
