#include "Hunter.h"
#include "graphics.h"
#include "Node.h"
#include <cstddef>

Hunter::Hunter(float x, float y) : Organism(x, y)
{
    addNode(new Node(x, y, 30.0f));
}

void Hunter::update(float dt)
{
    graphics::MouseState ms;
    graphics::getMouseState(ms);

    // (προς το παρόν) απλό follow - μετά το κάνουμε agar inertia
    m_x = ms.cur_pos_x;
    m_y = ms.cur_pos_y;

    // ενημέρωση nodes στη νέα θέση
    for (auto* node : m_nodes) {
        if (node) {
            node->setX(m_x);
            node->setY(m_y);
        }
    }
}

void Hunter::draw() const
{
    graphics::Brush br;
    br.outline_opacity = 1.0f;
    br.fill_opacity = 0.6f;

    // γραμμές μεταξύ nodes (αν έχεις πολλά)
    if (m_nodes.size() > 1) {
        for (size_t i = 0; i + 1 < m_nodes.size(); ++i) {
            graphics::drawLine(
                m_nodes[i]->getX(), m_nodes[i]->getY(),
                m_nodes[i + 1]->getX(), m_nodes[i + 1]->getY(),
                br
            );
        }
    }

    // draw nodes
    for (auto* node : m_nodes) {
        if (node) node->draw();
    }
}
