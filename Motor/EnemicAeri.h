#pragma once
#include "Enemic.h"

// Enemic que vola cap a l'esquerra fent ones (moviment sinusoidal)
class EnemicAeri : public Enemic {
public:
    EnemicAeri(sf::Vector2f posicio, float velocitat);
    void update(float dt) override;

private:
    float m_alturaBase = 0.0f;   // Y al voltant de la qual oscil.la
    float m_temps      = 0.0f;   // temps acumulat per calcular l'ona
};
