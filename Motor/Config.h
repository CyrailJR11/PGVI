#pragma once
// ============================================================
//  Config.h - TOTES les constants del joc en un sol fitxer
// ============================================================
#include <string>
#include <cstddef>
#include <SFML/Graphics/Color.hpp>

namespace Config {

    // ---- Finestra ----
    constexpr unsigned int AMPLADA_FINESTRA = 1024;
    constexpr unsigned int ALTURA_FINESTRA  = 768;
    constexpr unsigned int LIMIT_FPS        = 60;
    constexpr bool PANTALLA_COMPLETA        = true;   // F11 per canviar-ho durant el joc

    // ---- Mesura de rendiment (F5): sempre la mateixa escena ----
    constexpr unsigned int LLAVOR_MESURA     = 12345;  // mateixos enemics cada vegada
    constexpr std::size_t  ENEMICS_MESURA    = 8;      // enemics vius a la vegada
    constexpr float        ESCALFAMENT_MESURA = 1.0f;  // segons que no es compten
    constexpr float        SEGONS_MESURA     = 10.0f;  // segons de mesura

    // ---- Tilemap (punt 1) ----
    constexpr int   MIDA_RAJOLA = 64;                 // cada rajola fa 64x64 px
    // El terra del nivell comenca a la fila 9: 9 * 64 = 576 px
    constexpr float ALTURA_TERRA = ALTURA_FINESTRA * 0.75f;

    // ---- Fisica de l'heroi ----
    constexpr float GRAVETAT         = 9.80f;
    constexpr float MASSA_HEROI      = 200.0f;
    constexpr float VELOCITAT_SALT   = 750.0f;
    constexpr int   SALTS_MAXIMS     = 2;      // doble salt
    constexpr float VELOCITAT_HEROI  = 320.0f; // moviment lateral (px/s)

    // Caixa de col.lisio de l'heroi (mes petita que el dibuix)
    constexpr float HEROI_HITBOX_AMPLADA = 50.0f;
    constexpr float HEROI_HITBOX_ALTURA  = 110.0f;

    // ---- Animacio de l'heroi (spritesheet: 4 fotogrames de 92x126) ----
    constexpr int   HEROI_FOTOGRAMS         = 4;
    constexpr float HEROI_DURACIO_ANIM      = 0.6f;   // un cicle caminant
    constexpr int   HEROI_AMPLADA_FOTOGRAM  = 92;
    constexpr int   HEROI_ALTURA_FOTOGRAM   = 126;

    // ---- Vides i invulnerabilitat ----
    constexpr int   VIDES_INICIALS        = 3;
    constexpr float TEMPS_INVULNERABLE    = 1.5f;   // segons despres de rebre un cop

    // ---- Bala ----
    constexpr float VELOCITAT_BALA = 600.0f;
    constexpr std::size_t MAX_BALES = 5;   // maxim de bales simultanies
    constexpr float ALTURA_CANO    = 62.0f; // la bala surt a aquesta alcada sobre els peus

    // ---- Enemics: alcada de cada fila respecte al terra ----
    constexpr float ALTURA_FILA_ENEMIC[3]      = { 0.0f, 115.0f, 270.0f };
    constexpr float INCREMENT_VELOCITAT_FILA   = 125.0f;

    // ---- Dificultat (PATRO ESTATS) ----
    constexpr int   PUNTS_PER_PUJAR_NIVELL = 5;
    constexpr float INTERVAL_FACIL         = 1.5f;   // segons entre enemics
    constexpr float INTERVAL_MITJA         = 1.0f;
    constexpr float INTERVAL_DIFICIL       = 0.6f;
    constexpr float VELOCITAT_FACIL        = 200.0f;
    constexpr float VELOCITAT_MITJA        = 300.0f;
    constexpr float VELOCITAT_DIFICIL      = 420.0f;

    // ---- Enemic aeri (moviment ondulant) ----
    constexpr float AMPLADA_ONDULACIO    = 30.0f;
    constexpr float FREQUENCIA_ONDULACIO = 3.0f;

    // ---- Enemic saltador ----
    constexpr float VELOCITAT_SALT_ENEMIC = 650.0f;

    // ---- Parallax (punt 8): 0 = quiet, 1 = es mou igual que la camera ----
    constexpr float PARALLAX_CEL          = 0.10f;
    constexpr float PARALLAX_PAISATGE     = 0.45f;
    constexpr float PARALLAX_CEL_AUTO     = 12.0f;  // el cel es mou sol (px/s)

    // ---- Color de fons (per si falla la textura del cel) ----
    inline const sf::Color COLOR_FONS = sf::Color{ 135, 206, 235 };

    // ---- Fitxers (Assets) ----
    inline const std::string NIVELL_1      = "Assets/levels/nivell1.txt";
    inline const std::string TEXTURA_TILES = "Assets/graphics/tiles.png";

    inline const std::string TEXTURA_CEL   = "Assets/graphics/sky.png";
    inline const std::string TEXTURA_FONS  = "Assets/graphics/bg.png";
    inline const std::string TEXTURA_HEROI = "Assets/graphics/heroAnim.png";
    inline const std::string TEXTURA_VIDA  = "Assets/graphics/hero.png";   // icona de vida
    inline const std::string TEXTURA_BALA  = "Assets/graphics/rocket.png";

    // Cada enemic te la seva imatge (tematica Entre Dos Mons)
    inline const std::string TEXTURA_ENEMIC_TERRESTRE = "Assets/graphics/enemic_terrestre.png"; // bolet
    inline const std::string TEXTURA_ENEMIC_AERI      = "Assets/graphics/enemic_aeri.png";      // ratpenat
    inline const std::string TEXTURA_ENEMIC_SALTADOR  = "Assets/graphics/enemic_saltador.png";  // llimac de cristall

    inline const std::string FONT_TITOL     = "Assets/fonts/SnackerComic.ttf";
    inline const std::string FONT_PUNTUACIO = "Assets/fonts/arial.ttf";

    inline const std::string SO_MUSICA  = "Assets/audio/bgMusic.ogg";
    inline const std::string SO_DISPAR  = "Assets/audio/fire.ogg";
    inline const std::string SO_IMPACTE = "Assets/audio/hit.ogg";

}
