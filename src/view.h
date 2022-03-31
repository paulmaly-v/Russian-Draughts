#include <string>
#include <set>
#include <stack>
#include <algorithm>
#include <vector>
#include <bit>

// TO DO: EXCEPTION-SAFETY

namespace russian_draughts {

    enum Diffs {
        EASY = 1,
        NORMAL,
        HARD,
        INSANE,
        EXPERT,
    };
    enum Colors {
        WHITE = 0,
        BLACK,
    };
    enum GameResult {
        NOT_ENDED = 0,
        WHITE_WIN,
        BLACK_WIN,
        DRAW,
    };

    class Game {

        class FieldState {
        public:
            uint64_t white_checker_bitmask;
            uint64_t black_checker_bitmask;
            uint64_t white_king_bitmask;
            uint64_t black_king_bitmask;
            void RefreshField() {
                white_king_bitmask = black_king_bitmask = 0;
                for (uint64_t i = 0, w = 1, b = (1 << 63); i < 3; ++i) {
                    for (uint64_t j = 0; j < 4; ++j) {
                        white_checker_bitmask |= w;
                        w <<= 2;
                        black_checker_bitmask |= b;
                        b >>= 2;
                    }
                    i != 1 ? ((w <<= 1),
                        (b >>= 1)) : ((w >>= 1),
                            (b <<= 1));
                    
                }
            }
            
            FieldState() {
                RefreshField();
            }

            uint64_t* GetWhiteCheckerPtr() {
                return &white_checker_bitmask;
            }

            uint64_t* GetBlackCheckerPtr() {
                return &black_checker_bitmask;
            }

            uint64_t* GetWhiteKingPtr() {
                return &white_king_bitmask;
            }

            uint64_t* GetBlackKingPtr() {
                return &black_king_bitmask;
            }
            
        };
        
        FieldState field_state = FieldState();
        Diffs current_diff = Diffs::EASY;
        Colors current_color = Colors::WHITE;
        GameResult game_result = GameResult::NOT_ENDED;
        bool is_computer_move;
        bool must_capture;

    public:

        Game(Diffs diff, Colors color) : field_state(FieldState()), must_capture(false) {
            SetDiff(diff);
            SetColor(color);
        }

        Game() {}

        uint64_t GetFullField() {
            return -1;
        }

        bool IsWhiteMove() {
            return !is_computer_move ? current_color : !current_color;
        }

        bool IsKingByPairCell(const std::pair<uint8_t, uint8_t>& cell) {
            return (((*field_state.GetWhiteKingPtr()) & GetBitmaskPositionByPairCell(cell)) ||
                ((*field_state.GetBlackKingPtr()) & GetBitmaskPositionByPairCell(cell)));
        }

        bool CheckIfWhiteCanMove() {
            //TO DO
        }

        bool CheckIfBlackCanMove() {
            //TO DO
        }

        void CheckIfEnd() {
            if (std::__popcount(*field_state.GetBlackCheckerPtr()) + std::__popcount(*field_state.GetBlackKingPtr()) == 0) {
                game_result = GameResult::WHITE_WIN;
                return;
            }
            if (std::__popcount(*field_state.GetWhiteCheckerPtr()) + std::__popcount(*field_state.GetWhiteKingPtr()) == 0) {
                game_result = GameResult::BLACK_WIN;
                return;
            }

            if (IsWhiteMove() && !CheckIfWhiteCanMove()) {
                game_result = GameResult::BLACK_WIN;
                return;
            } else if (!IsWhiteMove() && !CheckIfBlackCanMove()) {
                game_result = GameResult::WHITE_WIN;
                return;
            }
            //TO DO: DRAW CONDITIONS
            
        }

        void MakeComputerMove() {
            // TO DO
        }

        std::vector<std::pair<uint8_t, uint8_t>> ParsePlayerMove(const std::string& str) {
            std::vector<std::pair<uint8_t, uint8_t>> move;
            for (size_t i = 0; i < str.length() / 2; ++i) {
                move.push_back(GetPairPositionByStringCell("" + str[i*2] + str[i*2 + 1]));
            }
        }

        uint64_t GetBitmaskPositionByStringCell(const std::string& cell) {
            uint64_t bitmask = 1;
            int curx = cell[0] - 'a', cury = cell[1] - '0';
            bitmask <<= (curx + 8 * (cury));
            return bitmask;
        }

        uint64_t GetBitmaskPositionByPairCell(const std::pair<uint8_t, uint8_t>& cell) {
            uint64_t bitmask = 1;
            int curx = cell.first, cury = cell.second;
            bitmask <<= (curx + 8 * (cury));
            return bitmask;
        }

        std::pair<uint8_t, uint8_t> GetPairPositionByStringCell(const std::string& cell) {
            uint64_t curx = cell[0] - 'a', cury = cell[1] - '0';
            return std::make_pair(curx, cury);
        }

        void CaptureBetweenCells(std::pair<uint8_t, uint8_t> cell1, std::pair<uint8_t, uint8_t> cell2) {
            uint64_t* checker_ptr = IsWhiteMove() ? field_state.GetBlackCheckerPtr() : field_state.GetWhiteCheckerPtr();
            uint64_t* king_ptr = IsWhiteMove() ? field_state.GetBlackKingPtr() : field_state.GetWhiteKingPtr();
            std::pair<int, int> direction = { 1, 1 };
            auto tmp = cell1;
            if (cell1.first > cell2.first) direction.first = -1;
            if (cell1.second > cell2.second) direction.second = -1;
            cell1 = { cell1.first + direction.first, cell1.second + direction.second };
            while (tmp != cell2) {
                *checker_ptr &= (GetFullField() - GetBitmaskPositionByPairCell(tmp));
                *king_ptr &= (GetFullField() - GetBitmaskPositionByPairCell(tmp));
                tmp = { tmp.first + direction.first, tmp.second + direction.second };
            }
        }

        bool WillBecomeKing(uint64_t* cur_mover, const std::vector<std::pair<uint8_t, uint8_t>> move) {
            uint8_t check_line = IsWhiteMove() ? 7 : 0;
            for (auto& x : move) {
                if (x.second == check_line) {
                    return true;
                }
            }
            return false;
        }

        void MakePlayerMove() {
            std::string move_str;
            std::cin >> move_str;
            size_t move_length = move_str.length() / 2;
            std::vector<std::pair<uint8_t, uint8_t>> move = ParsePlayerMove(move_str);

            //Change field state
            for (size_t i = 0; i < move.size() - 1; ++i) {
                CaptureBetweenCells(move[i], move[i + 1]);
            }

            uint64_t* cur_mover;
            if (IsWhiteMove()) {
                if (IsKingByPairCell(move[0])) {
                    cur_mover = field_state.GetWhiteKingPtr();
                } else {
                    cur_mover = field_state.GetWhiteCheckerPtr();
                }
            } else {
                if (IsKingByPairCell(move[0])) {
                    cur_mover = field_state.GetBlackKingPtr();
                } else {
                    cur_mover = field_state.GetBlackCheckerPtr();
                }
            }
            
            if (IsKingByPairCell(move[0]) ||
                (!IsKingByPairCell(move[0]) && !WillBecomeKing(cur_mover, move))) {
                *cur_mover &= (GetFullField() - GetBitmaskPositionByPairCell(move[0]));
                *cur_mover |= (GetBitmaskPositionByPairCell(move[move.size() - 1]));
            } else {
                *cur_mover &= (GetFullField() - GetBitmaskPositionByPairCell(move[0]));
                uint64_t* add_king = IsWhiteMove() ? field_state.GetWhiteKingPtr() :
                    field_state.GetBlackKingPtr();
                *add_king |= (GetBitmaskPositionByPairCell(move[move.size() - 1]));
            }
        }

        bool GetMustCapture() {
            return current_diff;
        }
        void SetMustCapture(bool must) {
            must_capture = must;
        }
        
        void MakeMove() {
            GetIsComputerMove() ? MakeComputerMove() : MakePlayerMove();
            SetColor(static_cast<Colors>(1 - current_color));
            SetIsComputerMove(!GetIsComputerMove());
            CheckIfEnd();
        }

        GameResult GetGameResult() {
            return game_result;
        }
        void SetGameResult(GameResult game_result) {
            this->game_result = game_result;
        }

        Diffs GetIsComputerMove() {
            return current_diff;
        }
        void SetIsComputerMove(bool is_computer_move) {
            this->is_computer_move = is_computer_move;
        }

        Diffs GetDiff() {
            return current_diff;
        }
        void SetDiff(Diffs diff) {
            current_diff = diff;
        }
        Colors GetColor() {
            return current_color;
        }
        void SetColor(Colors color) {
            current_color = color;
        }
        
        
    } game;

    };