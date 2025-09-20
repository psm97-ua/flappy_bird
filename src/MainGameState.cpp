#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "StateMachine.hpp"
#include <iostream>

extern "C"
{
#include "raylib.h"
}

MainGameState::MainGameState()
{
}

void MainGameState::init()
{
    pajaro.x = 200.0f;
    pajaro.y = 200.0f;
    pajaro.vy = 0.0f;

    spawnTimer = 0.0f;
    tuberias.clear();
    puntos = 0;

    // --- Cargar sprites ---
    birdSprite = LoadTexture("assets/bluebird-midflap.png");
    pipeSprite = LoadTexture("assets/pipe-green.png");

    // Tamaño del pájaro según sprite
    pajaro.width = (float)birdSprite.width;
    pajaro.height = (float)birdSprite.height;

    // Tamaño de las tuberías según sprite
    PIPE_W = pipeSprite.width;
    PIPE_H = pipeSprite.height;
}

void MainGameState::handleInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        pajaro.vy += -300.0f; // impulsamos hacia arriba al pajaro
    }
}

void MainGameState::update(float deltaTime)
{
    const float gravedad = 500.0f;
    pajaro.vy += gravedad * deltaTime;
    pajaro.y += pajaro.vy * deltaTime;

    Rectangle pajaroBB;
    pajaroBB.x = pajaro.x;
    pajaroBB.y = pajaro.y;
    pajaroBB.width = pajaro.width;
    pajaroBB.height = pajaro.height;

    spawnTimer += deltaTime;
    if (spawnTimer >= spawnEvery)
    {
        spawnTimer = 0.0f;

        int minOff = PIPE_H / 2;
        int maxOff = GetScreenHeight() / 2;
        int pipe_y_offset_top = GetRandomValue(minOff, maxOff);

        float startX = (float)GetScreenWidth();
        float topY = -(float)pipe_y_offset_top;

        int extra = GetRandomValue(PIPE_H / 2, GetScreenHeight() / 2);
        float botY = (float)(PIPE_H - pipe_y_offset_top + extra);

        PipePair par;
        par.top = {startX, topY, (float)PIPE_W, (float)PIPE_H};
        par.bot = {startX, botY, (float)PIPE_W, (float)PIPE_H};
        par.scored = false;

        tuberias.push_back(par);
    }

    // Movemos todas las tuberías hacia la izquierda
    for (auto &p : tuberias)
    {
        p.top.x -= PIPE_SPEED * deltaTime;
        p.bot.x -= PIPE_SPEED * deltaTime;
    }

    // Borramos del frente si salen de la pantalla (usa front() y pop_front())
    if (!tuberias.empty())
    {
        const PipePair &f = tuberias.front();
        if (f.top.x + f.top.width < 0)
        {
            tuberias.pop_front();
        }
    }

    for (auto &p : tuberias)
    {
        if (!p.scored && p.top.x + p.top.width < pajaro.x)
        {
            puntos++;
            p.scored = true;
        }
    }

    for (const auto &p : tuberias)
    {
        if (CheckCollisionRecs(pajaroBB, p.top) || CheckCollisionRecs(pajaroBB, p.bot))
        {
            this->state_machine->add_state(std::make_unique<GameOverState>(puntos), true);
            return; // salimos del update para no seguir ejecutando
        }
    }

    // Colisiones con bordes de pantalla
    if (pajaro.y < 0 || pajaro.y + pajaro.height > GetScreenHeight())
    {
        this->state_machine->add_state(std::make_unique<GameOverState>(puntos), true);
        return;
    }
}

void MainGameState::render()
{
    BeginDrawing();            // 1) Comienza el dibujado
    ClearBackground(RAYWHITE); // 2) Limpia el fotograma anterior
    DrawTexture(birdSprite, (int)pajaro.x, (int)pajaro.y, WHITE);

    for (const auto &p : tuberias)
    {
        // Tubería superior (rotada 180º)
        DrawTextureEx(pipeSprite,
                      {p.top.x, p.top.y + PIPE_H},
                      180.0f, // rotación
                      1.0f,   // escala
                      WHITE);

        // Tubería inferior (normal)
        DrawTextureEx(pipeSprite,
                      {p.bot.x, p.bot.y},
                      0.0f, // rotación
                      1.0f, // escala
                      WHITE);
    }

    std::string puntosStr = std::to_string(puntos);
    DrawText(puntosStr.c_str(), 10, 10, 30, BLACK);

    EndDrawing();
}