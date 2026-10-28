#include "Enemic.h"

Enemic::Enemic(sf::Vector2f posicio, float velocitat, const std::string& textura)
    : PlayableCharacter(textura, posicio)
{
    m_velocitat.x = velocitat;   // negativa = cap a l'esquerra
}

sf::FloatRect Enemic::getHitbox() const {
    // 80% del sprite: el xoc es mes just
    sf::FloatRect b = m_sprite.getGlobalBounds();
    b.position.x += b.size.x * 0.1f;
    b.position.y += b.size.y * 0.1f;
    b.size.x *= 0.8f;
    b.size.y *= 0.8f;
    return b;
}

bool Enemic::foraDePantalla(float limitEsquerra) const {
    // marge d'una amplada d'sprite: desapareix quan ja no es veu gens
    return m_sprite.getPosition().x < limitEsquerra - m_sprite.getGlobalBounds().size.x;
}
