#pragma once
#include <vector>
#include "Node.h"

class Organism {
protected:
    std::vector<Node*> m_nodes;
    float m_pos_x, m_pos_y;
    float vx, vy;

public:
    // ΕΔΩ ΜΟΝΟ Η ΔΗΛΩΣΗ (Πρόσεξε το ερωτηματικό στο τέλος)
    Organism(float x, float y);

    float getX() const { return m_pos_x; }
    float getY() const { return m_pos_y; }

    virtual void update(float dt) = 0;
    virtual void draw() = 0;
    virtual ~Organism();
    void addNode(Node* n);

    // Μέθοδος για το collision που φτιάξαμε πριν
    bool checkCollisionWithNode(const Node* target) const;
};