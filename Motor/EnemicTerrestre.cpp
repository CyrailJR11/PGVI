#include "EnemicTerrestre.h"

EnemicTerrestre::EnemicTerrestre(sf::Vector2f posicio, float velocitat)
    : Enemic(posicio, velocitat, Config::TEXTURA_ENEMIC_TERRESTRE) {
}

void EnemicTerrestre::update(float dt) {
    m_sprite.move({ m_velocitat * dt, 0.0f });   // linia recta, velocitat negativa
}
