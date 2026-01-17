#include "Virus.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

Virus::Virus(float x, float y) : Organism(x, y)
{
    float baseRadius = 12.0f + (float)(rand() % 22);
    auto* core = new Node(x, y, baseRadius);
    core->setColor(0.9f, 0.3f, 0.3f);
    addNode(core, 0.0f, 0.0f);

    const int ringCount = 5;
    float ringRadius = baseRadius * 0.9f;
    for (int i = 0; i < ringCount; ++i) {
        float angle = (6.2831853f / ringCount) * i;
        float ox = std::cos(angle) * ringRadius;
        float oy = std::sin(angle) * ringRadius;
        auto* node = new Node(x, y, baseRadius * 0.4f);
        node->setColor(0.7f, 0.15f, 0.2f);
        addNode(node, ox, oy);
        addEdge(0, (int)m_nodes.size() - 1);
        if (i > 0) {
            addEdge((int)m_nodes.size() - 2, (int)m_nodes.size() - 1);
        }
    }
    addEdge(1, (int)m_nodes.size() - 1);

    float mass = std::max(getMass(), 1.0f);
    m_speed = std::min(85.0f, 120.0f / std::sqrt(mass));
    m_wanderTimer = 0.0f;
}

void Virus::update(float dt)
{
    m_wanderTimer -= dt;
    if (m_wanderTimer <= 0.0f) {
        float angle = (float)rand() / (float)RAND_MAX * 6.2831853f;
        m_dirX = std::cos(angle);
        m_dirY = std::sin(angle);
        m_wanderTimer = 0.6f + (float)rand() / (float)RAND_MAX * 1.4f;
    }

    m_vx = m_dirX * m_speed;
    m_vy = m_dirY * m_speed;

    m_x += m_vx * dt;
    m_y += m_vy * dt;
    setPosition(m_x, m_y);
}

void Virus::draw(float camX, float camY) const
{
    drawEdges(camX, camY);
    for (auto* node : m_nodes) {
        if (!node) continue;
        node->draw(camX, camY);
    }
}
