#include <SDL/SDL.h>
#include <vector>
#include "Game.h"

class Application
{
public:
// metody klasy Application
    explicit Application(bool fullscreen_mode) 
             : m_fullscreen(fullscreen_mode) 
    {
    }

    void Run();

private:
    void Draw();
    void Update(double dt);
    void Resize(size_t width, size_t height);
    void ProcessEvents();

//pola klasy Application

    SDL_Surface* m_screen;
    bool m_fullscreen;      
    ApplicationStatePtr m_application_state;
};
