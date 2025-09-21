#include "MainGameState.hpp"
#include "GameOverState.hpp"
#include "StateMachine.hpp"
#include <iostream>
#include <string>

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

    dificultadTimer = 0.0f;
    gapFactor = 3.0f;
    pipeSpeed = PIPE_SPEED;

    // --- Cargar sprites ---
    birdSprite = LoadTexture("assets/bluebird-midflap.png");
    pipeSprite = LoadTexture("assets/pipe-green.png");

    birdFrames[0] = LoadTexture("assets/bluebird-upflap.png");
    birdFrames[1] = LoadTexture("assets/bluebird-midflap.png");
    birdFrames[2] = LoadTexture("assets/bluebird-downflap.png");
    currentFrame = 0;
    frameTime = 0.0f;
    frameDelay = 0.15f; // cada 0.15s cambia de sprite
    for (int i = 0; i < 10; i++)
    {
        std::string filename = "assets/" + std::to_string(i) + ".png";
        numberSprites[i] = LoadTexture(filename.c_str());
    }

    wingSound = LoadSound("assets/audio/sfx_wing.ogg");
    pointSound = LoadSound("assets/audio/sfx_point.ogg");
    hitSound = LoadSound("assets/audio/sfx_hit.ogg");

    // Tamaño del pájaro según sprite
    pajaro.width = (float)birdSprite.width;
    pajaro.height = (float)birdSprite.height;

    // Tamaño de las tuberías según sprite
    PIPE_W = pipeSprite.width;
    PIPE_H = pipeSprite.height;
    SetMasterVolume(0.5f);
}

void MainGameState::handleInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        pajaro.vy += -300.0f; // impulsamos hacia arriba al pajaro
        PlaySound(wingSound);
    }
}

void MainGameState::update(float deltaTime)
{
    // Actualizar animación del pájaro
    frameTime += deltaTime;
    if (frameTime >= frameDelay)
    {
        frameTime = 0.0f;
        currentFrame = (currentFrame + 1) % 3; // ciclo entre 0, 1 y 2
    }

    const float gravedad = 500.0f;
    pajaro.vy += gravedad * deltaTime;
    pajaro.y += pajaro.vy * deltaTime;

    Rectangle pajaroBB;
    pajaroBB.x = pajaro.x;
    pajaroBB.y = pajaro.y;
    pajaroBB.width = pajaro.width;
    pajaroBB.height = pajaro.height;

    // --- Actualizar dificultad progresiva ---
    dificultadTimer += deltaTime;
    if (dificultadTimer > 5.0f) // cada 5 segundos
    {
        if (gapFactor > 2.5f)
            gapFactor -= 0.35f; // huecos más pequeños
        pipeSpeed += 30.0f;     // tuberías más rápidas
        dificultadTimer = 0.0f; // reiniciar contador
        std::cout << "pipeSpeed: " << pipeSpeed << " gapFactor: " << gapFactor << std::endl;
    }
    spawnEvery = 1.5f * (80.0f / pipeSpeed);

    spawnTimer += deltaTime;
    if (spawnTimer >= spawnEvery)
    {
        spawnTimer = 0.0f;

        int minOff = PIPE_H / 2;
        int maxOff = GetScreenHeight() / 2;
        int pipe_y_offset_top = GetRandomValue(minOff, maxOff);

        float startX = (float)GetScreenWidth();
        float topY = -(float)pipe_y_offset_top;

        float gap = GetScreenHeight() / gapFactor;

        PipePair par;
        par.top = {startX, topY, (float)PIPE_W, (float)PIPE_H};
        par.bot = {startX, topY + PIPE_H + gap, (float)PIPE_W, (float)PIPE_H};

        par.scored = false;

        tuberias.push_back(par);
    }

    // Movemos todas las tuberías hacia la izquierda
    for (auto &p : tuberias)
    {
        p.top.x -= pipeSpeed * deltaTime;
        p.bot.x -= pipeSpeed * deltaTime;
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
            PlaySound(pointSound);
        }
    }

    for (const auto &p : tuberias)
    {
        if (CheckCollisionRecs(pajaroBB, p.top) || CheckCollisionRecs(pajaroBB, p.bot))
        {
            PlaySound(hitSound);
            this->state_machine->add_state(std::make_unique<GameOverState>(puntos), true);
            return; // salimos del update para no seguir ejecutando
        }
    }

    // Colisiones con bordes de pantalla
    if (pajaro.y < 0 || pajaro.y + pajaro.height > GetScreenHeight())
    {
        PlaySound(hitSound);
        this->state_machine->add_state(std::make_unique<GameOverState>(puntos), true);
        return;
    }
}

void MainGameState::render()
{
    BeginDrawing();            // 1) Comienza el dibujado
    ClearBackground(RAYWHITE); // 2) Limpia el fotograma anterior
    DrawTexture(birdFrames[currentFrame], (int)pajaro.x, (int)pajaro.y, WHITE);

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
    // Puntuación con sprites (arriba a la izquierda)
    std::string puntosStr = std::to_string(puntos);
    int x = 10; // margen izquierdo
    int y = 10; // margen superior

    for (char c : puntosStr)
    {
        int digit = c - '0'; // convierte '0'..'9' en 0..9
        DrawTexture(numberSprites[digit], x, y, WHITE);
        x += numberSprites[digit].width + 2; // separa cada dígito
    }

    EndDrawing();
}