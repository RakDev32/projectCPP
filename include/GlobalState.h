#pragma once
#include "graphics.h"
#include <vector>
#include "Organism.h"
class Hunter;

class GlobalState {
private:

    GlobalState() {}
    static GlobalState* m_instance;

    float m_canvas_width = 1000.0f;
    float m_canvas_height = 600.0f;
    int m_score = 0;
    int m_highScore = 0;
    float m_worldW = 4000.0f;
    float m_worldH = 4000.0f;

    float m_camX = 0.0f;
    float m_camY = 0.0f;

    float m_viewW = 1000.0f;
    float m_viewH = 600.0f;

    Hunter* m_player = nullptr;
    bool m_gameOver = false;
    int m_minNpcCount = 12;

    std::vector<class Organism*> m_entities;

    std::vector<class Node*> m_food;
    std::vector<class DroppedPellet*> m_pellets;
    float m_time = 0.0f;
    


public:
    static GlobalState* getInstance();

    static void destroyInstance();
    void init();
    void reset();
    void update(float dt);
    void draw();

    float getCamX() const { return m_camX; }
    float getCamY() const { return m_camY; }

    float getCanvasWidth() const { return m_canvas_width; }
    float getCanvasHeight() const { return m_canvas_height; }
    int getScore() const { return m_score; }
    int getHighScore() const { return m_highScore; }

    ~GlobalState();
};
