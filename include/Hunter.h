#pragma once
#include "Organism.h"

class Hunter : public Organism { // Κληρονομικότητα [cite: 77]
public:
    Hunter(float x, float y);
    
    // Υλοποίηση των virtual μεθόδων 
    void update(float dt) override;
    void draw() override;
};