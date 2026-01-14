#include "GlobalState.h"
#include "Hunter.h"
#include "Virus.h"
#include "Node.h"
#include <iostream>

// 1. Αρχικοποίηση του static δείκτη (ΥΠΟΧΡΕΩΤΙΚΟ)
GlobalState* GlobalState::m_instance = nullptr;

// 2. Υλοποίηση της getInstance
GlobalState* GlobalState::getInstance() {
    if (!m_instance) {
        m_instance = new GlobalState();
    }
    return m_instance;
}

// 3. Υλοποίηση του Destructor
GlobalState::~GlobalState() {
    for (auto* entity : m_entities) {
        delete entity;
    }
    m_entities.clear();
}

void GlobalState::init() {
    // ΜΟΝΟ ΔΗΜΙΟΥΡΓΙΑ ANTIKEIMENΩΝ ΕΔΩ
    m_entities.push_back(new Hunter(500, 300));
    for (int i = 0; i < 5; i++) {
        float rx = rand() % 1000;
        float ry = rand() % 600;
        m_entities.push_back(new Virus(rx, ry)); // ΠΟΛΥΜΟΡΦΙΣΜΟΣ
    }
}

void GlobalState::update(float dt) {
    for (auto* entity : m_entities) {
        if (entity) entity->update(dt);
    }
    auto* hunter = dynamic_cast<Hunter*>(m_entities[0]);

    if (hunter) {
        // Loop για το φαγητό (m_food)
        for (auto it = m_food.begin(); it != m_food.end(); ) {
            // Χρήση της μεθόδου που φτιάξαμε στον Organism
            if (hunter->checkCollisionWithNode(*it)) {
                m_score += 10; // Κέρδισε πόντους
                delete *it;    // Αποδέσμευση μνήμης
                it = m_food.erase(it); // Αφαίρεση από το STL vector

                // Πρόσθεσε νέο Node στον Hunter για να μεγαλώσει ο γράφος
                hunter->addNode(new Node(hunter->getX(), hunter->getY(), 10.0f));
            } else {
                ++it;
            }
        }
    }
}

void GlobalState::draw() {
    for (auto* entity : m_entities) {
        if (entity) entity->draw();
    }
}