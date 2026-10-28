#include "Menu.h"
#include "Config.h"
#include "GestorRecursos.h"

Menu::Menu()
    : m_titol(GestorFonts::getInstance().get(Config::FONT_TITOL), "Tiny Bazooka", 96),
      m_ajuda(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO),
              "Fletxes: triar   -   Enter: acceptar   -   Esc: sortir", 22),
      m_titolControls(GestorFonts::getInstance().get(Config::FONT_TITOL), "Controls", 80)
{
    const sf::Font& fontText = GestorFonts::getInstance().get(Config::FONT_PUNTUACIO);

    m_fosc.setSize({ (float)Config::AMPLADA_FINESTRA, (float)Config::ALTURA_FINESTRA });
    m_fosc.setFillColor(sf::Color(0, 0, 0, 140));

    m_titol.setFillColor(sf::Color::Red);
    m_titol.setOutlineColor(sf::Color::White);
    m_titol.setOutlineThickness(3.0f);
    centra(m_titol, Config::ALTURA_FINESTRA * 0.15f);

    // Les 5 opcions (el text definitiu el posa actualitzaTextos)
    for (int i = 0; i < 5; ++i) {
        m_opcions.emplace_back(fontText, "", 40);
        m_opcions.back().setOutlineColor(sf::Color::Black);
        m_opcions.back().setOutlineThickness(2.0f);
    }
    actualitzaTextos("Facil", true);

    m_ajuda.setFillColor(sf::Color(230, 230, 230));
    centra(m_ajuda, Config::ALTURA_FINESTRA * 0.92f);

    // ---- Pantalla de Controls / Tutorial ----
    m_titolControls.setFillColor(sf::Color::Red);
    m_titolControls.setOutlineColor(sf::Color::White);
    m_titolControls.setOutlineThickness(3.0f);
    centra(m_titolControls, Config::ALTURA_FINESTRA * 0.12f);

    const std::vector<std::string> linies = {
        "Fletxes Esquerra / Dreta : moure l'heroi",
        "Fletxa Amunt : saltar (premer 2 cops = doble salt)",
        "Fletxa Avall o Espai : disparar (maxim 5 coets)",
        "P : pausa      Esc : tornar al menu",
        "",
        "Objectiu: dispara als enemics (+1 punt cada un).",
        "Si un enemic et toca, o caus a l'aigua o a les punxes,",
        "perds una vida. Amb 0 vides: GAME OVER.",
        "",
        "F2: limit FPS  F4: tilemap  F5: mesura FPS  F11: pantalla completa",
        "",
        "Prem Enter o Esc per tornar"
    };
    float y = Config::ALTURA_FINESTRA * 0.27f;
    for (const std::string& l : linies) {
        m_liniesControls.emplace_back(fontText, l, 26);
        m_liniesControls.back().setFillColor(sf::Color::White);
        m_liniesControls.back().setOutlineColor(sf::Color::Black);
        m_liniesControls.back().setOutlineThickness(2.0f);
        centra(m_liniesControls.back(), y);
        y += 40.0f;
    }
}

void Menu::centra(sf::Text& text, float y) {
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ Config::AMPLADA_FINESTRA * 0.5f, y });
}

void Menu::actualitzaTextos(const std::string& dificultat, bool soActiu) {
    m_opcions[0].setString("Jugar");
    m_opcions[1].setString("Dificultat: < " + dificultat + " >");
    m_opcions[2].setString(std::string("So: ") + (soActiu ? "ON" : "OFF"));
    m_opcions[3].setString("Controls / Tutorial");
    m_opcions[4].setString("Sortir");

    for (std::size_t i = 0; i < m_opcions.size(); ++i)
        centra(m_opcions[i], Config::ALTURA_FINESTRA * 0.38f + i * 70.0f);

    colorejaOpcions();
}

void Menu::amunt() {
    m_seleccionada = (m_seleccionada + 4) % 5;   // dona la volta
    colorejaOpcions();
}

void Menu::avall() {
    m_seleccionada = (m_seleccionada + 1) % 5;
    colorejaOpcions();
}

void Menu::colorejaOpcions() {
    for (std::size_t i = 0; i < m_opcions.size(); ++i) {
        bool sel = ((int)i == m_seleccionada);
        m_opcions[i].setFillColor(sel ? sf::Color::Yellow : sf::Color::White);
        m_opcions[i].setScale(sel ? sf::Vector2f(1.15f, 1.15f) : sf::Vector2f(1.0f, 1.0f));
    }
}

void Menu::drawMenu(sf::RenderTarget& target) const {
    target.draw(m_fosc);
    target.draw(m_titol);
    for (const sf::Text& t : m_opcions)
        target.draw(t);
    target.draw(m_ajuda);
}

void Menu::drawControls(sf::RenderTarget& target) const {
    target.draw(m_fosc);
    target.draw(m_titolControls);
    for (const sf::Text& t : m_liniesControls)
        target.draw(t);
}
