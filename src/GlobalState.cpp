#include "GlobalState.h"
#include "Hunter.h"
#include "Virus.h"
#include "Node.h"
#include <algorithm>
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
    m_entities.clear();
    m_food.clear();

    // δημιούργησε player
    m_player = new Hunter(500.0f, 300.0f);
    m_entities.push_back(m_player);

    // δημιούργησε NPCs
    for (int i = 0; i < 15; ++i) {
        m_entities.push_back(new Virus((float)(rand() % 1000), (float)(rand() % 600)));
    }

    // δημιούργησε food
    for (int i = 0; i < 80; ++i) {
        m_food.push_back(new Node((float)(rand() % (int)m_worldW),
            (float)(rand() % (int)m_worldH),
            6.0f));
    }
}
static float clampf(float v, float lo, float hi)
{
    return (v < lo) ? lo : (v > hi) ? hi : v;
}
void GlobalState::destroyInstance()
{
    delete m_instance;
    m_instance = nullptr;


}

void GlobalState::update(float dt)
{
    if (dt <= 0.0f) return;
    if (dt > 0.05f) dt = 0.05f; // clamp dt για σταθερότητα

    // -----------------------------
    // 1) Update all entities
    // -----------------------------
    for (auto* e : m_entities) {
        if (e && e->isAlive()) {
            e->update(dt);
        }
    }

    // -----------------------------
    // 2) Keep entities inside world bounds (map)
    // -----------------------------
    for (auto* e : m_entities) {
        if (!e || !e->isAlive()) continue;

        float r = e->getRadius();
        float x = e->getX();
        float y = e->getY();

        x = clampf(x, r, m_worldW - r);
        y = clampf(y, r, m_worldH - r);

        e->setPosition(x, y);
    }

    // -----------------------------
    // 3) Eat & grow (entity vs entity)
    // -----------------------------
    const float EAT_MARGIN = 1.10f;

    for (int i = 0; i < (int)m_entities.size(); ++i) {
        for (int j = i + 1; j < (int)m_entities.size(); ++j) {
            Organism* A = m_entities[i];
            Organism* B = m_entities[j];

            if (!A || !B) continue;
            if (!A->isAlive() || !B->isAlive()) continue;

            float ax = A->getX(), ay = A->getY();
            float bx = B->getX(), by = B->getY();
            float rA = A->getRadius();
            float rB = B->getRadius();

            float dx = bx - ax;
            float dy = by - ay;

            float sum = rA + rB;
            float dist2 = dx * dx + dy * dy;

            if (dist2 < sum * sum) {
                // overlap -> eat if size advantage
                if (rA > rB * EAT_MARGIN) {
                    A->growByArea(rB);
                    B->kill();
                }
                else if (rB > rA * EAT_MARGIN) {
                    B->growByArea(rA);
                    A->kill();
                }
                // else: (προαιρετικό) separation αν δεν τρώει κανείς
            }
        }
    }

    // -----------------------------
    // 4) Player eats food (αν έχεις m_food)
    // -----------------------------
    if (m_player && m_player->isAlive()) {
        Node footprint(
            m_player->getX(),
            m_player->getY(),
            m_player->getRadius()
        );

        for (auto* f : m_food) {
            if (!f) continue;

            if (footprint.checkCollision(*f)) {
                m_player->growByArea(f->getRadius());
                f->setX((float)(rand() % (int)m_worldW));
                f->setY((float)(rand() % (int)m_worldH));
            }
        }
    }

    // -----------------------------
    // 5) Camera follows player
    // -----------------------------
    if (m_player) {
        float px = m_player->getX();
        float py = m_player->getY();

        m_camX = px - m_viewW * 0.5f;
        m_camY = py - m_viewH * 0.5f;

        m_camX = clampf(m_camX, 0.0f, m_worldW - m_viewW);
        m_camY = clampf(m_camY, 0.0f, m_worldH - m_viewH);
    }

    // -----------------------------
    // 6) Cleanup dead entities (safe erase/delete)
    // -----------------------------
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
            [](Organism* e) {
                if (!e) return true;
                if (!e->isAlive()) { delete e; return true; }
                return false;
            }),
        m_entities.end()
    );
}

void GlobalState::draw() {
        
    for (auto* f : m_food) {
        if (f) f->draw();
    }

    for (auto* entity : m_entities) {
        if (entity) entity->draw();
    }
    

    // 2) Draw entities
    for (auto* entity : m_entities) {
        if (entity) entity->draw();
    }

    // 3) Draw HUD
    graphics::Brush br;
    br.fill_color[0] = 1.0f; // R
    br.fill_color[1] = 1.0f; // G
    br.fill_color[2] = 1.0f; // B
    br.fill_opacity = 1.0f;
    graphics::drawText(10, 20, 16, "Score: " + std::to_string(m_score),br);
    graphics::drawText(10, 40, 16, "Food: " + std::to_string(m_food.size()),br);
}