#include <iostream>
#include <exception>
#include "Game.h"

// ============================================================
//  main - Programa principal: unicament crea el Motor i executa
//
//  CORRECCIO INC-01: si hi ha un error que no es pot arreglar (per
//  exemple falta la font), abans el programa es tancava amb abort()
//  sense cap explicacio. Ara es captura l'excepcio, s'explica que
//  passa i es demana premer Enter abans de tancar la consola.
// ============================================================
int main() {
    try {
        Game joc;
        joc.run();
    }
    catch (const std::exception& e) {
        std::cerr << "\n[ERROR] El joc no pot continuar: " << e.what() << std::endl;
        std::cerr << "Revisa que la carpeta Assets estigui al costat de l'executable." << std::endl;
        std::cerr << "Prem Enter per sortir..." << std::endl;
        std::cin.get();
        return 1;
    }
    return 0;
}
