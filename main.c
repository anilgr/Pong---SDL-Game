#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

#define W_HEIGHT 480
#define W_WIDTH 600

const int BALL_SIZE = 10;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect ball = {
    .x = W_WIDTH/2-BALL_SIZE/2,
    .y = W_HEIGHT/2-BALL_SIZE/2,
    .w = BALL_SIZE,
    .h = BALL_SIZE
};

bool Init();
void Update();
void Quit();

int main(int arc, char *argv[])
{
    atexit(Quit);
    bool loop = true;
    SDL_Event event;
    time_t x;
	srand((int)time(&x));

    if(!Init())
    {
        printf("Could not init game !");
    }

    while (loop)
    {
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {   
                loop = false;
                Quit();
            }
            
            
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);


        if(rand()%2==0)
        {
            ball.x -= 4;
        } else {
            ball.x += 4;
        }

        if(rand()%2==0)
        {
            ball.y -= 4;
        } else {
            ball.y += 4;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ball);

        SDL_RenderPresent(renderer);
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

    return true;
}

void Update()
{

}
void Quit()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
