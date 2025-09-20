#include "MainGameState.hpp"
#include <iostream>

extern "C" {
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

}

void MainGameState::handleInput()
{
    if (IsKeyPressed(KEY_SPACE)) {
        pajaro.vy += -300.0f;   // impulsamos hacia arriba al pajaro
    }

}

void MainGameState::update(float deltaTime)
{
    const float gravedad = 500.0f;  
    pajaro.vy += gravedad * deltaTime;
    pajaro.y += pajaro.vy * deltaTime;
    

}

void MainGameState::render()
{
    BeginDrawing();                              // 1) Comienza el dibujado
    ClearBackground(RAYWHITE);                   // 2) Limpia el fotograma anterior
    DrawCircle((int)pajaro.x, (int)pajaro.y, 17, RED);                              //    color
    EndDrawing();  

}