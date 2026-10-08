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

    // ---- Terreny (on aterra l'heroi) ----
    constexpr float ALTURA_TERRA = ALTURA_FINESTRA * 0.75f;

    // ---- Fisica de l'heroi ----
    constexpr float GRAVETAT         = 9.80f;
    constexpr float MASSA_HEROI      = 200.0f;
    constexpr float VELOCITAT_SALT   = 750.0f;
    constexpr int   SALTS_MAXIMS     = 2;      // doble salt

    // ---- Animacio de l'heroi (spritesheet: 4 fotogrames de 92x126) ----
    constexpr int   HEROI_FOTOGRAMS         = 4;
    constexpr float HEROI_DURACIO_ANIM      = 1.0f;
    constexpr int   HEROI_AMPLADA_FOTOGRAM  = 92;
    constexpr int   HEROI_ALTURA_FOTOGRAM   = 126;

    // ---- Bala ----
    constexpr float VELOCITAT_BALA = 400.0f;
    constexpr std::size_t MAX_BALES = 5;   // extensio: maxim de bales simultanies

    // ---- Enemics: alçada de cada fila respecte al terra ----
    constexpr float ALTURA_FILA_ENEMIC[3]      = { 0.0f, 115.0f, 270.0f };
    constexpr float INCREMENT_VELOCITAT_FILA   = 125.0f;

    // ---- Dificultat (PATRO ESTATS) ----
    constexpr int   PUNTS_PER_PUJAR_NIVELL = 5;
    constexpr float INTERVAL_FACIL         = 1.5f;   // segons entre enemics
    constexpr float INTERVAL_MITJA         = 1.0f;
    constexpr float INTERVAL_DIFICIL       = 0.6f;
    constexpr float VELOCITAT_FACIL        = 300.0f;
    constexpr float VELOCITAT_MITJA        = 450.0f;
    constexpr float VELOCITAT_DIFICIL      = 600.0f;

    // ---- Enemic aeri (moviment ondulant) ----
    constexpr float AMPLADA_ONDULACIO    = 30.0f;
    constexpr float FREQUENCIA_ONDULACIO = 3.0f;

    // ---- Vides ----
    constexpr int VIDES_INICIALS = 3;

    // ---- Color de fons (per si falla la textura del cel) ----
    inline const sf::Color COLOR_FONS = sf::Color{ 135, 206, 235 };

    // ---- Fitxers (Assets) ----
    inline const std::string TEXTURA_CEL   = "Assets/graphics/sky.png";
    inline const std::string TEXTURA_FONS  = "Assets/graphics/bg.png";
    inline const std::string TEXTURA_HEROI = "Assets/graphics/heroAnim.png";
    inline const std::string TEXTURA_BALA  = "Assets/graphics/rocket.png";

    // De moment tots dos tipus d'enemic usen la mateixa imatge;
    // l'aeri es distingeix amb un tint de color.
    inline const std::string TEXTURA_ENEMIC_TERRESTRE = "Assets/graphics/enemy.png";
    inline const std::string TEXTURA_ENEMIC_AERI      = "Assets/graphics/enemy.png";

    inline const std::string FONT_TITOL     = "Assets/fonts/SnackerComic.ttf";
    inline const std::string FONT_PUNTUACIO = "Assets/fonts/arial.ttf";

    inline const std::string SO_MUSICA  = "Assets/audio/bgMusic.ogg";
    inline const std::string SO_DISPAR  = "Assets/audio/fire.ogg";
    inline const std::string SO_IMPACTE = "Assets/audio/hit.ogg";

}
