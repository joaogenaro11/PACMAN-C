#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 10

typedef struct {
    int x, y;
} Entity;

typedef struct {
    int score;
    int lives;
    Entity pacman;
    Entity ghosts[2];
    bool pellets[WIDTH][HEIGHT];
} GameState;

void init_game(GameState *game) {
    game->score = 0;
    game->lives = 3;
    game->pacman.x = WIDTH / 2;
    game->pacman.y = HEIGHT / 2;
    game->ghosts[0].x = 1;
    game->ghosts[0].y = 1;
    game->ghosts[1].x = WIDTH - 2;
    game->ghosts[1].y = HEIGHT - 2;
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            game->pellets[i][j] = true;
        }
    }
}

void draw_game(GameState *game) {
    system("clear");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == game->pacman.x && y == game->pacman.y) {
                printf("P");
            } else if (x == game->ghosts[0].x && y == game->ghosts[0].y) {
                printf("G");
            } else if (x == game->ghosts[1].x && y == game->ghosts[1].y) {
                printf("G");
            } else if (game->pellets[x][y]) {
                printf(".");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("Score: %d  Lives: %d\n", game->score, game->lives);
}

void move_entity(Entity *entity, int dx, int dy) {
    entity->x += dx;
    entity->y += dy;
    if (entity->x < 0) entity->x = WIDTH - 1;
    if (entity->y < 0) entity->y = HEIGHT - 1;
    if (entity->x >= WIDTH) entity->x = 0;
    if (entity->y >= HEIGHT) entity->y = 0;
}

void update_game(GameState *game) {
    int dx = 0, dy = 0;
    char input = getchar();
    switch (input) {
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        default: break;
    }
    move_entity(&game->pacman, dx, dy);
    for (int i = 0; i < 2; i++) {
        int ghost_dx = (rand() % 3) - 1;
        int ghost_dy = (rand() % 3) - 1;
        move_entity(&game->ghosts[i], ghost_dx, ghost_dy);
    }
    if (game->pellets[game->pacman.x][game->pacman.y]) {
        game->score++;
        game->pellets[game->pacman.x][game->pacman.y] = false;
        printf("\a");  // Play sound on scoring
    }
    for (int i = 0; i < 2; i++) {
        if (game->pacman.x == game->ghosts[i].x && game->pacman.y == game->ghosts[i].y) {
            game->lives--;
            if (game->lives == 0) {
                printf("Game Over! Final Score: %d\n", game->score);
                exit(0);
            }
            game->pacman.x = WIDTH / 2;
            game->pacman.y = HEIGHT / 2;
        }
    }
}

void disable_buffering() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int main() {
    srand(time(NULL));
    disable_buffering();
    GameState game;
    init_game(&game);
    while (true) {
        draw_game(&game);
        update_game(&game);
        usleep(100000);  // Delay to control game speed
    }
    return 0;
}
