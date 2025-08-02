#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <SFML/Graphics.hpp>

class BellmanFordVisualizer : public Visualizer {
private:
    struct Node {
        sf::CircleShape shape;
        sf::Text label;
        sf::Text distLabel;
    };

    std::map<int, Node> m_nodes;
    std::vector<std::tuple<int, int, int>> m_edges; // {u, v, weight}
    std::map<std::pair<int, int>, sf::Text> m_edgeWeights;
    sf::Font m_font;
    
    std::vector<int> m_distances;
    
    bool m_isVisualizing;
    bool m_isDone;
    int m_iteration;
    int m_edgeIndex;
    bool m_negativeCycle;

    sf::Clock m_clock;
    float m_delay = 0.3f;

    void buildGraph();

public:
    BellmanFordVisualizer(sf::RenderWindow& window);
    ~BellmanFordVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
