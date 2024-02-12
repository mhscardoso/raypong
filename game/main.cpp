#include <raylib.h>
#include <stdint.h>
#include <stdio.h>

#define WIDTH      800
#define HEIGHT     600
#define BALL_VEL_X 450
#define BALL_VEL_Y 300

const uint8_t CenterLineWidth = 4;

// The ball definition
struct Ball
{
    Vector2 position;
    Vector2 velocity_factor;
    Color   color;
    float   radius;
};

// Kinds of player
enum PlayerType
{
    GAMER = 0x1,
    ENEMY = 0x2,
};

// Player definition
struct Player
{
    Rectangle  rectangle;
    PlayerType type;
    Color      color;
    int        velocity_y;
};

/* GAME VARIABLES ----------------------------------------- */

// Counter for points
int PlayerPoints = 0;
int EnemyPoints  = 0;

Ball ball = {
    .position        = { .x = WIDTH / 2 , .y = HEIGHT / 2 },
    .velocity_factor = { .x = BALL_VEL_X, .y = BALL_VEL_Y },
    .color           = WHITE,
    .radius          = 15.0f
};

// GAMER
Player player = {
    .rectangle   = { .x = 20.0f, .y = (HEIGHT - 100) / 2.0f,
                     .width = 10.0f, .height = 100.0f      },
    .type       = GAMER,
    .color      = WHITE,
    .velocity_y = 300
};

// ENEMY
Player enemy = {
    .rectangle   = { .x = WIDTH - 30.0f, .y = (HEIGHT - 100) / 2.0f,
                     .width = 10.0f, .height = 100.0f         },
    .type       = ENEMY,
    .color      = WHITE,
    .velocity_y = 240
};
/* GAME VARIABLES ----------------------------------------- */

/*
    0 - game continue
    1 - player lost
    2 - enemy lost
*/
static void MoveBall(Ball *ball, float elasticFactor)
{
    ball->position.x += ball->velocity_factor.x * GetFrameTime();
    ball->position.y += ball->velocity_factor.y * GetFrameTime();

    float top = ball->position.y - ball->radius;
    float bottom = ball->position.y + ball->radius;

    int width = GetScreenWidth();
    int height = GetScreenHeight();
    
    // Ball exit on left, player lost
    if (ball->position.x < 0) {
        ball->position.x = width / 2;
        ball->position.y = height / 2;

        ball->velocity_factor.x = -BALL_VEL_X;
        ball->velocity_factor.y = BALL_VEL_Y;

        player.rectangle.y = height / 2 - player.rectangle.height / 2;
        enemy.rectangle.y = height / 2 - enemy.rectangle.height / 2;

        EnemyPoints++;
    }

    // Ball exit on right, enemy lost
    if (ball->position.x > width) {
        ball->position.x = width / 2;
        ball->position.y = height / 2;

        ball->velocity_factor.x = BALL_VEL_X;
        ball->velocity_factor.y = BALL_VEL_Y;

        player.rectangle.y = height / 2 - player.rectangle.height / 2;
        enemy.rectangle.y = height / 2 - enemy.rectangle.height / 2;

        PlayerPoints++;
    }

    if (top <= 0) {
        ball->velocity_factor.y *= (-1) * elasticFactor;
        ball->position.y = ball->radius;
    }

    if (bottom >= height) {
        ball->velocity_factor.y *= (-1) * elasticFactor;
        ball->position.y = height - ball->radius;
    }
}


static void DrawBall(Ball *ball)
{
    DrawCircle(
        ball->position.x,
        ball->position.y,
        ball->radius,
        ball->color
    );
}

static void MovePlayer(Player *player)
{
    if (IsKeyDown(KEY_K) && player->rectangle.y >= 0) {
        player->rectangle.y -= (GetFrameTime() * (player->velocity_y));
    }

    if (IsKeyDown(KEY_J) && (player->rectangle.y + player->rectangle.height) <= GetScreenHeight()) {
        player->rectangle.y += (GetFrameTime() * (player->velocity_y));
    }
}


static void MoveEnemy(Player *enemy, Ball *ball)
{
    if (enemy->rectangle.y + (enemy->rectangle.height / 2) > ball->position.y &&
        enemy->rectangle.y >= 0
    )
    {
        enemy->rectangle.y -= (GetFrameTime() * (enemy->velocity_y));
    }
    else if (enemy->rectangle.y + (enemy->rectangle.height / 2) < ball->position.y &&
        enemy->rectangle.y + enemy->rectangle.height <= GetScreenHeight()
    )
    {
        enemy->rectangle.y += (GetFrameTime() * (enemy->velocity_y));
    }
}


static void Collide(Player *player, Ball *ball)
{
    if (CheckCollisionCircleRec(ball->position, ball->radius, player->rectangle)) {
        ball->velocity_factor.x *= (-1);
    }
}


static void DrawPlayer(Player *player)
{
    DrawRectangle(
        player->rectangle.x,
        player->rectangle.y,
        player->rectangle.width,
        player->rectangle.height,
        player->color
    );
}

/* ================================================ 
   MAIN
   ================================================ */

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Hello, from Raylib");
    SetTargetFPS(120);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Center Line
        DrawRectangle((WIDTH - CenterLineWidth) / 2, 
                        0, CenterLineWidth, HEIGHT, WHITE);

        DrawBall(&ball);
        MoveBall(&ball, 1);

        DrawPlayer(&player);
        MovePlayer(&player);

        DrawPlayer(&enemy);
        MoveEnemy(&enemy, &ball);

        Collide(&player, &ball);
        Collide(&enemy, &ball);

        DrawText(TextFormat("%i", EnemyPoints), WIDTH / 2 + 10, 10, 40, WHITE);
        DrawText(TextFormat("%i", PlayerPoints), WIDTH / 2 - 30, 10, 40, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}