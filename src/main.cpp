#include "GameEngine.hpp"

int main(int argc, char* argv[]) {
    GameEngine g("configs/assets.txt");
    g.run();
    return 0;
}