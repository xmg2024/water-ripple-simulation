#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <random>

const int WIDTH = 400;
const int HEIGHT = 400;
const float DAMPING = 0.98f;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Water Ripple Simulation",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    std::vector<std::vector<float>> current(WIDTH, std::vector<float>(HEIGHT, 0.0f));
    std::vector<std::vector<float>> previous(WIDTH, std::vector<float>(HEIGHT, 0.0f));
    std::vector<uint32_t> pixels(WIDTH * HEIGHT, 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, WIDTH - 2);
    std::uniform_int_distribution<> disY(1, HEIGHT - 2);
    std::uniform_int_distribution<> disChance(0, 100);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN || (e.type == SDL_MOUSEMOTION && (e.button.button & SDL_BUTTON_LMASK))) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 1) {
                    previous[x][y] = 255.0f;
                    // Splash effect
                    if (x > 1 && x < WIDTH - 2 && y > 1 && y < HEIGHT - 2) {
                        previous[x+1][y] = 255.0f;
                        previous[x-1][y] = 255.0f;
                        previous[x][y+1] = 255.0f;
                        previous[x][y-1] = 255.0f;
                    }
                }
            }
        }

        // Random raindrops
        if (disChance(gen) < 2) {
            previous[disX(gen)][disY(gen)] = 255.0f;
        }

        // Wave simulation
        for (int i = 1; i < WIDTH - 1; ++i) {
            for (int j = 1; j < HEIGHT - 1; ++j) {
                current[i][j] = (previous[i - 1][j] +
                                 previous[i + 1][j] +
                                 previous[i][j - 1] +
                                 previous[i][j + 1]) / 2.0f - current[i][j];
                current[i][j] *= DAMPING;
            }
        }

        // Render to pixel buffer
        for (int i = 0; i < WIDTH; ++i) {
            for (int j = 0; j < HEIGHT; ++j) {
                float val = current[i][j];
                
                // Color mapping: 
                // We want a nice water effect. 
                // Deep water is dark blue, peaks are light blue/white.
                
                int b = 150 + static_cast<int>(val);
                int g = 100 + static_cast<int>(val * 0.8f);
                int r = 50 + static_cast<int>(val * 0.5f);

                // Clamp colors
                if (r < 0) r = 0; if (r > 255) r = 255;
                if (g < 0) g = 0; if (g > 255) g = 255;
                if (b < 0) b = 0; if (b > 255) b = 255;
                
                pixels[j * WIDTH + i] = (255 << 24) | (r << 16) | (g << 8) | b;
            }
        }

        // Swap buffers
        std::swap(current, previous);

        SDL_UpdateTexture(texture, NULL, pixels.data(), WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
