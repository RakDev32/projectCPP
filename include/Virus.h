#pragma once
#include "Organism.h"

class Virus : public Organism {
public:
    Virus(float x, float y);
    void update(float dt) override;
    void draw(float camX, float camY) const override;
private:
    float m_wanderTimer = 0.0f;
    float m_dirX = 0.0f;
    float m_dirY = 0.0f;
    float m_speed = 70.0f;
};
