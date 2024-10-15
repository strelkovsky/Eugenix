#include <cstdio>

#include <SDL.h>
#include <SDL_opengl.h>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init error - %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* mainWindow = SDL_CreateWindow("Eugenix", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN);
    if (!mainWindow)
    {
        printf("Window could not be created! - %s", SDL_GetError());
        return false;
    }

    for (;;)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    return 0;
                break;
            }
        }

        SDL_GL_SwapWindow(mainWindow);
        SDL_Delay(1);
    }

    return 0;
}