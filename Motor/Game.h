#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "Heroi.h"
#include "Bala.h"
#include "Enemic.h"
#include "EnemicTerrestre.h"
#include "EnemicAeri.h"
#include "GestorTextures.h"

// ============================================================
//  Motor - classe principal del joc
//  Conte el bucle del joc, la finestra i tots els objectes.
// ============================================================
class Game {
public:
    Game();
    ~Game();                 // allibera els punters

    void run();           // bucle principal del joc

private:
    void processarEntrada();
    void actualitzar(float dt);
    void dibuixar();

    void dispara();                       // PAS 5: crea una Bala nova
    void generaEnemic();
    bool hiHaColisio(const sf::Sprite& a, const sf::Sprite& b) const;   // AABB
    void actualitzaTextPuntuacio();

    // Finestra
    sf::RenderWindow m_finestra;

    // Fons
    sf::Sprite m_cel;
    sf::Sprite m_fons;

    // Entitats del joc
    Heroi                m_heroi;
    std::vector<Bala*>   m_bales;         // PAS 5: les bales vives
    std::vector<Enemic*> m_enemics;       // polimorfisme: terrestres i aeris

    // Estat del joc
    bool  m_gameOver         = false;
    int   m_puntuacio        = 0;
    float m_tempsActual      = 0.0f;
    float m_tempsUltimEnemic = 0.0f;

    // Textos
    sf::Font m_fontTitol;
    sf::Font m_fontText;
    sf::Text m_textTitol;
    sf::Text m_textTutorial;
    sf::Text m_textPuntuacio;

    // PAS 9: so (el buffer s'ha de declarar ABANS que el sf::Sound que l'usa)
    sf::SoundBuffer m_bufferDispar;
    sf::SoundBuffer m_bufferImpacte;
    sf::Sound       m_soDispar;
    sf::Sound       m_soImpacte;
};
