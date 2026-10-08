#include "EnemicAeri.h"
#include <cmath>

EnemicAeri::EnemicAeri(sf::Vector2f posicio, float velocitat)
    : Enemic(posicio, velocitat, Config::TEXTURA_ENEMIC_AERI),
      m_alturaBase(posicio.y)
{
    m_sprite.setColor(sf::Color(255, 140, 140));   // tint per distingir-lo del terrestre
}

void EnemicAeri::update(float dt) {
    m_temps += dt;
    sf::Vector2f pos = m_sprite.getPosition();
    pos.x += m_velocitat * dt;
    pos.y = m_alturaBase + Config::AMPLADA_ONDULACIO
                         * std::sin(Config::FREQUENCIA_ONDULACIO * m_temps);
    m_sprite.setPosition(pos);
}
