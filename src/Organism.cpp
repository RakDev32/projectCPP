#include "Organism.h"
#include <algorithm>
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
    addNode(n, 0.0f, 0.0f);
}

void Organism::addNode(Node* n, float offsetX, float offsetY)
{
    if (n) {
        m_nodes.push_back(n);
        m_nodeOffsets.emplace_back(offsetX, offsetY);
    }
}

void Organism::setPosition(float x, float y)
{
    m_x = x;
    m_y = y;
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        auto* node = m_nodes[i];
        if (node) {
            node->setX(m_x + m_nodeOffsets[i].first);
            node->setY(m_y + m_nodeOffsets[i].second);
        }
    }
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
    if (m_nodes.empty()) return 0.0f;
    float maxR = 0.0f;
    for (auto* node : m_nodes) {
        if (node) {
            maxR = std::max(maxR, node->getRadius());
        }
    }
    return maxR;
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
    if (r <= 0.0f) return;
    float newR = std::sqrt(r * r + eatenRadius * eatenRadius);
    float scale = newR / r;
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        if (m_nodes[i]) {
            m_nodes[i]->setRadius(m_nodes[i]->getRadius() * scale);
        }
        m_nodeOffsets[i].first *= scale;
        m_nodeOffsets[i].second *= scale;
    }
    setPosition(m_x, m_y);
}
