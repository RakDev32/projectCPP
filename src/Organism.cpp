#include "Organism.h"
#include <cmath>

Organism::Organism(float x, float y)
    : m_x(x), m_y(y), m_vx(0.0f), m_vy(0.0f)
{
}

Organism::~Organism()
{
    for (auto* node : m_nodes) {
        delete node;
    }
    m_nodes.clear();
}

void Organism::addNode(Node* n)
{
    if (n) m_nodes.push_back(n);
}

bool Organism::checkCollisionWithNode(const Node* target) const
{
    if (!target) return false;

    for (auto* my_node : m_nodes) {
        if (my_node && my_node->checkCollision(*target)) {
            return true;
        }
    }
    return false;
}
float Organism::getRadius() const
{
    if (m_nodes.empty() || !m_nodes[0]) return 0.0f;
    return m_nodes[0]->getRadius();
}

void Organism::setRadius(float r)
{
    if (!m_nodes.empty() && m_nodes[0]) {
        m_nodes[0]->setRadius(r);
    }
}

void Organism::growByArea(float eatenRadius)
{
    float r = getRadius();
    float newR = std::sqrt(r * r + eatenRadius * eatenRadius);
    setRadius(newR);
}