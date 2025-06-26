#include <windows.h>
#include <conio.h>
#include <deque>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;
using Clock = chrono::steady_clock;
const int WIDTH         = 40;
const int HEIGHT        = 20;
const int STATUS_LINES  = 3;
const int INITIAL_DELAY = 100;
const char* CREATOR     = "Tux;
const char* GAME_NAME   = "DEAR EX SNAKE";
const char* PLAYER_NAME = "Victim";
const char* PLAYER_ID   = "456";
enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

class Snake {
public:
    Snake() : dir(LEFT), delay(INITIAL_DELAY), score(0), alive(true) {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        srand((unsigned)time(nullptr));
        setupConsole();
    }

    void start() {
        showStartScreen();
        reset();
        run();
    }

private:
    HANDLE hOut;
    deque<Point> body;
    Point food;
    Direction dir;
    int delay;
    int score;
    bool alive;
    Clock::time_point gameStart;
    float staminaSeconds = 0.0f;

    void setupConsole() {
        COORD buf = { (SHORT)WIDTH, (SHORT)(HEIGHT + STATUS_LINES) };
        SetConsoleScreenBufferSize(hOut, buf);
        SMALL_RECT win = { 0, 0, (SHORT)(WIDTH - 1), (SHORT)(HEIGHT + STATUS_LINES - 1) };
        SetConsoleWindowInfo(hOut, TRUE, &win);
        CONSOLE_CURSOR_INFO cci = { 1, FALSE };
        SetConsoleCursorInfo(hOut, &cci);
    }

    void printBoxLine(const string& txt, WORD color) {
        string t = txt;
        if ((int)t.size() > WIDTH - 2) t = t.substr(0, WIDTH - 2);
        int pad = WIDTH - 2 - (int)t.size();
        int left = pad/2;
        int right = pad - left;
        SetConsoleTextAttribute(hOut, color);
        cout << "#" << string(left, ' ') << t << string(right, ' ') << "#\n";
    }

void showStartScreen() {
    system("cls");
    WORD borderColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    SetConsoleTextAttribute(hOut, borderColor);
    cout << string(WIDTH, '#') << "\n";
    auto printColoredLine = [&](const string& text, WORD color) {
        int pad = WIDTH - 2 - (int)text.size();
        int left = pad / 2;
        int right = pad - left;
        SetConsoleTextAttribute(hOut, borderColor);
        cout << "#";
        SetConsoleTextAttribute(hOut, color);
        cout << string(left, ' ') << text << string(right, ' ');
        SetConsoleTextAttribute(hOut, borderColor);
        cout << "#\n";
    };
    printColoredLine(" ", borderColor);
    printColoredLine(string(" ") + GAME_NAME + " ", FOREGROUND_RED | FOREGROUND_INTENSITY);
    printColoredLine(" ", borderColor);
    printColoredLine(string("NAME: ") + PLAYER_NAME + string("   ID: ") + PLAYER_ID, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printColoredLine(" ", borderColor);
    printColoredLine("@Use WASD to control your Ex", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printColoredLine(" ", borderColor);
    SetConsoleTextAttribute(hOut, borderColor);
    cout << string(WIDTH, '#') << "\n";
    for (int c = 3; c >= 1; --c) {
        COORD pos = {0, 7};
        SetConsoleCursorPosition(hOut, pos);
        printColoredLine(string("Get ready!   ") + to_string(c), FOREGROUND_RED | FOREGROUND_INTENSITY);
        this_thread::sleep_for(chrono::seconds(1));
    }
    system("cls");
}
    void reset() {
        body.clear();
        int cx = WIDTH / 2, cy = HEIGHT / 2;
        for (int i = 0; i < 5; ++i) body.push_back({cx + i, cy});
        dir = LEFT;
        score = 0;
        delay = INITIAL_DELAY;
        alive = true;
        gameStart = Clock::now();
        placeFood();
    }
    void placeFood() {
        static mt19937 rng((unsigned)Clock::now().time_since_epoch().count());
        uniform_int_distribution<int> dx(1, WIDTH-2), dy(1, HEIGHT-2);
        Point p;
        do { p.x = dx(rng); p.y = dy(rng); }
        while (find(body.begin(), body.end(), p) != body.end());
        food = p;
    }
        void processInput() {
        if (!_kbhit()) return;
        int c = _getch();
        if (c == 0 || c == 224) {
            int arrow = _getch();
            switch (arrow) {
                case 72: if (dir != DOWN) dir = UP;    break;
                case 80: if (dir != UP)   dir = DOWN;  break;
                case 75: if (dir != RIGHT)dir = LEFT;  break;
                case 77: if (dir != LEFT) dir = RIGHT; break;
            }
        } else {
            char ch = static_cast<char>(c);
            switch (ch) {
                case 'w': case 'W': if (dir != DOWN)  dir = UP;    break;
                case 's': case 'S': if (dir != UP)    dir = DOWN;  break;
                case 'a': case 'A': if (dir != RIGHT) dir = LEFT;  break;
                case 'd': case 'D': if (dir != LEFT)  dir = RIGHT; break;
                case 'r': case 'R': reset();           break;
                case 'x': case 'X': alive = false;     break;
            }
        }
    }
    void update() {
        Point head = body.front();
        switch(dir) {
            case UP:    head.y--; break;
            case DOWN:  head.y++; break;
            case LEFT:  head.x--; break;
            case RIGHT: head.x++; break;
        }
        if (head.x<=0||head.x>=WIDTH-1||head.y<=0||head.y>=HEIGHT-1||
            find(body.begin(),body.end(),head)!=body.end()) {
            alive=false; return;
        }
        body.push_front(head);
        if (head==food) {
            score += 10;
            if (delay > 50) delay -= 5;
            placeFood();
        } else {
            body.pop_back();
        }
    }
    void render() {
        SetConsoleCursorPosition(hOut,{0,0});
        WORD border = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(hOut,border);
        cout << string(WIDTH,'#') << "\n";
        for (int y = 1; y < HEIGHT-1; ++y) {
            cout << "#";
            for (int x = 1; x < WIDTH-1; ++x) {
                Point p{x,y};
                if (p == body.front()) {
                    SetConsoleTextAttribute(hOut, FOREGROUND_RED|FOREGROUND_INTENSITY);
                    cout << 'O';
                } else if (find(body.begin(), body.end(), p) != body.end()) {
                    SetConsoleTextAttribute(hOut, FOREGROUND_RED|FOREGROUND_INTENSITY);
                    cout << 'o';
                } else if (p == food) {
                    SetConsoleTextAttribute(hOut, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
                    cout << '*';
                } else {
                    cout << ' ';
                }
                SetConsoleTextAttribute(hOut, border);
            }
            cout << "#\n";
        }
        cout << string(WIDTH,'#') << "\n";
        staminaSeconds = chrono::duration<float>(Clock::now() - gameStart).count();
        SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Your score: " << score << "    ";
        SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "stamina: " << fixed << setprecision(2) << staminaSeconds << " sec\n";
        SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << GAME_NAME << " created by " << CREATOR << "\n";
        SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "Use WASD for control\n";
    }
    void run() {
        while (alive) {
            auto frame = Clock::now();
            processInput();
            update();
            render();
            int used = (int)chrono::duration_cast<chrono::milliseconds>(Clock::now() - frame).count();
            this_thread::sleep_for(chrono::milliseconds(max(0, delay - used)));
        }
        staminaSeconds = chrono::duration<float>(Clock::now() - gameStart).count();
        showGameOver();
    }
    void showGameOver() {
        WORD gray = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
        for (int i = 0; i < 10; ++i) {
            SetConsoleCursorPosition(hOut,{0,0});
            SetConsoleTextAttribute(hOut, gray);
            for (int y = 0; y < HEIGHT; ++y) cout << string(WIDTH,'.') << "\n";
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        this_thread::sleep_for(chrono::milliseconds(200));
        system("cls");
        WORD border = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
        WORD textCol = FOREGROUND_RED|FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(hOut, border);
        cout << string(WIDTH,'#') << "\n";
        printBoxLine("GAME OVER!", textCol);
        SetConsoleTextAttribute(hOut, border);
        cout << string(WIDTH,'#') << "\n";
        printBoxLine("", border);
        ostringstream oss;
        oss << fixed << setprecision(2) << staminaSeconds;
        string scoreStr = "YOUR SCORE: " + to_string(score);
        string staminaStr = "STAMINA: " + oss.str() + " sec";
        int contentLen = (int)(scoreStr.length() + 4 + staminaStr.length());
        int pad = WIDTH - 2 - contentLen;
        int left = pad / 2;
        int right = pad - left;
        SetConsoleTextAttribute(hOut, border);
        cout << "#";
        cout << string(left, ' ');
        SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << scoreStr;
        cout << "    ";
        SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << staminaStr;
        SetConsoleTextAttribute(hOut, border);
        cout << string(right, ' ') << "#\n";
        printBoxLine("", border);
        printBoxLine("BETTER LUCK NEXT TIME", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printBoxLine(string("THANKS FOR PLAYING ") + GAME_NAME, border);
        printBoxLine("", border);
        printBoxLine("Press any key to continue", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        SetConsoleTextAttribute(hOut, border);
        cout << string(WIDTH,'#') << "\n";
        _getch();
    }
};

int main() {
    Snake game;
    game.start();
    return 0;
}
