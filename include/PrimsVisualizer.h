#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>

class PrimsVisualizer : public Visualizer {
private:
    struct Node {
        sf::CircleShape shape;
        sf::Text label;
        std::vector<std::pair<int, int>> adj; // {neighbor, weight}
    };

    std::map<int, Node> m_nodes;
    std::map<std::pair<int, int>, sf::Text> m_edgeWeights;
    sf::Font m_font;
    
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> m_pq; // {weight, u, v}
    std::vector<bool> m_inMst;
    std::vector<std::pair<int,int>> m_mstEdges;

    bool m_isVisualizing;
    bool m_isDone;
    int m_currentNode;

    sf::Clock m_clock;
    float m_delay = 1.0f;

    void buildGraph();
    void addEdges(int u);

public:
    PrimsVisualizer(sf::RenderWindow& window);
    ~PrimsVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
