#include <cmath>
#include <iostream>

#include "Creature.h"
#include "Engine.h"

Creature::Creature(double x, double y, double def_speed_x, double def_speed_y,
                   double def_acceleration_x, double def_acceleration_y)
                   : m_default_speed_x(def_speed_x),
                   m_default_speed_y(def_speed_y),
                   m_default_acceleration_x(def_acceleration_x),
                   m_default_acceleration_y(def_acceleration_y),
                   m_is_dead(false),m_state(ES::Stand),
                   m_x(x),m_y(y),m_sx(0.0),m_sy(0),
                   m_ax(def_acceleration_x),m_ay(def_acceleration_y),
                   m_is_on_ground(true),m_can_go_left(true),m_can_go_right(true)
{
    SetDefaultMove();
}

void Creature::SetSprites(SpritePtr left, SpritePtr right, SpritePtr stop) 
{
    m_left = left;
    m_right = right;
    m_stop = stop;
}

bool Creature::IsFieldBelowCreature(double dt, LevelPtr level) const 
{
    size_t current_x_tile, current_y_tile;
    GetCurrentTile(&current_x_tile, &current_y_tile);
    for (int x = -1; x < 2; ++x) 
    {
        if (level->Field(current_x_tile + x, current_y_tile + 1) == FT::None) 
        {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(current_x_tile + x, current_y_tile + 1);
        if (GetNextVerticalAabb(dt).IsOver(field_aabb)) 
        {
            return true;
        }
    }
    return false;
}

bool Creature::IsFieldAboveCreature(double dt, LevelPtr level) const
{
    size_t current_x_tile, current_y_tile;
    GetCurrentTile(&current_x_tile, &current_y_tile);
    for (int x = -1; x < 2; ++x)
    {
        if (level->Field(current_x_tile + x, current_y_tile - 1) == FT::None) 
        {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(current_x_tile + x, current_y_tile - 1);
        if (GetNextVerticalAabb(dt).IsUnder(field_aabb)) 
        {
            return true;
        }
    }
    return false;
}

bool Creature::IsFieldOnLeft(double dt, LevelPtr level) const 
{
    size_t current_x_tile, current_y_tile;
    GetCurrentTile(&current_x_tile, &current_y_tile);
    for (int y = -1; y < 2; ++y) 
    {
        if (level->Field(current_x_tile - 1, current_y_tile + y) == FT::None) 
        {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(current_x_tile - 1, current_y_tile + y);
        if (GetNextHorizontalAabb(dt).IsOnRightOf(field_aabb)) 
        {
            return true;
        }
    }
    return false;
}

bool Creature::IsFieldOnRight(double dt, LevelPtr level) const 
{
    size_t current_x_tile, current_y_tile;
    GetCurrentTile(&current_x_tile, &current_y_tile);
    for (int y = -1; y < 2; ++y) 
    {
        if (level->Field(current_x_tile + 1, current_y_tile + y) == FT::None) 
        {
            continue;
        }
        const Aabb field_aabb = level->GetFieldAabb(current_x_tile + 1, current_y_tile + y);
        if (GetNextHorizontalAabb(dt).IsOnLeftOf(field_aabb)) 
        {
            return true;
        }
    }
    return false;
}

bool Creature::DoFieldsEndOnLeft(double dt, LevelPtr level) const 
{
    size_t current_x_tile, current_y_tile;
    GetCurrentTile(&current_x_tile, &current_y_tile);
    if (level->Field(current_x_tile - 1, current_y_tile + 1) == FT::None
        && level->GetFieldAabb(current_x_tile - 1, current_y_tile).IsOnLeftOf(GetNextHorizontalAabb(dt))) 
    {
        return true;
    }
    return false;
}

bool Creature::DoFieldsEndOnRight(double dt, LevelPtr level) const 
{
    size_t current_x_tile, current_y_tile;
    GetCurrentTile(&current_x_tile, &current_y_tile);
    if (level->Field(current_x_tile + 1, current_y_tile + 1) == FT::None
        && level->GetFieldAabb(current_x_tile + 1, current_y_tile).IsOnRightOf(GetNextHorizontalAabb(dt))) 
    {
        return true;
    }
    return false;
}

void Creature::CheckCollisionsWithLevel(double dt, LevelPtr level) 
{
    if(IsFieldAboveCreature(dt, level))
    {
        Fall();  
    }
    
    if(IsFieldBelowCreature(dt, level))
    {
        CreatureOnGround(); 
    }
   
    if(IsFieldOnLeft(dt, level)) 
    {
        ReverseXSpeed();
    }

    if(IsFieldOnRight(dt, level)) 
    {
        ReverseXSpeed();
    }
}

void Creature::Update(double dt, LevelPtr level) 
{
    SetDefaultMove();
    CheckCollisionsWithLevel(dt, level);

// wyliczanie nowej pozycji oraz predkosci na osi Y
    if (!m_is_on_ground) 
    {
        m_y = GetNextYPosition(dt);
        m_sy += m_ay * dt;
    }

// je¿eli istota znajduje sie poni¿ej pierwszego kafla, to nie spada
    if (m_y < 0) 
    {
        m_y = 0;
        CreatureOnGround();
    }
// wyliczanie pozycji gracza w poziomie (X) 
    double next_x = GetNextXPosition(dt);
    if (next_x < m_x && m_can_go_left) 
    {
        m_x = next_x;
    } else 
    if (next_x > m_x && m_can_go_right) 
    {
        m_x = next_x;
    }
    
// brak mo¿liwoœci wyjœcia poza mape
    if (m_x < 1) 
    {
        m_x = 1;
    }

// ustalanie stanu ruchu gracza na podstawie predkoœci
    if (fabs(m_sx) < 0.00001) 
    {
        m_state = ES::Stand;
        m_sx = 0;
    } else 
    if (m_sx > 0.0) 
    {
        m_state = ES::GoRight;
    } else 
    {
        m_state = ES::GoLeft;
    }

//aktualizowanie animacji

    switch (m_state) 
    {
    case ES::Stand:
        m_stop->Update(dt);
        break;
    case ES::GoLeft:
        m_left->Update(dt);
        break;
    case ES::GoRight:
        m_right->Update(dt);
        break;
    }
}

void Creature::Draw() const 
{
    const double tile_width = Engine::Get().GetVisualization()->GetTileWidth();
    const double tile_height = Engine::Get().GetVisualization()->GetTileHeight();

// wyliczanie pozycji gracza
    const double pos_x = m_x * tile_width;
    const double pos_y = m_y * tile_height;

    switch (m_state) 
    {
    case ES::Stand:
        m_stop->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case ES::GoLeft:
        m_left->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case ES::GoRight:
        m_right->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    }
}

void Creature::KilledByPlayer() 
{
    SetIsDead(true);
}

void Creature::KilledbyBullet()
{
    SetIsDead(true);
}
