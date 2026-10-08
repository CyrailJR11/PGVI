#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Config.h"
#include "GestorTextures.h"

// ============================================================
//  Enemic.h - classe ABSTRACTA base de tots els enemics
//  Cada fill decideix com es mou implementant update(dt).
//  (Serveix de plantilla per a la classe Bala)
// ============================================================
class Enemic {
public:
    Enemic(sf::Vector2f posicio, float velocitat, const std::string& textura);
    virtual ~Enemic() = default;

    virtual void update(float dt) = 0;        // polimorfisme: cada fill es mou diferent
    void draw(sf::RenderWindow& finestra) const;
    bool foraDePantalla() const;              // ha sortit per l'esquerra?
    const sf::Sprite& getSprite() const { return m_sprite; }

protected:
    sf::Sprite m_sprite;
    float m_velocitat = 0.0f;                 // negativa = cap a l'esquerra
};
