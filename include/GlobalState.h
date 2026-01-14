#pragma once
#include "graphics.h"
#include <vector>
#include "Organism.h" // Η κλάση βάσης που θα φτιάξεις

class GlobalState {
private:
    // Singleton pattern: Ιδιωτικός constructor
    GlobalState() {}
    static GlobalState* m_instance;

    // Δεδομένα για τη ροή και την κατάσταση [cite: 86]
    float m_canvas_width = 1000.0f;
    float m_canvas_height = 500.0f;
    int m_score = 0;

    // Συλλογή STL για τη διαχείριση των οντοτήτων [cite: 81, 82]
    std::vector<class Organism*> m_entities;

    std::vector<class Node*> m_food; // STL συλλογή για τα "φαγητά"


public:
    // Κεντρικό σημείο πρόσβασης
    static GlobalState* getInstance();

    // Υποχρεωτικές μέθοδοι [cite: 87]
    void init();
    void update(float dt);
    void draw();

    // Getters για κεντρική πρόσβαση δεδομένων
    float getCanvasWidth() const { return m_canvas_width; }
    float getCanvasHeight() const { return m_canvas_height; }
    int getScore() const { return m_score; }
    void addScore(int s) { m_score += s; }

    // Καταστροφέας για αποδέσμευση δυναμικής μνήμης [cite: 76]
    ~GlobalState();
};