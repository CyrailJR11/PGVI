#include "Bala.h"

// PAS 2: textura des del Singleton i origen al centre
Bala::Bala(sf::Vector2f posicio, float velocitat)
    : m_sprite(GestorTextures::getInstance().get(Config::TEXTURA_BALA)),
      m_velocitat(velocitat)
{
    m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());
    m_sprite.setPosition(posicio);
}

// PAS 3: moviment recte cap a la dreta, independent dels FPS
void Bala::update(float dt) {
    m_sprite.move({ m_velocitat * dt, 0.0f });
}

void Bala::draw(sf::RenderWindow& finestra) const {
    finestra.draw(m_sprite);
}

// PAS 4: true quan ha passat la vora dreta de la finestra
bool Bala::foraDePantalla(float amplada) const {
    return m_sprite.getPosition().x > amplada;
}
