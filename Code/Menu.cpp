#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "Menu.h"
#include "Text.h"
#include "Engine.h"
#include "LevelChoice.h"
#include "WallOfGlory.h"


void Menu::Initialization() 
{
}

void Menu::Start() 
{
     Engine::Get().GetSound()->PlaySfx("menu");
}

void Menu::Draw() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Text t(0.1, 0.1);
    t.DrawText("Menu", 0.3, 0.8);
    
    if (m_selection == Sel::NewGame) 
    {
        Engine::Get().GetVisualization()->DrawQuad(0.3, 0.59, 0.72, 0.66,  .3, 0.8, 0.2, .5);
    }
    else 
    if (m_selection == Sel::WallOfGlory) {
        Engine::Get().GetVisualization()->DrawQuad(0.2, 0.49, 0.82, 0.56,  .3, 0.8, 0.2, .5);
    }
    else 
    if (m_selection == Sel::Quit) 
    {
        Engine::Get().GetVisualization()->DrawQuad(0.325, 0.39, 0.695, 0.46,  .3, 0.8, 0.2, .5);
    }

    t.SetSize(0.055, 0.055);
    t.DrawText("New Game", 0.29, 0.6);
    t.DrawText("Top of the Top", 0.15, 0.5);
    t.DrawText("Exit", 0.4, 0.4);

    SDL_GL_SwapBuffers();
}

bool Menu::Update(double /* dt */) 
{
    return !IsDone();
}

void Menu::ProcessEvents(const SDL_Event& event) 
{
    if (event.type == SDL_QUIT) 
    {
        SetDone();
    }
    else 
    if (event.type == SDL_KEYDOWN) 
    { 
        if (event.key.keysym.sym == SDLK_DOWN) 
        {
            m_selection = Sel::Selection((m_selection + 1));
            if (m_selection > Sel::Quit) 
            {
                m_selection = Sel::NewGame;
            }
        }
        else 
        if (event.key.keysym.sym == SDLK_UP) 
        {
            m_selection = Sel::Selection(m_selection - 1);
            if (m_selection < Sel::NewGame) 
            {
                m_selection = Sel::Quit;
            }
        }
        else 
        if (event.key.keysym.sym == SDLK_RETURN) 
        {
            if (m_selection == Sel::NewGame) 
            {
//                m_next_app_state.reset(new Game("1", PlayerPtr()) );
                m_next_application_state.reset(new LevelChoice(PlayerPtr()));
            }
            else 
            if (m_selection == Sel::WallOfGlory) 
            {
                m_next_application_state.reset(new WallOfGlory);
            }
            else 
            if (m_selection == Sel::Quit) 
            {
                m_next_application_state.reset();
            }

            SetDone();
        }
        else if (event.key.keysym.sym == SDLK_ESCAPE) 
        {
            m_next_application_state.reset();
            SetDone();
        }
    }
}

boost::shared_ptr<ApplicationState> Menu::NextApplicationState() const 
{
    return m_next_application_state;
}
