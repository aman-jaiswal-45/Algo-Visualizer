#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>

class DijkstraVisualizer : public Visualizer {
private:
    struct Node {
        sf::CircleShape shape;
        sf::Text label;
        sf::Text distLabel;
        std::vector<std::pair<int, int>> adj; // {neighbor, weight}
    };

    std::map<int, Node> m_nodes;
    std::vector<sf::VertexArray> m_edges;
    std::map<std::pair<int, int>, sf::Text> m_edgeWeights;
    sf::Font m_font;
    
    std::vector<int> m_distances;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> m_pq;
    std::vector<bool> m_visited;

    bool m_isVisualizing;
    bool m_isDone;
    sf::Clock m_clock;
    float m_delay = 0.6f;

    void buildGraph();

public:
    DijkstraVisualizer(sf::RenderWindow& window);
    ~DijkstraVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
