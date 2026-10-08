#include "Game.h"


Game::Game()
    : m_finestra(sf::VideoMode(sf::Vector2u(Config::AMPLADA_FINESTRA, Config::ALTURA_FINESTRA)),
        "Tiny Bazooka"),
    m_cel(GestorTextures::getInstance().get(Config::TEXTURA_CEL)),
    m_fons(GestorTextures::getInstance().get(Config::TEXTURA_FONS)),
    m_fontTitol(Config::FONT_TITOL),
    m_fontText(Config::FONT_PUNTUACIO),
    m_textTitol(m_fontTitol),
    m_textTutorial(m_fontText),
    m_textPuntuacio(m_fontText),
    m_bufferDispar(Config::SO_DISPAR),       // PAS 9: carrega el so del tret
    m_bufferImpacte(Config::SO_IMPACTE),
    m_soDispar(m_bufferDispar),
    m_soImpacte(m_bufferImpacte)
{
    m_finestra.setFramerateLimit(Config::LIMIT_FPS);



    m_textTitol.setString("Tiny Bazooka");
    m_textTitol.setCharacterSize(84);
    m_textTitol.setFillColor(sf::Color::Red);
    m_textTitol.setOrigin(m_textTitol.getLocalBounds().getCenter());
    m_textTitol.setPosition({ Config::AMPLADA_FINESTRA * 0.5f, Config::ALTURA_FINESTRA * 0.10f });

     //m_textTutorial.setFont(m_fontText);
    m_textTutorial.setString("Fletxa amunt: saltar (doble salt)  -  Fletxa avall: disparar i comencar");
    m_textTutorial.setCharacterSize(30);
    m_textTutorial.setFillColor(sf::Color::Red);
    m_textTutorial.setOrigin(m_textTutorial.getLocalBounds().getCenter());
    m_textTutorial.setPosition({ Config::AMPLADA_FINESTRA * 0.5f, Config::ALTURA_FINESTRA * 0.20f });

    m_textPuntuacio.setCharacterSize(28);
    m_textPuntuacio.setFillColor(sf::Color::White);
    m_textPuntuacio.setOutlineColor(sf::Color::Black);
    m_textPuntuacio.setOutlineThickness(2.0f);
    m_textPuntuacio.setPosition({ 20.0f, 15.0f });
    actualitzaTextPuntuacio();

    srand((unsigned int)time(nullptr));
}

// Regla d'or: per cada new, un delete. Game ha creat les bales i els enemics,
// per tant Game els esborra.
Game::~Game() {
    for (Bala* bala : m_bales)
        delete bala;
    for (Enemic* enemic : m_enemics)
        delete enemic;
}

// ------------------------------------------------------------
//  Bucle principal del joc
// ------------------------------------------------------------
void Game::run() {
    sf::Clock rellotge;

    while (m_finestra.isOpen()) {
        processarEntrada();

        float dt = rellotge.restart().asSeconds();   // temps entre frames

        if (!m_gameOver)
            actualitzar(dt);

        m_finestra.clear(Config::COLOR_FONS);
        dibuixar();
        m_finestra.display();
    }
}

void Game::processarEntrada() {
    while (const std::optional<sf::Event> esdeveniment = m_finestra.pollEvent()) {

        if (esdeveniment->is<sf::Event::Closed>())
            m_finestra.close();

        if (const auto* tecla = esdeveniment->getIf<sf::Event::KeyPressed>()) {
            if (tecla->code == sf::Keyboard::Key::Escape)
                m_finestra.close();

            if (tecla->code == sf::Keyboard::Key::Up)
                m_heroi.salta(Config::VELOCITAT_SALT);

            // PAS 6: fletxa avall = disparar (nomes si no estem en game over)
            if (tecla->code == sf::Keyboard::Key::Down) {
                if (!m_gameOver)
                    dispara();
            }
        }
    }
}

// PAS 5: la bala neix on es l'heroi i va cap a la dreta
void Game::dispara() {
    // EXTENSIO: maxim 5 bales simultanies
    if (m_bales.size() >= Config::MAX_BALES)
        return;

    m_bales.push_back(new Bala(m_heroi.getPosicio(), Config::VELOCITAT_BALA));
    m_soDispar.play();                                  // PAS 9
    actualitzaTextPuntuacio();
}

void Game::generaEnemic() {
    int fila  = rand() % 3;                             // a quina altura surt
    int tipus = rand() % 2;                             // terrestre o aeri
    float vel = Config::VELOCITAT_FACIL + fila * Config::INCREMENT_VELOCITAT_FILA * 0.5f;
    sf::Vector2f pos = { Config::AMPLADA_FINESTRA + 60.0f,
                         Config::ALTURA_TERRA - Config::ALTURA_FILA_ENEMIC[fila] };

    if (tipus == 0)
        m_enemics.push_back(new EnemicTerrestre(pos, -vel));
    else
        m_enemics.push_back(new EnemicAeri(pos, -vel * 0.8f));

    m_tempsUltimEnemic = m_tempsActual;
}

// Caixes englobants (AABB): hi ha col.lisio si els rectangles es solapen
bool Game::hiHaColisio(const sf::Sprite& a, const sf::Sprite& b) const {
    return a.getGlobalBounds().findIntersection(b.getGlobalBounds()).has_value();
}

void Game::actualitzaTextPuntuacio() {
    m_textPuntuacio.setString("Punts: " + std::to_string(m_puntuacio)
        + "    Bales: " + std::to_string(m_bales.size()) + "/" + std::to_string(Config::MAX_BALES));
}

void Game::actualitzar(float dt) {
    m_heroi.update(dt);

    // --- Enemics: apareixen cada cert temps i es mouen sols ---
    m_tempsActual += dt;
    if (m_tempsActual >= m_tempsUltimEnemic + Config::INTERVAL_FACIL)
        generaEnemic();

    for (Enemic* enemic : m_enemics)
        enemic->update(dt);                             // polimorfisme

    for (std::size_t i = 0; i < m_enemics.size(); ) {
        if (m_enemics[i]->foraDePantalla()) {
            delete m_enemics[i];
            m_enemics.erase(m_enemics.begin() + i);
        } else {
            ++i;
        }
    }

    // --- PAS 7: actualitzar les bales i esborrar les que surten ---
    for (Bala* bala : m_bales)
        bala->update(dt);

    for (std::size_t i = 0; i < m_bales.size(); ) {
        if (m_bales[i]->foraDePantalla((float)Config::AMPLADA_FINESTRA)) {
            delete m_bales[i];                          // primer alliberem la memoria
            m_bales.erase(m_bales.begin() + i);         // despres treiem el punter
        } else {
            ++i;                                        // nomes avancem si no esborrem
        }
    }

    // --- EXTENSIO: col.lisio bala-enemic, s'esborren tots dos i +1 punt ---
    for (std::size_t i = 0; i < m_bales.size(); ) {
        bool impacte = false;

        for (std::size_t j = 0; j < m_enemics.size(); ++j) {
            if (hiHaColisio(m_bales[i]->getSprite(), m_enemics[j]->getSprite())) {
                delete m_enemics[j];
                m_enemics.erase(m_enemics.begin() + j);
                impacte = true;
                break;                                  // una bala nomes mata un enemic
            }
        }

        if (impacte) {
            delete m_bales[i];
            m_bales.erase(m_bales.begin() + i);
            m_puntuacio++;
            m_soImpacte.play();
        } else {
            ++i;
        }
    }

    actualitzaTextPuntuacio();
}

void Game::dibuixar() {
    m_finestra.draw(m_cel);
    m_finestra.draw(m_fons);

    m_heroi.draw(m_finestra);

    for (const Enemic* enemic : m_enemics)
        enemic->draw(m_finestra);

    // PAS 8: dibuixar totes les bales, igual que els enemics
    for (const Bala* bala : m_bales)
        bala->draw(m_finestra);

    m_finestra.draw(m_textPuntuacio);

    if (m_gameOver) { //text d'introduccio
        m_finestra.draw(m_textTitol);
        m_finestra.draw(m_textTutorial);
    }
}
