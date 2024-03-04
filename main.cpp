#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;


int N = 21, blockSize = 32; 
int w = blockSize * N, h = blockSize * N; // Adjust the size of each block

int direction = 0, howManyFruitsPacmanHasEaten = 0;

bool allowMove = false, paused = false; 

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

struct Pacman 
{
    int x,y;
}pacman;

typedef struct
{
    int x,y;
} Fruit;

struct Ghost
{
    int x, y;
    int direction;
}ghost;

Ghost ghost1, ghost2;

// This function copies values from mazeMatrix to  
// foodMatrix and sets edge of the matrix to zero
void copyMatrixAndModify()
{
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

void movePacman() {
    if(allowMove){
        if(direction == 0 && mazeMatrix[pacman.y + 1][pacman.x]) pacman.y += 1;
        if(direction == 1 && mazeMatrix[pacman.y][pacman.x - 1]) pacman.x -= 1;
        if(direction == 2 && mazeMatrix[pacman.y][pacman.x + 1]) pacman.x += 1;
        if(direction == 3 && mazeMatrix[pacman.y - 1][pacman.x]) pacman.y -= 1;

        if(pacman.x >= N-1) pacman.x = 1; 
        if(pacman.x < 1)    pacman.x = N-1;
        if(pacman.y >= N-1) pacman.y = 1;
        if(pacman.y < 1)    pacman.y = N-1;
    }
}

void moveGhost(Ghost& ghost) {
    // Ghost movement
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    int nextX = ghost.x + dx[ghost.direction];
    int nextY = ghost.y + dy[ghost.direction];

    if (mazeMatrix[nextY][nextX]) 
    {
        ghost.x = nextX;
        ghost.y = nextY;
    } 
    else 
    {
        //srand(time(NULL));
        ghost.direction = rand() % 4; // Change direction randomly
    }

    if(ghost.x >= N-1) ghost.x = 1; 
    if(ghost.x < 1)    ghost.x = N-1;
    if(ghost.y >= N-1) ghost.y = 1;
    if(ghost.y < 1)    ghost.y = N-1;
}

void Tick() {
    // Only update the pacman and ghost if the game is not paused
    if (!paused) { 
        movePacman();
        moveGhost(ghost1);
        moveGhost(ghost2);
    }
}

void initializeFruits(std::vector<Fruit>& fruits) 
{
    copyMatrixAndModify();
    
    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if (foodMatrix[j][i]) {
                fruits.push_back({i, j});
            }
        }
    }

}

void removeEatenFruit(std::vector<Fruit>& fruits, int x, int y) 
{
    auto it = std::remove_if(fruits.begin(), fruits.end(), [x, y](const Fruit& fruit) {
        return fruit.x == x && fruit.y == y;
    });
    fruits.erase(it, fruits.end());
}

void displayImageAndText(RenderWindow& window, Sprite& game_overSprite, Text& instructionToExitGame, int score) {
    window.clear();
    game_overSprite.setPosition(window.getSize().x / 2 - game_overSprite.getLocalBounds().width / 2, 
                                 window.getSize().y / 2 - game_overSprite.getLocalBounds().height / 2);
    window.draw(game_overSprite);
    window.draw(instructionToExitGame);

    Event e;
    while (window.pollEvent(e)) {
        if (e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
            printf("Number of eaten fruit : %d\n", score);
        }
    }
    window.display();
}

int main()
{
    RenderWindow window(VideoMode(w,h+2*blockSize),"Pacman Game");

    std::vector<Fruit> fruits;
    initializeFruits(fruits);
    
    int numberOfFruitsOnMap = fruits.size();
    printf("Total number of fruits : %d\n", numberOfFruitsOnMap);

    RectangleShape block(Vector2f(blockSize, blockSize)); // Create a rectangle shape for grid and pacman
    RectangleShape line_vertical(Vector2f(1.f, h));       // Create a vertical line
    RectangleShape line_horisontal(Vector2f(w, 1.f));     // Create a horizontal line
    CircleShape fruitShape(blockSize / 4.5);              // Create a circle shape for the fruit

    fruitShape.setFillColor(Color::White);
    line_vertical.setFillColor(Color::Black);
    line_horisontal.setFillColor(Color::Black);

    Texture pacmanTexture, you_wonTexture, ghost_redTexture,ghost_blueTexture, game_overTexture;    
    
    if (!pacmanTexture.loadFromFile("images/pacman.png")  || 
        !you_wonTexture.loadFromFile("images/you_won.png") || 
        !ghost_redTexture.loadFromFile("images/ghost_red.png") ||
        !ghost_blueTexture.loadFromFile("images/ghost_blue.png") ||
        !game_overTexture.loadFromFile("images/game_over.png")) 
    {
        return EXIT_FAILURE;
    }

    Sprite pacmanSprite(pacmanTexture);
    Sprite you_wonSprite(you_wonTexture);
    Sprite game_overSprite(game_overTexture);
    Sprite ghost_redSprite(ghost_redTexture);
    Sprite ghost_blueSprite(ghost_blueTexture);

    Font font;
    if (!font.loadFromFile("font/Arial.ttf")) {
        return EXIT_FAILURE;
    }

    //This text is meant for printing text when 'space' button is pressed
    Text pausedText("Paused", font, 64);
    pausedText.setFillColor(Color::Yellow);
    pausedText.setStyle(Text::Bold);
    pausedText.setPosition(w / 2 - pausedText.getLocalBounds().width / 2, h / 2 - pausedText.getLocalBounds().height / 2);

    //This text is meant for printing how many fruit pacman has eaten
    Text currentScoreText;
    currentScoreText.setFont(font);
    currentScoreText.setCharacterSize(24);
    currentScoreText.setFillColor(Color::White);
    currentScoreText.setPosition(w / 2 - pausedText.getLocalBounds().width / 2 + blockSize, h + blockSize);

    //This text is meant for printing text when game is over to tell you how to exit window
    Text instructionToExitGame("-> Press Esc to exit <-", font, 48);
    instructionToExitGame.setFillColor(Color::Cyan);
    instructionToExitGame.setStyle(Text::Bold);
    instructionToExitGame.setPosition(w / 2 - instructionToExitGame.getLocalBounds().width / 2, h);

    Clock clock;
    float timer = 0, delay = 0.1;

    pacman.x = 1;
    pacman.y = 1;
    
    ghost1.x = 13;
    ghost1.y = 1;
    
    ghost2.x = 2;
    ghost2.y = 5;
    
    srand(time(NULL));
    ghost1.direction = rand() % 4; // Change direction randomly
    ghost2.direction = rand() % 4; // Change direction randomly

    while(window.isOpen())
    {
        Event e;
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        if(howManyFruitsPacmanHasEaten == 20)
        //if(howManyFruitsPacmanHasEaten == numberOfFruitsOnMap)
        {
            displayImageAndText(window, you_wonSprite, instructionToExitGame, howManyFruitsPacmanHasEaten);
        }
        else if(pacman.x == ghost1.x && pacman.y == ghost1.y || pacman.x == ghost2.x && pacman.y == ghost2.y)
        {
            displayImageAndText(window, game_overSprite, instructionToExitGame, howManyFruitsPacmanHasEaten);
        }
        
        else
        {
           while(window.pollEvent(e))
            {
                if(e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    window.close();
                    printf("Number of fruit : %d\n", howManyFruitsPacmanHasEaten);
                }
                if(e.type == Event::KeyPressed && e.key.code == Keyboard::Space) {
                    paused = !paused; // Toggle pause state when space is pressed
                }
            }
            
            if(Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))  {allowMove = true; direction = 1;}
            if(Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {allowMove = true; direction = 2;}
            if(Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))    {allowMove = true; direction = 3;}
            if(Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))  {allowMove = true; direction = 0;}

            if(timer > delay)
            {
                timer = 0;
                Tick();

                // Check if pacman meets fruit
                for (const auto& fruit : fruits) 
                {
                    if (pacman.x == fruit.x && pacman.y == fruit.y) 
                    {
                        removeEatenFruit(fruits, fruit.x, fruit.y);
                        howManyFruitsPacmanHasEaten++; // Increment fruit count
                    }
                }
            }

            window.clear();

            // Draw grid
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
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
            for(int j = 1; j < N; j++) {
                line_horisontal.setPosition(0, j * blockSize);
                window.draw(line_horisontal);
            }

            // Draw pacman
            pacmanSprite.setPosition(pacman.x * blockSize, pacman.y * blockSize);
            window.draw(pacmanSprite);

            // Draw ghost
            ghost_redSprite.setPosition(ghost1.x * blockSize, ghost1.y * blockSize);
            window.draw(ghost_redSprite);

            ghost_blueSprite.setPosition(ghost2.x * blockSize, ghost2.y * blockSize);
            window.draw(ghost_blueSprite);

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

            std::string fruit_eaten = std::to_string(howManyFruitsPacmanHasEaten);
            currentScoreText.setString("Score : " + fruit_eaten);
            
            window.draw(currentScoreText);
            window.display();
        }   
    }
    return 0;
}
