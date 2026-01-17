#include "Hunter.h"
#include "GlobalState.h"
#include "graphics.h"
#include "Node.h"
#include <algorithm>
#include <cstddef>
#include <cmath>

Hunter::Hunter(float x, float y) : Organism(x, y)
{
    auto* core = new Node(x, y, 24.0f);
    core->setColor(0.2f, 0.7f, 1.0f);
    addNode(core, 0.0f, 0.0f);

    auto* petalA = new Node(x, y, 10.0f);
    petalA->setColor(0.1f, 0.45f, 0.9f);
    addNode(petalA, 10.0f, 5.0f);

    auto* petalB = new Node(x, y, 8.0f);
    petalB->setColor(0.05f, 0.3f, 0.7f);
    addNode(petalB, -8.0f, -6.0f);

    m_maxSpeed = 90.0f;
    m_accel = 240.0f;
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

        float mass = std::max(getMass(), 1.0f);
        float speedScale = 120.0f / std::sqrt(mass);
        float maxSpeed = std::min(m_maxSpeed * speedScale, 140.0f);

        float targetVx = dirX * maxSpeed;
        float targetVy = dirY * maxSpeed;
        float steer = 8.0f;
        m_vx += (targetVx - m_vx) * steer * dt;
        m_vy += (targetVy - m_vy) * steer * dt;
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
    br.fill_opacity = 0.7f;

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
    graphics::Brush inner;
    inner.fill_color[0] = 1.0f;
    inner.fill_color[1] = 1.0f;
    inner.fill_color[2] = 1.0f;
    inner.fill_opacity = 0.5f;
    inner.outline_opacity = 0.0f;

    for (auto* node : m_nodes) {
        if (!node) continue;
        node->draw(camX, camY);
        graphics::drawDisk(node->getX() - camX, node->getY() - camY, node->getRadius() * 0.35f, inner);
    }
}
