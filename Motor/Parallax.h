#pragma once
// ============================================================
//  Parallax.h - efecte Parallax (punt 8)
//  Cada capa es mou a una fraccio de la velocitat de la camera:
//    cel (sky.png)      -> factor 0.10 (molt lluny, es mou poc)
//    paisatge (bg.png)  -> factor 0.45 (mes a prop, es mou mes)
//    tilemap            -> factor 1.00 (es mou igual que la camera)
//  Aquesta diferencia de velocitats dona sensacio de profunditat.
//
//  Tecnica: cada capa es dibuixa DUES vegades, una al costat de
//  l'altra. Quan la primera surt per l'esquerra, la segona ja omple
//  el forat, i aixi la imatge sembla infinita.
//  Les capes es dibuixen amb la camera de pantalla (vista UI).
// ============================================================
#include <SFML/Graphics.hpp>
#include <string>

class CapaParallax {
public:
    // factor: 0 = quiet, 1 = igual que la camera
    // velocitatAuto: es mou sola encara que la camera estigui quieta (px/s)
    CapaParallax(const std::string& textura, float factor, float velocitatAuto = 0.0f);

    void update(float dt, float cameraX);
    void draw(sf::RenderTarget& target) const;

private:
    sf::Sprite m_sprite;
    float m_factor;
    float m_velocitatAuto;
    float m_desplacamentAuto = 0.0f;
    float m_amplada = 0.0f;     // amplada de la capa a la pantalla
    float m_x = 0.0f;           // posicio de la primera copia
};

class Parallax {
public:
    Parallax();
    void update(float dt, float cameraX);
    void draw(sf::RenderTarget& target) const;

private:
    CapaParallax m_cel;
    CapaParallax m_paisatge;
};
