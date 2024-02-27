#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int N = 21, M = 21;

int blockSize = 32; // Adjust the size of each block
int w = blockSize * N;
int h = blockSize * N;

int dir = 0, num=1;

bool paused = false; // Variable to track if the game is paused or not

struct Pacman 
{
    int x,y;
}s[100];

struct Fruit
{
    int x,y;
}f;

const bool mazeMatrix[21][21] = {
        {0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0},
        {0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0},
        {0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0},
        {0,1,0,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0},
        {0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0},
        {0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0},
        {0,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0},
        {0,1,0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0},
        {0,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0},
        {0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0},
        {1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1},
        {0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0},
        {0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0},
        {0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0},
        {0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0},
        {1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1},
        {0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0}
        };

void Tick()
{
        // Only update the pacman if the game is not paused
    if (!paused) 
    { 
        if(dir == 0 && mazeMatrix[s[0].y + 1][s[0].x]) s[0].y += 1;
        if(dir == 1 && mazeMatrix[s[0].y][s[0].x - 1]) s[0].x -= 1;
        if(dir == 2 && mazeMatrix[s[0].y][s[0].x + 1]) s[0].x += 1;
        if(dir == 3 && mazeMatrix[s[0].y - 1][s[0].x]) s[0].y -= 1;

        if((s[0].x == f.x) && (s[0].y == f.y))
        {
            num++;
            
            // Generate random position for the fruit until it is in a valid location
            do {
                f.x = rand() % N;
                f.y = rand() % N;
            } while (!mazeMatrix[f.y][f.x]); // Repeat until fruit position is valid
        }


        if(s[0].x > N-2) s[0].x = 1; 
        if(s[0].x < 1)   s[0].x = N-2;
        if(s[0].y > N-2) s[0].y = 1;
        if(s[0].y < 1)   s[0].y = N-1;
    }
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(w,h),"Pacman Game");

    CircleShape fruitShape(blockSize / 2); // Create a circle shape for the fruit
    CircleShape pacman(blockSize / 2); // Create a circle shape for the fruit

    RectangleShape block(Vector2f(blockSize, blockSize)); // Create a rectangle shape for grid and pacman
    RectangleShape line_vertical(Vector2f(1.f, h)); // Create a vertical line
    RectangleShape line_horisontal(Vector2f(w, 1.f)); // Create a horizontal line
    
    Color gridColor = Color(0, 0, 139); // Color for the grid
    Color pacmanColor = Color::Yellow; // Color for the pacman
    Color fruitColor = Color::Blue; // Color for the fruit

    fruitShape.setFillColor(fruitColor);
    pacman.setFillColor(fruitColor);
    line_vertical.setFillColor(Color::Black);
    line_horisontal.setFillColor(Color::Black);

    Font font;
    if (!font.loadFromFile("font/Arial.ttf")) {
        return EXIT_FAILURE;
    }

    Text pausedText("Paused", font, 50);
    pausedText.setFillColor(Color::White);
    pausedText.setStyle(Text::Bold);
    pausedText.setPosition(w / 2 - pausedText.getLocalBounds().width / 2, h / 2 - pausedText.getLocalBounds().height / 2);

    Clock clock;
    float timer = 0;
    float delay = 0.1;
    f.x = 11;
    f.y = 11;
    s[0].x = 1;
    s[0].y = 1;

    while(window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        Event e;
        
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
                printf("Number of fruit : %d\n",num-1);
            }
            if(e.type == Event::KeyPressed && e.key.code == Keyboard::Space) {
                paused = !paused; // Toggle pause state when space is pressed
            }
        }
        
        if(Keyboard::isKeyPressed(Keyboard::A)) dir = 1;
        if(Keyboard::isKeyPressed(Keyboard::D)) dir = 2;
        if(Keyboard::isKeyPressed(Keyboard::W)) dir = 3;
        if(Keyboard::isKeyPressed(Keyboard::S)) dir = 0;

        if(timer > delay)
        {
            timer = 0;
            Tick();
        }

        window.clear();

        // Draw grid
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++) {
                block.setPosition(i * blockSize, j * blockSize);
                if (mazeMatrix[j][i]) {
                    block.setFillColor(Color::Black); // Set color to black if matrix value is one
                } else {
                    block.setFillColor(Color(0, 0, 139)); // Set color to dark blue if matrix value is zero
                }
                window.draw(block);
            }
        }

        // Draw vertical lines
        for(int i = 1; i < N; i++) {
            line_vertical.setPosition(i * blockSize, 0);
            window.draw(line_vertical);
        }

        // Draw horizontal lines
        for(int j = 1; j < M; j++) {
            line_horisontal.setPosition(0, j * blockSize);
            window.draw(line_horisontal);
        }

        // Draw pacman
        pacman.setPosition(s[0].x * blockSize, s[0].y * blockSize);
        pacman.setFillColor(pacmanColor);
        window.draw(pacman);

        // Draw fruit
        float fruitPosX = f.x * blockSize + (blockSize - fruitShape.getRadius() * 2) / 2;
        float fruitPosY = f.y * blockSize + (blockSize - fruitShape.getRadius() * 2) / 2;
        fruitShape.setPosition(fruitPosX, fruitPosY);
        window.draw(fruitShape);

        if (paused)
        {
            window.draw(pausedText);
        }

        window.display();
    }
    return 0;
}