#include <iostream>
#include "view.h"

using namespace russian_draughts;

void process_playing() {
    while (game.GetGameResult() == GameResult::NOT_ENDED) {
        game.MakeMove();        
    }
}

void initialize_game_state() { //checks of correctness of input are needed
    size_t diff, color;
    std::cout << "Choose difficulty (from 1 to 5): \n";
    std::cin >> diff;
    std::cout << "Type your color: either w or b if you want to play white or black respectively\n";
    std::cin >> color;
    game = Game(static_cast<Diffs>(diff),
        static_cast<Colors>(color));
    static_cast<Colors>(color) == Colors::WHITE ?
        game.SetIsComputerMove(false) : game.SetIsComputerMove(true);
}

void start_playing() {
    initialize_game_state();
    process_playing();
}

int main() {
    start_playing();
    return 0;
}
