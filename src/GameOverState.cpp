#include "GameOverState.hpp"
#include "StateMachine.hpp"
#include "MainGameState.hpp"

extern "C"
{
#include "raylib.h"
}

void GameOverState::handleInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        // Reiniciar el juego: añadir un nuevo MainGameState
        this->state_machine->add_state(std::make_unique<MainGameState>(), true);
    }
}

void GameOverState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Texto en el centro de la pantalla
    const char *msg = "Game Over";
    int fontSize = 30;
    int textWidth = MeasureText(msg, fontSize);
    DrawText(msg, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2, fontSize, BLACK);

    // Mostramos la puntuación final
    std::string scoreStr = "Puntos: " + std::to_string(puntuacionFinal);
    int scoreFontSize = 20;
    int scoreWidth = MeasureText(scoreStr.c_str(), scoreFontSize);
    DrawText(scoreStr.c_str(),
             GetScreenWidth() / 2 - scoreWidth / 2,
             GetScreenHeight() / 2 + 60,
             scoreFontSize,
             DARKGRAY);

    EndDrawing();
}
