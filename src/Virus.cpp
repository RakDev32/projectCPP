#include "Virus.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

Virus::Virus(float x, float y) : Organism(x, y)
{
    float baseRadius = 14.0f + (float)(rand() % 18);
    float r1 = baseRadius;
    float r2 = baseRadius * 0.5f;
    float r3 = baseRadius * 0.4f;

    float offsetX = baseRadius * 0.9f;
    float offsetY = baseRadius * 0.6f;

    addNode(new Node(x, y, r1), 0.0f, 0.0f);
    addNode(new Node(x, y, r2), -offsetX, offsetY);
    addNode(new Node(x, y, r3), offsetX * 0.9f, -offsetY);

    m_speed = 90.0f * (20.0f / std::max(baseRadius, 8.0f));
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
    for (auto* node : m_nodes) {
        if (node) node->draw(camX, camY);
    }
}
