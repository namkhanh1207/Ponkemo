#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include "Ponkemo.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main() {
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        cout << "Initialization failed: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ponkemo Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Window creation failed: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Renderer creation failed: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        cout << "Warning: Could not load font: " << TTF_GetError() << endl;
        cout << "Continuing without text display" << endl;
    }

    Ponkemo ponkemo = {"Ponkemo", 100, 100, 50, 50, 12, 20, 10, 30, 30};
    Ponkemo chardriza = {"Chardriza", 100, 100, 50, 50, 8, 15, 10, 25, 30};

    bool running = true, gameOver = false;
    SDL_Event e;
    SDL_Color white = {255, 255, 255, 255};
    vector<string> battleLog;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (!gameOver && e.type == SDL_KEYDOWN) {
                int choice = 0;
                switch (e.key.keysym.sym) {
                    case SDLK_1: choice = 1; break;
                    case SDLK_2: choice = 2; break;
                    case SDLK_3: choice = 3; break;
                }
                if (choice) {
                    attack(ponkemo, chardriza, choice, battleLog);
                    if (chardriza.hp > 0) {
                        int eChoice = rand() % 3 + 1;
                        attack(chardriza, ponkemo, eChoice, battleLog);
                    }

                    if (ponkemo.hp <= 0) {
                        battleLog.push_back("Game Over! " + chardriza.name + " wins!");
                        gameOver = true;
                    }
                    else if (chardriza.hp <= 0) {
                        battleLog.push_back("Victory! " + ponkemo.name + " wins!");
                        gameOver = true;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawHealthBar(renderer, 50, 50, 200, 20, ponkemo.hp, ponkemo.maxHp);
        drawHealthBar(renderer, 50, 100, 200, 20, chardriza.hp, chardriza.maxHp);

        string p1Status = ponkemo.name + " - HP: " + to_string(ponkemo.hp) + " EP: " + to_string(ponkemo.ep);
        string p2Status = chardriza.name + " - HP: " + to_string(chardriza.hp) + " EP: " + to_string(chardriza.ep);
        renderText(renderer, font, p1Status, 50, 20, white);
        renderText(renderer, font, p2Status, 50, 70, white);

        renderText(renderer, font, "Choose an action:", 50, 400, white);
        renderText(renderer, font, "1. Basic Attack (No EP)", 50, 430, white);
        renderText(renderer, font, "2. Medium Attack (-10 EP)", 50, 460, white);
        renderText(renderer, font, "3. Strong Attack (-30 EP)", 50, 490, white);

        int logY = 150;
        for (size_t i = 0; i < battleLog.size() && i < 5; ++i) {
            renderText(renderer, font, battleLog[battleLog.size() - 1 - i], 300, logY, white);
            logY += 30;
        }

        SDL_RenderPresent(renderer);
    }

    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
