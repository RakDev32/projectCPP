#pragma once
#include <vector>
#include <utility>
#include "Node.h"

class Organism {
protected:
    std::vector<Node*> m_nodes;
    std::vector<std::pair<int, int>> m_edges;
    std::vector<std::pair<float, float>> m_nodeVelocities;
    size_t m_coreIndex = 0;

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
    float getMass() const;
    size_t getNodeCount() const { return m_nodes.size(); }
    void setRadius(float r);
    void growByArea(float eatenRadius);
    void growNodeByArea(size_t nodeIndex, float eatenRadius);
    void addNodeNear(size_t baseIndex, float radius);
    // nodes
    void addNode(Node* n);
    void addEdge(int fromIndex, int toIndex);
    void drawEdges(float camX, float camY) const;
    void drawGlow(float camX, float camY) const;

    // collisions
    bool checkCollisionWithNode(const Node* target) const;
    int findCollidingNode(const Node* target) const;
    bool checkCollisionWithOrganism(const Organism& other, int* outMyIndex, int* outOtherIndex) const;
    void updateInfection(float dt);
    void applyGraphForces(float dt);
    void infectNode(size_t nodeIndex);

    // polymorphic API
    virtual void update(float dt) = 0;
    virtual void draw(float camX, float camY) const = 0;
};
