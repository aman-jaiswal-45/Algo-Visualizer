#include "GraphVisualizer.h"
#include <iostream>
#include <algorithm> // for std::reverse

using namespace std;

GraphVisualizer::GraphVisualizer(sf::RenderWindow& window, TraversalType type)
    : Visualizer(window), m_traversalType(type), m_isVisualizing(false), m_isDone(false), m_componentCheckIndex(0) {
    if (!m_font.loadFromFile("assets/arial.ttf")) {
        cerr << "Error loading font in GraphVisualizer" << endl;
    }
    m_traversalName = (type == TraversalType::DFS) ? "DFS" : "BFS";
}

void GraphVisualizer::buildGraph() {
    m_adjList.clear();
    m_edges.clear();
    
    // --- Component 1 ---
    map<int, sf::Vector2f> positions;
    positions[0] = {150, 250};
    positions[1] = {350, 150};
    positions[2] = {350, 350};
    positions[3] = {550, 250};
    
    // --- Component 2 ---
    positions[4] = {800, 500};
    positions[5] = {1000, 400};
    positions[6] = {1000, 600};


    for(const auto& pair : positions){
        Node node;
        node.shape.setRadius(30.f);
        node.shape.setOrigin(node.shape.getRadius(), node.shape.getRadius());
        node.shape.setPosition(pair.second);
        node.shape.setOutlineThickness(2.f);
        node.shape.setOutlineColor(sf::Color::White);

        node.label.setFont(m_font);
        node.label.setString(to_string(pair.first));
        node.label.setCharacterSize(24);
        node.label.setFillColor(sf::Color::White);
        sf::FloatRect textRect = node.label.getLocalBounds();
        node.label.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        node.label.setPosition(pair.second);
        
        m_adjList[pair.first] = node;
    }

    auto addEdge = [&](int u, int v) {
        m_adjList[u].adj.push_back(v);
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = m_adjList[u].shape.getPosition();
        line[0].color = sf::Color::White;
        line[1].position = m_adjList[v].shape.getPosition();
        line[1].color = sf::Color::White;
        m_edges.push_back(line);
    };

    // Edges for Component 1
    addEdge(0, 1);
    addEdge(0, 2);
    addEdge(1, 3);
    addEdge(2, 3);
    
    // Edges for Component 2
    addEdge(4, 5);
    addEdge(4, 6);
}

void GraphVisualizer::resetNodeStates() {
    for (auto& pair : m_adjList) {
        pair.second.state = Node::State::Unvisited;
    }
}

// Finds the next unvisited node and starts a traversal from it.
// Returns true if a new traversal was started, false otherwise.
bool GraphVisualizer::startNextTraversal() {
    for (int i = m_componentCheckIndex; i < m_adjList.size(); ++i) {
        if (m_adjList[i].state == Node::State::Unvisited) {
            if (m_traversalType == TraversalType::DFS) {
                m_dfsStack.push(i);
            } else {
                m_bfsQueue.push(i);
            }
            m_adjList[i].state = Node::State::Visiting;
            m_componentCheckIndex = i + 1;
            return true;
        }
    }
    return false; // No unvisited nodes found
}

void GraphVisualizer::reset() {
    buildGraph();
    resetNodeStates();
    
    while(!m_dfsStack.empty()) m_dfsStack.pop();
    while(!m_bfsQueue.empty()) m_bfsQueue.pop();

    m_isVisualizing = false;
    m_isDone = false;
    m_componentCheckIndex = 0;
    
    startNextTraversal(); // Start the very first traversal
    
    cout << "Graph " << m_traversalName << " Visualizer reset. Press SPACE to start." << endl;
}

void GraphVisualizer::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space && !m_isDone) {
            m_isVisualizing = !m_isVisualizing;
        }
        if (event.key.code == sf::Keyboard::R) {
            reset();
        }
    }
}

void GraphVisualizer::update() {
    if (!m_isVisualizing || m_isDone || m_clock.getElapsedTime().asSeconds() < m_delay) {
        return;
    }
    m_clock.restart();

    if (m_traversalType == TraversalType::DFS) {
        if (m_dfsStack.empty()) {
            // Current component is done, look for the next one
            if (!startNextTraversal()) {
                m_isDone = true;
                m_isVisualizing = false;
                cout << "DFS traversal complete!" << endl;
            }
            return;
        }
        int u = m_dfsStack.top();
        m_dfsStack.pop();
        m_adjList[u].state = Node::State::Visited;

        vector<int> neighbors = m_adjList[u].adj;
        reverse(neighbors.begin(), neighbors.end());

        for (int v : neighbors) {
            if (m_adjList[v].state == Node::State::Unvisited) {
                m_adjList[v].state = Node::State::Visiting;
                m_dfsStack.push(v);
            }
        }
    } 
    else // BFS
    {
        if (m_bfsQueue.empty()) {
            // Current component is done, look for the next one
            if (!startNextTraversal()) {
                m_isDone = true;
                m_isVisualizing = false;
                cout << "BFS traversal complete!" << endl;
            }
            return;
        }
        int u = m_bfsQueue.front();
        m_bfsQueue.pop();
        m_adjList[u].state = Node::State::Visited;
        
        for (int v : m_adjList[u].adj) {
            if (m_adjList[v].state == Node::State::Unvisited) {
                m_adjList[v].state = Node::State::Visiting;
                m_bfsQueue.push(v);
            }
        }
    }
}

void GraphVisualizer::draw() {
    m_window.clear(sf::Color(30, 30, 30));

    for (const auto& edge : m_edges) {
        m_window.draw(edge);
    }

    for (auto& pair : m_adjList) {
        Node& node = pair.second;
        switch (node.state) {
            case Node::State::Unvisited: node.shape.setFillColor(sf::Color(100, 100, 250)); break;
            case Node::State::Visiting:  node.shape.setFillColor(sf::Color::Yellow); break;
            case Node::State::Visited:   node.shape.setFillColor(sf::Color::Green); break;
        }
        m_window.draw(node.shape);
        m_window.draw(node.label);
    }
    
    sf::Text infoText;
    infoText.setFont(m_font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    
    string status = m_isDone ? "Complete!" : (m_isVisualizing ? "Visualizing..." : "Paused.");
    infoText.setString("Graph " + m_traversalName + " | " + status + " | 'R' to reset | ESC for menu");
    infoText.setPosition(10, 10);
    m_window.draw(infoText);
}
