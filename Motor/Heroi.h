#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "GestorTextures.h"

class Heroi {
public:
    Heroi();

    void update(float dt);
    void salta(float velocitat);
    void draw(sf::RenderWindow& finestra) const;

    sf::Vector2f getPosicio() const { return m_sprite.getPosition(); }

private:
    sf::Sprite  m_sprite;
    sf::Vector2f m_posicio;

    float m_velocitat  = 0.0f;
    float m_tempsAnim  = 0.0f;
    int   m_salts      = 0;
    bool  m_aTerra     = false;
};
