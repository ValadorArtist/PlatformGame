#include <cmath>    
#include <algorithm>
#include <iostream>

#include <SDL/SDL_opengl.h>
#include <boost/assign/std/vector.hpp>    
#include <boost/assign/list_inserter.hpp>

#include "Engine.h"
#include "LevelChoice.h"
#include "Menu.h"
#include "Game.h"
#include "Sprite.h"
#include "Configurator.h"
#include "Text.h"


LevelChoice::LevelChoice(PlayerPtr player) : m_face_pos(0, 0), m_current_from_node(0),
                                             m_current_to_node(0), m_horizontal_road_sprite(),
                                             m_vertical_road_sprite(), m_entry_enabled_sprite(),
                                             m_entry_disabled_sprite(), m_face_sprite(),
                                             m_tile_width(1.0 / 15), m_tile_height(1.0 / 15),
                                             m_player(player), m_next_application_state() 
{
    srand(time(0));

    using namespace boost::assign;
    m_connections += IntVector(), IntVector(), IntVector(), IntVector(), IntVector();
    m_connections.at(0) += 1;
    m_connections.at(1) += 2, 0;
    m_connections.at(2) += 3, 4, 1;
    m_connections.at(3) += 2;
    m_connections.at(4) += 2;

// definiowanie odwzorowania wez≥a
    insert(m_node_to_level_name)(0, "1")(1, "2")(2, "3")(3, "4")(4, "5");

    m_positions += Point(.1, .7), Point(.7, .7), Point(.7, .4), Point(.3, .4), Point(.7, .1);
    m_face_pos = m_positions.at(0);

// ≥adowanie spritÛw
    const DataConfigurator horizontal_road_data(DL::Foreground, 1, 1, 0 * 32, 14 * 32, 32, 32, false);
    const DataConfigurator vertical_road_data(DL::Foreground, 1, 1,  1 * 32, 14 * 32, 32, 32, false);
    const DataConfigurator entry_enabled_data(DL::Foreground, 1, 1,  6 * 32, 14 * 32, 32, 32, false);
    const DataConfigurator face_data(DL::Foreground, 8, .1,  8 * 32, 14 * 32, 32, 32, true);

    m_horizontal_road_sprite.reset(new Sprite(horizontal_road_data));
    m_vertical_road_sprite.reset(new Sprite(vertical_road_data));
    m_entry_enabled_sprite.reset(new Sprite(entry_enabled_data));
    m_entry_disabled_sprite.reset(new Sprite(entry_enabled_data));
    m_face_sprite.reset(new Sprite(face_data));

}

LevelChoice::~LevelChoice() 
{
}

void LevelChoice::Initialization() 
{
    m_next_application_state.reset();
    SetDone(false);
}

void LevelChoice::Start() 
{
}

void LevelChoice::DrawRoad(size_t from, size_t to) const 
{
    Point from_node_pos = m_positions.at(from);
    Point to_node_pos = m_positions.at(to);
// zachowanie gdy droga jest pionowa

    if (from_node_pos[0] - to_node_pos[0]) 
    {
        if (from_node_pos[0] > to_node_pos[0]) 
        {
            std::swap(from_node_pos, to_node_pos);
        }
        m_vertical_road_sprite->SetRepeat(m_tile_width, m_tile_height);
        m_vertical_road_sprite->DrawCurrentFrame
        (from_node_pos[0],from_node_pos[1] - m_tile_height / 2,
         to_node_pos[0] - from_node_pos[0],  m_tile_height);
    }
//zachowanie gdy droga jest pozioma
    else 
    if (from_node_pos[1] - to_node_pos[1]) 
    {
        if (from_node_pos[1] > to_node_pos[1]) 
        {
            std::swap(from_node_pos, to_node_pos);
        }
        m_horizontal_road_sprite->SetRepeat(m_tile_width, m_tile_height);
        m_horizontal_road_sprite->DrawCurrentFrame(from_node_pos[0] - m_tile_width / 2,
                                                   from_node_pos[1],m_tile_width,                         
                                                   to_node_pos[1] - from_node_pos[1]);
    }
}

void LevelChoice::Draw() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // narysuj drogi
    for (size_t from_node = 0; from_node < m_connections.size(); ++from_node) 
    {
        const IntVector roads = m_connections.at(from_node);
        for (IntVector::const_iterator to_node = roads.begin(); to_node != roads.end(); ++to_node) 
        {
            if (from_node < (size_t)*to_node) 
            {
                DrawRoad(from_node, (size_t)*to_node);
            } else 
            if ((size_t)*to_node < from_node) 
            {
// drogi dwukierunkowe sπ rysowane jednokrotnie
                const IntVector cs = m_connections.at(*to_node);
                if (std::find(cs.begin(), cs.end(), from_node) == cs.end()) 
                {
                    DrawRoad(*to_node, from_node);
                }
            }
        }
    }
    
//rysowanie wez≥Ûw
    for (size_t node_id = 0; node_id < m_positions.size(); ++node_id) 
    {
        const double x = m_positions[node_id][0] - m_tile_width / 2;
        const double y = m_positions[node_id][1] - m_tile_height / 2;

        bool node_enabled = true;
        if (node_enabled) 
        {
            m_entry_enabled_sprite->DrawCurrentFrame(x, y, m_tile_width, m_tile_height);
        } else 
        {
            m_entry_disabled_sprite->DrawCurrentFrame(x, y, m_tile_width, m_tile_height);
        }
    }

// rysowanie postaci
    m_face_sprite->DrawCurrentFrame(m_face_pos[0] - m_tile_width / 2,
                                    m_face_pos[1] - m_tile_height / 2,
                                    m_tile_width,
                                    m_tile_height);
    Text t;
    t.SetSize(.05, .06);
    t.DrawText("WYBIERZ POZIOM", .2, .85);
    SDL_GL_SwapBuffers();
}

// zwraca znak x
int sgn(float x) 
{
    return x ? (x > 0 ? 1 : -1) : 0;
}

int LevelChoice::NextNode(size_t node) const 
{
    if (false == m_connections.at(node).empty()) 
    {
        return m_connections.at(node).at(rand() % m_connections.at(node).size());
    }
    return node;
}

bool LevelChoice::Update(double dt) 
{
//zaktualizuj twarz postaci
    const double face_speed_x = .6; 
    const double face_speed_y = .5;
    const Point to_node_pos = m_positions.at(m_current_to_node);
    const double dist_x = to_node_pos.x - m_face_pos[0];
    const double dist_y = to_node_pos.y - m_face_pos[1];
    double speed_x = face_speed_x * sgn(dist_x);
    double speed_y = face_speed_y * sgn(dist_y);

// czy postac naleøy zatrzymac??
    if (fabs(dist_x) < .01 && fabs(dist_y) < .01) 
    {
        m_current_from_node = m_current_to_node;
        speed_x = speed_y = 0;
        m_face_pos[0] = to_node_pos.x;
        m_face_pos[1] = to_node_pos.y;
    }

//aktualizacja pozycji na podstawie predkoúci
    m_face_pos[0] += speed_x * dt;
    m_face_pos[1] += speed_y * dt;


// aktualizacja animacji
    m_horizontal_road_sprite->Update(dt);
    m_vertical_road_sprite->Update(dt);
    m_entry_enabled_sprite->Update(dt);
    m_face_sprite->Update(dt);

    return !IsDone();
}

bool LevelChoice::GoLeft() 
{
     
    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);

// czy postac stoi w weüle???
    if (m_current_from_node == m_current_to_node) 
    {
// czy istnieje droga w lewo
        IntVector dst_nodes = m_connections.at(m_current_from_node);
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it)
        {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[0] - from_node_pos[0] < 0) 
            {
                m_current_to_node = *it;
                return true;
            }
        }
        return false;
    }

//czy postac idzie w prawo
    if (to_node_pos[0] - from_node_pos[0] > 0) {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

bool LevelChoice::GoUpward() 
{
    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);
    {
        IntVector dst_nodes = m_connections.at(m_current_from_node);
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it) 
        {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[1] - from_node_pos[1] > 0) 
            {
                m_current_to_node = *it;
                return true;
            }
        }
        return false;
    }

//czy postac idzie w lewo???
    if (to_node_pos[1] - from_node_pos[1] < 0) 
    {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

bool LevelChoice::GoDown() 
{
    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);

    if (m_current_from_node == m_current_to_node) 
    {
        IntVector dst_nodes = m_connections.at(m_current_from_node); 
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it) 
        {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[1] - from_node_pos[1] < 0) 
            {
                m_current_to_node = *it;
                return true;
            }
        }
        return false;
    }

//czy postac idzie w prawo??
    if (to_node_pos[1] - from_node_pos[1] > 0) 
    {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

bool LevelChoice::GoRight() 
{
    const Point from_node_pos = m_positions.at(m_current_from_node);
    const Point to_node_pos = m_positions.at(m_current_to_node);
    
    if (m_current_from_node == m_current_to_node) 
    {
        IntVector dst_nodes = m_connections.at(m_current_from_node);
        for (IntVector::const_iterator it = dst_nodes.begin(); it
                != dst_nodes.end(); ++it) 
        {
            Point connection_node_pos = m_positions.at(*it);
            if (connection_node_pos[0] - from_node_pos[0] > 0) 
            {
                m_current_to_node = *it;
                return true;
            }
        }
        return false;
    }

//czy postac idzie w lewo
    if (to_node_pos[0] - from_node_pos[0] < 0) 
    {
        std::swap(m_current_from_node, m_current_to_node);
        return true;
    }
    return false;
}

std::string LevelChoice::NodeToLevelName(int node) 
{
    if (m_node_to_level_name.find(node)!=m_node_to_level_name.end()) 
    {
        return m_node_to_level_name[node];
    }
    return "";
}

void LevelChoice::RunLevelFromNode() 
{
// jeøeli postac nie jest w weüle, nie zmienia sie poziom
    if (m_current_from_node != m_current_to_node) 
    {
        return;
    }
    const std::string level_name = NodeToLevelName(m_current_to_node);
    boost::shared_ptr<LevelChoice> m_level_choice(shared_from_this());
    Game* game_state = new Game(level_name, m_player);
    game_state->BindLevelChoice(m_level_choice);
    m_next_application_state.reset(game_state);

    SetDone();
}

void LevelChoice::ProcessEvents(const SDL_Event & event) 
{
    if (event.type == SDL_QUIT) 
    {
        SetDone();
    }

    if (event.type == SDL_KEYDOWN) 
    {
        SDLKey key = event.key.keysym.sym;
        if (key == SDLK_ESCAPE) 
        {
            m_next_application_state = ApplicationStatePtr(new Menu());
            SetDone();
        } else 
        if (key == SDLK_LEFT) 
        {
            GoLeft();
        } else 
        if (key == SDLK_RIGHT) 
        {
            GoRight();
        } else 
        if (key == SDLK_UP) 
        {
            GoUpward();
        } else 
        if (key == SDLK_DOWN) 
        {
            GoDown();
        } else 
        if (key == SDLK_RETURN) 
        {
            RunLevelFromNode();
        }
    }
}

boost::shared_ptr<ApplicationState> LevelChoice::NextApplicationState() const 
{
    return m_next_application_state;
}

void LevelChoice::SetPlayer(PlayerPtr player) 
{
    m_player = player;
}
