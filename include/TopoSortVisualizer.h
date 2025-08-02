#pragma once
#include "Visualizer.h"
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <string>
#include <SFML/Graphics.hpp>

class TopoSortVisualizer : public Visualizer {
public:
    enum class AlgoType { DFS_BASED, KAHN };

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
    AlgoType m_algoType;
    std::string m_algoName;

    // Data structures for algorithms
    std::vector<int> m_inDegree;
    std::stack<int> m_dfsStack;
    std::queue<int> m_kahnQueue;
    std::vector<int> m_sortedResult;

    bool m_isVisualizing;
    bool m_isDone;
    sf::Clock m_clock;
    float m_delay = 0.8f;

    void buildGraph();
    void resetNodeStates();
    void dfs(int u);

public:
    TopoSortVisualizer(sf::RenderWindow& window, AlgoType type);
    ~TopoSortVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
