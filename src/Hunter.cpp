#include "Hunter.h"
#include "graphics.h"
#include "GlobalState.h"

Hunter::Hunter(float x, float y) : Organism(x, y) {
    // Ξεκινάμε με έναν κεντρικό κόμβο (Node) [cite: 76, 79]
    addNode(new Node(x, y, 30.0f));
}

void Hunter::update(float dt) {
    graphics::MouseState ms;
    graphics::getMouseState(ms);

    float dx = ms.cur_pos_x - m_pos_x;
    float dy = ms.cur_pos_y - m_pos_y;

    float len = std::sqrt(dx * dx + dy * dy);
    if (len > 1.0f) {
        dx /= len; dy /= len;
        const float speed = 250.0f;
        m_pos_x += dx * speed * dt;
        m_pos_y += dy * speed * dt;
    }

    for (auto* node : m_nodes) {
        if (node) { node->setX(m_pos_x); node->setY(m_pos_y); }
    }
}

void Hunter::draw() {
    graphics::Brush br;
    br.outline_opacity = 1.0f;
    br.fill_opacity = 0.5f;

    // Αν έχεις πάνω από έναν κόμβο, ένωσέ τους
    if (m_nodes.size() > 1) {
        for (size_t i = 0; i < m_nodes.size() - 1; ++i) {
            graphics::drawLine(m_nodes[i]->getX(), m_nodes[i]->getY(),
                              m_nodes[i+1]->getX(), m_nodes[i+1]->getY(), br);
        }
    }

    // Σχεδίασε τους ίδιους τους κόμβους
    for (auto* node : m_nodes) node->draw();

}