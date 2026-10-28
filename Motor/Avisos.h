#pragma once
// ============================================================
//  Avisos.h - registre d'incidencies de carrega (correccio INC-01/02/03)
//  Quan falta un recurs o el nivell es incorrecte, el joc NO es tanca:
//  apunta l'avis aqui, l'escriu a la consola (std::cerr) i el mostra
//  a la pantalla perque es vegi tambe en pantalla completa.
// ============================================================
#include <iostream>
#include <string>
#include <vector>

class Avisos {
public:
    static void afegeix(const std::string& missatge) {
        llista().push_back(missatge);
        std::cerr << "[AVIS] " << missatge << std::endl;
    }

    static const std::vector<std::string>& tots() { return llista(); }

private:
    static std::vector<std::string>& llista() {
        static std::vector<std::string> avisos;
        return avisos;
    }
};
