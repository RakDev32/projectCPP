#pragma once
#include "Organism.h"

class Virus : public Organism {
public:
    Virus(float x, float y);
    void update(float dt) override;
    void draw() override;
};