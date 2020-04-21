#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <string>

#define H (40 + 2)
#define W (80 + 2)

int map[W][H];
bool full = 0, diagonal = 0;

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
    if (map[x - 1][y] == z - 1)
    {
        map[x - 1][y] = -4;
        back_check(z - 1, x - 1, y);
        return 1;
    }
    if (map[x + 1][y] == z - 1)
    {
        map[x + 1][y] = -4;
        back_check(z - 1, x + 1, y);
        return 1;
    }
    if (map[x][y - 1] == z - 1)
    {
        map[x][y - 1] = -4;
        back_check(z - 1, x, y - 1);
        return 1;
    }
    if (map[x][y + 1] == z - 1)
    {
        map[x][y + 1] = -4;
        back_check(z - 1, x, y + 1);
        return 1;
    }
    if (diagonal)
    {
        if (map[x - 1][y - 1] == z - 1)
        {
            map[x - 1][y - 1] = -4;
            back_check(z - 1, x - 1, y - 1);
            return 1;
        }
        if (map[x - 1][y + 1] == z - 1)
        {
            map[x - 1][y] = -4;
            back_check(z - 1, x - 1, y + 1);
            return 1;
        }
        if (map[x + 1][y - 1] == z - 1)
        {
            map[x + 1][y - 1] = -4;
            back_check(z - 1, x + 1, y - 1);
            return 1;
        }
        if (map[x + 1][y + 1] == z - 1)
        {
            map[x + 1][y + 1] = -4;
            back_check(z - 1, x + 1, y + 1);
            return 1;
        }
    }
};


int check(int x1, int y1, int x2, int y2)
{
    std::list<MyStruct> stek;
    bool da = 1;
    stek.push_back(MyStruct(x1, y1, 0));
    while (stek.size() != 0)
    {
        auto i = *stek.begin();
        stek.pop_front();
        if (i.x == x2 && i.y == y2 && da)
        {
            da = 0;
            back_check(i.z, x2, y2);
            if (!full)
            {
                return 0;
            }
        }
        if (map[i.x - 1][i.y] == -1 || ((i.x - 1 == x2 && i.y == y2) && da))
        {
            stek.push_back(MyStruct(i.x - 1, i.y, i.z + 1));
            map[i.x - 1][i.y] = i.z + 1;
        }
        if (map[i.x + 1][i.y] == -1 || ((i.x + 1 == x2 && i.y == y2) && da))
        {
            stek.push_back(MyStruct(i.x + 1, i.y, i.z + 1));
            map[i.x + 1][i.y] = i.z + 1;
        }
        if (map[i.x][i.y - 1] == -1 || ((i.x == x2 && i.y - 1 == y2) && da))
        {
            stek.push_back(MyStruct(i.x, i.y - 1, i.z + 1));
            map[i.x][i.y - 1] = i.z + 1;
        }
        if (map[i.x][i.y + 1] == -1 || ((i.x == x2 && i.y + 1 == y2) && da))
        {
            stek.push_back(MyStruct(i.x, i.y + 1, i.z + 1));
            map[i.x][i.y + 1] = i.z + 1;
        }
        if (diagonal)
        {
            if (map[i.x - 1][i.y - 1] == -1 || ((i.x - 1 == x2 && i.y - 1 == y2) && da))
            {
                stek.push_back(MyStruct(i.x - 1, i.y - 1, i.z + 1));
                map[i.x - 1][i.y - 1] = i.z + 1;
            }
            if (map[i.x - 1][i.y + 1] == -1 || ((i.x - 1 == x2 && i.y + 1 == y2) && da))
            {
                stek.push_back(MyStruct(i.x - 1, i.y + 1, i.z + 1));
                map[i.x - 1][i.y + 1] = i.z + 1;
            } 
            if (map[i.x + 1][i.y - 1] == -1 || ((i.x + 1 == x2 && i.y - 1 == y2) && da))
            {
                stek.push_back(MyStruct(i.x + 1, i.y - 1, i.z + 1));
                map[i.x + 1][i.y - 1] = i.z + 1;
            }
            if (map[i.x + 1][i.y + 1] == -1 || ((i.x  + 1 == x2 && i.y + 1 == y2) && da))
            {
                stek.push_back(MyStruct(i.x + 1, i.y + 1, i.z + 1));
                map[i.x + 1][i.y + 1] = i.z + 1;
            }
        }
    }
    return 0;
};

int main()
{
    RenderWindow window(VideoMode((W - 2) * 20 - 1, (H - 2) * 20 - 1), "");
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            if (x == 0 || x == W - 1 || y == 0 || y == H - 1)
                map[x][y] = -2;
            else
                map[x][y] = -1;
    int x1 = 5, y1 = 10, x2 = 15, y2 = 10;
    bool debug = 0;
    RectangleShape rectangle(Vector2f(19, 19));
    map[x1][y1] = 0;
    map[x2][y2] = -5;
    Font font;
    font.loadFromFile("font.ttf");
    Text text("", font, 12);
    text.setFillColor(Color::Black);
    while (window.isOpen())
    {
        auto pos = window.mapPixelToCoords(Mouse::getPosition(window));
        int X = pos.x / 20 + 1;
        int Y = pos.y / 20 + 1;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::LAlt)
                    full = !full;
                if (event.key.code == Keyboard::LControl)
                    debug = !debug;
                if (event.key.code == Keyboard::Enter)
                    diagonal = !diagonal;
            }
        }
        if (X > 0 && X < W - 1 && Y > 0 && Y < H - 1)
        {
            if (!Keyboard::isKeyPressed(Keyboard::LShift))
            {
                if (!(X == x1 && Y == y1) && !(X == x2 && Y == y2))
                {
                    if (Mouse::isButtonPressed(Mouse::Left))
                        map[X][Y] = -2;
                    else if (Mouse::isButtonPressed(Mouse::Right))
                        map[X][Y] = -1;
                }
            }
            else
            {
                if (Mouse::isButtonPressed(Mouse::Left) && !(X == x2 && Y == y2))
                {
                    map[x1][y1] = -1;
                    x1 = X;
                    y1 = Y;
                    map[x1][y1] = 0;
                }
                else if (Mouse::isButtonPressed(Mouse::Right) && !(X == x1 && Y == y1))
                {
                    map[x2][y2] = -1;
                    x2 = X;
                    y2 = Y;
                    map[x2][y2] = -5;
                }
            }
        }
        for (int y = 1; y < H - 1; y++)
            for (int x = 1; x < W - 1; x++)
                if (map[x][y] > 0 || map[x][y] == -4)
                    map[x][y] = -1;
        map[x1][y1] = 0;
        check(x1, y1, x2, y2);
        window.clear(Color::White);
        for (int y = 1; y < H - 1; y++)
            for (int x = 1; x < W - 1; x++)
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
                case -5:
                    rectangle.setFillColor(Color(0, 0, 255));
                    break;
                case -4:
                    rectangle.setFillColor(Color(255, 0, 0));
                    break;
                default:
                    rectangle.setFillColor(Color(180, 180, 180));
                    break;
                }
                if (x == x1 && y == y1)
                    rectangle.setFillColor(Color(0, 255, 0));
                if (x == x2 && y == y2)
                    rectangle.setFillColor(Color(0, 0, 255));
                rectangle.setPosition((x - 1) * 20, (y - 1) * 20);
                window.draw(rectangle);
                if (debug)
                {
                    text.setString(std::to_string(map[x][y]));
                    text.setPosition((x - 1) * 20, (y - 1) * 20);
                    window.draw(text);
                }
            }
        window.display();
    }
}