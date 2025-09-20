#pragma once
#include "GameState.hpp"


struct Bird {
    float x;   // posición horizontal
    float y;   // posición vertical
    float vy;  // velocidad vertical
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

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
        Bird pajaro;
};