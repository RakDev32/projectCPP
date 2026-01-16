#include "Virus.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

Virus::Virus(float x, float y) : Organism(x, y)
{
    float baseRadius = 12.0f + (float)(rand() % 16);
    float r1 = baseRadius;
    float r2 = baseRadius * 0.45f;
    float r3 = baseRadius * 0.35f;

    float offsetX = baseRadius * 0.5f;
    float offsetY = baseRadius * 0.35f;

    addNode(new Node(x, y, r1), 0.0f, 0.0f);
    addNode(new Node(x, y, r2), -offsetX, offsetY);
    addNode(new Node(x, y, r3), offsetX * 0.7f, -offsetY * 0.8f);

    float mass = std::max(getMass(), 1.0f);
    m_speed = 110.0f / std::sqrt(mass);
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
