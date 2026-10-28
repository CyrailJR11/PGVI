#pragma once
// ============================================================
//  Hud.h - informacio dibuixada durant la partida (punt 2)
//    - puntuacio
//    - vides restants (amb icones del personatge)
//    - temps de partida (mm:ss)
//    - extra: bales, nivell de dificultat i comptador de FPS
//  Es dibuixa amb la camera UI, per aixo no es mou amb el mon.
// ============================================================
#include <SFML/Graphics.hpp>
#include <string>

class Hud {
public:
    Hud();

    void actualitza(int punts, int vides, float tempsPartida,
                    std::size_t bales, const std::string& dificultat);
    void actualitzaFps(float fps, bool limitActiu, bool tilemapOptimitzat);

    void draw(sf::RenderTarget& target) const;
    void drawFps(sf::RenderTarget& target) const;

    static std::string formatTemps(float segons);   // 83.4 -> "01:23"

private:
    sf::Text   m_textPunts;
    sf::Text   m_textVides;
    sf::Text   m_textTemps;
    sf::Text   m_textInfo;
    sf::Text   m_textFps;
    sf::Sprite m_iconaVida;
    int        m_vides = 0;
};
