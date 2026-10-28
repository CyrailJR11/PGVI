#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "Config.h"
#include "GestorRecursos.h"
#include "TileMap.h"
#include "Parallax.h"
#include "Heroi.h"
#include "Bala.h"
#include "Enemic.h"
#include "EnemicTerrestre.h"
#include "EnemicAeri.h"
#include "EnemicSaltador.h"
#include "DificultatEstat.h"
#include "Menu.h"
#include "Hud.h"
#include "Avisos.h"
#include <iostream>
#include <cstdio>

// Pantalles (estats) del joc
enum class Pantalla {
    Menu,
    Controls,
    Jugant,
    GameOver
};

// ============================================================
//  Motor - classe principal del joc
//  Conte el bucle del joc, la finestra, les DUES cameres i
//  tots els objectes.
// ============================================================
class Game {
public:
    Game();
    ~Game();                 // allibera els punters

    void run();              // bucle principal del joc

private:
    // ---- Bucle ----
    void processarEntrada();
    void entradaMenu(sf::Keyboard::Key tecla);
    void entradaJoc(sf::Keyboard::Key tecla);
    void actualitzar(float dt);
    void dibuixar();
    void comptaFps();
    void creaFinestra();     // pantalla completa o finestra (F11)
    void ajustaVistes();     // franges negres per mantenir la proporcio 4:3

    // ---- Partida ----
    void novaPartida();      // tambe serveix per REINICIAR
    void netejaEntitats();
    void dispara();
    void generaEnemic(float desplacament = 0.0f);
    void perdVida();
    void actualitzaCamera();
    bool hiHaColisio(const sf::FloatRect& a, const sf::FloatRect& b) const;   // AABB
    void canviaSo(bool actiu);
    void reprodueix(sf::Sound& so);

    // ---- Mesura de rendiment (F5) ----
    void iniciaMesura();
    void registraFrameMesura(float dtReal);
    void acabaMesura();

    // Finestra
    sf::RenderWindow m_finestra;

    // DUES CAMERES (sf::View)
    sf::View m_vistaJoc;     // camera 1: segueix l'heroi pel mon (tilemap, enemics...)
    sf::View m_vistaUI;      // camera 2: fixa a la pantalla (menu, HUD, parallax)

    // Mon i capes
    TileMap  m_mapa;
    Parallax m_parallax;

    // Entitats del joc
    Heroi                m_heroi;
    std::vector<Bala*>   m_bales;
    std::vector<Enemic*> m_enemics;       // polimorfisme: terrestres, aeris i saltadors

    // Interficie
    Menu m_menu;
    Hud  m_hud;
    sf::Text m_textGameOver;
    sf::Text m_textResultat;
    sf::Text m_textPausa;
    sf::RectangleShape m_fosc;
    sf::Text m_textAvisos;        // incidencies de carrega (INC-01/02/03)
    sf::Text m_textMesura;        // resultat de la mesura de FPS
    sf::RectangleShape m_panellMesura;

    // Estat del joc
    Pantalla m_pantalla       = Pantalla::Menu;
    bool  m_pausa             = false;
    int   m_puntuacio         = 0;
    int   m_vides             = Config::VIDES_INICIALS;
    float m_tempsPartida      = 0.0f;
    float m_tempsActual       = 0.0f;
    float m_tempsUltimEnemic  = 0.0f;

    // Opcions del menu
    int  m_dificultatInicial = 0;     // 0 facil, 1 mitja, 2 dificil
    bool m_soActiu           = true;

    // PATRO ESTATS: dificultat actual (Game n'es el propietari)
    DificultatEstat* m_estat = nullptr;

    // So (els buffers son al GestorRecursos<sf::SoundBuffer>)
    sf::Sound m_soDispar;
    sf::Sound m_soImpacte;
    sf::Music m_musica;
    bool      m_musicaCarregada = false;

    // Mesura de rendiment (punt 4: FPS abans/despres de les millores)
    sf::Clock m_rellotgeFps;
    int   m_framesComptats    = 0;
    float m_fps               = 0.0f;
    bool  m_limitFps          = true;    // F2
    bool  m_tilemapOptimitzat = true;    // F4
    bool  m_pantallaCompleta  = Config::PANTALLA_COMPLETA;   // F11

    // Mesura de rendiment (F5): FPS mitja, FPS minim i temps de frame mitja
    struct ResultatMesura {
        bool  fet = false;
        float fpsMitja = 0.0f;
        float fpsMinim = 0.0f;
        float msFrame  = 0.0f;
    };
    bool   m_mesurant          = false;
    bool   m_mostraMesura      = false;
    bool   m_limitAbansMesura  = true;
    float  m_tempsMesura       = 0.0f;   // inclou l'escalfament
    double m_sumaDt            = 0.0;
    float  m_dtMaxim           = 0.0f;
    long   m_framesMesura      = 0;
    ResultatMesura m_resultat[2];         // [0] sprites (v1.0)  [1] VertexArray (v2.0)
};
