#pragma once
#include "Organism.h"

class Hunter : public Organism { // Κληρονομικότητα [cite: 77]
public:
    Hunter(float x, float y);

    void update(float dt) override;
    void draw() const override;
private:
    float m_maxSpeed;
    float m_accel;
};