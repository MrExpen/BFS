#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>

#define H 40
#define W 80
int map[W][H];

using namespace sf;

struct MyStruct
{
public:
    int x;
    int y;
    int z;
    MyStruct(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

int back_check(int z, int x, int y)
{
    if (z == 0)
        return 0;
    if (x > 0)
        if (map[x - 1][y] == z - 1)
        {
            map[x - 1][y] = -4;
            back_check(z - 1, x - 1, y);
            return 1;
        }
    if (x < W - 1)
        if (map[x + 1][y] == z - 1)
        {
            map[x + 1][y] = -4;
            back_check(z - 1, x + 1, y);
            return 1;
        }
    if (y > 0)
        if (map[x][y - 1] == z - 1)
        {
            map[x][y - 1] = -4;
            back_check(z - 1, x, y - 1);
            return 1;
        }
    if (y < H - 1)
        if (map[x][y + 1] == z - 1)
        {
            map[x][y + 1] = -4;
            back_check(z - 1, x, y + 1);
            return 1;
        }

};


int check(int x1, int y1, int x2, int y2)
{
    std::list<MyStruct> stek;
    stek.push_back(MyStruct(x1, y1, 0));
    while (stek.size() != 0)
    {
        auto i = *stek.begin();
        stek.pop_front();
        if (i.x == x2 && i.y == y2)
        {
            back_check(i.z, x2, y2);
            return 0;
        }
        if (i.x > 0)
            if (map[i.x - 1][i.y] == -1 || (i.x - 1 == x2 && i.y == y2))
            {
                stek.push_back(MyStruct(i.x - 1, i.y, i.z + 1));
                map[i.x - 1][i.y] = i.z + 1;
            }
        if (i.x < W - 1)
            if (map[i.x + 1][i.y] == -1 || (i.x + 1 == x2 && i.y == y2))
            {
                stek.push_back(MyStruct(i.x + 1, i.y, i.z + 1));
                map[i.x + 1][i.y] = i.z + 1;
            }
        if (i.y > 0)
            if (map[i.x][i.y - 1] == -1 || (i.x == x2 && i.y - 1 == y2))
            {
                stek.push_back(MyStruct(i.x, i.y - 1, i.z + 1));
                map[i.x][i.y - 1] = i.z + 1;
            }
        if (i.y < H - 1)
            if (map[i.x][i.y + 1] == -1 || (i.x == x2 && i.y + 1 == y2))
            {
                stek.push_back(MyStruct(i.x, i.y + 1, i.z + 1));
                map[i.x][i.y + 1] = i.z + 1;
            }
    }
};

int main()
{
    RenderWindow window(VideoMode(W * 20 - 1, H * 20  - 1), "");
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            map[x][y] = -1;
    int x1 = 5, y1 = 10, x2 = 15, y2 = 10;
    RectangleShape rectangle(Vector2f(19, 19));
    map[x1][y1] = 0;
    map[x2][y2] = -3;
    while (window.isOpen())
    {
        auto pos = window.mapPixelToCoords(Mouse::getPosition(window));
        int X = pos.x / 20;
        int Y = pos.y / 20;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if (X >= 0 && X < W && Y >= 0 && Y < H)
        {
            if (!Keyboard::isKeyPressed(Keyboard::LShift))
            {
                if (!(X == x1 && Y == y1) && !(X == x2 && Y == y2))
                {
                    if (Mouse::isButtonPressed(Mouse::Left))
                        map[int(pos.x) / 20][int(pos.y) / 20] = -2;
                    else if (Mouse::isButtonPressed(Mouse::Right))
                        map[int(pos.x) / 20][int(pos.y) / 20] = -1;
                }
            }
            else
            {
                if (Mouse::isButtonPressed(Mouse::Left) && !(X == x2 && Y == y2))
                {
                    map[x1][y1] = -1;
                    x1 = pos.x / 20;
                    y1 = pos.y / 20;
                    map[x1][y1] = 0;
                }
                else if (Mouse::isButtonPressed(Mouse::Right) && !(X == x1 && Y == y1))
                {
                    map[x2][y2] = -1;
                    x2 = pos.x / 20;
                    y2 = pos.y / 20;
                    map[x2][y2] = -3;
                }
            }
        }
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++)
                if (map[x][y] > 0 || map[x][y] == -4)
                    map[x][y] = -1;
        check(x1, y1, x2, y2);
        window.clear(Color::White);
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++)
            {
                switch (map[x][y])
                {
                case -1:
                    rectangle.setFillColor(Color(150, 150, 150));
                    break;
                case -2:
                    rectangle.setFillColor(Color(0, 0, 0));
                    break;
                case 0:
                    rectangle.setFillColor(Color(0, 255, 0));
                    break;
                case -3:
                    rectangle.setFillColor(Color(0, 0, 255));
                    break;
                case -4:
                    rectangle.setFillColor(Color(255, 0, 0));
                    break;
                default:
                    rectangle.setFillColor(Color(170, 170, 170));
                    break;
                }
                if (x == x1 && y == y1)
                    rectangle.setFillColor(Color(0, 255, 0));
                if (x == x2 && y == y2)
                    rectangle.setFillColor(Color(0, 0, 255));
                rectangle.setPosition(x * 20, y * 20);
                window.draw(rectangle);
            }
        window.display();
    }
}