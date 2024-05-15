#include <iostream>
#include <chrono>
#include <unordered_map>
#include <functional>

namespace result
{
    const int cross = 1;
    const int circle = -1;
    const int draw = 0;
    const int still_going = 6;
}

namespace turn
{
    const int cross = 1;
    const int circle = -1;
}

namespace bitboard
{
    const int empty = 0;
    const int fill = 511;
    const int over[] = {7, 56, 448, 273, 84, 292, 146, 73};
}

struct Board
{
    int X = bitboard::empty;
    int O = bitboard::empty;

    int evaluate()
    {
        if ((X & 7) == 7 || (X & 56) == 56 || (X & 448) == 448 || (X & 273) == 273 || (X & 84) == 84 || (X & 292) == 292 || (X & 146) == 146 || (X & 73) == 73)
            return result::cross;
        else if ((O & 7) == 7 || (O & 56) == 56 || (O & 448) == 448 || (O & 273) == 273 || (O & 84) == 84 || (O & 292) == 292 || (O & 146) == 146 || (O & 73) == 73)
            return result::circle;
        else if ((X | O) == bitboard::fill)
            return result::draw;
        else
            return result::still_going;
    }

    bool isEmpty(int bit_position)
    {
        int mask = 1 << bit_position;
        return !((X | O) & mask);
    }

    void playMove(int index, int turn)
    {
        int mask = 1 << index;
        if (turn == turn::cross)
            X = X | mask;
        else
            O = O | mask;
    }

    void undoMove(int index)
    {
        int mask = ~(1 << index);
        X = X & mask;
        O = O & mask;
    }

    void print()
    {
        std::cout << "\n-------------\n";
        for (int i = 0; i < 3; i++)
        {
            std::cout << "|";
            for (int j = 0; j < 3; j++)
            {
                int mask = 1 << (i * 3 + j);
                if (X & mask)
                    std::cout << " X |";
                else if (O & mask)
                    std::cout << " O |";
                else
                    std::cout << " * |";
            }
            std::cout << "\n-------------\n";
        }
    }

    bool operator==(const Board &other) const
    {
        return X == other.X && O == other.O;
    }
};

namespace std
{
    template<>
    struct hash<Board>
    {
        std::size_t operator()(const Board& b) const
        {
            return hash<int>()(b.X) ^ (hash<int>()(b.O) << 1);
        }
    };
}

std::unordered_map<Board, int> transpositionTable;

int negamax(Board position, int alpha, int beta, int turn)
{
    if (transpositionTable.find(position) != transpositionTable.end())
        return transpositionTable[position];

    int result = position.evaluate();
    if (result != result::still_going)
    {
        result = (result == result::cross) ? turn : (result == result::circle) ? -turn : 0;
        transpositionTable[position] = result;
        return result;
    }

    int best_score = -1;
    for (int i = 0; i < 9; i++)
    {
        if (position.isEmpty(i))
        {
            position.playMove(i, turn);
            int score = -negamax(position, -beta, -alpha, -turn);
            position.undoMove(i);
            best_score = std::max(best_score, score);
            alpha = std::max(alpha, score);
            if (alpha >= beta)
                break;
        }
    }

    transpositionTable[position] = best_score;
    return best_score;
}

void analysisPrinter(Board position, int turn)
{
    for (int i = 0; i < 9; i++)
    {
        if (position.isEmpty(i))
        {
            position.playMove(i, turn);
            std::cout << i << "\t" << -negamax(position, -1, 1, -turn) << "\n";
            position.undoMove(i);
        }
    }
}

int bestMove(Board position, int turn)
{
    int move;
    int s = -1;

    for (int i = 0; i < 9; i++)
    {
        if (position.isEmpty(i))
        {
            position.playMove(i, turn);
            int eval = -negamax(position, -1, 1, -turn);
            position.undoMove(i);
            if (eval >= s)
            {
                move = i;
                s = eval;
            }
        }
    }

    return move;
}

void play()
{
    Board gameBoard;
    std::string cache;
    int turn = turn::cross;

    std::cout << "enter turn (X/O) : ";
    std::cin >> cache;
    if (cache == "O" || cache == "o" || cache == "circle")
        turn = turn::circle;

    int currentTurn = turn::cross;
    while (true)
    {
        int gameStatus = gameBoard.evaluate();
        if (gameStatus != result::still_going)
        {
            gameBoard.print();
            std::cout << "\n-------------------------------\n\nGame Over : " << gameStatus << "\n\n-------------------------------\n"
                      << std::endl;
            std::cin >> cache;
            break;
        }

        gameBoard.print();
        if (turn == currentTurn)
        {
            int move;
            std::cout << "enter a move (the coordinates are number between [0,8]) : ";
            std::cin >> move;
            gameBoard.playMove(move, currentTurn);
        }
        else
        {
            int move = bestMove(gameBoard, currentTurn);
            gameBoard.playMove(move, currentTurn);
        }

        currentTurn *= -1;
    }
}

int main()
{
    play();
    // Board a;
    // auto start = std::chrono::high_resolution_clock::now();
    // bestMove(a, 1);
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    // std::cout << "Execution time: " << duration.count() << " nanoseconds" << std::endl;
    return 0;
}
