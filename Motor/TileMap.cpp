#include "TileMap.h"
#include "Config.h"
#include "GestorRecursos.h"
#include "Avisos.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>

// Nivell de reserva: s'usa si el fitxer no existeix o esta buit (correccio INC-02)
static const std::vector<std::string> NIVELL_PER_DEFECTE = {
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "................................",
    "..........PPPP........PPPP......",
    "................................",
    "..H.............R...............",
    "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
    "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT",
    "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT",
};

static bool esCaracterValid(char c) {
    return c == '.' || c == 'G' || c == 'T' || c == 'R' || c == 'P' || c == 'A' || c == 'S' || c == 'H';
}

TileMap::TileMap(const std::string& fitxerNivell, const std::string& fitxerTileset) {
    // ---- 1) Llegir les linies del fitxer ----
    std::vector<std::string> linies;
    std::vector<int> numLinia;   // numero de linia real al fitxer (per als avisos)

    std::ifstream fitxer(fitxerNivell);
    if (!fitxer.is_open()) {
        // CORRECCIO INC-02: abans es llancava una excepcio i el joc es tancava
        Avisos::afegeix("No s'ha trobat " + fitxerNivell + " -> es carrega un nivell per defecte");
    }
    else {
        std::string linia;
        int n = 0;
        while (std::getline(fitxer, linia)) {
            ++n;
            if (!linia.empty() && linia.back() == '\r')   // fitxers creats a Windows
                linia.pop_back();
            if (linia.empty() || linia[0] == '#')         // linies de comentari
                continue;
            linies.push_back(linia);
            numLinia.push_back(n);
        }
        if (linies.empty())
            Avisos::afegeix("El nivell " + fitxerNivell + " esta buit -> es carrega un nivell per defecte");
    }
    if (linies.empty()) {
        linies = NIVELL_PER_DEFECTE;
        numLinia.assign(linies.size(), 0);
    }

    // ---- 2) Validar el contingut (CORRECCIO INC-03: abans es carregava en silenci) ----
    int desconeguts = 0;
    bool hiHaHeroi = false;
    bool irregular = false;
    for (std::size_t i = 0; i < linies.size(); ++i) {
        if (linies[i].size() != linies[0].size())
            irregular = true;
        for (std::size_t col = 0; col < linies[i].size(); ++col) {
            char& c = linies[i][col];
            if (c == 'H')
                hiHaHeroi = true;
            else if (!esCaracterValid(c)) {
                if (desconeguts < 3)
                    Avisos::afegeix("Nivell, linia " + std::to_string(numLinia[i]) + " columna "
                        + std::to_string(col + 1) + ": caracter '" + std::string(1, c)
                        + "' desconegut -> es tracta com a buit");
                ++desconeguts;
                c = '.';
            }
        }
    }
    if (desconeguts > 3)
        Avisos::afegeix("... i " + std::to_string(desconeguts - 3) + " caracters desconeguts mes");
    if (irregular)
        Avisos::afegeix("Les files del nivell no tenen la mateixa llargada -> s'omplen amb buit");
    if (!hiHaHeroi)
        Avisos::afegeix("El nivell no te cap 'H' -> l'heroi comenca a la posicio per defecte");

    const int filesEsperades = (int)(Config::ALTURA_FINESTRA / Config::MIDA_RAJOLA);   // 12
    if ((int)linies.size() < filesEsperades) {
        Avisos::afegeix("El nivell te " + std::to_string(linies.size()) + " files (se n'esperaven "
            + std::to_string(filesEsperades) + ") -> s'afegeixen files buides a dalt");
        linies.insert(linies.begin(), filesEsperades - linies.size(), std::string());
    }
    else if ((int)linies.size() > filesEsperades) {
        Avisos::afegeix("El nivell te " + std::to_string(linies.size()) + " files (se n'esperaven "
            + std::to_string(filesEsperades) + ") -> les de sota no es veuran");
    }

    // ---- 3) Convertir els caracters en rajoles ----
    // Valor per defecte: a l'esquerra, damunt del terra
    m_inici = { Config::MIDA_RAJOLA * 2.0f, Config::ALTURA_TERRA };

    for (std::size_t fila = 0; fila < linies.size(); ++fila) {
        std::vector<TipusRajola> filaRajoles;
        for (std::size_t col = 0; col < linies[fila].size(); ++col) {
            char c = linies[fila][col];
            if (c == 'H') {   // posicio inicial de l'heroi (peus a la base de la casella)
                m_inici = { (col + 0.5f) * Config::MIDA_RAJOLA,
                            (fila + 1.0f) * Config::MIDA_RAJOLA };
                c = '.';
            }
            filaRajoles.push_back(charATipus(c));
        }
        m_columnes = std::max(m_columnes, (int)filaRajoles.size());
        m_rajoles.push_back(filaRajoles);
    }
    m_files = (int)m_rajoles.size();

    // Totes les files amb la mateixa llargada (s'omple amb buit)
    // i com a minim una pantalla d'ample perque la camera no surti del mapa
    m_columnes = std::max(m_columnes, (int)(Config::AMPLADA_FINESTRA / Config::MIDA_RAJOLA));
    for (auto& f : m_rajoles)
        f.resize(m_columnes, TipusRajola::Buit);

    m_tileset = &GestorTextures::getInstance().get(fitxerTileset);
    construeixVertexs();
}

TipusRajola TileMap::charATipus(char c) const {
    switch (c) {
        case 'G': return TipusRajola::Gespa;
        case 'T': return TipusRajola::Terra;
        case 'R': return TipusRajola::Roca;
        case 'P': return TipusRajola::Plataforma;
        case 'A': return TipusRajola::Aigua;
        case 'S': return TipusRajola::Punxes;
        default:  return TipusRajola::Buit;
    }
}

// Ordre de les caselles dins de tiles.png (d'esquerra a dreta)
int TileMap::indexTileset(TipusRajola t) const {
    switch (t) {
        case TipusRajola::Gespa:      return 0;
        case TipusRajola::Terra:      return 1;
        case TipusRajola::Roca:       return 2;
        case TipusRajola::Plataforma: return 3;
        case TipusRajola::Aigua:      return 4;
        case TipusRajola::Punxes:     return 5;
        default:                      return -1;
    }
}

// Cada rajola son 2 triangles = 6 vertexs dins d'un unic VertexArray
void TileMap::construeixVertexs() {
    m_vertexs.clear();
    const float m = (float)Config::MIDA_RAJOLA;

    for (int f = 0; f < m_files; ++f) {
        for (int c = 0; c < m_columnes; ++c) {
            int idx = indexTileset(m_rajoles[f][c]);
            if (idx < 0) continue;   // buit: no es dibuixa

            sf::Vector2f p0(c * m, f * m), p1((c + 1) * m, f * m),
                         p2((c + 1) * m, (f + 1) * m), p3(c * m, (f + 1) * m);
            sf::Vector2f t0(idx * m, 0.f), t1((idx + 1) * m, 0.f),
                         t2((idx + 1) * m, m), t3(idx * m, m);

            m_vertexs.append(sf::Vertex{ p0, sf::Color::White, t0 });
            m_vertexs.append(sf::Vertex{ p1, sf::Color::White, t1 });
            m_vertexs.append(sf::Vertex{ p2, sf::Color::White, t2 });
            m_vertexs.append(sf::Vertex{ p0, sf::Color::White, t0 });
            m_vertexs.append(sf::Vertex{ p2, sf::Color::White, t2 });
            m_vertexs.append(sf::Vertex{ p3, sf::Color::White, t3 });
        }
    }
}

void TileMap::draw(sf::RenderTarget& target, bool optimitzat) const {
    if (optimitzat) {
        // MILLORA: 1 sola crida a draw per a tot el mapa
        sf::RenderStates estats;
        estats.texture = m_tileset;
        target.draw(m_vertexs, estats);
        return;
    }

    // Mode ANTIC (sense optimitzar): un sprite i una crida a draw per rajola
    const int m = Config::MIDA_RAJOLA;
    sf::Sprite sprite(*m_tileset);
    for (int f = 0; f < m_files; ++f) {
        for (int c = 0; c < m_columnes; ++c) {
            int idx = indexTileset(m_rajoles[f][c]);
            if (idx < 0) continue;
            sprite.setTextureRect(sf::IntRect({ idx * m, 0 }, { m, m }));
            sprite.setPosition({ (float)(c * m), (float)(f * m) });
            target.draw(sprite);
        }
    }
}

TipusRajola TileMap::getRajola(int columna, int fila) const {
    if (fila < 0 || fila >= m_files || columna < 0 || columna >= m_columnes)
        return TipusRajola::Buit;
    return m_rajoles[fila][columna];
}

bool TileMap::esSolida(int columna, int fila) const {
    // Les vores esquerra i dreta del mon fan de paret
    if (columna < 0 || columna >= m_columnes)
        return true;
    TipusRajola t = getRajola(columna, fila);
    return t == TipusRajola::Gespa || t == TipusRajola::Terra || t == TipusRajola::Roca;
}

bool TileMap::esPlataforma(int columna, int fila) const {
    return getRajola(columna, fila) == TipusRajola::Plataforma;
}

bool TileMap::esPerillosa(int columna, int fila) const {
    TipusRajola t = getRajola(columna, fila);
    return t == TipusRajola::Aigua || t == TipusRajola::Punxes;
}

bool TileMap::tocaPerill(const sf::FloatRect& zona) const {
    const float m = (float)Config::MIDA_RAJOLA;
    int c0 = (int)std::floor(zona.position.x / m);
    int c1 = (int)std::floor((zona.position.x + zona.size.x - 0.01f) / m);
    int f0 = (int)std::floor(zona.position.y / m);
    int f1 = (int)std::floor((zona.position.y + zona.size.y - 0.01f) / m);

    for (int f = f0; f <= f1; ++f)
        for (int c = c0; c <= c1; ++c)
            if (esPerillosa(c, f))
                return true;
    return false;
}

float TileMap::getAmplada() const { return (float)(m_columnes * Config::MIDA_RAJOLA); }
float TileMap::getAltura()  const { return (float)(m_files    * Config::MIDA_RAJOLA); }
