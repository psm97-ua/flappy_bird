#pragma once
#include "GameState.hpp"
#include <deque>

extern "C"
{
#include "raylib.h" // define Rectangle
}

struct Bird
{
    float x;      // posición horizontal
    float y;      // posición vertical
    float vy;     // velocidad vertical
    float width;  // ancho del sprite
    float height; // alto del sprite
};

struct PipePair
{
    Rectangle top;
    Rectangle bot;
    bool scored = false;
};

class MainGameState : public GameState
{
public:
    MainGameState();
    ~MainGameState() = default;

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause() {};
    void resume() {};

private:
    char entered_key;
    Bird pajaro;

    // --- Sprites ---
    Texture2D birdSprite;
    Texture2D pipeSprite;
    Texture2D numberSprites[10]; // sprites de los dígitos 0..9
    Texture2D birdFrames[3];     // array con los 3 sprites
    int currentFrame;            // índice del sprite actual
    float frameTime;             // acumulador de tiempo
    float frameDelay;            // cuánto tarda en cambiar de sprite

    std::deque<PipePair> tuberias;

    const float PIPE_GAP_Y = 120.0f;
    const float PIPE_SPEED = 80.0f;

    int PIPE_W;
    int PIPE_H;

    float spawnTimer = 0.0f;
    float spawnEvery = 1.5f;

    int puntos = 0;

    // --- Sonidos ---
    Sound wingSound;
    Sound pointSound;
    Sound hitSound;

    // --- Dificultad progresiva ---
    float dificultadTimer;
    float gapFactor;
    float pipeSpeed;
};