#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 100
#define BALL_SIZE 10

typedef struct {
    float x, y;
    float w, h;
} Rect;

void updateTitle(SDL_Window *window, int leftScore, int rightScore) {
    char title[100];
    sprintf(title, "Pong | Player: %d  AI: %d", leftScore, rightScore);
    SDL_SetWindowTitle(window, title);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Rect leftPaddle = {20, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    Rect rightPaddle = {WINDOW_WIDTH - 30, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
    Rect ball = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, BALL_SIZE, BALL_SIZE};

    float ballVelX = 5.0f;
    float ballVelY = 3.0f;

    int leftScore = 0;
    int rightScore = 0;

    bool running = true;
    SDL_Event event;

    updateTitle(window, leftScore, rightScore);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        // Player paddle (W/S)
        if (keys[SDL_SCANCODE_W] && leftPaddle.y > 0)
            leftPaddle.y -= 6;
        if (keys[SDL_SCANCODE_S] && leftPaddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
            leftPaddle.y += 6;

        // --- AI paddle ---
        float paddleCenter = rightPaddle.y + PADDLE_HEIGHT / 2;

        if (ball.y < paddleCenter - 10)
            rightPaddle.y -= 5;
        else if (ball.y > paddleCenter + 10)
            rightPaddle.y += 5;

        // Clamp AI paddle
        if (rightPaddle.y < 0) rightPaddle.y = 0;
        if (rightPaddle.y + PADDLE_HEIGHT > WINDOW_HEIGHT)
            rightPaddle.y = WINDOW_HEIGHT - PADDLE_HEIGHT;

        // Move ball
        ball.x += ballVelX;
        ball.y += ballVelY;

        // Top/bottom collision
        if (ball.y <= 0 || ball.y + BALL_SIZE >= WINDOW_HEIGHT)
            ballVelY = -ballVelY;

        // Paddle collision (left)
        if (ball.x <= leftPaddle.x + leftPaddle.w &&
            ball.y < leftPaddle.y + leftPaddle.h &&
            ball.y + BALL_SIZE > leftPaddle.y) {

            ballVelX = -ballVelX;

            // Add angle based on hit position
            float offset = (ball.y - (leftPaddle.y + PADDLE_HEIGHT / 2)) / (PADDLE_HEIGHT / 2);
            ballVelY = offset * 5;
        }

        // Paddle collision (right)
        if (ball.x + BALL_SIZE >= rightPaddle.x &&
            ball.y < rightPaddle.y + rightPaddle.h &&
            ball.y + BALL_SIZE > rightPaddle.y) {

            ballVelX = -ballVelX;

            float offset = (ball.y - (rightPaddle.y + PADDLE_HEIGHT / 2)) / (PADDLE_HEIGHT / 2);
            ballVelY = offset * 5;
        }

        // Scoring
        if (ball.x < 0) {
            rightScore++;
            ball.x = WINDOW_WIDTH / 2;
            ball.y = WINDOW_HEIGHT / 2;
            ballVelX = 5.0f;
            ballVelY = 3.0f;
            updateTitle(window, leftScore, rightScore);
            SDL_Delay(500);
        }

        if (ball.x > WINDOW_WIDTH) {
            leftScore++;
            ball.x = WINDOW_WIDTH / 2;
            ball.y = WINDOW_HEIGHT / 2;
            ballVelX = -5.0f;
            ballVelY = 3.0f;
            updateTitle(window, leftScore, rightScore);
            SDL_Delay(500);
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect lp = {leftPaddle.x, leftPaddle.y, leftPaddle.w, leftPaddle.h};
        SDL_Rect rp = {rightPaddle.x, rightPaddle.y, rightPaddle.w, rightPaddle.h};
        SDL_Rect b = {ball.x, ball.y, ball.w, ball.h};

        SDL_RenderFillRect(renderer, &lp);
        SDL_RenderFillRect(renderer, &rp);
        SDL_RenderFillRect(renderer, &b);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
