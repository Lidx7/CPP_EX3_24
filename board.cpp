#include "catan.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>

Board::Board() {
    setupBoard();
    setupAdjacencyList();
}

void Board::setupBoard() {
    std::vector<TerrainType> terrains = {
        TerrainType::Forest, TerrainType::Forest, TerrainType::Forest, TerrainType::Forest,
        TerrainType::Hills, TerrainType::Hills, TerrainType::Hills,
        TerrainType::Mountains, TerrainType::Mountains, TerrainType::Mountains,
        TerrainType::Fields, TerrainType::Fields, TerrainType::Fields, TerrainType::Fields,
        TerrainType::Pasture, TerrainType::Pasture, TerrainType::Pasture, TerrainType::Pasture,
        TerrainType::Desert
    };
    std::vector<int> numbers = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(terrains.begin(), terrains.end(), g);
    std::shuffle(numbers.begin(), numbers.end(), g);

    for (size_t i = 0; i < terrains.size(); ++i) {
        if (terrains[i] == TerrainType::Desert) {
            tiles.emplace_back(terrains[i], 7);
        } else {
            tiles.emplace_back(terrains[i], numbers[i < numbers.size() ? i : i - 1]);
        }
    }
}

void Board::setupAdjacencyList() {
    adjacencyList = std::vector<std::vector<int>>(54, std::vector<int>());
    for (int i = 0; i < 54; ++i) {
        if (i % 2 == 0) {
            adjacencyList[i].push_back((i + 1) % 54);
            adjacencyList[i].push_back((i + 2) % 54);
        } else {
            adjacencyList[i].push_back((i + 1) % 54);
            adjacencyList[i].push_back((i + 53) % 54);
        }
    }
}

void Board::printBoard() const {
    std::cout << "Board:\n";
    for (size_t i = 0; i < tiles.size(); ++i) {
        std::cout << "Tile " << i << ": " << static_cast<int>(tiles[i].terrain) << ":" << tiles[i].number << "\n";
    }
}

Resource Board::terrainToResource(TerrainType terrain) const {
    switch (terrain) {
        case TerrainType::Forest: return Resource::Wood;
        case TerrainType::Hills: return Resource::Brick;
        case TerrainType::Mountains: return Resource::Ore;
        case TerrainType::Fields: return Resource::Grain;
        case TerrainType::Pasture: return Resource::Wool;
        default: throw std::runtime_error("Invalid terrain type");
    }
}

void Board::printMap() {
    std::cout << "Catan Map:\n\n";

    printRow(0, 3, 10);
    printRow(3, 7, 5);
    printRow(7, 12, 1);
    printRow(12, 16, 6);
    printRow(16, 19, 11);

    // Print legend
    std::cout << "\nLegend:\n";
    std::cout << "F: Forest, H: Hills, M: Mountains, G: Fields, P: Pasture, D: Desert\n";
    std::cout << "Numbers represent dice roll values\n";
}

void Board::printRow(int start, int end, int padding) {
    std::string pad(padding, ' ');
    
    // Top of hexagons
    std::cout << pad;
    for (int i = start; i < end; ++i) {
        std::cout << " /''''\\ ";
        if (i < end - 1) std::cout << "  ";
    }
    std::cout << "\n";

    // Number line
    std::cout << pad;
    for (int i = start; i < end; ++i) {
        std::cout << "/  " << std::setw(2) << std::left << formatNumber(tiles[i].number) << "  \\ ";
        if (i < end - 1) std::cout << " ";
    }
    std::cout << "\n";

    // Terrain type line
    std::cout << pad;
    for (int i = start; i < end; ++i) {
        std::cout << "|  " << terrainToChar(tiles[i].terrain) << "   | ";
        if (i < end - 1) std::cout << " ";
    }
    std::cout << "\n";

    // Bottom of hexagons
    std::cout << pad;
    for (int i = start; i < end; ++i) {
        std::cout << " \\____/ ";
        if (i < end - 1) std::cout << "  ";
    }
    std::cout << "\n";
}

std::string Board::formatNumber(int num) const {
    if (num == 7) return " "; // For desert
    return std::to_string(num);
}

char Board::terrainToChar(TerrainType terrain) const {
    switch (terrain) {
        case TerrainType::Forest: return 'F';
        case TerrainType::Hills: return 'H';
        case TerrainType::Mountains: return 'M';
        case TerrainType::Fields: return 'G';
        case TerrainType::Pasture: return 'P';
        case TerrainType::Desert: return 'D';
        default: return '?';
    }
}