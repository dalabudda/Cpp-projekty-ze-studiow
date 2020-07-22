#include <iostream>
#include <string>

int board[11][11];
int visited[11][11];
int board_size = 0;
int red_number = 0;
int blue_number = 0;

void loadBoard()
{
    board_size = 0;
    red_number = 0;
    blue_number = 0;
    bool first_half = true;
    int dept = 0;
    int y = 0;
    int x = 0;
    std::string str;
    while (std::cin >> str)
    {
        if (str == "--<")
        {
            if (first_half)
                board_size++;
            board[y][x] = 0;
        }
        if (str == "<")
        {
            board_size++;
            board[y][x] = 0;
            dept = 0;
            first_half = false;
        }
        if (str == ">-<")
        {
            y--;
            x++;
            board[y][x] = 0;
        }
        if (str == "r")
        {
            board[y][x] = 1;
            red_number++;
        }
        if (str == "b")
        {
            board[y][x] = 2;
            blue_number++;
        }
        if (str == ">--" || str == ">")
        {
            if (first_half)
            {
                x = 0;
                dept++;
                y = dept;
            }
            else
            {
                y = board_size - 1;
                dept++;
                x = dept;
            }
        }
        if (str == "---")
        {
            /*for (int Y = 0; Y < board_size; Y++)
            {
                for (int X = 0; X < board_size; X++)
                {
                    std::cout << board[Y][X] << " ";
                }
                std::cout << "\n";
            }*/

            break;
        }

    }
}

bool isBoardCorrect()
{
    return red_number == blue_number || red_number == (blue_number+1);
}

void checkWin(const int x, const int y, const int player, std::string &win)
{
    if (x >= 0 && x < board_size && y >= 0 && y < board_size)
    {
        if (!visited[y][x])
        {
            if (board[y][x] == player)
            {
                if (player == 1)
                {
                    if (x == board_size-1)
                    {
                        win = "YES RED";
                        return;
                    }
                }
                else //player 2
                {
                    if (y == board_size-1)
                    {
                        win = "YES BLUE";
                        return;
                    }
                }
                visited[y][x] = true;
                checkWin(x-1, y-1, player, win);
                checkWin(x, y-1, player, win);
                checkWin(x-1, y, player, win);
                checkWin(x+1, y, player, win);
                checkWin(x, y+1, player, win);
                checkWin(x+1, y+1, player, win);
            }
        }
    }
}

void checkGameOver(std::string &win)
{
    for (int y = 0; y < board_size; y++)
        for (int x = 0; x < board_size; x++)
            visited[y][x] = false;

    for (int y = 0; y < board_size; y++)
        checkWin(0, y, 1, win);
    if (win == "NO")
        for (int x = 0; x < board_size; x++)
            checkWin(x, 0, 2, win);
}

void checkIfFakeWin(std::string &win, const int player)
{
    for (int y = 0; y < board_size; y++)
    {
        for (int x = 0; x < board_size; x++)
        {
            if (board[y][x] == player)
            {
                win = "NO";
                board[y][x] = 0;
                checkGameOver(win);
                board[y][x] = player;
                if (win == "NO")
                    return;
            }
        }
    }
}

std::string strIsPossible()
{
    std::string win = "NO";
    checkGameOver(win);
    if (win == "NO")
        return "YES\n";
    else
    {
        int player;
        if (win == "YES RED")
        {
            if (red_number <= blue_number)
                return "NO\n";
            player = 1;
        }
        else
        {
            if (red_number != blue_number)
                return "NO\n";
            player = 2;
        }
        checkIfFakeWin(win, player);
        if (win == "NO")
            return "YES\n";
        else
            return "NO\n";
    }
}

int countEmpty()
{
    int number = 0;
    for (int y = 0; y < board_size; y++)
    {
        for (int x = 0; x < board_size; x++)
        {
            if (board[y][x] == 0)
                number++;
        }
    }
    return number;
}

std::string strOneMoveNaive(const int player)
{
    int turn = (red_number == blue_number)? 1: 2;
    if (countEmpty() >= 1 + (player != turn))
    {
        std::string win = "NO";
        checkGameOver(win);
        if (win != "NO")
            return "NO\n";

        for (int y = 0; y < board_size; y++)
        {
            for (int x = 0; x < board_size; x++)
            {
                if (board[y][x] == 0)
                {
                    board[y][x] = player;
                    checkGameOver(win);
                    board[y][x] = 0;
                    if (win != "NO")
                        return "YES\n";
                }
            }
        }
        return "NO\n";
    }
    else
        return "NO\n";
}

std::string strTwoMovesNaive(const int player)
{
    int turn = (red_number == blue_number)? 1: 2;
    if (countEmpty() >= 3 + (player != turn))
    {
        std::string win = "NO";
        checkGameOver(win);
        if (win != "NO")
            return "NO\n";

        for (int y = 0; y < board_size; y++)
        {
            for (int x = 0; x < board_size; x++)
            {
                if (board[y][x] == 0)
                {
                    board[y][x] = player;
                    checkGameOver(win);
                    if (win == "NO")
                    {
                        for (int Y = 0; Y < board_size; Y++)
                        {
                            for (int X = 0; X < board_size; X++)
                            {
                                if (board[Y][X] == 0)
                                {
                                    board[Y][X] = player;
                                    checkGameOver(win);
                                    board[Y][X] = 0;
                                    if (win != "NO")
                                    {
                                        board[y][x] = 0;
                                        return "YES\n";
                                    }

                                }
                            }
                        }
                    }
                    else
                        win = "NO";

                    board[y][x] = 0;
                }
            }
        }
        return "NO\n";
    }
    else
        return "NO\n";
}

int main()
{
    std::string str;
    while (std::cin >> str)
    {
        if (str == "---")
        {
            loadBoard();
        }
        else if (str == "BOARD_SIZE")
        {
            std::cout << board_size << "\n";
        }
        else if (str == "PAWNS_NUMBER")
        {
            std::cout << (red_number+blue_number) << "\n";
        }
        else if (str == "IS_BOARD_CORRECT")
        {
            if (isBoardCorrect())
                std::cout << "YES\n";
            else
                std::cout << "NO\n";
        }
        else if (str == "IS_GAME_OVER")
        {
            if (isBoardCorrect())
            {
                std::string win = "NO";
                checkGameOver(win);
                std::cout << win << "\n";
            }
            else
                std::cout << "NO\n";
        }
        else if (str == "IS_BOARD_POSSIBLE")
        {
            if (isBoardCorrect())
                std::cout << strIsPossible();
            else
                std::cout << "NO\n";
        }
        else if (str == "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT")
        {
            if (isBoardCorrect())
                std::cout << strOneMoveNaive(1);
            else
                std::cout << "NO\n";
        }
        else if (str == "CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT")
        {
            if (isBoardCorrect())
                std::cout << strOneMoveNaive(2);
            else
                std::cout << "NO\n";
        }
        else if (str == "CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT")
        {
            if (isBoardCorrect())
                std::cout << strTwoMovesNaive(1);
            else
                std::cout << "NO\n";
        }
        else if (str == "CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT")
        {
            if (isBoardCorrect())
                std::cout << strTwoMovesNaive(2);
            else
                std::cout << "NO\n";
        }
        else if (str == "CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT")
        {

        }
        else if (str == "CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT")
        {

        }
        else if (str == "CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT")
        {

        }
        else if (str == "CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT")
        {

        }
    }

    return 0;
}
