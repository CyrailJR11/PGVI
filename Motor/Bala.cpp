#include "Bala.h"

// Textura des del Singleton i origen al centre
Bala::Bala(sf::Vector2f posicio, float velocitat)
    : m_sprite(GestorTextures::getInstance().get(Config::TEXTURA_BALA)),
      m_velocitat(velocitat)
{
    m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());
    m_sprite.setPosition(posicio);
    if (velocitat < 0.0f)
        m_sprite.setScale({ -1.0f, 1.0f });   // el coet mira cap a l'esquerra
}

// Moviment recte, independent dels FPS
void Bala::update(float dt) {
    m_sprite.move({ m_velocitat * dt, 0.0f });
}

void Bala::draw(sf::RenderTarget& target) const {
    target.draw(m_sprite);
}

// true quan ha sortit per qualsevol costat de la camera
bool Bala::foraDePantalla(float limitEsquerra, float limitDreta) const {
    float x = m_sprite.getPosition().x;
    return x > limitDreta || x < limitEsquerra;
}
