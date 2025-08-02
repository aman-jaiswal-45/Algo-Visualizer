#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

class FloydWarshallVisualizer : public Visualizer {
private:
    int m_nodeCount;
    std::vector<std::vector<int>> m_distMatrix;
    sf::Font m_font;
    std::vector<sf::Text> m_matrixText;

    bool m_isVisualizing;
    bool m_isDone;
    int m_k, m_i, m_j; // Loop indices

    sf::Clock m_clock;
    float m_delay = 0.05f;

    void buildGraph();
    void setupText();

public:
    FloydWarshallVisualizer(sf::RenderWindow& window);
    ~FloydWarshallVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
