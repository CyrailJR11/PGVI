#include "EnemicSaltador.h"

EnemicSaltador::EnemicSaltador(sf::Vector2f posicio, float velocitat)
    : Enemic(posicio, velocitat, Config::TEXTURA_ENEMIC_SALTADOR),
      m_terra(posicio.y)
{
    m_velocitat.y = -Config::VELOCITAT_SALT_ENEMIC;
}

void EnemicSaltador::update(float dt) {
    // Mateixa fisica que l'heroi: la gravetat el fa baixar
    m_velocitat.y += Config::MASSA_HEROI * Config::GRAVETAT * dt;

    sf::Vector2f pos = m_sprite.getPosition();
    pos.x += m_velocitat.x * dt;
    pos.y += m_velocitat.y * dt;

    if (pos.y >= m_terra) {                            // toca terra: torna a saltar
        pos.y = m_terra;
        m_velocitat.y = -Config::VELOCITAT_SALT_ENEMIC;
    }
    m_sprite.setPosition(pos);
}
