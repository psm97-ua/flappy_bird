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

}

void MainGameState::handleInput()
{

}

void MainGameState::update(float deltaTime)
{

}

void MainGameState::render()
{
    BeginDrawing();                              // 1) Comienza el dibujado
    ClearBackground(RAYWHITE);                   // 2) Limpia el fotograma anterior
    DrawText("Bienvenido a Flappy Bird DCA",     // 3) Dibuja el texto
             10, 10,                             //    posición (x=10, y=10)
             20,                                 //    tamaño de fuente
             RED);                             //    color
    EndDrawing();  

}