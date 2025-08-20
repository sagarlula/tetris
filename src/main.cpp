#include <common.hpp>

const int M = 20;
const int N = 10;

int field[M][N] = {{0}};

struct Point
{
        int x, y;
} a[4], b[4];

int figures[7][4] =
{
        {1, 3, 5, 7}, // I
        {2, 4, 5, 7}, // Z
        {3, 5, 4, 6}, // S
        {3, 5, 4, 7}, // T
        {2, 3, 5, 7}, // L
        {3, 5, 7, 6}, // J
        {2, 3, 4, 5}, // O
};

bool check()
{
        for (int i = 0; i < 4; i++)
                if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
                        return 0;
                else if (field[a[i].y][a[i].x])
                        return 0;

        return 1;
};

int main()
{
        srand(time(0));

        sf::RenderWindow window(sf::VideoMode({320, 480}), "Tetris");

        sf::Texture t1, t2, t3;
        [[maybe_unused]] bool okt1 = t1.loadFromFile(std::format("{}/images/tiles.png", ASSETS_PATH));
        [[maybe_unused]] bool okt2 = t2.loadFromFile(std::format("{}/images/background.png", ASSETS_PATH));
        [[maybe_unused]] bool okt3 = t3.loadFromFile(std::format("{}/images/frame.png", ASSETS_PATH));

        sf::Sprite s(t1), background(t2), frame(t3);

        int dx = 0;
        bool rotate = 0;
        int colorNum = 1;
        float timer = 0, delay = 0.3;

        sf::Clock clock;

        while (window.isOpen())
        {
                float time = clock.getElapsedTime().asSeconds();
                clock.restart();
                timer += time;

                while (const std::optional event = window.pollEvent())
                {
                        if (event->is<sf::Event::Closed>())
                                window.close();

                        if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                        {
                                if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
                                {
                                        rotate = true;
                                }
                                else if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
                                {
                                        dx = -1;
                                }
                                else if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
                                {
                                        dx = 1;
                                }
                        }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
                        delay = 0.05;

                //// <- Move -> ///
                for (int i = 0; i < 4; i++)
                {
                        b[i] = a[i];
                        a[i].x += dx;
                }
                if (!check())
                        for (int i = 0; i < 4; i++)
                                a[i] = b[i];

                //////Rotate//////
                if (rotate)
                {
                        Point p = a[1]; // center of rotation
                        for (int i = 0; i < 4; i++)
                        {
                                int x = a[i].y - p.y;
                                int y = a[i].x - p.x;
                                a[i].x = p.x - x;
                                a[i].y = p.y + y;
                        }
                        if (!check())
                                for (int i = 0; i < 4; i++)
                                        a[i] = b[i];
                }

                ///////Tick//////
                if (timer > delay)
                {
                        for (int i = 0; i < 4; i++)
                        {
                                b[i] = a[i];
                                a[i].y += 1;
                        }

                        if (!check())
                        {
                                for (int i = 0; i < 4; i++)
                                        field[b[i].y][b[i].x] = colorNum;

                                colorNum = 1 + rand() % 7;
                                int n = rand() % 7;
                                for (int i = 0; i < 4; i++)
                                {
                                        a[i].x = figures[n][i] % 2;
                                        a[i].y = figures[n][i] / 2;
                                }
                        }

                        timer = 0;
                }

                ///////check lines//////////
                int k = M - 1;
                for (int i = M - 1; i > 0; i--)
                {
                        int count = 0;
                        for (int j = 0; j < N; j++)
                        {
                                if (field[i][j])
                                        count++;
                                field[k][j] = field[i][j];
                        }
                        if (count < N)
                                k--;
                }

                dx = 0;
                rotate = 0;
                delay = 0.3;

                /////////draw//////////
                window.clear(sf::Color::White);
                window.draw(background);

                for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                        {
                                if (field[i][j] == 0)
                                        continue;
                                s.setTextureRect(sf::IntRect({field[i][j] * 18, 0}, {18, 18}));
                                s.setPosition({static_cast<float>(j * 18), static_cast<float>(i * 18)});
                                s.move({28, 31}); // offset
                                window.draw(s);
                        }

                for (int i = 0; i < 4; i++)
                {
                        s.setTextureRect(sf::IntRect({colorNum * 18, 0}, {18, 18}));
                        s.setPosition({static_cast<float>(a[i].x * 18), static_cast<float>(a[i].y * 18)});
                        s.move({28, 31}); // offset
                        window.draw(s);
                }

                window.draw(frame);
                window.display();
        }

        return 0;
}
