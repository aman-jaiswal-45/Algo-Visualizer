#include "TopoSortVisualizer.h"
#include <iostream>
#include <algorithm>

using namespace std;

TopoSortVisualizer::TopoSortVisualizer(sf::RenderWindow& window, AlgoType type)
    : Visualizer(window), m_algoType(type), m_isVisualizing(false), m_isDone(false) {
    if (!m_font.loadFromFile("assets/arial.ttf")) {
        cerr << "Error loading font in TopoSortVisualizer" << endl;
    }
    m_algoName = (type == AlgoType::DFS_BASED) ? "Topo Sort (DFS)" : "Topo Sort (Kahn's)";
}

void TopoSortVisualizer::buildGraph() {
    m_adjList.clear();
    m_edges.clear();
    map<int, sf::Vector2f> positions = {
        {0, {200, 150}}, {1, {200, 350}}, {2, {400, 250}},
        {3, {600, 150}}, {4, {600, 350}}, {5, {800, 250}}
    };
    for(const auto& p : positions) {
        Node node;
        node.shape.setRadius(30.f);
        node.shape.setOrigin(30.f, 30.f);
        node.shape.setPosition(p.second);
        node.label.setFont(m_font);
        node.label.setString(to_string(p.first));
        node.label.setCharacterSize(24);
        sf::FloatRect textRect = node.label.getLocalBounds();
        node.label.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        node.label.setPosition(p.second);
        m_adjList[p.first] = node;
    }
    auto addEdge = [&](int u, int v) {
        m_adjList[u].adj.push_back(v);
        sf::VertexArray line(sf::Lines, 2);
        line[0] = {m_adjList[u].shape.getPosition(), sf::Color::White};
        line[1] = {m_adjList[v].shape.getPosition(), sf::Color::White};
        m_edges.push_back(line);
    };
    addEdge(0, 2); addEdge(1, 2); addEdge(2, 3);
    addEdge(2, 4); addEdge(3, 5); addEdge(4, 5);
}

void TopoSortVisualizer::resetNodeStates() {
    for (auto& pair : m_adjList) pair.second.state = Node::State::Unvisited;
}

void TopoSortVisualizer::reset() {
    buildGraph();
    resetNodeStates();
    m_sortedResult.clear();
    m_isVisualizing = false;
    m_isDone = false;

    if (m_algoType == AlgoType::DFS_BASED) {
        while(!m_dfsStack.empty()) m_dfsStack.pop();
    } else { // Kahn's
        m_inDegree.assign(m_adjList.size(), 0);
        for(const auto& pair : m_adjList) {
            for(int neighbor : pair.second.adj) m_inDegree[neighbor]++;
        }
        while(!m_kahnQueue.empty()) m_kahnQueue.pop();
        for(size_t i = 0; i < m_adjList.size(); ++i) {
            if (m_inDegree[i] == 0) m_kahnQueue.push(i);
        }
    }
    cout << m_algoName << " reset. Press SPACE to start." << endl;
}

void TopoSortVisualizer::dfs(int u) {
    m_adjList[u].state = Node::State::Visiting;
    for (int v : m_adjList[u].adj) {
        if (m_adjList[v].state == Node::State::Unvisited) dfs(v);
    }
    m_adjList[u].state = Node::State::Visited;
    m_dfsStack.push(u);
}

void TopoSortVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isDone) m_isVisualizing = !m_isVisualizing;
        if (event.key.code == sf::Keyboard::R) reset();
    }
}

void TopoSortVisualizer::update() {
    if (!m_isVisualizing || m_isDone || m_clock.getElapsedTime().asSeconds() < m_delay) return;
    m_clock.restart();

    if (m_algoType == AlgoType::DFS_BASED) {
        if (m_sortedResult.size() == m_adjList.size()) {
            m_isDone = true; m_isVisualizing = false; return;
        }
        // Run full DFS at once, then pop from stack for visualization
        if (m_dfsStack.empty()) {
            for(size_t i = 0; i < m_adjList.size(); ++i) {
                if (m_adjList[i].state == Node::State::Unvisited) dfs(i);
            }
        }
        int u = m_dfsStack.top();
        m_dfsStack.pop();
        m_sortedResult.push_back(u);
    } else { // Kahn's
        if (m_kahnQueue.empty()) {
            m_isDone = true; m_isVisualizing = false; return;
        }
        int u = m_kahnQueue.front();
        m_kahnQueue.pop();
        m_sortedResult.push_back(u);
        m_adjList[u].state = Node::State::Visited;
        for (int v : m_adjList[u].adj) {
            m_inDegree[v]--;
            if (m_inDegree[v] == 0) m_kahnQueue.push(v);
        }
    }
}

void TopoSortVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));
    for (const auto& edge : m_edges) m_window.draw(edge);
    for (auto& pair : m_adjList) {
        if (pair.second.state == Node::State::Visited) pair.second.shape.setFillColor(sf::Color::Green);
        else if (pair.second.state == Node::State::Visiting) pair.second.shape.setFillColor(sf::Color::Yellow);
        else pair.second.shape.setFillColor(sf::Color(100, 100, 250));
        m_window.draw(pair.second.shape);
        m_window.draw(pair.second.label);
    }
    sf::Text infoText;
    infoText.setFont(m_font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    string status = m_isDone ? "Complete!" : (m_isVisualizing ? "Visualizing..." : "Paused.");
    infoText.setString(m_algoName + " | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);

    sf::Text resultText;
    resultText.setFont(m_font);
    resultText.setCharacterSize(24);
    resultText.setFillColor(sf::Color::White);
    string resultStr = "Sorted Order: ";
    for(int node : m_sortedResult) resultStr += to_string(node) + " ";
    resultText.setString(resultStr);
    resultText.setPosition(10, m_window.getSize().y - 40);
    m_window.draw(resultText);
}
