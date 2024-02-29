#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int N = 21, M = 21;

int blockSize = 32; // Adjust the size of each block
int w = blockSize * N;
int h = blockSize * N;

int direction = 0;
int num = 1;

bool paused = false; // Variable to track if the game is paused or not

struct Pacman 
{
    int x,y;
}s[100];

struct Fruit
{
    int x,y;
}f;

bool foodMatrix[21][21];
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

// This function copies values from mazeMatrix to  
// foodMatrix and sets edge of the matrix to zero
void copyMatrixAndModify(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
           foodMatrix[i][j] = mazeMatrix[i][j]; 
        }
    }
    for(int i = 0; i < N; i++){
        foodMatrix[i][0] = 0;
        foodMatrix[i][N-1] = 0; 
    }

    for(int i = 0; i < N; i++){
        foodMatrix[0][i] = 0;
        foodMatrix[N-1][i] = 0; 
    }
}

void Tick()
{
    // Only update the pacman if the game is not paused
    if (!paused) 
    { 
        if(direction == 0 && mazeMatrix[s[0].y + 1][s[0].x]) s[0].y += 1;
        if(direction == 1 && mazeMatrix[s[0].y][s[0].x - 1]) s[0].x -= 1;
        if(direction == 2 && mazeMatrix[s[0].y][s[0].x + 1]) s[0].x += 1;
        if(direction == 3 && mazeMatrix[s[0].y - 1][s[0].x]) s[0].y -= 1;

        if(s[0].x >= N-1) s[0].x = 1; 
        if(s[0].x < 1)    s[0].x = N-1;
        if(s[0].y >= N-1) s[0].y = 1;
        if(s[0].y < 1)   s[0].y = N-1;
    }
}

void initializeFruits(std::vector<Fruit>& fruits) {
    copyMatrixAndModify();
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < M; ++j) {
            if (foodMatrix[j][i]) {
                fruits.push_back({i, j});
            }
        }
    }

}

void removeEatenFruit(std::vector<Fruit>& fruits, int x, int y) {
    auto it = std::remove_if(fruits.begin(), fruits.end(), [x, y](const Fruit& fruit) {
        return fruit.x == x && fruit.y == y;
    });
    fruits.erase(it, fruits.end());
}

int main()
{
    std::vector<Fruit> fruits;
    initializeFruits(fruits);
    
    int number_of_fruits_on_map = number_of_fruits_on_map = fruits.size();
    printf("Total number of fruits : %d\n", number_of_fruits_on_map);

    RenderWindow window(VideoMode(w,h),"Pacman Game");

    CircleShape fruitShape(blockSize / 4.5); // Create a circle shape for the fruit

    Texture pacmanTexture, you_wonTexture;    
    
    if (!pacmanTexture.loadFromFile("images/pacman.png") and !you_wonTexture.loadFromFile("images/you_won.png")) {
        return EXIT_FAILURE;
    }

    if (!you_wonTexture.loadFromFile("images/you_won.png")) {
        return EXIT_FAILURE;
    }

    Sprite pacmanSprite(pacmanTexture);
    Sprite you_wonSprite(you_wonTexture);

    RectangleShape block(Vector2f(blockSize, blockSize)); // Create a rectangle shape for grid and pacman
    RectangleShape line_vertical(Vector2f(1.f, h)); // Create a vertical line
    RectangleShape line_horisontal(Vector2f(w, 1.f)); // Create a horizontal line
    
    Color gridColor = Color(0, 0, 139); // Color for the grid
    Color fruitColor = Color::White; // Color for the fruit

    fruitShape.setFillColor(fruitColor);
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
        Event e;
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        if(num == 20)
        {
            window.clear();
            you_wonSprite.setPosition(w / 2 - you_wonSprite.getLocalBounds().width / 2, h / 2 - you_wonSprite.getLocalBounds().height / 2);
            window.draw(you_wonSprite);

            if(e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }
            window.display();
        }
        
        else
        {
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
            
            if(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))  direction = 1;
            if(Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) direction = 2;
            if(Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))    direction = 3;
            if(Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))  direction = 0;

            if(timer > delay)
            {
                timer = 0;
                Tick();

                // Check if pacman meets fruit
                for (const auto& fruit : fruits) 
                {
                    if (s[0].x == fruit.x && s[0].y == fruit.y) 
                    {
                        removeEatenFruit(fruits, fruit.x, fruit.y);
                        num++; // Increment fruit count
                    }
                }
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
            pacmanSprite.setPosition(s[0].x * blockSize, s[0].y * blockSize);
            window.draw(pacmanSprite);

            // Draw remaining fruits
            for (const Fruit& fruit : fruits)  
            {
                float fruitPosX = fruit.x * blockSize + (blockSize - fruitShape.getRadius() * 2) / 2;
                float fruitPosY = fruit.y * blockSize + (blockSize - fruitShape.getRadius() * 2) / 2;
                fruitShape.setPosition(fruitPosX, fruitPosY);
                window.draw(fruitShape);
            }

            if (paused)
            {
                window.draw(pausedText);
            }
            window.display();
        }
            
        
    }
    
    return 0;
}

