#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

#define W_HEIGHT 480
#define W_WIDTH 600

const int BALL_SIZE = 10;

SDL_Window *window;
SDL_Renderer *renderer;

typedef struct Ball
{
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} Ball;

Ball b;

bool Init(void);
void Update(float);
void Quit(void);
Ball MakeBall(int);
bool CoinFlip(void);
void RenderBall(Ball*);
void UpdateBall(Ball*, float);

int main(int arc, char *argv[])
{
    atexit(Quit);
    srand((unsigned int)time(NULL));
    bool loop = true;
    SDL_Event event;

    if (!Init())
    {
        exit(1);
        printf("Could not init game !");
    }
    Uint32 lasttick = SDL_GetTicks();
    while (loop)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                loop = false;
                Quit();
            }
        }

        Uint32 currentTick = SDL_GetTicks();
        Uint32 diff = currentTick - lasttick;
        float elapsed = diff / 1000.0f;
        Update(elapsed);
        lasttick = currentTick;
    }

    Quit();
    return 0;
}

bool Init()
{
    //init sdl
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
    {
        printf("Could not init SDL !");
        return false;
    }

    //create an sdl window
    window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        W_WIDTH,
        W_HEIGHT,
        SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        printf("Could not create window !");
        return false;
    }

    //create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("Could not create renderer !");
        return false;
    }

    b = MakeBall(BALL_SIZE);

    return true;
}

void Update(float elapsed)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    UpdateBall(&b, elapsed);
    RenderBall(&b);
    SDL_RenderPresent(renderer);
}

void Quit()
{
    if (window != NULL)
        SDL_DestroyWindow(window);
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

Ball MakeBall(int size)
{
    const float SPEED = 120;
    Ball ball = {
        .x = W_WIDTH / 2 - size / 2,
        .y = W_HEIGHT / 2 - size / 2,
        .size = size,
        .xSpeed = SPEED * (CoinFlip() ? 1 : -1),
        .ySpeed = SPEED * (CoinFlip() ? 1 : -1)
    };
    return ball;
}

bool CoinFlip()
{
    return rand() % 2 == 0 ? true : false;
}

void RenderBall(Ball *ball) 
{
    SDL_Rect ballRect = {
        .x = ball->x,
        .y = ball->y,
        .h = ball->size,
        .w = ball->size
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &ballRect);
}

void UpdateBall(Ball *ball, float elapsed)
{
    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;

    if(ball->x < 0) {
        ball->xSpeed = fabs(ball->xSpeed);
    }

    if(ball->x > W_WIDTH - BALL_SIZE) {
        ball->xSpeed = -1 * fabs(ball->xSpeed); 
    }

    if(ball->y < 0) {
        ball->ySpeed = fabs(ball->ySpeed); 
    }

    if(ball->y > W_HEIGHT - BALL_SIZE) {
        ball->ySpeed = -1 * fabs(ball->ySpeed); 
    }
}