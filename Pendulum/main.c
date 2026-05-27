#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>

// GAME INIT CONSTANTS
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define TITLE "DOUBLE PENDULUM"
#define TARGETFPS 30

#pragma region pendulum defines
// STARTING PENDULUM CONSTANTS
#define L1 100
#define L2 100
#define M1 1.0
#define M2 2.0
#define START_THETA1 2.0
#define START_THETA2 -1
#define START_SPEED 0

// WORLD CONSTANTS
#define G 9.8
#define MASS_RADIUS 8
#define SCREEN_MIDDLE (Vector2){SCREEN_WIDTH / 2, 0}
#pragma endregion

#define PAUSEBUTTONSTART (Vector2){10, 10}
#define PAUSEBUTTONSIZE (Vector2){70, 20}

#pragma region pendulum functions

// PENDULUM STRUCTURE
typedef struct
{
    Vector2 startPos;
    float length;
    float theta;
    float speed;
    float mass;
} Pendulum;

// GETS THE ENDPOINT VECTOR OF A PENDULUM
Vector2 get_endpoint(Pendulum *pendulum)
{
    Vector2 endPoint;
    endPoint.x = pendulum->startPos.x + sinf(pendulum->theta) * pendulum->length;
    endPoint.y = pendulum->startPos.y + cosf(pendulum->theta) * pendulum->length;
    return endPoint;
}

// DRAWS A PENDULUM
void draw_pendulum(Pendulum pendulum)
{
    Vector2 endPoint = get_endpoint(&pendulum);
    DrawLineV(pendulum.startPos, endPoint, RAYWHITE);
    DrawCircleV(get_endpoint(&pendulum), MASS_RADIUS, RED);
}

// HANDLES PHYSICS CALCULATIONS FOR A DOUBLE PENDULUM
void step_pendulum(Pendulum *pendulum1, Pendulum *pendulum2)
{

    pendulum1->speed += (-G * sinf(pendulum1->theta) * (2 * pendulum1->mass + pendulum2->mass) - pendulum2->mass * G * sinf(pendulum1->theta - 2 * pendulum2->theta) - 2 * sinf(pendulum1->theta - pendulum2->theta) * pendulum2->mass * (pendulum2->speed * pendulum2->speed * pendulum2->length + pendulum1->speed * pendulum1->speed * pendulum1->length * cosf(pendulum1->theta - pendulum2->theta))) / (pendulum1->length * (2 * pendulum1->mass + pendulum2->mass - pendulum2->mass * cosf(2 * pendulum1->theta - 2 * pendulum2->theta)));
    pendulum2->speed += (2 * sinf(pendulum1->theta - pendulum2->theta) * (pendulum1->speed * pendulum1->speed * pendulum1->length * (pendulum1->mass + pendulum2->mass) + G * cosf(pendulum1->theta) * (pendulum1->mass + pendulum2->mass) + pendulum2->speed * pendulum2->speed * pendulum2->length * pendulum2->mass * cosf(pendulum1->theta - pendulum2->theta))) / (pendulum2->length * (2 * pendulum1->mass + pendulum2->mass - pendulum2->mass * cosf(2 * pendulum1->theta - 2 * pendulum2->theta)));

    pendulum1->theta += pendulum1->speed;
    pendulum2->theta += pendulum2->speed;
    pendulum2->startPos = get_endpoint(pendulum1);
}
#pragma endregion

typedef struct
{
    bool active;
    char *text;
    Rectangle rectangle;
} Button;

bool check_button_howered(Button button)
{
    Vector2 mouse = GetMousePosition();

    if (mouse.x <= button.rectangle.x)
        return false;
    else if (mouse.x >= button.rectangle.x + button.rectangle.width)
        return false;
    else if (mouse.y <= button.rectangle.y)
        return false;
    else if (mouse.y >= button.rectangle.y + button.rectangle.height)
        return false;
    return true;
}

void draw_button(Button button)
{
    Color textColor = button.active ? BLACK : WHITE;
    Color bgColor = button.active ? DARKBLUE : BLUE;
    float offset = button.rectangle.height / 20;
    DrawRectangleRec(button.rectangle, bgColor);
    DrawText(button.text, button.rectangle.x + offset, button.rectangle.y + offset, button.rectangle.width / 8, textColor);
}

void draw_texts(Pendulum pendulum1, Pendulum pendulum2)
{
    char text[64];
    sprintf(text, "Length 2: %.0f cm", pendulum2.length);
    DrawText(text, 10, SCREEN_HEIGHT - 20, 10, WHITE);

    sprintf(text, "Length 1: %.0f cm", pendulum1.length);
    DrawText(text, 10, SCREEN_HEIGHT - 40, 10, WHITE);

    sprintf(text, "Mass 2: %.0f kg", pendulum2.mass);
    DrawText(text, 110, SCREEN_HEIGHT - 20, 10, WHITE);

    sprintf(text, "Mass 1: %.0f kg", pendulum1.mass);
    DrawText(text, 110, SCREEN_HEIGHT - 40, 10, WHITE);

    sprintf(text, "Theta 2: %.3f rad", pendulum2.theta);
    DrawText(text, 190, SCREEN_HEIGHT - 20, 10, WHITE);

    sprintf(text, "Theta 1: %.3f rad", pendulum1.theta);
    DrawText(text, 190, SCREEN_HEIGHT - 40, 10, WHITE);

    sprintf(text, "Angular velocity 2: %.3f 1/rad", pendulum2.speed);
    DrawText(text, 320, SCREEN_HEIGHT - 20, 10, WHITE);

    sprintf(text, "Angular velocity 1: %.3f 1/rad", pendulum1.speed);
    DrawText(text, 320, SCREEN_HEIGHT - 40, 10, WHITE);
}

int main()
{
    bool running = true;
    bool paused = false;

    Pendulum pendulum1 = (Pendulum){SCREEN_MIDDLE, L1, START_THETA1, START_SPEED, M1};
    Pendulum pendulum2 = (Pendulum){get_endpoint(&pendulum1), L2, START_THETA2, START_SPEED, M2};

    Rectangle pause_button_rectangle = {PAUSEBUTTONSTART.x, PAUSEBUTTONSTART.y, PAUSEBUTTONSIZE.x, PAUSEBUTTONSIZE.y};
    Button pause_button = {false, "PAUSE", pause_button_rectangle};

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    SetTargetFPS(TARGETFPS);
    SetExitKey(0);

    while (running)
    {
        running = !WindowShouldClose();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (check_button_howered(pause_button))
            {
                paused = !paused;
                pause_button.active = paused;
            }
        }
        if (!paused)
        {
            step_pendulum(&pendulum1, &pendulum2);
        };
        BeginDrawing();
        ClearBackground(DARKGRAY);
        draw_button(pause_button);
        draw_pendulum(pendulum2);
        draw_pendulum(pendulum1);
        draw_texts(pendulum1, pendulum2);
        EndDrawing();
    }
}