#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 20
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    int x, y;
} Vector2Int;

typedef struct {
    Vector2Int body[100];
    int size;
    char direction[6];
    int score;
} Snake;

Vector2Int fruit;
Music gameMusic;

// Initialisation du jeu
void InitGame(Snake *snake) {
    snake->size = 1;
    snake->body[0].x = SCREEN_WIDTH / 2 / GRID_SIZE;
    snake->body[0].y = SCREEN_HEIGHT / 2 / GRID_SIZE;
    strcpy(snake->direction, "RIGHT");
    snake->score = 0;

    // Placement aléatoire du fruit
    fruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE);
    fruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE);
}

// Déplacement du serpent
void MoveSnake(Snake *snake) {
    for (int i = snake->size; i > 0; i--)
        snake->body[i] = snake->body[i - 1];

    if (strcmp(snake->direction, "UP") == 0) snake->body[0].y--;
    if (strcmp(snake->direction, "DOWN") == 0) snake->body[0].y++;
    if (strcmp(snake->direction, "LEFT") == 0) snake->body[0].x--;
    if (strcmp(snake->direction, "RIGHT") == 0) snake->body[0].x++;

    // Vérification si le serpent mange un fruit
    if (snake->body[0].x == fruit.x && snake->body[0].y == fruit.y) {
        snake->size++;
        snake->score++;  // Augmentation du score

        fruit.x = rand() % (SCREEN_WIDTH / GRID_SIZE);
        fruit.y = rand() % (SCREEN_HEIGHT / GRID_SIZE);
    }
}

// Vérification des collisions
int CheckCollision(Snake *snake) {
    if (snake->body[0].x < 0 || snake->body[0].x >= SCREEN_WIDTH / GRID_SIZE ||
        snake->body[0].y < 0 || snake->body[0].y >= SCREEN_HEIGHT / GRID_SIZE)
        return 1;

    for (int i = 1; i < snake->size; i++)
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y)
            return 1;

    return 0;
}

// Boucle principale du jeu
int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    InitAudioDevice();  // Activation du son

    gameMusic = LoadMusicStream("game_music.wav");  // Charger la musique
    PlayMusicStream(gameMusic);  // Jouer la musique

    SetTargetFPS(10);

    Snake snake;
    InitGame(&snake);

    while (!WindowShouldClose()) {
        UpdateMusicStream(gameMusic);  // Mise à jour de la musique

        // Gestion des touches
        if (IsKeyPressed(KEY_UP) && strcmp(snake.direction, "DOWN") != 0) strcpy(snake.direction, "UP");
        if (IsKeyPressed(KEY_DOWN) && strcmp(snake.direction, "UP") != 0) strcpy(snake.direction, "DOWN");
        if (IsKeyPressed(KEY_LEFT) && strcmp(snake.direction, "RIGHT") != 0) strcpy(snake.direction, "LEFT");
        if (IsKeyPressed(KEY_RIGHT) && strcmp(snake.direction, "LEFT") != 0) strcpy(snake.direction, "RIGHT");

        MoveSnake(&snake);

        if (CheckCollision(&snake)) break; // Fin du jeu

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner le serpent
        for (int i = 0; i < snake.size; i++)
            DrawRectangle(snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, DARKGREEN);

        // Dessiner le fruit
        DrawRectangle(fruit.x * GRID_SIZE, fruit.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);

        // Afficher le score
        DrawText(TextFormat("Score: %d", snake.score), 10, 10, 20, BLACK);

        EndDrawing();
    }

    UnloadMusicStream(gameMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
