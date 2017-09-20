#include <fstream>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "WallOfGlory.h"
#include "Text.h"
#include "Engine.h"
#include "Menu.h"

void WallOfGlory::Initialization() 
{
}

void WallOfGlory::Start() 
{
}

WallOfGlory::WallOfGlory() : m_is_done(false) 
{
    LoadFromFile();
}

void WallOfGlory::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Text t(0.07, 0.07);
    t.DrawText("Wall Of Glory", 0.01, 0.8);

    t.SetSize(0.05, 0.05);
    double y = 0.7;
    double x = 0.15;
    for (size_t i = 0; i < m_entries.size(); ++i) {
        t.DrawText(m_entries.at(i).name, x, y);
        t.DrawNumber(m_entries.at(i).points, x+0.4, y, 8);
        y -= 0.07;
    }

    SDL_GL_SwapBuffers();
}

bool WallOfGlory::Update(double dt)
{
    return !IsDone();
}

void WallOfGlory::ProcessEvents(const SDL_Event& event)
 {
    if (event.type == SDL_KEYDOWN) 
    {
        SetDone();
    }
}


void WallOfGlory::LoadFromFile() 
{
    std::ifstream in("data/ranking.txt");
    if (!in) 
    {
        std::cerr << "Nie moge odczytac Wall of Glory\n";
        return;
    }

    Entry entry;
    while (in >> entry.name >> entry.points) 
    {
        m_entries.push_back(entry);
    }
     Engine::Get().GetSound()->PlaySfx("success");
}

ApplicationStatePtr WallOfGlory::NextApplicationState() const 
{
    return ApplicationStatePtr(new Menu);
}
