#pragma once
#include "Visualizer.h"
#include <vector>

class InsertionSortVisualizer : public Visualizer {
private:
    std::vector<int> m_data;
    int m_i, m_j;
    int m_key;
    bool m_isSorting;
    bool m_isSorted;

    void initializeData();

public:
    InsertionSortVisualizer(sf::RenderWindow& window);
    ~InsertionSortVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
