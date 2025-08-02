#include "DijkstraVisualizer.h"
#include <iostream>
#include <limits>

using namespace std;

DijkstraVisualizer::DijkstraVisualizer(sf::RenderWindow& window)
    : Visualizer(window), m_isVisualizing(false), m_isDone(false) {
    if (!m_font.loadFromFile("assets/arial.ttf")) cerr << "Error loading font" << endl;
}

void DijkstraVisualizer::buildGraph() {
    m_nodes.clear(); m_edges.clear(); m_edgeWeights.clear();
    map<int, sf::Vector2f> positions = {
        {0, {150, 360}}, {1, {350, 200}}, {2, {350, 520}},
        {3, {650, 200}}, {4, {650, 520}}, {5, {850, 360}}, {6, {1050, 360}}
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
        m_nodes[u].adj.push_back({v, w});
        m_nodes[v].adj.push_back({u, w}); // Undirected
        sf::VertexArray line(sf::Lines, 2);
        line[0] = {m_nodes[u].shape.getPosition(), sf::Color::White};
        line[1] = {m_nodes[v].shape.getPosition(), sf::Color::White};
        m_edges.push_back(line);
        sf::Text weightText(to_string(w), m_font, 20);
        weightText.setPosition((line[0].position + line[1].position) / 2.f);
        m_edgeWeights[{min(u,v), max(u,v)}] = weightText;
    };
    addEdge(0, 1, 4); addEdge(0, 2, 1); addEdge(1, 3, 1);
    addEdge(2, 1, 2); addEdge(2, 4, 5); addEdge(3, 5, 3);
    addEdge(4, 5, 1); addEdge(5, 6, 2);
}

void DijkstraVisualizer::reset() {
    buildGraph();
    int n = m_nodes.size();
    m_distances.assign(n, numeric_limits<int>::max());
    m_visited.assign(n, false);
    while(!m_pq.empty()) m_pq.pop();

    m_distances[0] = 0;
    m_pq.push({0, 0}); // {distance, node}
    m_isVisualizing = false;
    m_isDone = false;
    cout << "Dijkstra's reset. Press SPACE to start." << endl;
}

void DijkstraVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isDone) m_isVisualizing = !m_isVisualizing;
        if (event.key.code == sf::Keyboard::R) reset();
    }
}

void DijkstraVisualizer::update() {
    if (!m_isVisualizing || m_isDone || m_clock.getElapsedTime().asSeconds() < m_delay) return;
    m_clock.restart();

    if (m_pq.empty()) {
        m_isDone = true; m_isVisualizing = false; return;
    }

    int u = m_pq.top().second;
    m_pq.pop();

    if (m_visited[u]) return;
    m_visited[u] = true;

    for (const auto& edge : m_nodes[u].adj) {
        int v = edge.first;
        int weight = edge.second;
        if (m_distances[u] + weight < m_distances[v]) {
            m_distances[v] = m_distances[u] + weight;
            m_pq.push({m_distances[v], v});
        }
    }
}

void DijkstraVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));
    for (const auto& edge : m_edges) m_window.draw(edge);
    for (const auto& pair : m_edgeWeights) m_window.draw(pair.second);

    for (auto& pair : m_nodes) {
        if (m_visited[pair.first]) pair.second.shape.setFillColor(sf::Color::Green);
        else pair.second.shape.setFillColor(sf::Color(100, 100, 250));
        
        string distStr = (m_distances[pair.first] == numeric_limits<int>::max()) ? "inf" : to_string(m_distances[pair.first]);
        pair.second.distLabel.setString(distStr);
        pair.second.distLabel.setPosition(pair.second.shape.getPosition() + sf::Vector2f(0, 35));
        
        m_window.draw(pair.second.shape);
        m_window.draw(pair.second.label);
        m_window.draw(pair.second.distLabel);
    }
    std::string status = m_isDone ? "Complete!" : (m_isVisualizing ? "Visualizing..." : "Paused.");
    sf::Text infoText(std::string("Dijkstra's Algorithm | ") + status, m_font, 20); 
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
