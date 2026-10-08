#pragma once
#include "Enemic.h"

// Enemic que corre per terra en linia recta cap a l'esquerra
class EnemicTerrestre : public Enemic {
public:
    EnemicTerrestre(sf::Vector2f posicio, float velocitat);
    void update(float dt) override;
};
