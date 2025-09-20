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

    Texture2D birdSprite;
    Texture2D pipeSprite;

    std::deque<PipePair> tuberias;

    const float PIPE_GAP_Y = 120.0f;
    const float PIPE_SPEED = 80.0f;

    int PIPE_W;
    int PIPE_H;

    float spawnTimer = 0.0f;
    float spawnEvery = 1.5f;

    int puntos = 0;
};