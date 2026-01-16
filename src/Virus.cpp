#include "Virus.h"
#include "graphics.h"
#include "Node.h"
#include <cstdlib>
#include <cmath>

Virus::Virus(float x, float y) : Organism(x, y)
{
    addNode(new Node(x, y, 20.0f));
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
