#pragma once
#include "Visualizer.h"
#include <vector>

class SelectionSortVisualizer : public Visualizer {
private:
    std::vector<int> m_data;
    int m_i, m_j, m_minIndex;
    bool m_isSorting;
    bool m_isSorted;

    void initializeData();

public:
    SelectionSortVisualizer(sf::RenderWindow& window);
    ~SelectionSortVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
