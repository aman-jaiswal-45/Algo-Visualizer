#include "BellmanFordVisualizer.h"
#include <iostream>
#include <limits>

using namespace std;

BellmanFordVisualizer::BellmanFordVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_isVisualizing(false), m_isDone(false), m_iteration(0), m_edgeIndex(0), m_negativeCycle(false) {
    if (!m_font.loadFromFile("assets/arial.ttf")) cerr << "Error loading font" << endl;
}

void BellmanFordVisualizer::buildGraph() {
    m_nodes.clear(); m_edges.clear(); m_edgeWeights.clear();
    map<int, sf::Vector2f> positions = {
        {0, {150, 360}}, {1, {400, 200}}, {2, {400, 520}}, {3, {700, 360}}, {4, {950, 360}}
    };
    for(const auto& p : positions) {
        Node node;
        node.shape.setRadius(30.f); node.shape.setOrigin(30.f, 30.f);
        node.shape.setPosition(p.second);
        node.label.setFont(m_font); node.label.setString(to_string(p.first));
        node.label.setCharacterSize(24);
        sf::FloatRect r = node.label.getLocalBounds();
        node.label.setOrigin(r.left + r.width/2.f, r.top + r.height/2.f);
        node.label.setPosition(p.second);
        node.distLabel.setFont(m_font); node.distLabel.setCharacterSize(18);
        m_nodes[p.first] = node;
    }
    auto addEdge = [&](int u, int v, int w) {
        m_edges.emplace_back(u, v, w);
        sf::Text weightText(to_string(w), m_font, 20);
        sf::Vector2f pos = (m_nodes[u].shape.getPosition() + m_nodes[v].shape.getPosition()) / 2.f;
        weightText.setPosition(pos);
        m_edgeWeights[{u,v}] = weightText;
    };
    addEdge(0, 1, 6); addEdge(0, 2, 7); addEdge(1, 3, 5);
    addEdge(1, 2, 8); addEdge(2, 3, -4); addEdge(1, 4, -2); addEdge(3, 4, 3);
}

void BellmanFordVisualizer::reset() {
    buildGraph();
    m_distances.assign(m_nodes.size(), numeric_limits<int>::max());
    m_distances[0] = 0;
    m_isVisualizing = false;
    m_isDone = false;
    m_iteration = 0;
    m_edgeIndex = 0;
    m_negativeCycle = false;
    cout << "Bellman-Ford reset. Press SPACE to start." << endl;
}

void BellmanFordVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isDone) m_isVisualizing = !m_isVisualizing;
        if (event.key.code == sf::Keyboard::R) reset();
    }
}

void BellmanFordVisualizer::update() {
    if (!m_isVisualizing || m_isDone || m_clock.getElapsedTime().asSeconds() < m_delay) return;
    m_clock.restart();

    if (m_iteration < m_nodes.size() - 1) {
        auto [u, v, w] = m_edges[m_edgeIndex];
        if (m_distances[u] != numeric_limits<int>::max() && m_distances[u] + w < m_distances[v]) {
            m_distances[v] = m_distances[u] + w;
        }
        m_edgeIndex++;
        if (m_edgeIndex >= m_edges.size()) {
            m_edgeIndex = 0;
            m_iteration++;
        }
    } else { // Check for negative cycle
        for(const auto& edge : m_edges) {
            auto [u, v, w] = edge;
            if (m_distances[u] != numeric_limits<int>::max() && m_distances[u] + w < m_distances[v]) {
                m_negativeCycle = true; break;
            }
        }
        m_isDone = true; m_isVisualizing = false;
    }
}

void BellmanFordVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));
    for(const auto& edge : m_edges) {
        sf::Vertex line[] = {m_nodes[get<0>(edge)].shape.getPosition(), m_nodes[get<1>(edge)].shape.getPosition()};
        if(m_isVisualizing && !m_isDone && m_edgeIndex == (&edge - &m_edges[0])) line[0].color = line[1].color = sf::Color::Yellow;
        m_window.draw(line, 2, sf::Lines);
    }
    for (const auto& pair : m_edgeWeights) m_window.draw(pair.second);
    for (auto& pair : m_nodes) {
        pair.second.shape.setFillColor(sf::Color(100, 100, 250));
        string distStr = (m_distances[pair.first] == numeric_limits<int>::max()) ? "inf" : to_string(m_distances[pair.first]);
        pair.second.distLabel.setString(distStr);
        pair.second.distLabel.setPosition(pair.second.shape.getPosition() + sf::Vector2f(0, 35));
        m_window.draw(pair.second.shape);
        m_window.draw(pair.second.label);
        m_window.draw(pair.second.distLabel);
    }
    string status = m_isDone ? (m_negativeCycle ? "Negative Cycle Detected!" : "Complete!") : (m_isVisualizing ? "Iter " + to_string(m_iteration+1) : "Paused.");
    sf::Text infoText("Bellman-Ford | " + status, m_font, 20);
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
