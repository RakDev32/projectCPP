#pragma once
#include "Organism.h"

class Hunter : public Organism { // Κληρονομικότητα [cite: 77]
public:
    Hunter(float x, float y);
    float m_vx = 0.0f, m_vy = 0.0f;
    // Υλοποίηση των virtual μεθόδων 
    void update(float dt) override;
    void draw() override;
};