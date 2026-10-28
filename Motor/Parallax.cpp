#include "Parallax.h"
#include "Config.h"
#include "GestorRecursos.h"
#include <cmath>

CapaParallax::CapaParallax(const std::string& textura, float factor, float velocitatAuto)
    : m_sprite(GestorTextures::getInstance().get(textura)),
      m_factor(factor),
      m_velocitatAuto(velocitatAuto)
{
    sf::Texture& tex = GestorTextures::getInstance().get(textura);
    tex.setSmooth(false);   // pixel art: sense suavitzar, pixels nitids

    // Deixem fora la primera i l'ultima columna de pixels: a bg.png son
    // semitransparents i farien una linia vertical a la unio de les copies
    sf::Vector2u mida = tex.getSize();
    m_sprite.setTextureRect(sf::IntRect({ 1, 0 }, { (int)mida.x - 2, (int)mida.y }));

    // Escalem nomes segons l'alcada: aixi la imatge no es deforma
    float escala = (float)Config::ALTURA_FINESTRA / mida.y;
    m_sprite.setScale({ escala, escala });
    m_amplada = (mida.x - 2) * escala;   // amplada d'una copia a la pantalla
}

void CapaParallax::update(float dt, float cameraX) {
    m_desplacamentAuto += m_velocitatAuto * dt;

    // Quant s'ha mogut la capa: camera * factor + moviment automatic
    float desplacament = std::fmod(cameraX * m_factor + m_desplacamentAuto, m_amplada);
    if (desplacament < 0.0f)
        desplacament += m_amplada;

    m_x = -desplacament;   // la capa va cap a l'esquerra quan la camera va a la dreta
}

void CapaParallax::draw(sf::RenderTarget& target) const {
    // Dibuixem copies una al costat de l'altra fins a omplir l'amplada de la finestra
    sf::Sprite copia = m_sprite;
    for (float x = m_x; x < (float)Config::AMPLADA_FINESTRA; x += m_amplada) {
        copia.setPosition({ x, 0.0f });
        target.draw(copia);
    }
}

// ------------------------------------------------------------
Parallax::Parallax()
    : m_cel(Config::TEXTURA_CEL, Config::PARALLAX_CEL, Config::PARALLAX_CEL_AUTO),
      m_paisatge(Config::TEXTURA_FONS, Config::PARALLAX_PAISATGE)
{
}

void Parallax::update(float dt, float cameraX) {
    m_cel.update(dt, cameraX);
    m_paisatge.update(dt, cameraX);
}

void Parallax::draw(sf::RenderTarget& target) const {
    m_cel.draw(target);        // primer el mes llunya
    m_paisatge.draw(target);
}
