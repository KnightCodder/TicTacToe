#include <iostream>

constexpr int cross = 1;
constexpr int circle = -1;
constexpr int empty = 0;
constexpr int still_going = 6;

int max(int a, int b)
{
    if (a < b)
        return b;
    return a;
}
int evaluate(int a[9])
{
    if ((a[0] == cross && a[1] == cross && a[2] == cross) || (a[3] == cross && a[4] == cross && a[5] == cross) || (a[6] == cross && a[7] == cross && a[8] == cross) || (a[0] == cross && a[3] == cross && a[6] == cross) || (a[1] == cross && a[4] == cross && a[7] == cross) || (a[2] == cross && a[5] == cross && a[8] == cross) || (a[0] == cross && a[4] == cross && a[8] == cross) || (a[6] == cross && a[4] == cross && a[2] == cross))
        return cross;
    else if ((a[0] == circle && a[1] == circle && a[2] == circle) || (a[3] == circle && a[4] == circle && a[5] == circle) || (a[6] == circle && a[7] == circle && a[8] == circle) || (a[0] == circle && a[3] == circle && a[6] == circle) || (a[1] == circle && a[4] == circle && a[7] == circle) || (a[2] == circle && a[5] == circle && a[8] == circle) || (a[0] == circle && a[4] == circle && a[8] == circle) || (a[6] == circle && a[4] == circle && a[2] == circle))
        return circle;
    else if (a[0] != empty && a[1] != empty && a[2] != empty && a[3] != empty && a[4] != empty && a[5] != empty && a[6] != empty && a[7] != empty && a[8] != empty)
        return empty;
    else
        return still_going;
}
int negamax(int position[9], int alpha, int beta, int turn)
{
    int result = evaluate(position);
    if (result != still_going)
        return (result == cross) ? turn : (result == circle) ? -turn
                                                             : 0;
    int best_score = -1;
    for (int i = 0; i < 9; i++)
    {
        if (position[i] == empty)
        {
            int new_position[9];
            std::copy(position, position + 9, new_position);
            new_position[i] = turn;
            int score = -negamax(new_position, -beta, -alpha, -turn);
            best_score = max(best_score, score);
            alpha = max(alpha, score);
            if (alpha >= beta)
                break;
        }
    }
    return best_score;
}
void analysisPrinter(int position[9], int turn)
{
    for (int i = 0; i < 9; i++)
    {
        if (position[i] == 0)
        {
            int new_position[9];
            std::copy(position, position + 9, new_position);
            new_position[i] = turn;
            std::cout << i << "\t" << -negamax(new_position, -1, 1, -turn) << "\n";
        }
    }
}
void printBoard(int a[9])
{
    std::cout << "\n-------------\n";
    for (int i = 0; i < 3; i++)
    {
        std::cout << "|";
        for (int j = 0; j < 3; j++)
        {
            if (a[i*3 + j] == cross)
                std::cout << " X |";
            if (a[i*3 + j] == circle)
                std::cout << " O |";
            if (a[i*3 + j] == empty)
                std::cout << " * |";
        }
        std::cout << "\n-------------\n";
    }
}
int bestMove(int position[9], int turn)
{
    int move;
    int s = -1;

    for (int i = 0; i < 9; i++)
    {
        if (position[i] == 0)
        {
            int new_position[9];
            std::copy(position, position + 9, new_position);
            new_position[i] = turn;
            int eval = -negamax(new_position, -1, 1, -turn);
            if (eval >= s)
            {
                move = i;
                s = eval;
            }
        }
    }

    return move;
}

int main()
{
    int x[] = {0, 0, 0,
               0, 0, 0,
               0, 0, 0};

    std::string cache;

    int turn = cross;
    std::cout << "enter turn (X/O) : ";
    std::cin >> cache;
    if (cache == "O" || cache == "o" || cache == "circle")
        turn = circle;
    
    int currentTurn = cross;
    for (int i = 0; i < 9; i++)
    {
        printBoard(x);

        if (turn == currentTurn)
        {
            int move;
            std::cout << "enter a move (the coordinates are number between [0,8]) : ";
            std::cin >> move;

            x[move] = turn;
        }
        else
        {
            int move = bestMove(x, currentTurn);
            x[move] = currentTurn;
        }

        currentTurn *= -1;
    }
    printBoard(x);

    return 0;
}