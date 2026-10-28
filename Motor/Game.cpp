#include "Game.h"
#include <algorithm>

Game::Game()
    : m_vistaJoc(sf::FloatRect({ 0.0f, 0.0f },
                 { (float)Config::AMPLADA_FINESTRA, (float)Config::ALTURA_FINESTRA })),
      m_vistaUI(sf::FloatRect({ 0.0f, 0.0f },
                 { (float)Config::AMPLADA_FINESTRA, (float)Config::ALTURA_FINESTRA })),
      m_mapa(Config::NIVELL_1, Config::TEXTURA_TILES),
      m_textGameOver(GestorFonts::getInstance().get(Config::FONT_TITOL), "GAME OVER", 110),
      m_textResultat(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO), "", 30),
      m_textPausa(GestorFonts::getInstance().get(Config::FONT_TITOL), "PAUSA", 110),
      m_textAvisos(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO), "", 16),
      m_textMesura(GestorFonts::getInstance().get(Config::FONT_PUNTUACIO), "", 22),
      m_soDispar(GestorSons::getInstance().get(Config::SO_DISPAR)),
      m_soImpacte(GestorSons::getInstance().get(Config::SO_IMPACTE))
{
    creaFinestra();   // la finestra es crea aqui (pantalla completa o no)

    m_heroi.setMapa(&m_mapa);
    m_heroi.reinicia(m_mapa.getInici());

    // Textos de les pantalles
    m_fosc.setSize({ (float)Config::AMPLADA_FINESTRA, (float)Config::ALTURA_FINESTRA });
    m_fosc.setFillColor(sf::Color(0, 0, 0, 150));

    for (sf::Text* t : { &m_textGameOver, &m_textPausa }) {
        t->setFillColor(sf::Color::Red);
        t->setOutlineColor(sf::Color::White);
        t->setOutlineThickness(3.0f);
        t->setOrigin(t->getLocalBounds().getCenter());
        t->setPosition({ Config::AMPLADA_FINESTRA * 0.5f, Config::ALTURA_FINESTRA * 0.35f });
    }
    m_textResultat.setFillColor(sf::Color::White);
    m_textResultat.setOutlineColor(sf::Color::Black);
    m_textResultat.setOutlineThickness(2.0f);

    // Musica de fons (si no es troba el fitxer, el joc funciona igual)
    m_musicaCarregada = m_musica.openFromFile(Config::SO_MUSICA);
    if (m_musicaCarregada) {
        m_musica.setLooping(true);
        m_musica.setVolume(40.0f);
    }
    canviaSo(m_soActiu);

    srand((unsigned int)time(nullptr));
    m_estat = DificultatEstat::crea(m_dificultatInicial);
    m_menu.actualitzaTextos(DificultatEstat::nomNivell(m_dificultatInicial), m_soActiu);
    m_hud.actualitza(m_puntuacio, m_vides, m_tempsPartida, m_bales.size(), m_estat->nom());
    actualitzaCamera();

    // Panell del resultat de la mesura de FPS (F5)
    m_panellMesura.setSize({ 700.0f, 330.0f });
    m_panellMesura.setOrigin(m_panellMesura.getSize() / 2.0f);
    m_panellMesura.setPosition({ Config::AMPLADA_FINESTRA * 0.5f, Config::ALTURA_FINESTRA * 0.5f });
    m_panellMesura.setFillColor(sf::Color(10, 16, 30, 225));
    m_panellMesura.setOutlineColor(sf::Color(255, 210, 80));
    m_panellMesura.setOutlineThickness(3.0f);
    m_textMesura.setFillColor(sf::Color::White);
    m_textMesura.setLineSpacing(1.15f);

    // Incidencies de carrega visibles a la pantalla (tambe en pantalla completa)
    const auto& avisos = Avisos::tots();
    if (!avisos.empty()) {
        std::string text = "AVISOS DE CARREGA (" + std::to_string(avisos.size()) + "):";
        for (std::size_t i = 0; i < avisos.size() && i < 6; ++i)
            text += "\n- " + avisos[i];
        m_textAvisos.setString(text);
        m_textAvisos.setFillColor(sf::Color(255, 170, 60));
        m_textAvisos.setOutlineColor(sf::Color::Black);
        m_textAvisos.setOutlineThickness(2.0f);
        float alt = m_textAvisos.getLocalBounds().size.y;
        m_textAvisos.setPosition({ 10.0f, Config::ALTURA_FINESTRA - 95.0f - alt });   // per sobre de l'ajuda del menu
    }
}

// ------------------------------------------------------------
//  Pantalla completa
// ------------------------------------------------------------
void Game::creaFinestra() {
    if (m_pantallaCompleta) {
        // Agafa la resolucio del monitor (per exemple 1920x1080)
        m_finestra.create(sf::VideoMode::getDesktopMode(), "Tiny Bazooka", sf::State::Fullscreen);
    }
    else {
        m_finestra.create(sf::VideoMode(sf::Vector2u(Config::AMPLADA_FINESTRA, Config::ALTURA_FINESTRA)),
                          "Tiny Bazooka");
    }

    // Quan es torna a crear la finestra es perd la configuracio: la tornem a posar
    m_finestra.setFramerateLimit(m_limitFps ? Config::LIMIT_FPS : 0);
    m_finestra.setKeyRepeatEnabled(false);              // un salt per pulsacio
    m_finestra.setMouseCursorVisible(!m_pantallaCompleta);
    ajustaVistes();
}

// El joc es dissenya a 1024x768 (proporcio 4:3). Un monitor 16:9 es mes ample:
// en lloc d'estirar la imatge, deixem franges negres als costats.
// El viewport diu quina part de la finestra ocupa cada camera (valors de 0 a 1).
void Game::ajustaVistes() {
    sf::Vector2u mida = m_finestra.getSize();
    float proporcioFinestra = (float)mida.x / (float)mida.y;
    float proporcioJoc = (float)Config::AMPLADA_FINESTRA / (float)Config::ALTURA_FINESTRA;

    sf::FloatRect viewport({ 0.0f, 0.0f }, { 1.0f, 1.0f });
    if (proporcioFinestra > proporcioJoc) {
        // Monitor mes ample que el joc: franges a esquerra i dreta
        viewport.size.x = proporcioJoc / proporcioFinestra;
        viewport.position.x = (1.0f - viewport.size.x) / 2.0f;
    }
    else {
        // Monitor mes alt que el joc: franges a dalt i a baix
        viewport.size.y = proporcioFinestra / proporcioJoc;
        viewport.position.y = (1.0f - viewport.size.y) / 2.0f;
    }

    m_vistaJoc.setViewport(viewport);   // les DUES cameres fan servir la mateixa zona
    m_vistaUI.setViewport(viewport);
}

// Regla d'or: per cada new, un delete.
Game::~Game() {
    netejaEntitats();
    delete m_estat;
}

void Game::netejaEntitats() {
    for (Bala* bala : m_bales)
        delete bala;
    m_bales.clear();
    for (Enemic* enemic : m_enemics)
        delete enemic;
    m_enemics.clear();
}

// ------------------------------------------------------------
//  Bucle principal del joc
// ------------------------------------------------------------
void Game::run() {
    sf::Clock rellotge;

    while (m_finestra.isOpen()) {
        processarEntrada();

        float dtReal = rellotge.restart().asSeconds(); // temps real entre frames
        float dt = std::min(dtReal, 0.05f);            // evita salts enormes si la finestra es bloqueja

        actualitzar(dt);

        m_finestra.clear(sf::Color::Black);   // negre: color de les franges laterals
        dibuixar();
        m_finestra.display();

        comptaFps();
        if (m_mesurant)
            registraFrameMesura(dtReal);   // la mesura fa servir el temps real, sense retallar
    }
}

void Game::comptaFps() {
    ++m_framesComptats;
    float temps = m_rellotgeFps.getElapsedTime().asSeconds();
    if (temps >= 0.5f) {                     // es refresca dos cops per segon
        m_fps = m_framesComptats / temps;
        m_framesComptats = 0;
        m_rellotgeFps.restart();
        m_hud.actualitzaFps(m_fps, m_limitFps, m_tilemapOptimitzat);
        // Comptador tambe al titol de la finestra (es veu en mode finestra)
        m_finestra.setTitle("Tiny Bazooka - FPS: " + std::to_string((int)(m_fps + 0.5f)));
    }
}

// ------------------------------------------------------------
//  Entrada
// ------------------------------------------------------------
void Game::processarEntrada() {
    while (const std::optional<sf::Event> esdeveniment = m_finestra.pollEvent()) {

        if (esdeveniment->is<sf::Event::Closed>())
            m_finestra.close();

        // Si l'usuari canvia la mida de la finestra, es recalculen les franges
        if (esdeveniment->is<sf::Event::Resized>())
            ajustaVistes();

        if (const auto* tecla = esdeveniment->getIf<sf::Event::KeyPressed>()) {
            // Durant la mesura no es toca res: l'escena ha de ser sempre la mateixa
            if (m_mesurant)
                continue;

            // F5: mesura de rendiment (FPS mitja, minim i temps de frame)
            if (tecla->code == sf::Keyboard::Key::F5) {
                iniciaMesura();
                continue;
            }
            // Qualsevol altra tecla (menys F2/F4) amaga el panell del resultat
            if (m_mostraMesura && tecla->code != sf::Keyboard::Key::F2 && tecla->code != sf::Keyboard::Key::F4)
                m_mostraMesura = false;

            // Tecles globals per a les proves de rendiment
            if (tecla->code == sf::Keyboard::Key::F2) {
                m_limitFps = !m_limitFps;
                m_finestra.setFramerateLimit(m_limitFps ? Config::LIMIT_FPS : 0);
            }
            if (tecla->code == sf::Keyboard::Key::F4)
                m_tilemapOptimitzat = !m_tilemapOptimitzat;

            // F11: alterna pantalla completa / finestra
            if (tecla->code == sf::Keyboard::Key::F11) {
                m_pantallaCompleta = !m_pantallaCompleta;
                creaFinestra();
                return;   // la finestra s'ha tornat a crear: els events antics ja no valen
            }

            switch (m_pantalla) {
                case Pantalla::Menu:
                    entradaMenu(tecla->code);
                    break;
                case Pantalla::Controls:
                    if (tecla->code == sf::Keyboard::Key::Escape || tecla->code == sf::Keyboard::Key::Enter)
                        m_pantalla = Pantalla::Menu;
                    break;
                case Pantalla::Jugant:
                    entradaJoc(tecla->code);
                    break;
                case Pantalla::GameOver:
                    if (tecla->code == sf::Keyboard::Key::Enter)
                        novaPartida();                       // REINICI
                    else if (tecla->code == sf::Keyboard::Key::Escape)
                        m_pantalla = Pantalla::Menu;
                    break;
            }
        }
    }
}

void Game::entradaMenu(sf::Keyboard::Key tecla) {
    using K = sf::Keyboard::Key;

    if (tecla == K::Up)   m_menu.amunt();
    if (tecla == K::Down) m_menu.avall();
    if (tecla == K::Escape) { m_finestra.close(); return; }

    OpcioMenu opcio = m_menu.getSeleccionada();
    bool accepta = (tecla == K::Enter || tecla == K::Space);
    bool lateral = (tecla == K::Left || tecla == K::Right);

    if (opcio == OpcioMenu::Dificultat && (accepta || lateral)) {
        int pas = (tecla == K::Left) ? 2 : 1;           // esquerra = enrere
        m_dificultatInicial = (m_dificultatInicial + pas) % 3;
    }
    else if (opcio == OpcioMenu::So && (accepta || lateral)) {
        canviaSo(!m_soActiu);
    }
    else if (accepta) {
        switch (opcio) {
            case OpcioMenu::Jugar:    novaPartida();                    break;
            case OpcioMenu::Controls: m_pantalla = Pantalla::Controls;  break;
            case OpcioMenu::Sortir:   m_finestra.close();               break;
            default: break;
        }
    }

    m_menu.actualitzaTextos(DificultatEstat::nomNivell(m_dificultatInicial), m_soActiu);
}

void Game::entradaJoc(sf::Keyboard::Key tecla) {
    using K = sf::Keyboard::Key;

    if (tecla == K::Escape) {          // abandona la partida i torna al menu
        m_pausa = false;
        m_pantalla = Pantalla::Menu;
        return;
    }
    if (tecla == K::P)
        m_pausa = !m_pausa;
    if (m_pausa)
        return;

    if (tecla == K::Up)
        m_heroi.salta(Config::VELOCITAT_SALT);
    if (tecla == K::Down || tecla == K::Space)
        dispara();
}

// ------------------------------------------------------------
//  Partida
// ------------------------------------------------------------
void Game::novaPartida() {
    netejaEntitats();
    m_heroi.reinicia(m_mapa.getInici());

    m_puntuacio        = 0;
    m_vides            = Config::VIDES_INICIALS;
    m_tempsPartida     = 0.0f;
    m_tempsActual      = 0.0f;
    m_tempsUltimEnemic = 0.0f;
    m_pausa            = false;

    delete m_estat;
    m_estat = DificultatEstat::crea(m_dificultatInicial);

    m_pantalla = Pantalla::Jugant;
    actualitzaCamera();
}

void Game::canviaSo(bool actiu) {
    m_soActiu = actiu;
    if (!m_musicaCarregada) return;
    if (m_soActiu) m_musica.play();
    else           m_musica.pause();
}

void Game::reprodueix(sf::Sound& so) {
    if (m_soActiu && !m_mesurant)   // durant la mesura, sense sons
        so.play();
}

// La bala neix al cano del bazooka i va cap on mira l'heroi
void Game::dispara() {
    if (m_bales.size() >= Config::MAX_BALES)
        return;

    float mirada = m_heroi.getMirada();
    sf::Vector2f pos = m_heroi.getPosicio() + sf::Vector2f(mirada * 40.0f, -Config::ALTURA_CANO);
    m_bales.push_back(new Bala(pos, mirada * Config::VELOCITAT_BALA));
    reprodueix(m_soDispar);
}

void Game::generaEnemic(float desplacament) {
    float dretaCamera = m_vistaJoc.getCenter().x + m_vistaJoc.getSize().x / 2.0f;
    float x = dretaCamera + 60.0f + desplacament;       // neix just fora de la camera
    float vel = m_estat->velocitat();

    int tipus = rand() % 3;                             // terrestre, aeri o saltador
    if (tipus == 0) {
        sf::Vector2f pos = { x, Config::ALTURA_TERRA - Config::ALTURA_FILA_ENEMIC[0] };
        m_enemics.push_back(new EnemicTerrestre(pos, -vel));
    }
    else if (tipus == 1) {
        int fila = 1 + rand() % 2;                      // vola a la fila 1 o 2
        sf::Vector2f pos = { x, Config::ALTURA_TERRA - Config::ALTURA_FILA_ENEMIC[fila] };
        float v = vel + fila * Config::INCREMENT_VELOCITAT_FILA * 0.5f;
        m_enemics.push_back(new EnemicAeri(pos, -v * 0.8f));
    }
    else {
        sf::Vector2f pos = { x, Config::ALTURA_TERRA };
        m_enemics.push_back(new EnemicSaltador(pos, -vel * 0.7f));
    }

    m_tempsUltimEnemic = m_tempsActual;
}

void Game::perdVida() {
    if (m_mesurant)      // durant la mesura l'heroi no pot perdre
        return;
    m_vides--;
    reprodueix(m_soImpacte);
    m_heroi.ferInvulnerable();
    if (m_vides <= 0) {
        m_vides = 0;
        m_pantalla = Pantalla::GameOver;
        m_textResultat.setString("Punts: " + std::to_string(m_puntuacio)
            + "     Temps: " + Hud::formatTemps(m_tempsPartida)
            + "\n\nEnter: tornar a jugar     Esc: menu");
        m_textResultat.setOrigin(m_textResultat.getLocalBounds().getCenter());
        m_textResultat.setPosition({ Config::AMPLADA_FINESTRA * 0.5f, Config::ALTURA_FINESTRA * 0.58f });
    }
}

// Caixes englobants (AABB): hi ha col.lisio si els rectangles es solapen
bool Game::hiHaColisio(const sf::FloatRect& a, const sf::FloatRect& b) const {
    return a.findIntersection(b).has_value();
}

// Camera 1: segueix l'heroi en horitzontal sense sortir del mapa
void Game::actualitzaCamera() {
    float meitat = Config::AMPLADA_FINESTRA / 2.0f;
    float x = std::clamp(m_heroi.getPosicio().x, meitat, m_mapa.getAmplada() - meitat);
    m_vistaJoc.setCenter({ x, Config::ALTURA_FINESTRA / 2.0f });
}

// ------------------------------------------------------------
//  Actualitzacio
// ------------------------------------------------------------
void Game::actualitzar(float dt) {
    float esquerraCamera = m_vistaJoc.getCenter().x - m_vistaJoc.getSize().x / 2.0f;
    m_parallax.update(dt, esquerraCamera);   // el cel es mou fins i tot al menu

    if (m_pantalla != Pantalla::Jugant || m_pausa)
        return;

    m_tempsPartida += dt;

    // --- Heroi: moviment amb les fletxes laterals (tecla mantinguda) ---
    float direccio = 0.0f;
    if (!m_mesurant) {   // durant la mesura l'heroi es queda quiet
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  direccio -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direccio += 1.0f;
    }
    m_heroi.setDireccio(direccio);
    m_heroi.update(dt);

    // --- Zones no trepitjables: aigua o punxes ---
    if (m_heroi.haTocatPerill()) {
        if (!m_heroi.esInvulnerable())
            perdVida();
        m_heroi.reapareix();
    }

    actualitzaCamera();
    esquerraCamera = m_vistaJoc.getCenter().x - m_vistaJoc.getSize().x / 2.0f;
    float dretaCamera = esquerraCamera + m_vistaJoc.getSize().x;

    // --- Enemics: apareixen cada cert temps i es mouen sols ---
    m_tempsActual += dt;
    if (m_mesurant) {
        // Escena fixa: sempre el mateix nombre d'enemics i dispar continu
        while (m_enemics.size() < Config::ENEMICS_MESURA)
            generaEnemic((float)(rand() % 600));
        dispara();
    }
    else if (m_tempsActual >= m_tempsUltimEnemic + m_estat->interval())
        generaEnemic();

    for (Enemic* enemic : m_enemics)
        enemic->update(dt);                             // polimorfisme (funcio virtual)

    for (std::size_t i = 0; i < m_enemics.size(); ) {
        if (m_enemics[i]->foraDePantalla(esquerraCamera)) {
            delete m_enemics[i];
            m_enemics.erase(m_enemics.begin() + i);
        } else {
            ++i;
        }
    }

    // --- Bales: actualitzar i esborrar les que surten de la camera ---
    for (Bala* bala : m_bales)
        bala->update(dt);

    for (std::size_t i = 0; i < m_bales.size(); ) {
        if (m_bales[i]->foraDePantalla(esquerraCamera, dretaCamera)) {
            delete m_bales[i];
            m_bales.erase(m_bales.begin() + i);
        } else {
            ++i;
        }
    }

    // --- Col.lisio bala-enemic: s'esborren tots dos i +1 punt ---
    for (std::size_t i = 0; i < m_bales.size(); ) {
        bool impacte = false;

        for (std::size_t j = 0; j < m_enemics.size(); ++j) {
            if (hiHaColisio(m_bales[i]->getSprite().getGlobalBounds(), m_enemics[j]->getHitbox())) {
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
            reprodueix(m_soImpacte);
        } else {
            ++i;
        }
    }

    // --- Col.lisio heroi-enemic: es perd una vida ---
    if (!m_heroi.esInvulnerable()) {
        for (std::size_t j = 0; j < m_enemics.size(); ++j) {
            if (hiHaColisio(m_heroi.getHitbox(), m_enemics[j]->getHitbox())) {
                delete m_enemics[j];
                m_enemics.erase(m_enemics.begin() + j);
                perdVida();
                break;
            }
        }
    }

    // --- PATRO ESTATS: l'estat decideix si cal canviar de dificultat ---
    if (DificultatEstat* nou = m_estat->seguent(m_puntuacio)) {
        delete m_estat;
        m_estat = nou;
    }

    m_hud.actualitza(m_puntuacio, m_vides, m_tempsPartida, m_bales.size(), m_estat->nom());
}

// ------------------------------------------------------------
//  Dibuix: cada cosa amb la seva camera
// ------------------------------------------------------------
void Game::dibuixar() {
    // 1) Fons amb parallax -> camera UI (fixa)
    m_finestra.setView(m_vistaUI);
    m_parallax.draw(m_finestra);

    // 2) Mon -> camera de joc (segueix l'heroi)
    m_finestra.setView(m_vistaJoc);
    m_mapa.draw(m_finestra, m_tilemapOptimitzat);

    for (const Enemic* enemic : m_enemics)
        enemic->draw(m_finestra);

    for (const Bala* bala : m_bales)
        bala->draw(m_finestra);

    m_heroi.draw(m_finestra);

    // 3) Interficie -> SEGONA CAMERA (UI)
    m_finestra.setView(m_vistaUI);

    switch (m_pantalla) {
        case Pantalla::Menu:
            m_menu.drawMenu(m_finestra);
            break;
        case Pantalla::Controls:
            m_menu.drawControls(m_finestra);
            break;
        case Pantalla::Jugant:
            m_hud.draw(m_finestra);
            if (m_mesurant) {
                m_finestra.draw(m_textMesura);
            }
            else if (m_mostraMesura) {
                m_finestra.draw(m_fosc);
                m_finestra.draw(m_panellMesura);
                m_finestra.draw(m_textMesura);
            }
            else if (m_pausa) {
                m_finestra.draw(m_fosc);
                m_finestra.draw(m_textPausa);
            }
            break;
        case Pantalla::GameOver:
            m_hud.draw(m_finestra);
            m_finestra.draw(m_fosc);
            m_finestra.draw(m_textGameOver);
            m_finestra.draw(m_textResultat);
            break;
    }

    m_hud.drawFps(m_finestra);
    if (!Avisos::tots().empty())
        m_finestra.draw(m_textAvisos);
}

// ------------------------------------------------------------
//  Mesura de rendiment (F5)
//  Metode: mateixa escena (llavor fixa, 8 enemics, dispar continu fins
//  a 5 bales, heroi quiet), 1 s d'escalfament + 10 s de mesura, sense
//  limit de FPS. Resultat per std::cout, al titol (setTitle) i en pantalla.
// ------------------------------------------------------------
static std::string fmt1(float valor) {
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%.1f", valor);
    return buffer;
}

static std::string fmtPercent(float abans, float despres) {
    if (abans <= 0.0f) return "-";
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%+.1f%%", (despres - abans) / abans * 100.0f);
    return buffer;
}

void Game::iniciaMesura() {
    srand(Config::LLAVOR_MESURA);       // mateixos enemics cada vegada
    novaPartida();
    m_mesurant         = true;
    m_mostraMesura     = false;
    m_limitAbansMesura = m_limitFps;
    m_finestra.setFramerateLimit(0);    // sense limit: volem el maxim que dona l'ordinador
    m_tempsMesura  = 0.0f;
    m_sumaDt       = 0.0;
    m_dtMaxim      = 0.0f;
    m_framesMesura = 0;

    m_textMesura.setString("MESURANT RENDIMENT...");
    m_textMesura.setOrigin({ 0.0f, 0.0f });
    m_textMesura.setPosition({ Config::AMPLADA_FINESTRA * 0.5f - 130.0f, 60.0f });
    std::cout << "[MESURA] Inici (" << (m_tilemapOptimitzat ? "VertexArray" : "sprites")
              << ", " << Config::ENEMICS_MESURA << " enemics, " << Config::SEGONS_MESURA << " s)" << std::endl;
}

void Game::registraFrameMesura(float dtReal) {
    m_tempsMesura += dtReal;
    if (m_tempsMesura <= Config::ESCALFAMENT_MESURA)
        return;                          // el primer segon no compta (carrega inicial)

    m_sumaDt += dtReal;
    m_framesMesura++;
    m_dtMaxim = std::max(m_dtMaxim, dtReal);

    int queden = (int)(Config::ESCALFAMENT_MESURA + Config::SEGONS_MESURA - m_tempsMesura) + 1;
    m_textMesura.setString("MESURANT RENDIMENT... " + std::to_string(queden) + " s");

    if (m_tempsMesura >= Config::ESCALFAMENT_MESURA + Config::SEGONS_MESURA)
        acabaMesura();
}

void Game::acabaMesura() {
    ResultatMesura r;
    r.fet      = true;
    r.fpsMitja = (float)(m_framesMesura / m_sumaDt);
    r.fpsMinim = (m_dtMaxim > 0.0f) ? 1.0f / m_dtMaxim : 0.0f;   // el frame mes lent
    r.msFrame  = (float)(m_sumaDt / m_framesMesura * 1000.0);
    int idx = m_tilemapOptimitzat ? 1 : 0;
    m_resultat[idx] = r;

    m_mesurant = false;
    m_limitFps = m_limitAbansMesura;
    m_finestra.setFramerateLimit(m_limitFps ? Config::LIMIT_FPS : 0);
    m_pausa        = true;
    m_mostraMesura = true;

    std::string mode = m_tilemapOptimitzat ? "v2.0 VertexArray" : "v1.0 sprites";
    std::string text = "MESURA DE RENDIMENT  (" + std::to_string(Config::ENEMICS_MESURA) + " enemics, 5 bales, "
        + fmt1(Config::SEGONS_MESURA) + " s)\n\n"
        + "Mode: " + mode + "   (" + std::to_string(m_framesMesura) + " frames)\n"
        + "FPS mitja: " + fmt1(r.fpsMitja) + "     FPS minim: " + fmt1(r.fpsMinim)
        + "     Temps frame: " + fmt1(r.msFrame) + " ms\n";

    if (m_resultat[0].fet && m_resultat[1].fet) {
        const ResultatMesura& a = m_resultat[0];
        const ResultatMesura& d = m_resultat[1];
        text += "\nComparativa   v1.0 (sprites)  ->  v2.0 (VertexArray)\n";
        text += "FPS mitja:      " + fmt1(a.fpsMitja) + "  ->  " + fmt1(d.fpsMitja) + "   (" + fmtPercent(a.fpsMitja, d.fpsMitja) + ")\n";
        text += "FPS minim:      " + fmt1(a.fpsMinim) + "  ->  " + fmt1(d.fpsMinim) + "   (" + fmtPercent(a.fpsMinim, d.fpsMinim) + ")\n";
        text += "Temps frame:   " + fmt1(a.msFrame) + "  ->  " + fmt1(d.msFrame) + " ms   (" + fmtPercent(a.msFrame, d.msFrame) + ")\n";
    }
    else {
        text += "\nPrem F4 per canviar de mode i F5 per mesurar l'altre.\n";
    }
    text += "\nPrem P per continuar";
    m_textMesura.setString(text);
    m_textMesura.setOrigin(m_textMesura.getLocalBounds().getCenter());
    m_textMesura.setPosition(m_panellMesura.getPosition());

    std::cout << "[MESURA] " << mode << " | FPS mitja " << fmt1(r.fpsMitja)
              << " | FPS minim " << fmt1(r.fpsMinim) << " | temps frame " << fmt1(r.msFrame)
              << " ms | " << m_framesMesura << " frames" << std::endl;
    m_finestra.setTitle("Tiny Bazooka - " + mode + " - FPS mitja " + fmt1(r.fpsMitja)
                        + " | minim " + fmt1(r.fpsMinim) + " | " + fmt1(r.msFrame) + " ms");
}
