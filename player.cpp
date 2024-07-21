#include "catan.hpp"
#include <iostream>

Player::Player(std::string n) : name(n), victoryPoints(0), knightsPlayed(0) {
    for (int i = 0; i < 5; i++) {
        resources[static_cast<Resource>(i)] = 0;
    }
}

void Player::addResource(Resource r, int amount) {
    resources[r] += amount;
}

bool Player::hasResources(const std::map<Resource, int>& cost) const {
    for (const auto& [resource, amount] : cost) {
        if (resources.at(resource) < amount) {
            return false;
        }
    }
    return true;
}

void Player::removeResources(const std::map<Resource, int>& cost) {
    for (const auto& [resource, amount] : cost) {
        resources[resource] -= amount;
    }
}

void Player::printResources() const {
    std::cout << name << "'s resources:\n";
    for (const auto& [resource, amount] : resources) {
        std::cout << resourceToString(resource) << ": " << amount << "\n";
    }
}

void Player::printCards() const {
    std::cout << name << "'s development cards:\n";
    for (const auto& card : cards) {
        std::cout << card->getName() << "\n";
    }
}

void Player::buildRoad(bool isFree) {
    std::cout << "Road built by " << name << (isFree ? " for free" : "") << std::endl;
}