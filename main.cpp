#include "catan.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    CatanGame game;
    
    if (argc > 1 && std::string(argv[1]) == "catan") {
        std::cout << "Showcasing one round of Catan\n";
        game.showcaseOneRound();
    } else {
        game.play();
    }

    return 0;
}