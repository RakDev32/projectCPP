#pragma once
#include <vector>
#include "Node.h"

class Organism {
protected:
    std::vector<Node*> m_nodes;

    float m_x = 0.0f;
    float m_y = 0.0f;
      
    float m_vx = 0.0f;
    float m_vy = 0.0f;

    bool m_alive = true;


public:
    Organism(float x, float y);
    virtual ~Organism();
    bool isAlive() const { return m_alive; }
    void kill() { m_alive = false; }

    // position
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    void setPosition(float x, float y);

    // velocity
    float getVx() const { return m_vx; }
    float getVy() const { return m_vy; }
    void setVelocity(float vx, float vy) { m_vx = vx; m_vy = vy; }
    
    float getRadius() const;
    void setRadius(float r);
    void growByArea(float eatenRadius);
    // nodes
    void addNode(Node* n);

    // collisions
    bool checkCollisionWithNode(const Node* target) const;

    // polymorphic API
    virtual void update(float dt) = 0;
    virtual void draw(float camX, float camY) const = 0;
};
