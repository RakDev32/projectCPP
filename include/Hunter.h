#pragma once
#include "Organism.h"

class Hunter : public Organism {
public:
    Hunter(float x, float y);

    void update(float dt) override;
    void draw(float camX, float camY) const override;
private:
    float m_maxSpeed;
    float m_accel;
};
