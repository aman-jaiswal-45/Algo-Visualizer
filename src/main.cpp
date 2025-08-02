#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <sstream>

using namespace std;

#include "Visualizer.h"
#include "BubbleSortVisualizer.h"
#include "InsertionSortVisualizer.h"
#include "SelectionSortVisualizer.h"
#include "QuickSortVisualizer.h"
#include "MergeSortVisualizer.h"
#include "HeapSortVisualizer.h"
#include "TreeTraversalVisualizer.h"
#include "GraphVisualizer.h"
#include "TopoSortVisualizer.h"
#include "DijkstraVisualizer.h"
#include "BellmanFordVisualizer.h"
#include "KruskalVisualizer.h"
#include "PrimsVisualizer.h"
#include "FloydWarshallVisualizer.h"
#include "BSTVisualizer.h"

// --- UI Helper Structs ---
struct Textbox {
    sf::RectangleShape box;
    sf::Text text;
    string inputString;
    bool isSelected = false;

    Textbox(sf::Font& font) {
        box.setSize({200, 50});
        box.setFillColor(sf::Color(80, 80, 80));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::White);
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
    }

    void handleEvent(sf::Event& event) {
        if (!isSelected) return;
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') { // Backspace
                if (!inputString.empty()) inputString.pop_back();
            } else if (event.text.unicode >= '0' && event.text.unicode <= '9') { // Digits only
                if (inputString.length() < 3) inputString += static_cast<char>(event.text.unicode);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        text.setString(inputString);
        window.draw(box);
        window.draw(text);
    }

    void setPosition(sf::Vector2f pos) {
        box.setPosition(pos);
        text.setPosition(pos.x + 10, pos.y + 10);
    }
};

struct Button {
    sf::RectangleShape shape;
    sf::Text text;

    Button(const string& label, sf::Font& font, sf::Vector2f size, sf::Color bgColor) {
        shape.setSize(size);
        shape.setFillColor(bgColor);
        text.setString(label);
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
    }

    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(pos.x + shape.getSize().x / 2.0f, pos.y + shape.getSize().y / 2.0f);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        return shape.getGlobalBounds().contains(mousePos);
    }
};


// --- Configuration ---
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const string WINDOW_TITLE = "Algorithm Visualizer";

// --- Helper Function to display the menu ---
void showMenu(sf::RenderWindow& window, const vector<string>& options, int selectedIndex, sf::Font& font) {
    window.clear(sf::Color(20, 20, 40)); 

    sf::Text title("Select an Algorithm to Visualize:", font, 40);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(WINDOW_WIDTH / 2.0f - title.getGlobalBounds().width / 2.0f, 20);
    window.draw(title);
    
    sf::Text instructions("Use UP/DOWN arrow keys. Press ENTER to select.", font, 20);
    instructions.setFillColor(sf::Color(150, 150, 150));
    instructions.setPosition(WINDOW_WIDTH / 2.0f - instructions.getGlobalBounds().width / 2.0f, 80);
    window.draw(instructions);

    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text optionText(options[i], font, 22);
        float xPos = (i < 10) ? 50.f : 700.f;
        float yPos = (i < 10) ? 140.f + (i * 45.f) : 140.f + ((i-10) * 45.f);
        optionText.setPosition(xPos, yPos);
        if (i == (size_t)selectedIndex) {
            optionText.setFillColor(sf::Color::Yellow);
            optionText.setStyle(sf::Text::Bold);
        } else {
            optionText.setFillColor(sf::Color::White);
        }
        window.draw(optionText);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        cerr << "CRITICAL ERROR: Could not load font 'assets/arial.ttf'." << endl;
        return -1;
    }

    enum class AppState { MainMenu, BSTInput, Visualizing };
    AppState currentState = AppState::MainMenu;

    // --- UI Elements for BST Input Screen ---
    Textbox textbox(font);
    textbox.setPosition({WINDOW_WIDTH / 2.f - 100, 200});
    Button insertButton("Insert", font, {120, 50}, sf::Color::Green);
    insertButton.setPosition({WINDOW_WIDTH / 2.f - 200, 300});
    Button searchButton("Search", font, {120, 50}, sf::Color(0, 150, 255));
    searchButton.setPosition({WINDOW_WIDTH / 2.f - 60, 300});
    Button deleteButton("Delete", font, {120, 50}, sf::Color::Red);
    deleteButton.setPosition({WINDOW_WIDTH / 2.f + 80, 300});


    unique_ptr<Visualizer> currentVisualizer = nullptr;
    vector<function<unique_ptr<Visualizer>(sf::RenderWindow&)>> visualizerFactories;
    vector<string> visualizerNames;

    // --- Register your algorithms here ---
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<BubbleSortVisualizer>(win); });
    visualizerNames.push_back("Sort: Bubble");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<SelectionSortVisualizer>(win); });
    visualizerNames.push_back("Sort: Selection");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<InsertionSortVisualizer>(win); });
    visualizerNames.push_back("Sort: Insertion");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<MergeSortVisualizer>(win); });
    visualizerNames.push_back("Sort: Merge");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<QuickSortVisualizer>(win); });
    visualizerNames.push_back("Sort: Quick");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<HeapSortVisualizer>(win); });
    visualizerNames.push_back("Sort: Heap");
    
    visualizerFactories.push_back(nullptr); // Placeholder for BST
    visualizerNames.push_back("BST: Interactive Operations");

    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<TreeTraversalVisualizer>(win, TreeTraversalVisualizer::TraversalType::IN_ORDER); });
    visualizerNames.push_back("Tree: In-Order");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<TreeTraversalVisualizer>(win, TreeTraversalVisualizer::TraversalType::PRE_ORDER); });
    visualizerNames.push_back("Tree: Pre-Order");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<TreeTraversalVisualizer>(win, TreeTraversalVisualizer::TraversalType::POST_ORDER); });
    visualizerNames.push_back("Tree: Post-Order");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<TreeTraversalVisualizer>(win, TreeTraversalVisualizer::TraversalType::BFS); });
    visualizerNames.push_back("Tree: BFS");
    
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<GraphVisualizer>(win, GraphVisualizer::TraversalType::DFS); });
    visualizerNames.push_back("Graph: DFS");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<GraphVisualizer>(win, GraphVisualizer::TraversalType::BFS); });
    visualizerNames.push_back("Graph: BFS");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<TopoSortVisualizer>(win, TopoSortVisualizer::AlgoType::DFS_BASED); });
    visualizerNames.push_back("Graph: Topo Sort (DFS)");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<TopoSortVisualizer>(win, TopoSortVisualizer::AlgoType::KAHN); });
    visualizerNames.push_back("Graph: Topo Sort (Kahn's)");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<DijkstraVisualizer>(win); });
    visualizerNames.push_back("Path: Dijkstra's");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<BellmanFordVisualizer>(win); });
    visualizerNames.push_back("Path: Bellman-Ford");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<FloydWarshallVisualizer>(win); });
    visualizerNames.push_back("Path: Floyd-Warshall");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<KruskalVisualizer>(win); });
    visualizerNames.push_back("MST: Kruskal's");
    visualizerFactories.push_back([](sf::RenderWindow& win){ return make_unique<PrimsVisualizer>(win); });
    visualizerNames.push_back("MST: Prim's");

    int selectedAlgorithmIndex = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (currentState == AppState::Visualizing) {
                if (currentVisualizer) currentVisualizer->handleEvent(event);
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = AppState::MainMenu;
                    currentVisualizer = nullptr;
                }
            } 
            else if (currentState == AppState::BSTInput) {
                textbox.handleEvent(event);
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (textbox.box.getGlobalBounds().contains(window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}))) {
                        textbox.isSelected = true;
                    } else {
                        textbox.isSelected = false;
                    }
                    
                    auto startVisualization = [&](BSTVisualizer::Operation op) {
                        if (textbox.inputString.empty()) return;
                        int value = stoi(textbox.inputString);
                        currentVisualizer = make_unique<BSTVisualizer>(window, op, value);
                        currentVisualizer->reset();
                        currentState = AppState::Visualizing;
                    };

                    if (insertButton.isMouseOver(window)) startVisualization(BSTVisualizer::Operation::INSERT);
                    if (searchButton.isMouseOver(window)) startVisualization(BSTVisualizer::Operation::SEARCH);
                    if (deleteButton.isMouseOver(window)) startVisualization(BSTVisualizer::Operation::DELETE);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = AppState::MainMenu;
                }
            }
            else if (currentState == AppState::MainMenu) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Down) {
                        selectedAlgorithmIndex = (selectedAlgorithmIndex + 1) % visualizerNames.size();
                    } else if (event.key.code == sf::Keyboard::Up) {
                        selectedAlgorithmIndex = (selectedAlgorithmIndex - 1 + visualizerNames.size()) % visualizerNames.size();
                    } else if (event.key.code == sf::Keyboard::Enter) {
                        if (visualizerFactories[selectedAlgorithmIndex] == nullptr) { // It's the BST option
                            currentState = AppState::BSTInput;
                        } else {
                            currentVisualizer = visualizerFactories[selectedAlgorithmIndex](window);
                            currentVisualizer->reset();
                            currentState = AppState::Visualizing;
                        }
                    }
                }
            }
        }

        if (currentState == AppState::Visualizing && currentVisualizer) {
            currentVisualizer->update();
        }

        window.clear();

        if (currentState == AppState::Visualizing && currentVisualizer) {
            currentVisualizer->draw();
        } else if (currentState == AppState::BSTInput) {
            window.clear(sf::Color(20, 20, 40));
            sf::Text bstTitle("BST Operations", font, 40);
            bstTitle.setPosition(WINDOW_WIDTH/2.f - bstTitle.getGlobalBounds().width/2.f, 50);
            sf::Text bstPrompt("Enter a number (0-999) and select an operation:", font, 24);
            bstPrompt.setPosition(WINDOW_WIDTH/2.f - bstPrompt.getGlobalBounds().width/2.f, 150);
            window.draw(bstTitle);
            window.draw(bstPrompt);
            textbox.draw(window);
            insertButton.draw(window);
            searchButton.draw(window);
            deleteButton.draw(window);
        } else {
            showMenu(window, visualizerNames, selectedAlgorithmIndex, font);
        }

        window.display();
    }

    return 0;
}
