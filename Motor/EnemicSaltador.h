#pragma once
#include "Enemic.h"

// ============================================================
//  EnemicSaltador - nova tipologia d'enemic (punt 6)
//  Avanca cap a l'esquerra fent salts continus sobre el terra.
//  Es mes facil de disparar quan es a baix i mes perillos quan salta.
// ============================================================
class EnemicSaltador : public Enemic {
public:
    EnemicSaltador(sf::Vector2f posicio, float velocitat);
    void update(float dt) override;

private:
    float m_terra = 0.0f;   // Y on toca de peus
};
