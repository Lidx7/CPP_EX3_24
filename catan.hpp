#ifndef CATAN_HPP
#define CATAN_HPP

#include <string>
#include <vector>
#include <map>
#include <random>

enum class Resource { Wood, Brick, Ore, Grain, Wool };
enum class TerrainType { Forest, Hills, Mountains, Fields, Pasture, Desert };

class Tile {
public:
    TerrainType terrain;
    int number;
    Tile(TerrainType t, int n);
};

Resource stringToResource(const std::string& s);
std::string resourceToString(Resource r);

class Player;  //TODO: remove this Forward declaration, just move the class definition above the Card class

class Card {
public:
    virtual void use(Player& player, std::vector<Player>& players) = 0;
    virtual std::string getName() const = 0;
    virtual ~Card() {}
};

class Knight : public Card {
public:
    void use(Player& player, std::vector<Player>& players) override;
    std::string getName() const override;
};

class VictoryPoint : public Card {
public:
    void use(Player& player, std::vector<Player>& players) override;
    std::string getName() const override;
};

class YearOfPlenty : public Card {
public:
    void use(Player& player, std::vector<Player>& players) override;
    std::string getName() const override;
};

class RoadBuilding : public Card {
public:
    void use(Player& player, std::vector<Player>& players) override;
    std::string getName() const override;
};

class Monopoly : public Card {
public:
    void use(Player& player, std::vector<Player>& players) override;
    std::string getName() const override;
};

class Player {
public:
    std::string name;
    std::map<Resource, int> resources;
    std::vector<Card*> cards;
    int victoryPoints;
    int knightsPlayed;
    std::vector<std::pair<int, int>> settlements;
    std::vector<std::pair<int, int>> roads;

    Player(std::string n);
    void addResource(Resource r, int amount = 1);
    bool hasResources(const std::map<Resource, int>& cost) const;
    void removeResources(const std::map<Resource, int>& cost);
    void printResources() const;
    void printCards() const;
    void buildRoad(bool isFree = false);
};

class Board {
public:
    std::vector<Tile> tiles;
    std::vector<std::vector<int>> adjacencyList;

    Board();
    void setupBoard();
    void setupAdjacencyList();
    void printBoard() const;
    Resource terrainToResource(TerrainType terrain) const;
    void printMap();

    void printRow(int start, int end, int padding);

    void printRowTop(int start, int end);

    void printRowMiddle1(int start, int end);

    void printRowMiddle(int start, int end);

    void printRowMiddle2(int start, int end);

    void printRowBottom(int start, int end);

    void addHexagonRow(std::vector<std::string> &mapLines, int start, int end, int padding);

private:
    void printRow(int start, int end);
    void printBottomRow(int start, int end);
    std::string formatNumber(int num) const;
    char terrainToChar(TerrainType terrain) const;
};

class CatanGame {
private:
    Board board;
    std::vector<Player> players;
    int currentPlayerIndex;
    std::vector<Card*> developmentCards;
    std::mt19937 rng;

public:
    CatanGame();
    void initializeDevelopmentCards();
    void playTurn();
    int rollDice();
    void distributeResources(int diceRoll);
    void trade(Player& player);
    void build(Player& player);
    void buildSettlement(Player& player, bool isInitial = false);
    void buildRoad(Player& player, bool isInitial = false);
    void buyDevelopmentCard(Player& player);
    void useDevelopmentCard(Player& player);
    bool checkWinCondition();
    void play();
    void setupInitialSettlements();
    std::vector<int> getAdjacentTiles(int tileNum, int vertexNum);
    bool playOneTurn();
    Player *getWinner() const;

    void playOneRound();
    void autoSetupInitialSettlements();
    void autoTrade(Player &player);
    void autoBuild(Player &player);
    void autoPlayOneRound();
    void showcaseOneRound();
};

#endif // CATAN_HPP