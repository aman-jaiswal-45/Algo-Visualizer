#pragma once
#include "Visualizer.h"
#include <vector>
#include <stack>
#include <utility> // For std::pair

class QuickSortVisualizer : public Visualizer {
private:
    std::vector<int> m_data;
    std::stack<std::pair<int, int>> m_stack;
    
    // State for visualization
    int m_low, m_high;
    int m_pivotValue;
    int m_pivotIndex;
    int m_partitionIndex;
    int m_scanIndex; // Used to track the 'j' in a standard partition
    
    enum class State { IDLE, PARTITIONING, DONE } m_state;
    bool m_isSorting;

    void initializeData();

public:
    QuickSortVisualizer(sf::RenderWindow& window);
    ~QuickSortVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
