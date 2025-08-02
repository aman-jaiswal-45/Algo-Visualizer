#pragma once
#include "Visualizer.h"
#include <vector>

class HeapSortVisualizer : public Visualizer {
private:
    std::vector<int> m_data;
    int m_heapSize;
    int m_heapifyIndex;
    int m_heapifyRoot;
    int m_heapifyLargest;
    
    enum class Phase { IDLE, BUILDING, SORTING, HEAPIFYING } m_phase;
    bool m_isSorting;
    bool m_isSorted;

    void initializeData();
    void startHeapify(int n, int i);

public:
    HeapSortVisualizer(sf::RenderWindow& window);
    ~HeapSortVisualizer() override = default;

    void handleEvent(sf::Event& event) override;
    void update() override;
    void draw() override;
    void reset() override;
};
