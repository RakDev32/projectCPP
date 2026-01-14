#include "Virus.h"
#include "graphics.h"
#include <cstdlib>

Virus::Virus(float x, float y) : Organism(x, y) {
    addNode(new Node(x, y, 20.0f));
}

void Virus::update(float dt) {
    // Τυχαία κίνηση (Random Walk) ή απλό AI
    m_pos_x += (rand() % 3 - 1) * 2.0f;
    m_pos_y += (rand() % 3 - 1) * 2.0f;

    if (!m_nodes.empty()) {
        m_nodes[0]->setX(m_pos_x);
        m_nodes[0]->setY(m_pos_y);
    }

    for (auto* node : m_nodes) {
        if (node) {
            // Υποθέτουμε ότι έχεις setX/setY στον Node
            node->setX(m_pos_x);
            node->setY(m_pos_y);
        }

    }
}

void Virus::draw() {
    for (auto* node : m_nodes) {
        if (node) node->draw();
    }
}