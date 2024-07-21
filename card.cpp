#include "catan.hpp"
#include <iostream>

void Knight::use(Player& player, std::vector<Player>& players) {
    std::cout << "Knight card used\n";
    player.knightsPlayed++;
}

std::string Knight::getName() const {
    return "Knight";
}

void VictoryPoint::use(Player& player, std::vector<Player>& players) {
    std::cout << "Victory Point card used\n";
    player.victoryPoints++;
}

std::string VictoryPoint::getName() const {
    return "Victory Point";
}

void YearOfPlenty::use(Player& player, std::vector<Player>& players) {
    std::cout << "Year of Plenty card used. Choose two resources to receive.\n";
    for (int i = 0; i < 2; ++i) {
        std::cout << "Choose resource " << i+1 << " (Wood/Brick/Ore/Grain/Wool): ";
        std::string resourceStr;
        std::cin >> resourceStr;
        Resource resource = stringToResource(resourceStr);
        player.addResource(resource);
    }
}

std::string YearOfPlenty::getName() const { 
    return "Year of Plenty"; 
}

void RoadBuilding::use(Player& player, std::vector<Player>& players) {
    std::cout << "Road Building card used. Build two roads for free.\n";
    for (int i = 0; i < 2; ++i) {
        std::cout << "Building road " << i+1 << ":\n";
        player.buildRoad(true);
    }
}

std::string RoadBuilding::getName() const { 
    return "Road Building"; 
}

void Monopoly::use(Player& player, std::vector<Player>& players) {
    std::cout << "Monopoly card used. Choose a resource to monopolize.\n";
    std::cout << "Enter resource type (Wood/Brick/Ore/Grain/Wool): ";
    std::string resourceStr;
    std::cin >> resourceStr;
    Resource resource = stringToResource(resourceStr);
    int totalGained = 0;
    for (auto& otherPlayer : players) {
        if (&otherPlayer != &player) {
            int amount = otherPlayer.resources[resource];
            otherPlayer.resources[resource] = 0;
            player.resources[resource] += amount;
            totalGained += amount;
        }
    }
    std::cout << "Gained " << totalGained << " " << resourceStr << " from other players.\n";
}

std::string Monopoly::getName() const { 
    return "Monopoly"; 
}