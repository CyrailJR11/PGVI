#include "Hud.h"
#include "Config.h"
#include "GestorRecursos.h"
#include <cstdio>

// Dona el mateix estil a tots els textos del HUD
static void estil(sf::Text& t, unsigned int mida) {
    t.setCharacterSize(mida);
    t.setFillColor(sf::Color::White);
    t.setOutlineColor(sf::Color::Black);
    t.setOutlineThickness(2.0f);
}

Hud::Hud()
    : m_textPunts(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO)),
      m_textVides(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO)),
      m_textTemps(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO)),
      m_textInfo(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO)),
      m_textFps(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO)),
      m_iconaVida(GestorTextures::getInstance().get(Config::TEXTURA_VIDA))
{
    estil(m_textPunts, 28);
    estil(m_textVides, 28);
    estil(m_textTemps, 28);
    estil(m_textInfo, 22);
    estil(m_textFps, 18);
    m_textFps.setFillColor(sf::Color::Yellow);

    m_textPunts.setPosition({ 20.0f, 12.0f });
    m_textVides.setPosition({ 20.0f, 52.0f });
    m_textVides.setString("Vides:");
    m_textInfo.setPosition({ 20.0f, 100.0f });
    m_textFps.setPosition({ 10.0f, Config::ALTURA_FINESTRA - 30.0f });

    m_iconaVida.setScale({ 0.30f, 0.30f });   // 91x124 -> ~27x37
}

std::string Hud::formatTemps(float segons) {
    int total = (int)segons;
    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%02d:%02d", total / 60, total % 60);
    return buffer;
}

void Hud::actualitza(int punts, int vides, float tempsPartida,
                     std::size_t bales, const std::string& dificultat) {
    m_vides = vides;
    m_textPunts.setString("Punts: " + std::to_string(punts));

    m_textTemps.setString("Temps " + formatTemps(tempsPartida));
    m_textTemps.setOrigin({ m_textTemps.getLocalBounds().size.x / 2.0f, 0.0f });
    m_textTemps.setPosition({ Config::AMPLADA_FINESTRA * 0.5f, 12.0f });

    m_textInfo.setString("Bales: " + std::to_string(bales) + "/" + std::to_string(Config::MAX_BALES)
                         + "    Nivell: " + dificultat);
    m_textInfo.setOrigin({ m_textInfo.getLocalBounds().size.x, 0.0f });
    m_textInfo.setPosition({ Config::AMPLADA_FINESTRA - 20.0f, 16.0f });
}

void Hud::actualitzaFps(float fps, bool limitActiu, bool tilemapOptimitzat) {
    m_textFps.setString("FPS: " + std::to_string((int)(fps + 0.5f))
        + "   [F2] limit: " + (limitActiu ? "60" : "OFF")
        + "   [F4] tilemap: " + (tilemapOptimitzat ? "VertexArray (optimitzat)" : "sprites (sense optimitzar)"));
}

void Hud::draw(sf::RenderTarget& target) const {
    target.draw(m_textPunts);
    target.draw(m_textVides);
    target.draw(m_textTemps);
    target.draw(m_textInfo);

    // Una icona per cada vida que queda
    sf::Sprite icona = m_iconaVida;
    float x = m_textVides.getPosition().x + m_textVides.getLocalBounds().size.x + 14.0f;
    for (int i = 0; i < m_vides; ++i) {
        icona.setPosition({ x + i * 34.0f, 50.0f });
        target.draw(icona);
    }
}

void Hud::drawFps(sf::RenderTarget& target) const {
    target.draw(m_textFps);
}
