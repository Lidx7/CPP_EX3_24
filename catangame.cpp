#include "catan.hpp"
#include <iostream>
#include <algorithm>
#include <random>


CatanGame::CatanGame() : currentPlayerIndex(0), rng(std::random_device()()) {
    for (int i = 0; i < 3; ++i) {
        players.emplace_back("Player " + std::to_string(i + 1));
    }
    initializeDevelopmentCards();
}

void CatanGame::initializeDevelopmentCards() {
    for (int i = 0; i < 14; ++i) developmentCards.push_back(new Knight());
    for (int i = 0; i < 5; ++i) developmentCards.push_back(new VictoryPoint());
    for (int i = 0; i < 2; ++i) developmentCards.push_back(new YearOfPlenty());
    for (int i = 0; i < 2; ++i) developmentCards.push_back(new RoadBuilding());
    for (int i = 0; i < 2; ++i) developmentCards.push_back(new Monopoly());
    std::shuffle(developmentCards.begin(), developmentCards.end(), rng);
}

void CatanGame::play() {
    setupInitialSettlements();
    while (!checkWinCondition()) {
        board.printBoard();
        for (const auto& player : players) {
            player.printResources();
        }
        playTurn();
    }
}

void CatanGame::playTurn() {
    Player& currentPlayer = players[currentPlayerIndex];
    std::cout << "\n" << currentPlayer.name << "'s turn\n";
    
    int diceRoll = rollDice();
    distributeResources(diceRoll);
    
    trade(currentPlayer);
    build(currentPlayer);

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

int CatanGame::rollDice() {
    std::uniform_int_distribution<> dis(1, 6);
    int roll1 = dis(rng);
    int roll2 = dis(rng);
    int total = roll1 + roll2;
    std::cout << "Dice roll: " << total << " (" << roll1 << " + " << roll2 << ")\n";
    return total;
}

void CatanGame::distributeResources(int diceRoll) {
    for (size_t i = 0; i < board.tiles.size(); ++i) {
        if (board.tiles[i].number == diceRoll && board.tiles[i].terrain != TerrainType::Desert) {
            Resource resource = board.terrainToResource(board.tiles[i].terrain);
            for (Player& player : players) {
                for (const auto& settlement : player.settlements) {
                    if (settlement.first == i) {
                        player.addResource(resource);
                    }
                }
            }
        }
    }
}

void CatanGame::trade(Player& player) {
    std::cout << "Trade phase\n";
    player.printResources();
    std::cout << "Do you want to trade? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y') {
        std::cout << "Enter the resource you want to give: ";
        std::string giveResource;
        std::cin >> giveResource;
        std::cout << "Enter the resource you want to receive: ";
        std::string receiveResource;
        std::cin >> receiveResource;

        Resource give = stringToResource(giveResource);
        Resource receive = stringToResource(receiveResource);

        if (player.resources[give] > 0) {
            player.resources[give]--;
            player.resources[receive]++;
            std::cout << "Trade successful\n";
        } else {
            std::cout << "Not enough resources for trade\n";
        }
    }
}

void CatanGame::build(Player& player) {
    std::cout << "Build phase\n";
    player.printResources();
    std::cout << "1. Build settlement (1 wood, 1 brick, 1 grain, 1 wool)\n";
    std::cout << "2. Build road (1 wood, 1 brick)\n";
    std::cout << "3. Buy development card (1 ore, 1 grain, 1 wool)\n";
    std::cout << "4. Use development card\n";
    std::cout << "5. End turn\n";

    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            buildSettlement(player, false);
            break;
        case 2:
            buildRoad(player, false);
            break;
        case 3:
            buyDevelopmentCard(player);
            break;
        case 4:
            useDevelopmentCard(player);
            break;
        case 5:
            std::cout << "Turn ended\n";
            break;
        default:
            std::cout << "Invalid choice\n";
    }
}

void CatanGame::buildSettlement(Player& player, bool isInitial) {
    if (!isInitial) {
        std::map<Resource, int> cost = {{Resource::Wood, 1}, {Resource::Brick, 1}, {Resource::Grain, 1}, {Resource::Wool, 1}};
        if (!player.hasResources(cost)) {
            std::cout << "Not enough resources\n";
            return;
        }
    }

    std::cout << "Enter the tile number to build settlement: ";
    int tileNum;
    std::cin >> tileNum;
    std::cout << "Enter the vertex number (0-5): ";
    int vertexNum;
    std::cin >> vertexNum;

    bool canPlace = true;
    for (const auto& p : players) {
        for (const auto& s : p.settlements) {
            if (s.first == tileNum && (s.second == vertexNum || 
                std::find(board.adjacencyList[s.second].begin(), board.adjacencyList[s.second].end(), vertexNum) != board.adjacencyList[s.second].end())) {
                canPlace = false;
                break;
            }
        }
        if (!canPlace) break;
    }

    if (canPlace) {
        if (!isInitial) {
            std::map<Resource, int> cost = {{Resource::Wood, 1}, {Resource::Brick, 1}, {Resource::Grain, 1}, {Resource::Wool, 1}};
            player.removeResources(cost);
        }
        player.settlements.emplace_back(tileNum, vertexNum);
        player.victoryPoints++;
        std::cout << "Settlement built\n";
    } else {
        std::cout << "Cannot place settlement here\n";
    }
}

void CatanGame::buildRoad(Player& player, bool isInitial) {
    if (!isInitial) {
        std::map<Resource, int> cost = {{Resource::Wood, 1}, {Resource::Brick, 1}};
        if (!player.hasResources(cost)) {
            std::cout << "Not enough resources\n";
            return;
        }
    }

    std::cout << "Enter the starting vertex: ";
    int start;
    std::cin >> start;
    std::cout << "Enter the ending vertex: ";
    int end;
    std::cin >> end;

    bool canPlace = false;
    for (const auto& s : player.settlements) {
        if (s.second == start || s.second == end) {
            canPlace = true;
            break;
        }
    }
    for (const auto& r : player.roads) {
        if (r.first == start || r.first == end || r.second == start || r.second == end) {
            canPlace = true;
            break;
        }
    }

    if (canPlace && std::find(board.adjacencyList[start].begin(), board.adjacencyList[start].end(), end) != board.adjacencyList[start].end()) {
        if (!isInitial) {
            std::map<Resource, int> cost = {{Resource::Wood, 1}, {Resource::Brick, 1}};
            player.removeResources(cost);
        }
        player.roads.emplace_back(start, end);
        std::cout << "Road built\n";
    } else {
        std::cout << "Cannot place road here\n";
    }
}

void CatanGame::buyDevelopmentCard(Player& player) {
    std::map<Resource, int> cost = {{Resource::Ore, 1}, {Resource::Grain, 1}, {Resource::Wool, 1}};
    if (player.hasResources(cost) && !developmentCards.empty()) {
        player.removeResources(cost);
        Card* card = developmentCards.back();
        developmentCards.pop_back();
        player.cards.push_back(card);
        std::cout << "Development card bought: " << card->getName() << "\n";
    } else {
        std::cout << "Cannot buy development card\n";
    }
}

void CatanGame::useDevelopmentCard(Player& player) {
    if (player.cards.empty()) {
        std::cout << "No development cards to use\n";
        return;
    }

    player.printCards();
    std::cout << "Enter the index of the card to use: ";
    int index;
    std::cin >> index;

    if (index >= 0 && index < player.cards.size()) {
        Card* card = player.cards[index];
        card->use(player, players);
        player.cards.erase(player.cards.begin() + index);
        delete card;
    } else {
        std::cout << "Invalid card index\n";
    }
}

bool CatanGame::checkWinCondition() {
    for (const auto& player : players) {
        if (player.victoryPoints >= 10) {
            std::cout << player.name << " wins with " << player.victoryPoints << " victory points!\n";
            return true;
        }
    }
    return false;
}

void CatanGame::setupInitialSettlements() {
    std::cout << "Initial setup phase\n";
    for (int round = 0; round < 2; ++round) {
        for (size_t i = 0; i < players.size(); ++i) {
            int playerIndex = (round == 0) ? i : (players.size() - 1 - i);
            Player& currentPlayer = players[playerIndex];
            
            std::cout << currentPlayer.name << ", place your initial settlement\n";
            buildSettlement(currentPlayer, true);
            
            if (round == 1) {
                for (const auto& settlement : currentPlayer.settlements) {
                    if (settlement == currentPlayer.settlements.back()) {
                        for (const auto& adjacentTile : getAdjacentTiles(settlement.first, settlement.second)) {
                            if (board.tiles[adjacentTile].terrain != TerrainType::Desert) {
                                currentPlayer.addResource(board.terrainToResource(board.tiles[adjacentTile].terrain));
                            }
                        }
                    }
                }
            }
            
            std::cout << currentPlayer.name << ", place your initial road\n";
            buildRoad(currentPlayer, true);
        }
    }
    board.printMap();
}

std::vector<int> CatanGame::getAdjacentTiles(int tileNum, int vertexNum) {
    std::vector<int> adjacentTiles;
    adjacentTiles.push_back(tileNum);
    if (tileNum > 0) adjacentTiles.push_back(tileNum - 1);
    if (tileNum < 18) adjacentTiles.push_back(tileNum + 1);
    return adjacentTiles;
}

bool CatanGame::playOneTurn() {
    playTurn();
    return checkWinCondition();
}

Player* CatanGame::getWinner() const {
    for (const auto& player : players) {
        if (player.victoryPoints >= 10) {
            return const_cast<Player*>(&player);
        }
    }
    return nullptr;
}

void CatanGame::playOneRound() {
    std::cout << "\n--- Playing one round ---\n";
    for (size_t i = 0; i < players.size(); ++i) {
        Player& currentPlayer = players[currentPlayerIndex];
        std::cout << "\n" << currentPlayer.name << "'s turn\n";
        
        int diceRoll = rollDice();
        distributeResources(diceRoll);
        
        trade(currentPlayer);
        build(currentPlayer);

        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }

    std::cout << "\n--- Round complete ---\n";
    for (const auto& player : players) {
        std::cout << player.name << " has " << player.victoryPoints << " victory points.\n";
        player.printResources();
    }
}


void CatanGame::autoSetupInitialSettlements() {
    std::cout << "Auto-setting up initial settlements and roads\n";
    for (int round = 0; round < 2; ++round) {
        for (size_t i = 0; i < players.size(); ++i) {
            int playerIndex = (round == 0) ? i : (players.size() - 1 - i);
            Player& currentPlayer = players[playerIndex];
            
            int tileNum = rand() % 19; 
            int vertexNum = rand() % 6; 
            currentPlayer.settlements.emplace_back(tileNum, vertexNum);
            std::cout << currentPlayer.name << " placed a settlement at tile " << tileNum << ", vertex " << vertexNum << "\n";
            
            int startVertex = rand() % 54;  
            int endVertex = board.adjacencyList[startVertex][rand() % board.adjacencyList[startVertex].size()];
            currentPlayer.roads.emplace_back(startVertex, endVertex);
            std::cout << currentPlayer.name << " placed a road from vertex " << startVertex << " to " << endVertex << "\n";
            
            if (round == 1) {
                for (const auto& adjacentTile : getAdjacentTiles(tileNum, vertexNum)) {
                    if (board.tiles[adjacentTile].terrain != TerrainType::Desert) {
                        currentPlayer.addResource(board.terrainToResource(board.tiles[adjacentTile].terrain));
                    }
                }
            }
        }
    }
}

void CatanGame::autoTrade(Player& player) {
    Resource give = static_cast<Resource>(rand() % 5);
    Resource receive = static_cast<Resource>(rand() % 5);
    if (give != receive && player.resources[give] > 0) {
        player.resources[give]--;
        player.resources[receive]++;
        std::cout << player.name << " traded 1 " << resourceToString(give) << " for 1 " << resourceToString(receive) << "\n";
    }
}

void CatanGame::autoBuild(Player& player) {
    int choice = rand() % 4;
    switch (choice) {
        case 0:
            if (player.hasResources({{Resource::Wood, 1}, {Resource::Brick, 1}, {Resource::Grain, 1}, {Resource::Wool, 1}})) {
                int tileNum = rand() % 19;
                int vertexNum = rand() % 6;
                player.settlements.emplace_back(tileNum, vertexNum);
                player.removeResources({{Resource::Wood, 1}, {Resource::Brick, 1}, {Resource::Grain, 1}, {Resource::Wool, 1}});
                player.victoryPoints++;
                std::cout << player.name << " built a settlement at tile " << tileNum << ", vertex " << vertexNum << "\n";
            }
            break;
        case 1:
            if (player.hasResources({{Resource::Wood, 1}, {Resource::Brick, 1}})) {
                int startVertex = rand() % 54;
                int endVertex = board.adjacencyList[startVertex][rand() % board.adjacencyList[startVertex].size()];
                player.roads.emplace_back(startVertex, endVertex);
                player.removeResources({{Resource::Wood, 1}, {Resource::Brick, 1}});
                std::cout << player.name << " built a road from vertex " << startVertex << " to " << endVertex << "\n";
            }
            break;
        case 2:
            buyDevelopmentCard(player);
            break;
        case 3:
            if (!player.cards.empty()) {
                useDevelopmentCard(player);
            }
            break;
    }
}

void CatanGame::autoPlayOneRound() {
    std::cout << "\n--- Playing one automated round ---\n";
    for (size_t i = 0; i < players.size(); ++i) {
        Player& currentPlayer = players[currentPlayerIndex];
        std::cout << "\n" << currentPlayer.name << "'s turn\n";
        
        int diceRoll = rollDice();
        distributeResources(diceRoll);
        
        autoTrade(currentPlayer);
        autoBuild(currentPlayer);

        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }

    std::cout << "\n--- Round complete ---\n";
    for (const auto& player : players) {
        std::cout << player.name << " has " << player.victoryPoints << " victory points.\n";
        player.printResources();
    }
}

void CatanGame::showcaseOneRound() {
    autoSetupInitialSettlements();
    board.printMap();
    autoPlayOneRound();
}