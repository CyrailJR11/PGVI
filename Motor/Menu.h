#pragma once
// ============================================================
//  Menu.h - menu principal amb 5 opcions (punt 3)
//    Jugar / Dificultat / So ON-OFF / Controls-Tutorial / Sortir
//  Es dibuixa SEMPRE amb la segona camera (vista UI), que es fixa
//  a la pantalla i no es mou amb l'heroi.
// ============================================================
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class OpcioMenu {
    Jugar = 0,
    Dificultat,
    So,
    Controls,
    Sortir
};

class Menu {
public:
    Menu();

    void amunt();
    void avall();
    OpcioMenu getSeleccionada() const { return (OpcioMenu)m_seleccionada; }

    // Refresca els textos que depenen de la configuracio
    void actualitzaTextos(const std::string& dificultat, bool soActiu);

    void drawMenu(sf::RenderTarget& target) const;
    void drawControls(sf::RenderTarget& target) const;

private:
    void colorejaOpcions();
    void centra(sf::Text& text, float y);

    sf::RectangleShape   m_fosc;      // capa semitransparent sobre el joc
    sf::Text             m_titol;
    std::vector<sf::Text> m_opcions;
    sf::Text             m_ajuda;
    sf::Text             m_titolControls;
    std::vector<sf::Text> m_liniesControls;
    int m_seleccionada = 0;
};
