#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>

class GraphVisualizer : public Visualizer {
public:
    enum class TraversalType { DFS, BFS };

private:
    struct Node {
        sf::CircleShape shape;
        sf::Text label;
        std::vector<int> adj;
        enum class State { Unvisited, Visiting, Visited } state = State::Unvisited;
    };

    std::map<int, Node> m_adjList;
    std::vector<sf::VertexArray> m_edges;
    sf::Font m_font;
    TraversalType m_traversalType;
    std::string m_traversalName;

    // Data structures for traversals
    std::stack<int> m_dfsStack;
    std::queue<int> m_bfsQueue;

    // State for visualization
    bool m_isVisualizing;
    bool m_isDone;
    int m_componentCheckIndex; // To find next unvisited component
    sf::Clock m_clock;
    float m_delay = 0.5f; // seconds

    void buildGraph();
    void resetNodeStates();
    bool startNextTraversal();

public:
    GraphVisualizer(sf::RenderWindow& window, TraversalType type);
    ~GraphVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
