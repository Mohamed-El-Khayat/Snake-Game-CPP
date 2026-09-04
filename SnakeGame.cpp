/* اللهم انفعنا بما علمتنا، وعلمنا ما ينفعنا، وزدنا علما. */

#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

bool GameOver, Pause, Win;

int W = 21, H = 11; // width and height of the game board
int snake_x, snake_y, fruit_x, fruit_y; // positions
int score;

enum Direction { STOP, LEFT, RIGHT, UP, DOWN };

Direction dir;
Direction Prev;

vector<pair<int, int> > Tail;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dist_x(1, 2 * W - 3);
uniform_int_distribution<int> dist_y(0, H - 1);

const int GAME_SPEED = 100;

void GenFruit() {
    if (Tail.size() + 1 == (2 * W - 3) * H)
        return;
    while (true) {
        fruit_x = dist_x(gen);
        fruit_y = dist_y(gen);
        if (fruit_x == snake_x and fruit_y == snake_y) continue;
        bool FruitOnTail = false;
        for (auto &k: Tail) {
            if (k.first == fruit_x and k.second == fruit_y) {
                FruitOnTail = true;
                break;
            }
        }
        if (!FruitOnTail) return;
    }
}

void HideCursor() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);

    cursorInfo.bVisible = false;

    SetConsoleCursorInfo(console, &cursorInfo);
}

void GameOverScreen() {
    system("cls");

    cout << R"(
       _____                         ____
      / ____|                       / __ \
     | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __
     | | |_ |/ _` | '_ ` _ \ / _ \ | |  | \ \ / / _ \ '__|
     | |__| | (_| | | | | | |  __/ | |__| |\ V /  __/ |
      \_____|\__,_|_| |_| |_|\___|  \____/  \_/ \___|_|

    )";

    cout << "\nScore: " << score << '\n';
}

void WinScreen() {
    system("cls");

    cout << R"(

 __      __ ___ _   _
 \ \    / /|_ _| \ | |
  \ \/\/ /  | ||  \| |
   \_/\_/  |___|_|\__|

  CONGRATULATIONS!

)";

    cout << "\nFinal Score: " << score << '\n';
}

void Start() {
    GameOver = false;
    Pause = false;
    Win = false;

    score = 0;

    snake_x = W - 1;
    snake_y = H / 2;

    GenFruit();

    dir = STOP;
    Prev = STOP;

    Tail.clear();
}

void Board() {
    system("cls");
    for (int i = 0; i < W; i++) {
        if (i == W / 2) cout << ". ";
        else cout << "# ";
    }
    cout << '\n';
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < 2 * W; j++) {
            if (j == 0 or j == 2 * W - 2) {
                if (i == H / 2) {
                    if (j == 0) cout << '[';
                    else if (j == 2 * W - 2) cout << ']';
                } else cout << '#';
            } else if (j == snake_x and i == snake_y) {
                cout << '0';
            } else if (j == fruit_x and i == fruit_y) {
                cout << '*';
            } else {
                bool print = false;
                for (auto &k: Tail) {
                    if (k.first == j and k.second == i) {
                        cout << 'O';
                        print = true;
                        break;
                    }
                }
                if (!print) {
                    cout << ' ';
                }
            }
        }
        cout << '\n';
    }
    for (int i = 0; i < W; i++) {
        if (i == W / 2) cout << ". ";
        else cout << "# ";
    }
    cout << '\n' << "Score: " << score << '\n';
    cout << "\nLeft:  A/a \nDown:  S/s \nRight: D/d \nUP:    w/w \nPause: P/p";
}

void Input() {
    if (_kbhit()) {
        switch (getch()) {
            case 'a':
            case 'A':
                if (Prev != RIGHT or Tail.empty())
                    dir = LEFT;
                break;
            case 'w':
            case 'W':
                if (Prev != DOWN or Tail.empty())
                    dir = UP;
                break;
            case 'd':
            case 'D':
                if (Prev != LEFT or Tail.empty())
                    dir = RIGHT;
                break;
            case 's':
            case 'S':
                if (Prev != UP or Tail.empty())
                    dir = DOWN;
                break;
            case 'p':
            case 'P':
                Pause = true;
                break;
            default:
                break;;
        }
    }
}

void Logic() {
    for (int i = (int) Tail.size() - 1; i > 0; i--) {
        Tail[i] = Tail[i - 1];
    }
    if (!Tail.empty())
        Tail[0] = {snake_x, snake_y};

    switch (dir) {
        case STOP:
            break;
        case LEFT:
            snake_x--;
            Prev = LEFT;
            break;
        case RIGHT:
            snake_x++;
            Prev = RIGHT;
            break;
        case UP:
            snake_y--;
            Prev = UP;
            break;
        case DOWN:
            snake_y++;
            Prev = DOWN;
            break;
    }

    if (snake_x < 1 or snake_x > 2 * W - 3 or snake_y < 0 or snake_y > H - 1) {
        if (snake_x == 2 * W - 2 and snake_y == H / 2) {
            snake_x = 1;
            score-=2;
        } else if (snake_x == 0 and snake_y == H / 2) {
            snake_x = 2 * W - 3;
            score-=2;
        } else if (snake_y == H and snake_x == W - 1) {
            snake_y = 0;
            score-=1;
        } else if (snake_y == -1 and snake_x == W - 1) {
            snake_y = H - 1;
            score-=1;
        } else GameOver = true;
    }
    for (auto &k: Tail) {
        if (snake_x == k.first and snake_y == k.second) {
            GameOver = true;
            break;
        }
    }
    if (snake_x == fruit_x and snake_y == fruit_y) {
        Tail.emplace_back(0, 0);
        score += 10;
        if (Tail.size() + 1 == (2 * W - 3) * H) {
            GameOver = true;
            Win = true;
            return;
        }
        GenFruit();
    }
}

signed main() {
    Start();
    HideCursor();
    while (!GameOver) {
        Board();
        Input();
        Logic();
        Sleep(GAME_SPEED);
        while (Pause) {
            if (_kbhit()) {
                if (getch() == 'p' or getch() == 'P') {
                    Pause = false;
                }
            }
        }
    }
    if (Win)
        WinScreen();
    else
        GameOverScreen();
    return 0;
}

/*El-Khayat*/
