#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

typedef enum
{
    LEFT = 0,
    DOWN = 1,
    UP = 2,
    RIGHT = 3
} Direction;

constexpr int MAX_X = 7;
constexpr int MAX_Y = 12;

// X: Left -> Up, Right -> Down
// Y: Left -> Down, Right -> Up
constexpr int MirrorTypeX = 1;
constexpr int MirrorTypeY = 3;

int p1_goalpost = 0;
int p2_goalpost = 2;

int mirror[MAX_Y][MAX_X];
int laser[MAX_Y][MAX_X];

Direction mirror_direction(int x, int y, Direction type)
{
    if (mirror[x][y] == MirrorTypeX)
    {
        if (type == LEFT) return UP;
        else if (type == UP) return LEFT;
        else if (type == RIGHT) return DOWN;
        else if (type == DOWN) return RIGHT;
    }
    if (mirror[x][y] == MirrorTypeY)
    {
        if (type == LEFT) return DOWN;
        else if (type == DOWN) return LEFT;
        else if (type == RIGHT) return UP;
        else if (type == UP) return RIGHT;
    }
}

pair<int, int> dfs(int x, int y, Direction type)
{
    if (x <= -1 || x >= MAX_Y)
    {
        if (x == -1) return {x + 1, y};
        return {x - 1, y};
    }
    if (y <= -1 || y >= MAX_X) return {-1, -1}; // 왼쪽 오른쪽 모서리에 도달

    laser[x][y] = 1;
    if (mirror[x][y]) type = mirror_direction(x, y, type); //거울에 도달했을 경우 방향 전환

    if (type == LEFT) return dfs(x, y - 1, type);
    else if (type == RIGHT) return dfs(x, y + 1, type);
    else if (type == DOWN) return dfs(x + 1, y, type);
    else if (type == UP) return dfs(x - 1, y, type);
    return {0, 0};
}


void init()
{
    cout << "Please Input Data (x, y, mirror_type)\n";
    for (int i = 0; i < MAX_Y; ++i) laser[i][MAX_X / 2] = 1;
}

// x: 레이저의 x 좌표, *p: 승리한 사용자를 구분하기 위한 포인터 변수
bool goalin(pair<int, int> coordinate, int *p)
{
    if (coordinate.first == 0 && coordinate.second == p1_goalpost)
    {
        *p = 2;
        return true;
    }
    if (coordinate.first == MAX_Y - 1 && coordinate.second == p2_goalpost)
    {
        *p = 1;
        return true;
    }
    *p = -1;
    return false;
}

int encode_input(char mirror_type)
{
    if (mirror_type == '\\') return 1;
    else if (mirror_type == '/') return 3;
}

// x: 거울을 설치할 x 좌표, 거울을 설치할 y 좌표
void input(int x, int y, char mirror_type)
{
    mirror[x][y] = encode_input(mirror_type);
}

void pprint()
{
    cout << "<< laser >>\n";
    for (auto &i: laser)
    {
        for (int j: i) cout << j << ' ';
        cout << '\n';
    }
    cout << "<< Mirror >>\n";
    for (auto &i: mirror)
    {
        for (int j: i) cout << j << ' ';
        cout << '\n';
    }
}

int main()
{
    init();
    string cmd;
    int x, y, flag = 1;
    char mirror_type;
    while (true)
    {
        int player;
        pprint();
        printf("플레이어%d 공격 턴: (install/update/break): ", flag);
        cin >> cmd;
        if (cmd == "install")
        {
            cin >> x >> y >> mirror_type;
            input(x, y, mirror_type);
        }
        else if (cmd == "update")
        {
            cin >> x >> y;
            if (mirror[y][x] == MirrorTypeX) input(x, y, MirrorTypeY);
            else input(x, y, MirrorTypeX);
        }
        else if (cmd == "break") break;
        flag = flag == 1 ? 2 : 1;
        for (auto &i: laser) for (int &j: i) j = 0;
        if (goalin(dfs(0, MAX_X / 2, DOWN), &player))
        {
            cout << "플레이어" << player << "이 승리했습니다!";
            break;
        }
    }
}