#pragma once
// ============================================================
//  GestorRecursos.h - PATRO SINGLETON + PLANTILLA (punt 7)
//  Generalitzacio de l'antic GestorTextures (TextureHolder):
//  ara emmagatzema QUALSEVOL tipus de recurs de SFML.
//
//    GestorRecursos<sf::Texture>      -> textures
//    GestorRecursos<sf::Font>         -> fonts
//    GestorRecursos<sf::SoundBuffer>  -> sons
//
//  Cada recurs es carrega UNA SOLA VEGADA i es reutilitza.
//
//  CORRECCIO INC-01: si un fitxer no existeix, el joc ja no es tanca.
//  Es fa servir un SUBSTITUT (textura de quadres magenta o so en silenci)
//  i s'apunta un avis. Nomes les fonts son obligatories (sense font no
//  es pot dibuixar cap text): en aquest cas es llanca un error clar.
// ============================================================
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <map>
#include <stdexcept>
#include <string>
#include "Avisos.h"

// ---- Substituts per a cada tipus de recurs ----
// Textura: quadres magenta i negre de 64x64 (es veu de seguida que falta)
inline bool creaSubstitut(sf::Texture& textura) {
    sf::Image imatge(sf::Vector2u(64, 64), sf::Color::Magenta);
    for (unsigned int y = 0; y < 64; ++y)
        for (unsigned int x = 0; x < 64; ++x)
            if (((x / 16) + (y / 16)) % 2 == 0)
                imatge.setPixel({ x, y }, sf::Color::Black);
    return textura.loadFromImage(imatge);
}

// So: un buffer buit (silenci). El joc continua sense aquest efecte.
inline bool creaSubstitut(sf::SoundBuffer&) {
    return true;
}

// Font: no hi ha substitut possible
inline bool creaSubstitut(sf::Font&) {
    return false;
}

template <typename T>
class GestorRecursos {
public:
    // Punt d'acces global a la unica instancia de cada tipus (Meyers Singleton)
    static GestorRecursos& getInstance() {
        static GestorRecursos instancia;
        return instancia;
    }

    // Prohibim copiar (no te sentit copiar un singleton)
    GestorRecursos(const GestorRecursos&)            = delete;
    GestorRecursos& operator=(const GestorRecursos&) = delete;

    // Retorna el recurs del fitxer; el carrega nomes la primera vegada.
    // Retorna una referencia NO const perque es pugui configurar
    // (per exemple setSmooth(false) a les textures del parallax).
    T& get(const std::string& nomFitxer) {
        auto it = m_recursos.find(nomFitxer);
        if (it != m_recursos.end())
            return it->second;                       // ja estava carregat

        try {
            // Es construeix directament dins del map (sense copies)
            auto resultat = m_recursos.try_emplace(nomFitxer, std::filesystem::path(nomFitxer));
            return resultat.first->second;
        }
        catch (const std::exception&) {
            T substitut;
            if (!creaSubstitut(substitut))
                throw std::runtime_error("Falta un recurs obligatori: " + nomFitxer);

            Avisos::afegeix("Falta " + nomFitxer + " -> es fa servir un substitut");
            // Es guarda amb el mateix nom: no es tornara a intentar carregar
            auto resultat = m_recursos.emplace(nomFitxer, std::move(substitut));
            return resultat.first->second;
        }
    }

    std::size_t quantitat() const { return m_recursos.size(); }

private:
    GestorRecursos() = default;   // unic constructor: privat
    std::map<std::string, T> m_recursos;
};

// Noms curts per no haver d'escriure la plantilla cada vegada.
// GestorTextures es mante perque el codi antic segueixi funcionant.
using GestorTextures = GestorRecursos<sf::Texture>;
using GestorFonts    = GestorRecursos<sf::Font>;
using GestorSons     = GestorRecursos<sf::SoundBuffer>;
