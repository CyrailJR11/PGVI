#pragma once
// ============================================================
//  PlayableCharacter.h - classe BASE comuna (punt 5)
//  Conte els atributs i metodes que comparteixen l'Heroi i
//  tots els Enemics: sprite, posicio, velocitat, dibuix i
//  caixa de col.lisio. Cada fill implementa el seu update().
//
//    PlayableCharacter (abstracta)
//      |-- Heroi
//      |-- Enemic (abstracta)
//            |-- EnemicTerrestre
//            |-- EnemicAeri
//            |-- EnemicSaltador
// ============================================================
#include <SFML/Graphics.hpp>
#include <string>
#include "Config.h"
#include "GestorRecursos.h"

class PlayableCharacter {
public:
    PlayableCharacter(const std::string& textura, sf::Vector2f posicio);
    virtual ~PlayableCharacter() = default;   // imprescindible amb herencia

    // FUNCIO VIRTUAL PURA: cada fill decideix com s'actualitza
    virtual void update(float dt) = 0;

    // Virtual: un fill la pot redefinir (l'heroi parpelleja quan es invulnerable)
    virtual void draw(sf::RenderTarget& target) const;

    // Caixa de col.lisio. Per defecte, els limits del sprite.
    virtual sf::FloatRect getHitbox() const;

    sf::Vector2f getPosicio() const { return m_sprite.getPosition(); }
    void setPosicio(sf::Vector2f posicio) { m_sprite.setPosition(posicio); }
    sf::Vector2f getVelocitat() const { return m_velocitat; }
    const sf::Sprite& getSprite() const { return m_sprite; }

protected:
    sf::Sprite   m_sprite;            // origen als PEUS (centre de la base)
    sf::Vector2f m_velocitat;         // px/s. x>0 dreta, y>0 avall (com SFML)
};
