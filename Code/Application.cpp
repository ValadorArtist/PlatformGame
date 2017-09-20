#include <cassert>

#include "Application.h"
#include "Engine.h"
#include "Menu.h"
#include "WallOfGlory.h"

void Application::ProcessEvents()
{
// przegl¹danie zdarzeñ
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_VIDEORESIZE)
        {
            Resize(event.resize.w, event.resize.h);
        } 
        else 
        {
            m_application_state->ProcessEvents(event);
        }
    }
}

void Application::Run() 
{
// inicjalizacja okna za pomoc¹ biblioteki SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Resize(Engine::Get().GetWindow()->GetWidth(),Engine::Get().GetWindow()->GetHeight());
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // podwójne buforowanie
    Engine::Get().GetSound()->LoadSounds();

// inicjalizacja biblioteki OpenGL, tworzenie t³a
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST); /* funkcja zapobiega wyswietlaniu przezroczystych
                                elementów spritów */
    glAlphaFunc(GL_GEQUAL, 0.1);

// ³adowanie bitmapy
    const std::string textures_filename = "data/textures.bmp";
    Engine::Get().GetVisualization()->LoadTexture(textures_filename);

    m_application_state.reset(new Menu);
    m_application_state->Initialization();
    m_application_state->Start();

// g³ówna petla
    size_t recent_ticks = SDL_GetTicks();
    while (true) 
    {
        if (m_application_state->IsDone()) 
        {
           m_application_state = m_application_state->NextApplicationState();
           if (!m_application_state) 
           {
                return;
           }
            m_application_state->Initialization();
            m_application_state->Start();
        }

        ProcessEvents();

//aktualizacja czasu
        size_t new_ticks = SDL_GetTicks();
        double delta_time = (new_ticks - recent_ticks)/1000.0;
        recent_ticks = new_ticks;

//aktualizowanie i generowanie obrazu
        if (delta_time > 0) 
        {
            Update(delta_time);
        }
        Draw();
    }
    SDL_Quit();
}

void Application::Update(double dt) 
{
    m_application_state->Update(dt);
}

void Application::Draw() 
{
    m_application_state->Draw();
}

void Application::Resize(size_t width, size_t height) 
{
    m_screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_RESIZABLE | SDL_HWSURFACE);
    assert(m_screen && "Wyst¹pi³ problem w ustawieniach video");
    Engine::Get().GetWindow()->SetSize(width, height);
    Engine::Get().GetVisualization()->SetProjection(width, height);
}
