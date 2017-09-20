#include <cmath>
#include <algorithm>

#include "BulletCreator.h"
#include "Convert.h"
#include "Engine.h"
#include "Player.h"
#include "Text.h"

Player::Player(double x, double y, size_t level_width, size_t lifes = DefaultLifesCount, size_t score = 0)
               : Creature(x, y, DefaultXSpeed, DefaultYSpeed, 
                 DefaultXAcceleration, DefaultYAcceleration),m_state(PS::Stand),
                 m_running_factor(1.0),m_jump_allowed(true), m_level_width(level_width),
                 m_total_scores(score),m_is_immortal(false),m_lifes(lifes),
                 m_double_shot_enabled(false), m_is_level_completed(false),
                 m_max_x_position(std::max(x, 9.0)) 
{
    SetDefaultMove();
}

void Player::Jump(double y_speed) 
{
// jesli jest mo¿liwosc wykonywany jest skok
    if (m_jump_allowed) 
    {
        m_jump_allowed = false;
        m_is_on_ground = false;
        
        SetYSpeed(y_speed);
        SetYAcceleration(DefaultYAcceleration);
        Engine::Get().GetSound()->PlaySfx("jump");
    }
}

void Player::CheckCollisionsWithLevel(double dt, LevelPtr level) 
{
    size_t x_tile, y_tile;
    GetCurrentTile(&x_tile, &y_tile);
    if (level->Field(x_tile, y_tile - 1) == FT::EndOfLevel) 
    {
        LevelCompleted();
    }

// strawdzanie kolizji postaci z ka¿dej strony
    if (IsFieldAboveCreature(dt, level)) 
    {
        Fall();
    }

    if (IsFieldBelowCreature(dt, level)) 
    {
        PlayerOnGround();
    }

    if (IsFieldOnLeft(dt, level)) 
    {
        CannotGoLeft();
    }
    
    if (IsFieldOnRight(dt, level)) 
    {
        CannotGoRight();
    }
}

void Player::Update(double dt, LevelPtr level) 
{
    CheckCollisionsWithLevel(dt, level);
//wyliczanie predkoœci i po³o¿enia
    if (!m_is_on_ground) 
    {
        m_y = GetNextYPosition(dt);
        m_sy += m_ay * dt;
    }

// je¿eli poni¿ej kafla - odbierz ¿ycie
    if (m_y < .5) 
    {
        LooseLife();
    }

// aktualizacja informacji o niesmiertelnoœci
    const double immortality_time = 3; // 3 sekundy
    if (m_immortal_duration > immortality_time) 
    {
        m_is_immortal = false;
        m_immortal_duration = 0;
    } else 
    {
        m_immortal_duration += dt;
    }

//wyliczanie pozycji gracza na osi X
    double next_x = GetNextXPosition(dt);
    if (next_x < m_x && m_can_go_left) 
    {
        m_x = next_x;
    } else 
    if (next_x > m_x && m_can_go_right) 
    {
        m_x = next_x;
    }
    
//brak mo¿liwoœci wyjœcia poza mape, poza pierwszy i ostatni kafel
    if (m_x < 0) 
    {
        m_x = 0;
    } else 
    if (m_x > m_level_width - 1) 
    {
        m_x = m_level_width - 1;
    }

//brak mo¿liwoœci cofania sie do miejsca, którego nie widac
    if (m_x > m_max_x_position) 
    {
        m_max_x_position = m_x;
    }
    const size_t half_screen_tiles_count = (Engine::Get().GetVisualization()->GetHorizontalTilesOnScreenCount()-1)/2;
    if (m_x < m_max_x_position - half_screen_tiles_count) 
    {
        m_x = m_max_x_position - half_screen_tiles_count;
    } 
    if (m_x > m_level_width - half_screen_tiles_count - 2) 
    {
        m_max_x_position = m_level_width - half_screen_tiles_count - 2;
    }

// ustalanie ruchu gracza na podstawie predkoœci
    if (fabs(m_sx) < 0.001 && (m_state != PS::TurnLeft || m_state != PS::TurnRight)) 
    {
        // nie zmieniamy stanu
    } else 
    if (m_sx > 0.0) 
    {
        m_state = PS::GoRight;
    } else 
    {
        m_state = PS::GoLeft;
    }

// aktualizacja animacji
    switch (m_state) 
    {
    case PS::Stand:
        m_stop->Update(dt);
        break;
    case PS::GoLeft:
        m_left->Update(dt);
        break;
    case PS::GoRight:
        m_right->Update(dt);
        break;
    case PS::TurnLeft:
    case PS::TurnRight:
// gdy animacja ma jedn¹ klatke to nic nie aktualizujemy
        break;
    }
}

void Player::Draw() const 
{
// wypisywanie liczby punktów zdobytych przez gracza
    Text scores_text;
    scores_text.SetSize(0.05, 0.05);
    scores_text.DrawText("Punkty " + IntToStr(GetScores()), .01, .9);
    scores_text.DrawText("Zycia " + IntToStr(GetLifesCount()), .65, .9);

// je¿eli bohater uzyska³ nieœmiertelnoœc to miga
    if (IsImmortal() && int(m_immortal_duration * 10) % 2 == 1) 
    {
        return;
    }

    VisualizationPtr visualization = Engine::Get().GetVisualization();
    const double tile_width = visualization->GetTileWidth();
    const double tile_height = visualization->GetTileHeight();

    const double pos_x = m_x * tile_width;
    const double pos_y = m_y * tile_height;

    switch (m_state) 
    {
    case PS::Stand:
        m_stop->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::GoLeft:
        m_left->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::GoRight:
        m_right->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::TurnLeft:
        m_left->SetCurrentFrame(0);
        m_left->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    case PS::TurnRight:
        m_right->SetCurrentFrame(0);
        m_right->DrawCurrentFrame(pos_x, pos_y, tile_width, tile_height);
        break;
    }
}

void Player::CollisionOnRight(CreaturePtr creature) 
{
    LooseLife();
}

void Player::CollisionOnLeft(CreaturePtr creature) 
{
    LooseLife();
}

void Player::CollisionOverPlayer(CreaturePtr creature) 
{
    LooseLife();
}

void Player::CollisionUnderPlayer(CreaturePtr creature) 
{
    AllowToJump();
    Jump(GetDefaultYSpeed() + 6);
    AddScores(creature->WhenKilledAddScore() * 2);
    creature->KilledByPlayer();
}

void Player::FireBullet() 
{
// strzelanie zale¿y od pozycji gracza(prawo-lewo)

    double x, xspeed;
    const double eps = 0.0001;  // dowolna predkosc
    if (m_state == PS::TurnLeft) 
    {
        x = GetX() - .3;
        xspeed = -eps;
    } else 
    if (m_state == PS::TurnRight) 
    {
        x = GetX() + .7;
        xspeed = eps;
    } else 
    {
        x = GetXSpeed() < 0 ? GetX() - .3 : GetX() + .7;
        xspeed = GetXSpeed();
    }

    const double y = GetY() + .5;
    AddCreator(CreatorPtr(new BulletCreator(x, y, xspeed, 0)));
    if (IsDoubleShotEnabled()) 
    {
        AddCreator(CreatorPtr(new BulletCreator(x, y + .5, xspeed + 2, 0)));
    }
}

void Player::LooseLife() 
{
//strata ¿ycia 
    Engine::Get().GetSound()->PlaySfx("lost_life");
    m_lifes--;

    m_is_immortal = true;
    m_immortal_duration = 0;

// ustawianie nowej pozycji po stracie ¿ycia
    SetPosition(2, 2);
}

void Player::LevelCompleted() 
{
    m_is_level_completed = true;
}
