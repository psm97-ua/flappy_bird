#pragma once
#include "GameState.hpp"
#include "MainGameState.hpp"

class GameOverState : public GameState
{
public:
    GameOverState() = default;
    ~GameOverState() = default;
    GameOverState(int score) : puntuacionFinal(score) {}

    void init() override {}
    void handleInput() override;
    void update(float deltaTime) override {}
    void render() override;
    void pause() override {}
    void resume() override {}

private:
    int puntuacionFinal = 0;
};
