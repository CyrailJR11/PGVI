#include "Heroi.h"
#include <cmath>

Heroi::Heroi()
    : PlayableCharacter(Config::TEXTURA_HEROI,
                        { Config::AMPLADA_FINESTRA * 0.25f, Config::ALTURA_TERRA })
{
    // El spritesheet te 4 fotogrames: ens quedem amb el primer i
    // tornem a posar l'origen als peus d'UN fotograma (no de tota la imatge)
    m_sprite.setTextureRect(sf::IntRect({ 0, 0 },
        { Config::HEROI_AMPLADA_FOTOGRAM, Config::HEROI_ALTURA_FOTOGRAM }));
    m_sprite.setOrigin({ Config::HEROI_AMPLADA_FOTOGRAM / 2.0f,
                         (float)Config::HEROI_ALTURA_FOTOGRAM });
    m_ultimaPosicioSegura = m_sprite.getPosition();
}

void Heroi::reinicia(sf::Vector2f posicio) {
    m_sprite.setPosition(posicio);
    m_ultimaPosicioSegura = posicio;
    m_velocitat = { 0.0f, 0.0f };
    m_direccio = 0.0f;
    m_mirada = 1.0f;
    m_salts = 0;
    m_aTerra = false;
    m_tocaPerill = false;
    m_tempsInvulnerable = 0.0f;
    m_tempsAnim = 0.0f;
}

void Heroi::reapareix() {
    m_sprite.setPosition(m_ultimaPosicioSegura);
    m_velocitat = { 0.0f, 0.0f };
    m_salts = 0;
    m_tocaPerill = false;
    ferInvulnerable();
}

void Heroi::ferInvulnerable() {
    m_tempsInvulnerable = Config::TEMPS_INVULNERABLE;
}

void Heroi::salta(float velocitat) {
    if (m_salts < Config::SALTS_MAXIMS) {   // permet el doble salt
        m_salts++;
        m_velocitat.y = -velocitat;         // negatiu = cap amunt
        m_aTerra = false;
    }
}

// Caixa mes petita que el dibuix, centrada als peus
sf::FloatRect Heroi::getHitbox() const {
    sf::Vector2f p = m_sprite.getPosition();
    return sf::FloatRect({ p.x - Config::HEROI_HITBOX_AMPLADA / 2.0f,
                           p.y - Config::HEROI_HITBOX_ALTURA },
                         { Config::HEROI_HITBOX_AMPLADA, Config::HEROI_HITBOX_ALTURA });
}

void Heroi::update(float dt) {
    if (m_tempsInvulnerable > 0.0f)
        m_tempsInvulnerable -= dt;

    // Separem els eixos: primer X i despres Y (col.lisions mes simples)
    mouHoritzontal(dt);
    mouVertical(dt);

    // --- Zones NO trepitjables: aigua i punxes ---
    m_tocaPerill = false;
    if (m_mapa) {
        sf::FloatRect zona = getHitbox();
        zona.position.x += 8.0f;  zona.size.x -= 16.0f;   // una mica de marge
        zona.position.y += 10.0f; zona.size.y -= 10.0f;
        if (m_mapa->tocaPerill(zona))
            m_tocaPerill = true;
        if (getPosicio().y > m_mapa->getAltura() + 200.0f)   // ha caigut del mon
            m_tocaPerill = true;
    }

    animacio(dt);
}

void Heroi::mouHoritzontal(float dt) {
    m_velocitat.x = m_direccio * Config::VELOCITAT_HEROI;
    if (m_direccio != 0.0f)
        m_mirada = m_direccio;

    sf::Vector2f pos = m_sprite.getPosition();
    pos.x += m_velocitat.x * dt;
    m_sprite.setPosition(pos);
    if (!m_mapa || m_velocitat.x == 0.0f) return;

    const float m = (float)Config::MIDA_RAJOLA;
    const float meitat = Config::HEROI_HITBOX_AMPLADA / 2.0f;
    sf::FloatRect hb = getHitbox();
    int f0 = (int)std::floor(hb.position.y / m);
    int f1 = (int)std::floor((hb.position.y + hb.size.y - 0.01f) / m);

    // Nomes cal mirar la columna cap on avancem
    int col = (m_velocitat.x > 0.0f)
        ? (int)std::floor((hb.position.x + hb.size.x - 0.01f) / m)
        : (int)std::floor(hb.position.x / m);

    for (int f = f0; f <= f1; ++f) {
        if (m_mapa->esSolida(col, f)) {
            if (m_velocitat.x > 0.0f) pos.x = col * m - meitat - 0.01f;
            else                      pos.x = (col + 1) * m + meitat + 0.01f;
            m_sprite.setPosition(pos);
            break;
        }
    }
}

void Heroi::mouVertical(float dt) {
    // Gravetat (mateixa formula que abans: massa * gravetat)
    m_velocitat.y += Config::MASSA_HEROI * Config::GRAVETAT * dt;

    sf::Vector2f pos = m_sprite.getPosition();
    float peusAbans = pos.y;
    pos.y += m_velocitat.y * dt;
    m_sprite.setPosition(pos);

    m_aTerra = false;
    if (m_mapa) {
        const float m = (float)Config::MIDA_RAJOLA;
        sf::FloatRect hb = getHitbox();
        int c0 = (int)std::floor(hb.position.x / m);
        int c1 = (int)std::floor((hb.position.x + hb.size.x - 0.01f) / m);

        if (m_velocitat.y > 0.0f) {
            // Caient: mirem la fila on son els peus
            int fila = (int)std::floor((pos.y - 0.01f) / m);
            float dalt = fila * m;   // la part de dalt d'aquesta fila
            for (int c = c0; c <= c1; ++c) {
                bool solida = m_mapa->esSolida(c, fila);
                // Les plataformes nomes aguanten si veniem de dalt
                bool plataforma = m_mapa->esPlataforma(c, fila) && peusAbans <= dalt + 1.0f;
                if (solida || plataforma) {
                    pos.y = dalt;
                    m_velocitat.y = 0.0f;
                    m_aTerra = true;
                    m_salts = 0;
                    m_sprite.setPosition(pos);
                    break;
                }
            }
        }
        else if (m_velocitat.y < 0.0f) {
            // Pujant: si el cap toca una rajola solida, parem
            int fila = (int)std::floor(hb.position.y / m);
            for (int c = c0; c <= c1; ++c) {
                if (m_mapa->esSolida(c, fila)) {
                    pos.y = (fila + 1) * m + Config::HEROI_HITBOX_ALTURA;
                    m_velocitat.y = 0.0f;
                    m_sprite.setPosition(pos);
                    break;
                }
            }
        }
    }

    // Si cau d'un precipici sense saltar, nomes li queda 1 salt a l'aire
    if (!m_aTerra && m_salts == 0)
        m_salts = 1;

    // Guardem l'ultima posicio segura (els dos peus sobre terra ferma)
    if (m_aTerra && m_mapa) {
        const float m = (float)Config::MIDA_RAJOLA;
        sf::FloatRect hb = getHitbox();
        int fila = (int)std::floor(pos.y / m);
        int cEsq = (int)std::floor(hb.position.x / m);
        int cDre = (int)std::floor((hb.position.x + hb.size.x) / m);
        bool esqOk = m_mapa->esSolida(cEsq, fila) || m_mapa->esPlataforma(cEsq, fila);
        bool dreOk = m_mapa->esSolida(cDre, fila) || m_mapa->esPlataforma(cDre, fila);
        if (esqOk && dreOk)
            m_ultimaPosicioSegura = pos;
    }
}

// ---- Punt 1.1: animacio del personatge principal ----
void Heroi::animacio(float dt) {
    int fotograma = 0;

    if (!m_aTerra) {
        fotograma = 1;                                  // a l'aire: postura fixa
    }
    else if (m_direccio != 0.0f) {
        m_tempsAnim += dt;                              // caminant: recorre els 4 fotogrames
        fotograma = (int)((m_tempsAnim / Config::HEROI_DURACIO_ANIM) * Config::HEROI_FOTOGRAMS)
                    % Config::HEROI_FOTOGRAMS;
    }
    else {
        m_tempsAnim = 0.0f;                             // quiet: primer fotograma
    }

    m_sprite.setTextureRect(sf::IntRect({ fotograma * Config::HEROI_AMPLADA_FOTOGRAM, 0 },
                                        { Config::HEROI_AMPLADA_FOTOGRAM,
                                          Config::HEROI_ALTURA_FOTOGRAM }));

    // Mirada: escala negativa = imatge girada (l'origen es al centre, no es desplaca)
    m_sprite.setScale({ m_mirada, 1.0f });
}

void Heroi::draw(sf::RenderTarget& target) const {
    // Parpelleig mentre es invulnerable: es dibuixa un frame si i un no (cada 0,1 s)
    if (esInvulnerable() && ((int)(m_tempsInvulnerable * 10.0f) % 2 == 0))
        return;
    PlayableCharacter::draw(target);
}
