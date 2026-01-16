#pragma once
#include "graphics.h"
#include <vector>
#include "Organism.h" // Η κλάση βάσης που θα φτιάξεις
class Hunter;

class GlobalState {
private:

    // Singleton pattern: Ιδιωτικός constructor
    GlobalState() {}
    static GlobalState* m_instance;

    // Δεδομένα για τη ροή και την κατάσταση [cite: 86]
    float m_canvas_width = 1000.0f;
    float m_canvas_height = 600.0f;
    int m_score = 0;
    float m_worldW = 4000.0f;
    float m_worldH = 4000.0f;

    float m_camX = 0.0f;
    float m_camY = 0.0f;

    float m_viewW = 1000.0f;
    float m_viewH = 600.0f;

    Hunter* m_player = nullptr;

    // Συλλογή STL για τη διαχείριση των οντοτήτων [cite: 81, 82]
    std::vector<class Organism*> m_entities;

    std::vector<class Node*> m_food; // STL συλλογή για τα "φαγητά"
    


public:
    // Κεντρικό σημείο πρόσβασης
    static GlobalState* getInstance();

    // Υποχρεωτικές μέθοδοι [cite: 87]
    static void destroyInstance();
    void init();
    void update(float dt);
    void draw();

    float getCamX() const { return m_camX; }
    float getCamY() const { return m_camY; }

    // Getters για κεντρική πρόσβαση δεδομένων
    float getCanvasWidth() const { return m_canvas_width; }
    float getCanvasHeight() const { return m_canvas_height; }
    int getScore() const { return m_score; }
    void addScore(int s) { m_score += s; }



    // Καταστροφέας για αποδέσμευση δυναμικής μνήμης [cite: 76]
    ~GlobalState();
};
