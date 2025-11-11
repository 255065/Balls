#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 400
#define TITLE "Balls and their admirers"
#define BALL_COUNT 100
#define FPS 60
#define VEL_MAX 5
#define RADIUS_MAX 20
#define RADIUS_MIN 5

Color COLORS[] = { LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK,
                   RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE,
                   DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN };

typedef struct Ball { 
    int posx, posy;
    int velx, vely;
    int radius;
    Color color;
    struct Ball *follows;
} Ball;

Ball balls[BALL_COUNT];

Ball *init_ball_random(Ball *p) {
    p->posx = rand() % WIDTH;
    p->posy = rand() % HEIGHT;
    p->velx = rand() % (2 * VEL_MAX + 1) - VEL_MAX;
    p->vely = rand() % (2 * VEL_MAX + 1) - VEL_MAX;
    p->radius = rand() % (RADIUS_MAX - RADIUS_MIN + 1) + RADIUS_MIN;
    p->color = COLORS[rand() % (sizeof(COLORS)/sizeof(COLORS[0]))];

    int my_index = p - balls;
    int leader_index;
    do {
        leader_index = rand() % BALL_COUNT;
    } while (leader_index == my_index);
    p->follows = &balls[leader_index];

    return p;
}

void init_balls_random() {
    for (int i = 0; i < BALL_COUNT; i++)
        init_ball_random(&balls[i]);
}

Ball *draw_ball(Ball *p) {
    DrawCircle(p->posx, p->posy, p->radius, p->color);
    return p;
}

Ball *update_pos(Ball *p) {
    p->posx = (WIDTH + p->posx + p->velx) % WIDTH;
    p->posy = (HEIGHT + p->posy + p->vely) % HEIGHT;
    return p;
}

Ball *update_vel_for_following(Ball *p) {
    int errx = p->follows->posx - p->posx;
    int erry = p->follows->posy - p->posy;
    p->velx = errx > 0 ? 1 : -1;
    p->vely = erry > 0 ? 1 : -1;
    return p;
}

void update_elements() {
    for (int i = 0; i < BALL_COUNT; i++)
        draw_ball(update_pos(update_vel_for_following(&balls[i])));
}

int main() {
    srand(time(NULL));
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    init_balls_random();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        update_elements();
        EndDrawing();
    }
}
