#pragma once
// ============================================================
//  TileMap.h - Mon propi carregat des d'un fitxer de text (punt 1)
//
//  Cada caracter del fitxer es una rajola de 64x64:
//    '.'  buit          (no es dibuixa, no es trepitja)
//    'G'  gespa         TREPITJABLE (solida)
//    'T'  terra         TREPITJABLE (solida)
//    'R'  roca          TREPITJABLE (solida)
//    'P'  plataforma    TREPITJABLE nomes per dalt (es pot travessar saltant)
//    'A'  aigua         NO TREPITJABLE (perillosa: treu una vida)
//    'S'  punxes        NO TREPITJABLE (cristalls: treuen una vida)
//
//  Dibuix optimitzat: tot el mapa es un unic sf::VertexArray
//  (1 sola crida a draw). Amb F4 es pot canviar al mode antic
//  (un sprite per rajola) per mesurar els FPS abans/despres.
// ============================================================
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class TipusRajola {
    Buit,
    Gespa,
    Terra,
    Roca,
    Plataforma,
    Aigua,
    Punxes
};

class TileMap {
public:
    // Carrega el nivell del fitxer de text. Llanca excepcio si no existeix.
    TileMap(const std::string& fitxerNivell, const std::string& fitxerTileset);

    void draw(sf::RenderTarget& target, bool optimitzat) const;

    // Consultes per a les col.lisions (coordenades de rajola)
    TipusRajola getRajola(int columna, int fila) const;
    bool esSolida(int columna, int fila) const;       // bloqueja per tots costats
    bool esPlataforma(int columna, int fila) const;   // nomes es trepitja per dalt
    bool esPerillosa(int columna, int fila) const;    // no trepitjable: fa mal

    // Hi ha alguna rajola perillosa dins d'aquest rectangle (coordenades del mon)?
    bool tocaPerill(const sf::FloatRect& zona) const;

    int   getColumnes() const { return m_columnes; }
    int   getFiles()    const { return m_files; }
    float getAmplada()  const;   // en pixels
    float getAltura()   const;

    sf::Vector2f getInici() const { return m_inici; }   // on apareix l'heroi ('H')

private:
    TipusRajola charATipus(char c) const;
    int indexTileset(TipusRajola t) const;   // quina casella del tileset.png
    void construeixVertexs();

    std::vector<std::vector<TipusRajola>> m_rajoles;   // [fila][columna]
    int m_columnes = 0;
    int m_files    = 0;
    sf::Vector2f m_inici;

    const sf::Texture* m_tileset = nullptr;
    sf::VertexArray    m_vertexs{ sf::PrimitiveType::Triangles };
};
