#pragma once
#include "PlayableCharacter.h"
#include "TileMap.h"

// ============================================================
//  Heroi - el personatge del jugador (hereta de PlayableCharacter)
//  - Es mou a esquerra/dreta, salta (doble salt) i xoca amb el tilemap
//  - Animacio (punt 1.1): quiet, caminant i saltant; es gira segons
//    cap on mira i parpelleja quan es invulnerable
// ============================================================
class Heroi : public PlayableCharacter {
public:
    Heroi();

    void update(float dt) override;                     // funcio virtual redefinida
    void draw(sf::RenderTarget& target) const override;
    sf::FloatRect getHitbox() const override;

    void setMapa(const TileMap* mapa) { m_mapa = mapa; }
    void setDireccio(float direccio) { m_direccio = direccio; }   // -1, 0 o 1
    void salta(float velocitat);

    void reinicia(sf::Vector2f posicio);     // nova partida
    void reapareix();                        // despres de caure a l'aigua o punxes
    void ferInvulnerable();

    bool esInvulnerable()  const { return m_tempsInvulnerable > 0.0f; }
    bool haTocatPerill()   const { return m_tocaPerill; }
    bool estaATerra()      const { return m_aTerra; }
    int  getSalts()        const { return m_salts; }
    float getMirada()      const { return m_mirada; }   // 1 dreta, -1 esquerra

private:
    void mouHoritzontal(float dt);
    void mouVertical(float dt);
    void animacio(float dt);

    const TileMap* m_mapa = nullptr;

    float m_direccio   = 0.0f;   // entrada del teclat
    float m_mirada     = 1.0f;   // cap on mira el sprite
    float m_tempsAnim  = 0.0f;
    int   m_salts      = 0;
    bool  m_aTerra     = false;
    bool  m_tocaPerill = false;
    float m_tempsInvulnerable = 0.0f;

    sf::Vector2f m_ultimaPosicioSegura;   // on reapareix
};
