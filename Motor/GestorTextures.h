#pragma once
// ============================================================
//  GestorTextures.h - PATRO SINGLETON
//  Carrega cada textura UNA SOLA VEGADA i la reutilitza.
//  Tota la carrega de sprites del joc passa per aqui.
// ============================================================
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <stdexcept>

class GestorTextures {
public:
    // Punt d'acces global a la unica instancia (Meyers Singleton)
    static GestorTextures& getInstance() {
        static GestorTextures instancia;
        return instancia;
    }

    // Prohibim copiar (no te sentit copiar un singleton)
    GestorTextures(const GestorTextures&)            = delete;
    GestorTextures& operator=(const GestorTextures&) = delete;

    // Retorna la textura del fitxer; la carrega nomes la primera vegada
    const sf::Texture& get(const std::string& nomFitxer) {
        auto it = m_textures.find(nomFitxer);
        if (it != m_textures.end()) {
            return it->second;                       // ja estava carregada
        }
        sf::Texture textura;
        if (!textura.loadFromFile(nomFitxer)) {
            throw std::runtime_error("No s'ha pogut carregar: " + nomFitxer);
        }
        auto resultat = m_textures.emplace(nomFitxer, std::move(textura));
        return resultat.first->second;
    }

private:
    GestorTextures() = default;   // unic constructor: privat
    std::map<std::string, sf::Texture> m_textures;
};
