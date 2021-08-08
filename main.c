#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

#define W_HEIGHT 480
#define W_WIDTH 600

const int BALL_SIZE = 10;
const int PLAYER_HEIGHT = 100;
const int PLAYER_WIDTH = 10;
const int PLAYER_MARGIN = 10;
const float PLAYER_MOVE_SPEED = 400;
const float BALL_SPEED = 200;
bool served = false;

SDL_Window *window;
SDL_Renderer *renderer;
TTF_Font *font;
SDL_Texture *label;

typedef struct Ball
{
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} Ball;

Ball b;

typedef struct Player
{
    int score;
    float yPosition;
} Player;

Player playerA, playerB;

bool Init(void);
void Update(float);
void Quit(void);
bool CoinFlip(void);

Ball MakeBall(int);
void RenderBall(Ball*);
void UpdateBall(Ball*, float);

Player MakePlayer(void);
void UpdatePlayers(float);
void RenderPlayers();

bool TextInit(void);
void UpdateScore(int, int);
void RenderScore(void);

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

    if(!TextInit())
    {
        printf("Could not init ttf");
        exit(1);
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
    playerA = MakePlayer();
    playerB = MakePlayer();

    return true;
}

bool TextInit() 
{
    if(TTF_Init() == 0) {
        return true;
    } else {
        return false;
    }
}

void Update(float elapsed)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    UpdateBall(&b, elapsed);
    RenderBall(&b);

    UpdatePlayers(elapsed);
    RenderPlayers();
    RenderScore();

    SDL_RenderPresent(renderer);
}

void Quit()
{
    if (window != NULL)
        SDL_DestroyWindow(window);
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    SDL_Quit();

    SDL_DestroyTexture(label);
    TTF_CloseFont(font);
    TTF_Quit();
}

Ball MakeBall(int size)
{
    const float SPEED = BALL_SPEED;
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
    SDL_RenderFillRect(renderer, &ballRect);
}

void UpdateBall(Ball *ball, float elapsed)
{
    if(!served) {

        ball->x = W_WIDTH/2 - BALL_SIZE/2;
        ball->y = W_HEIGHT/2 - BALL_SIZE/2;
        return;
    }
    ball->x += ball->xSpeed * elapsed;
    ball->y += ball->ySpeed * elapsed;

    if(ball->x < 0) {
        // ball->xSpeed = fabs(ball->xSpeed);
        UpdateScore(2, 100);
        served = false;
    }

    if(ball->x > W_WIDTH - BALL_SIZE) {
        // ball->xSpeed = -1 * fabs(ball->xSpeed); 
        UpdateScore(1, 100);
        served = false;
    }

    if(ball->y < 0) {
        ball->ySpeed = fabs(ball->ySpeed); 
    }

    if(ball->y > W_HEIGHT - BALL_SIZE) {
        ball->ySpeed = -1 * fabs(ball->ySpeed); 
    }
}

Player MakePlayer(void)
{
    Player player = {
        .score = 0,
        .yPosition = W_HEIGHT / 2
    };

    return player;
}

void UpdatePlayers(float elapsed)
{
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    if(keyboardState[SDL_SCANCODE_SPACE]) {
        served = true;
    }

    if(keyboardState[SDL_SCANCODE_W]) {
        playerA.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }

    if(keyboardState[SDL_SCANCODE_S]) {
        playerA.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }

    if(keyboardState[SDL_SCANCODE_UP]) {
        playerB.yPosition -= PLAYER_MOVE_SPEED * elapsed;
    }

    if(keyboardState[SDL_SCANCODE_DOWN]) {
        playerB.yPosition += PLAYER_MOVE_SPEED * elapsed;
    }

    if(playerA.yPosition > W_HEIGHT - PLAYER_HEIGHT/2) {
        playerA.yPosition = W_HEIGHT - PLAYER_HEIGHT/2;
    }

    if(playerB.yPosition > W_HEIGHT - PLAYER_HEIGHT/2) {
        playerB.yPosition = W_HEIGHT - PLAYER_HEIGHT/2;
    }


    if(playerA.yPosition < PLAYER_HEIGHT/2) {
        playerA.yPosition = PLAYER_HEIGHT/2;
    }

    if(playerB.yPosition < PLAYER_HEIGHT/2) {
        playerB.yPosition =  PLAYER_HEIGHT/2;
    }

    SDL_Rect player_a = {
        .x = PLAYER_MARGIN,
        .y = playerA.yPosition - PLAYER_HEIGHT / 2,
        .h = PLAYER_HEIGHT,
        .w = PLAYER_WIDTH
    };
    
    SDL_Rect player_b = {
        .x = W_WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = playerB.yPosition - PLAYER_HEIGHT / 2,
        .h = PLAYER_HEIGHT,
        .w = PLAYER_WIDTH
    };

    SDL_Rect ballRect = {
        .x = b.x,
        .y = b.y,
        .h = b.size,
        .w = b.size
    };

    if(SDL_HasIntersection(&player_a, &ballRect)) {
        b.xSpeed = fabs(b.xSpeed);
    }

    if(SDL_HasIntersection(&player_b, &ballRect)) {
        b.xSpeed = - fabs(b.xSpeed);
    }

}

void RenderPlayers()
{
    SDL_Rect player_a = {
        .x = PLAYER_MARGIN,
        .y = playerA.yPosition - PLAYER_HEIGHT / 2,
        .h = PLAYER_HEIGHT,
        .w = PLAYER_WIDTH
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player_a);

    SDL_Rect player_b = {
        .x = W_WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = playerB.yPosition - PLAYER_HEIGHT / 2,
        .h = PLAYER_HEIGHT,
        .w = PLAYER_WIDTH
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player_b);
}

void UpdateScore(int player, int points) 
{
    if(player == 1) 
    {
        playerA.score += points; 
    }

    if(player == 2) 
    {
        playerB.score += points; 
    }
    
}

void RenderScore() 
{

    char *fmt = "Player A : %d | Player B : %d";
    int length = snprintf(NULL, 0, fmt, playerA.score, playerA.score);
    char buf[length*2];
    snprintf(buf, length*2, fmt, playerA.score, playerB.score);

    font = TTF_OpenFont("IndieFlower-Regular.ttf", 32);
    SDL_Color foregroundColor = { 255, 255, 255, 30 };
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, buf, foregroundColor);
    label = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, label, &(SDL_Rect){0,0,textSurface->w,textSurface->h}, &(SDL_Rect){W_WIDTH/2-textSurface->w/2,W_HEIGHT/2-textSurface->h/2,textSurface->w,textSurface->h});
    SDL_FreeSurface(textSurface);

}