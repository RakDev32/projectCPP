#include "Hunter.h"
#include "GlobalState.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstddef>
#include <cmath>

Hunter::Hunter(float x, float y) : Organism(x, y)
{
    addNode(new Node(x, y, 30.0f), 0.0f, 0.0f);
    addNode(new Node(x, y, 16.0f), 28.0f, 6.0f);
    addNode(new Node(x, y, 12.0f), -24.0f, -10.0f);
    m_maxSpeed = 240.0f;
    m_accel = 520.0f;
}

void Hunter::update(float dt)
{
    graphics::MouseState ms;
    graphics::getMouseState(ms);

    GlobalState* gs = GlobalState::getInstance();
    float mouseWorldX = ms.cur_pos_x + gs->getCamX();
    float mouseWorldY = ms.cur_pos_y + gs->getCamY();

    float dx = mouseWorldX - m_x;
    float dy = mouseWorldY - m_y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 1.0f) {
        float dirX = dx / dist;
        float dirY = dy / dist;

        float radius = getRadius();
        float speedScale = 30.0f / std::max(radius, 10.0f);
        float maxSpeed = m_maxSpeed * speedScale;

        m_vx += dirX * m_accel * dt;
        m_vy += dirY * m_accel * dt;

        float speed = std::sqrt(m_vx * m_vx + m_vy * m_vy);
        if (speed > maxSpeed) {
            float s = maxSpeed / speed;
            m_vx *= s;
            m_vy *= s;
        }
    } else {
        m_vx *= 0.9f;
        m_vy *= 0.9f;
    }

    m_x += m_vx * dt;
    m_y += m_vy * dt;
    setPosition(m_x, m_y);
}

void Hunter::draw(float camX, float camY) const
{
    graphics::Brush br;
    br.outline_opacity = 1.0f;
    br.fill_opacity = 0.6f;

    // γραμμές μεταξύ nodes (αν έχεις πολλά)
    if (m_nodes.size() > 1) {
        for (size_t i = 0; i + 1 < m_nodes.size(); ++i) {
            graphics::drawLine(
                m_nodes[i]->getX() - camX, m_nodes[i]->getY() - camY,
                m_nodes[i + 1]->getX() - camX, m_nodes[i + 1]->getY() - camY,
                br
            );
        }
    }

    // draw nodes
    for (auto* node : m_nodes) {
        if (node) node->draw(camX, camY);
    }
}
