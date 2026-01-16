#include "GlobalState.h"
#include "Hunter.h"
#include "Virus.h"
#include "Node.h"
#include <algorithm>
#include <cmath>
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
    m_score = 0;
    m_gameOver = false;

    // δημιούργησε player
    m_player = new Hunter(m_worldW * 0.5f, m_worldH * 0.5f);
    m_entities.push_back(m_player);

    // δημιούργησε NPCs
    for (int i = 0; i < 15; ++i) {
        m_entities.push_back(new Virus((float)(rand() % (int)m_worldW), (float)(rand() % (int)m_worldH)));
    }

    // δημιούργησε food
    for (int i = 0; i < 80; ++i) {
        m_food.push_back(new Node((float)(rand() % (int)m_worldW),
            (float)(rand() % (int)m_worldH),
            6.0f));
    }
}

void GlobalState::reset()
{
    for (auto* entity : m_entities) {
        delete entity;
    }
    m_entities.clear();
    m_food.clear();
    init();
}
static float clampf(float v, float lo, float hi)
{
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

static void separateEntities(Organism* a, Organism* b, float dx, float dy, float dist, float overlap)
{
    if (!a || !b) return;
    if (dist < 0.001f) {
        dx = 1.0f;
        dy = 0.0f;
        dist = 1.0f;
    }

    float nx = dx / dist;
    float ny = dy / dist;
    float push = overlap * 0.5f;

    a->setPosition(a->getX() - nx * push, a->getY() - ny * push);
    b->setPosition(b->getX() + nx * push, b->getY() + ny * push);

    a->setVelocity(a->getVx() - nx * push * 2.0f, a->getVy() - ny * push * 2.0f);
    b->setVelocity(b->getVx() + nx * push * 2.0f, b->getVy() + ny * push * 2.0f);
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
    if (m_gameOver) {
        if (graphics::getKeyState(graphics::SCANCODE_R)) {
            reset();
        }
        return;
    }

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

        float clampedX = clampf(x, r, m_worldW - r);
        float clampedY = clampf(y, r, m_worldH - r);

        if (clampedX != x) {
            e->setVelocity(-e->getVx() * 0.5f, e->getVy());
        }
        if (clampedY != y) {
            e->setVelocity(e->getVx(), -e->getVy() * 0.5f);
        }

        e->setPosition(clampedX, clampedY);
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
                float dist = std::sqrt(dist2);
                float overlap = sum - dist;
                // overlap -> eat if size advantage
                if (rA > rB * EAT_MARGIN) {
                    A->growByArea(rB);
                    if (A == m_player) {
                        addScore(10);
                    }
                    if (B == m_player) {
                        m_gameOver = true;
                    }
                    B->kill();
                }
                else if (rB > rA * EAT_MARGIN) {
                    B->growByArea(rA);
                    if (B == m_player) {
                        addScore(10);
                    }
                    if (A == m_player) {
                        m_gameOver = true;
                    }
                    A->kill();
                }
                else {
                    separateEntities(A, B, dx, dy, dist, overlap);
                }
            }
        }
    }

    // -----------------------------
    // 4) Entities eat food
    // -----------------------------
    for (auto* entity : m_entities) {
        if (!entity || !entity->isAlive()) continue;

        for (auto* f : m_food) {
            if (!f) continue;

            if (entity->checkCollisionWithNode(f)) {
                entity->growByArea(f->getRadius());
                if (entity == m_player) {
                    addScore(1);
                }
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

    int npcCount = 0;
    for (auto* entity : m_entities) {
        if (entity && entity != m_player) {
            ++npcCount;
        }
    }
    while (npcCount < m_minNpcCount) {
        m_entities.push_back(new Virus((float)(rand() % (int)m_worldW), (float)(rand() % (int)m_worldH)));
        ++npcCount;
    }
}

void GlobalState::draw() {
    for (auto* f : m_food) {
        if (f) f->draw(m_camX, m_camY);
    }

    for (auto* entity : m_entities) {
        if (entity) entity->draw(m_camX, m_camY);
    }

    // 3) Draw HUD
    graphics::Brush br;
    br.fill_color[0] = 1.0f; // R
    br.fill_color[1] = 1.0f; // G
    br.fill_color[2] = 1.0f; // B
    br.fill_opacity = 1.0f;
    graphics::drawText(10, 20, 16, "Score: " + std::to_string(m_score),br);
    graphics::drawText(10, 40, 16, "Food: " + std::to_string(m_food.size()),br);
    if (m_gameOver) {
        graphics::Brush overlay;
        overlay.fill_color[0] = 0.0f;
        overlay.fill_color[1] = 0.0f;
        overlay.fill_color[2] = 0.0f;
        overlay.fill_opacity = 0.6f;
        overlay.outline_opacity = 0.0f;
        graphics::drawRect(m_viewW * 0.5f, m_viewH * 0.5f, m_viewW, m_viewH, overlay);

        graphics::Brush textBr;
        textBr.fill_color[0] = 1.0f;
        textBr.fill_color[1] = 1.0f;
        textBr.fill_color[2] = 1.0f;
        textBr.fill_opacity = 1.0f;
        graphics::drawText(m_viewW * 0.5f - 80.0f, m_viewH * 0.5f - 10.0f, 24, "Game Over", textBr);
        graphics::drawText(m_viewW * 0.5f - 130.0f, m_viewH * 0.5f + 20.0f, 16, "Press R to play again", textBr);
    }

    // 4) Minimap
    const float miniW = 180.0f;
    const float miniH = 180.0f;
    const float miniPadding = 12.0f;
    const float miniCenterX = m_viewW - miniW * 0.5f - miniPadding;
    const float miniCenterY = miniH * 0.5f + miniPadding;

    graphics::Brush miniBr;
    miniBr.fill_color[0] = 0.05f;
    miniBr.fill_color[1] = 0.05f;
    miniBr.fill_color[2] = 0.05f;
    miniBr.fill_opacity = 0.6f;
    miniBr.outline_opacity = 1.0f;
    graphics::drawRect(miniCenterX, miniCenterY, miniW, miniH, miniBr);

    float scaleX = miniW / m_worldW;
    float scaleY = miniH / m_worldH;
    float miniLeft = miniCenterX - miniW * 0.5f;
    float miniTop = miniCenterY - miniH * 0.5f;

    graphics::Brush dotBr;
    dotBr.fill_opacity = 1.0f;
    dotBr.outline_opacity = 0.0f;

    for (auto* f : m_food) {
        if (!f) continue;
        float sx = miniLeft + f->getX() * scaleX;
        float sy = miniTop + f->getY() * scaleY;
        dotBr.fill_color[0] = 0.2f;
        dotBr.fill_color[1] = 0.9f;
        dotBr.fill_color[2] = 0.2f;
        graphics::drawDisk(sx, sy, 2.0f, dotBr);
    }

    for (auto* entity : m_entities) {
        if (!entity) continue;
        float sx = miniLeft + entity->getX() * scaleX;
        float sy = miniTop + entity->getY() * scaleY;
        if (entity == m_player) {
            dotBr.fill_color[0] = 0.2f;
            dotBr.fill_color[1] = 0.6f;
            dotBr.fill_color[2] = 1.0f;
            graphics::drawDisk(sx, sy, 3.5f, dotBr);
        } else {
            dotBr.fill_color[0] = 1.0f;
            dotBr.fill_color[1] = 0.4f;
            dotBr.fill_color[2] = 0.4f;
            graphics::drawDisk(sx, sy, 3.0f, dotBr);
        }
    }

    graphics::Brush viewBr;
    viewBr.fill_opacity = 0.0f;
    viewBr.outline_opacity = 1.0f;
    viewBr.outline_color[0] = 1.0f;
    viewBr.outline_color[1] = 1.0f;
    viewBr.outline_color[2] = 1.0f;

    float viewMiniW = m_viewW * scaleX;
    float viewMiniH = m_viewH * scaleY;
    float viewCenterX = miniLeft + (m_camX + m_viewW * 0.5f) * scaleX;
    float viewCenterY = miniTop + (m_camY + m_viewH * 0.5f) * scaleY;
    graphics::drawRect(viewCenterX, viewCenterY, viewMiniW, viewMiniH, viewBr);
}
