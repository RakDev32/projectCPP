#include "Virus.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

Virus::Virus(float x, float y) : Organism(x, y)
{
    float baseRadius = 8.0f + (float)(rand() % 33);
    auto* core = new Node(x, y, baseRadius);
    core->setColor(0.9f, 0.3f, 0.3f);
    addNode(core);

    int ringCount = 2 + (rand() % 11); // 2..12

    float ringRadius = baseRadius * 0.7f;
    for (int i = 0; i < ringCount; ++i) {
        float angle = (6.2831853f / ringCount) * i;
        float ox = std::cos(angle) * ringRadius;
        float oy = std::sin(angle) * ringRadius;
        float scale = 0.35f + ((float)rand() / (float)RAND_MAX) * 0.20f;
        auto* node = new Node(x + ox, y + oy, baseRadius * scale);
        node->setColor(0.7f, 0.15f, 0.2f);
        addNode(node);
        addEdge(0, (int)m_nodes.size() - 1);
    }
    recomputeLayout();
    rebuildTopology();

    float mass = std::max(getMass(), 1.0f);
    m_speed = std::min(80.0f, 110.0f / std::sqrt(mass));
    m_wanderTimer = 0.0f;
}

void Virus::update(float dt)
{
    m_wanderTimer -= dt;
    if (m_wanderTimer <= 0.0f) {
        float angle = (float)rand() / (float)RAND_MAX * 6.2831853f;
        m_dirX = std::cos(angle);
        m_dirY = std::sin(angle);
        m_wanderTimer = 0.6f + (float)rand() / (float)RAND_MAX * 1.2f;
    }

    m_vx = m_dirX * m_speed;
    m_vy = m_dirY * m_speed;

    m_x += m_vx * dt;
    m_y += m_vy * dt;
    if (!m_nodes.empty() && m_nodes[0]) {
        m_nodes[0]->setX(m_x);
        m_nodes[0]->setY(m_y);
    }
    recomputeLayout();
}

void Virus::draw(float camX, float camY) const
{
    drawGlow(camX, camY);
    drawEdges(camX, camY);
    for (auto* node : m_nodes) {
        if (!node) continue;
        node->draw(camX, camY);
    }
}
