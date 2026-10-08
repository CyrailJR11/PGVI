#include "Heroi.h"


Heroi::Heroi() : m_sprite(GestorTextures::getInstance().get(Config::TEXTURA_HEROI)) {
    m_sprite.setOrigin(sf::Vector2f((float)Config::HEROI_AMPLADA_FOTOGRAM,
                                    (float)Config::HEROI_ALTURA_FOTOGRAM) / 2.0f);
    m_posicio = { Config::AMPLADA_FINESTRA * 0.25f, Config::ALTURA_FINESTRA * 0.5f };
    m_sprite.setPosition(m_posicio);
}

void Heroi::salta(float velocitat) {
    if (m_salts < Config::SALTS_MAXIMS) {   // permet el doble salt
        m_salts++;
        m_velocitat = velocitat;
        m_aTerra = false;
    }
}

void Heroi::update(float dt) {
    // --- Animacio: recorrem els fotogrames del spritesheet ---
    m_tempsAnim += dt;
    int fotograma = (int)((m_tempsAnim / Config::HEROI_DURACIO_ANIM) * Config::HEROI_FOTOGRAMS)
                    % Config::HEROI_FOTOGRAMS;
    m_sprite.setTextureRect(sf::IntRect({ fotograma * Config::HEROI_AMPLADA_FOTOGRAM, 0 },
                                        { Config::HEROI_AMPLADA_FOTOGRAM,
                                          Config::HEROI_ALTURA_FOTOGRAM }));

    // --- Fisica: gravetat i moviment vertical ---
    m_velocitat -= Config::MASSA_HEROI * Config::GRAVETAT * dt;
    m_posicio.y -= m_velocitat * dt;
    m_sprite.setPosition(m_posicio);

    // --- Terra: quan arriba a baix, es reinicia el salt ---
    if (m_posicio.y >= Config::ALTURA_TERRA) {
        m_posicio.y = Config::ALTURA_TERRA;
        m_velocitat = 0.0f;
        m_aTerra = true;
        m_salts = 0;
    }
}

void Heroi::draw(sf::RenderWindow& finestra) const {
    finestra.draw(m_sprite);
}
