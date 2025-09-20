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
    pajaroBB.x = pajaro.x - 17; // 17 = radio
    pajaroBB.y = pajaro.y - 17;
    pajaroBB.width = 34; // diámetro
    pajaroBB.height = 34;

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

    for (const auto &p : tuberias)
    {
        if (CheckCollisionRecs(pajaroBB, p.top) || CheckCollisionRecs(pajaroBB, p.bot))
        {
            this->state_machine->add_state(std::make_unique<GameOverState>(), true);
            return; // salimos del update para no seguir ejecutando
        }
    }

    // Colisiones con bordes de pantalla
    if (pajaro.y - 17 < 0 || pajaro.y + 17 > GetScreenHeight())
    {
        this->state_machine->add_state(std::make_unique<GameOverState>(), true);
        return;
    }
}

void MainGameState::render()
{
    BeginDrawing();                                    // 1) Comienza el dibujado
    ClearBackground(RAYWHITE);                         // 2) Limpia el fotograma anterior
    DrawCircle((int)pajaro.x, (int)pajaro.y, 17, RED); //    color

    for (const auto &p : tuberias)
    {
        DrawRectangle((int)p.top.x, (int)p.top.y, (int)p.top.width, (int)p.top.height, DARKGREEN);
        DrawRectangle((int)p.bot.x, (int)p.bot.y, (int)p.bot.width, (int)p.bot.height, DARKGREEN);
    }
    EndDrawing();
}