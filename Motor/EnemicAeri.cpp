#include "EnemicAeri.h"
#include <cmath>

EnemicAeri::EnemicAeri(sf::Vector2f posicio, float velocitat)
    : Enemic(posicio, velocitat, Config::TEXTURA_ENEMIC_AERI),
      m_alturaBase(posicio.y)
{
}

void EnemicAeri::update(float dt) {
    m_temps += dt;
    sf::Vector2f pos = m_sprite.getPosition();
    pos.x += m_velocitat.x * dt;
    pos.y = m_alturaBase + Config::AMPLADA_ONDULACIO
                         * std::sin(Config::FREQUENCIA_ONDULACIO * m_temps);
    m_sprite.setPosition(pos);
}
