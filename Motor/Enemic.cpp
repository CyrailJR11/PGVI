#include "Enemic.h"

Enemic::Enemic(sf::Vector2f posicio, float velocitat, const std::string& textura)
    : m_sprite(GestorTextures::getInstance().get(textura)),
      m_velocitat(velocitat)
{
    m_sprite.setOrigin(m_sprite.getLocalBounds().getCenter());   // origen al centre
    m_sprite.setPosition(posicio);
}

void Enemic::draw(sf::RenderWindow& finestra) const {
    finestra.draw(m_sprite);
}

bool Enemic::foraDePantalla() const {
    // marge d'una amplada d'sprite: desapareix quan ja no es veu gens
    return m_sprite.getPosition().x < -m_sprite.getGlobalBounds().size.x;
}
