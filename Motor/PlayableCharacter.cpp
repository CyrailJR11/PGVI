#include "PlayableCharacter.h"

PlayableCharacter::PlayableCharacter(const std::string& textura, sf::Vector2f posicio)
    : m_sprite(GestorTextures::getInstance().get(textura)),
      m_velocitat(0.0f, 0.0f)
{
    // Origen al centre de la base: la posicio son els peus del personatge
    sf::FloatRect limits = m_sprite.getLocalBounds();
    m_sprite.setOrigin({ limits.size.x / 2.0f, limits.size.y });
    m_sprite.setPosition(posicio);
}

void PlayableCharacter::draw(sf::RenderTarget& target) const {
    target.draw(m_sprite);
}

sf::FloatRect PlayableCharacter::getHitbox() const {
    return m_sprite.getGlobalBounds();
}
