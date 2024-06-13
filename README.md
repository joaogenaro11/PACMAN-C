Esse é um jogo simples inspirado no Pac-Man, programado em C. No jogo, o jogador controla o Pac-Man que deve coletar todos os pellets (pontinhos) enquanto evita ser pego pelos fantasmas.

Principais Componentes do Código
Inclusão de Bibliotecas:

c
Copy code
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <stdbool.h>
stdio.h: Para operações de entrada e saída.
stdlib.h: Para funções utilitárias como rand() e exit().
unistd.h: Para funções de controle do sistema, como usleep().
termios.h: Para modificar o comportamento do terminal.
time.h: Para operações relacionadas ao tempo, como srand().
stdbool.h: Para usar o tipo de dado booleano (true/false).
Definição do Tamanho do Jogo:

c
Copy code
#define WIDTH 20
#define HEIGHT 10
Define a largura e altura do campo de jogo.

Estruturas de Dados:

c
Copy code
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
Entity: Representa uma entidade no jogo, como Pac-Man ou fantasmas, com posições x e y.
GameState: Armazena o estado do jogo, incluindo a pontuação, vidas, posição do Pac-Man, posição dos fantasmas e a matriz de pellets.
Função init_game:

c
Copy code
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
Inicializa o estado do jogo: configura a pontuação, vidas, posições do Pac-Man e fantasmas, e distribui pellets pelo campo.

Função draw_game:

c
Copy code
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
        printf("\\n");
    }
    printf("Score: %d  Lives: %d\\n", game->score, game->lives);
}
Desenha o campo de jogo no terminal. P representa o Pac-Man, G os fantasmas, . os pellets, e espaços vazios.

Função move_entity:

c
Copy code
void move_entity(Entity *entity, int dx, int dy) {
    entity->x += dx;
    entity->y += dy;
    if (entity->x < 0) entity->x = WIDTH - 1;
    if (entity->y < 0) entity->y = HEIGHT - 1;
    if (entity->x >= WIDTH) entity->x = 0;
    if (entity->y >= HEIGHT) entity->y = 0;
}
Move uma entidade (Pac-Man ou fantasma) pelo campo de jogo. A entidade aparece do outro lado se sair da borda.

Função update_game:

c
Copy code
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
        printf("\\a");  // Toca som ao marcar pontos
    }
    for (int i = 0; i < 2; i++) {
        if (game->pacman.x == game->ghosts[i].x && game->pacman.y == game->ghosts[i].y) {
            game->lives--;
            if (game->lives == 0) {
                printf("Game Over! Final Score: %d\\n", game->score);
                exit(0);
            }
            game->pacman.x = WIDTH / 2;
            game->pacman.y = HEIGHT / 2;
        }
    }
}
Atualiza o estado do jogo baseado na entrada do usuário e movimentos dos fantasmas. Verifica colisões e atualiza pontuação.

Função disable_buffering:

c
Copy code
void disable_buffering() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ICANON;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
Desabilita o buffering do terminal para que o jogo receba entradas de teclado instantaneamente sem precisar pressionar Enter.

Função main:

c
Copy code
int main() {
    srand(time(NULL));
    disable_buffering();
    GameState game;
    init_game(&game);
    while (true) {
        draw_game(&game);
        update_game(&game);
        usleep(100000);  // Delay para controlar a velocidade do jogo
    }
    return 0;
}
Função principal que inicializa o jogo, desabilita o buffering do terminal e entra no loop principal do jogo, desenhando e atualizando continuamente.

Explicação do Jogo
Objetivo: Coletar todos os pellets (.) para aumentar a pontuação, enquanto evita os fantasmas (G).
Controles: Use as teclas w (cima), a (esquerda), s (baixo) e d (direita) para mover o Pac-Man (P).
Fim de Jogo: O jogo termina quando todas as vidas se esgotam e exibe a pontuação final.
