#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <SFML/Graphics.hpp>

class KruskalVisualizer : public Visualizer {
private:
    struct Node {
        sf::CircleShape shape;
        sf::Text label;
    };
    
    struct Edge {
        int u, v, weight;
        bool inMst = false;
        bool isBeingTested = false;
    };

    std::map<int, Node> m_nodes;
    std::vector<Edge> m_edges;
    std::map<std::pair<int, int>, sf::Text> m_edgeWeights;
    sf::Font m_font;
    
    // DSU structure
    std::vector<int> m_parent;
    int findSet(int i);
    void unionSets(int i, int j);

    bool m_isVisualizing;
    bool m_isDone;
    int m_edgeIndex;

    sf::Clock m_clock;
    float m_delay = 1.0f;

    void buildGraph();

public:
    KruskalVisualizer(sf::RenderWindow& window);
    ~KruskalVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
