#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdexcept>

#include "Creature.h"

class Player : public Creature
{
public:
    explicit Player(double x, double y, size_t level_width, size_t lifes_count, size_t score);

    void Update(double dt, LevelPtr level);
    void Draw() const;
    void CheckCollisionsWithLevel(double dt, LevelPtr level);
    CR::CreatureType GetType() const 
    {
        throw std::logic_error("Gracz to nie jednostka");
    }

    virtual void GoLeft()       
    { 
            m_sx -= GetDefaultXSpeed(); m_state=PS::GoLeft; 
    }
    
    virtual void GoRight()      
    { 
            m_sx += GetDefaultXSpeed(); 
            m_state=PS::GoRight; 
    }
    
    virtual void StopLeft()     
    { 
            m_sx += GetDefaultXSpeed(); 
            TurnLeft(); 
    }
    
    virtual void StopRight()    
    { 
            m_sx -= GetDefaultXSpeed(); 
            TurnRight(); 
    }
    
    virtual void StopMove() 
    { 
            m_sx = 0; m_state = PS::Stand; 
    }
    
    void TurnLeft()             
    { 
         m_state=PS::TurnLeft; 
    }
    
    void TurnRight()            
    { 
         m_state=PS::TurnRight; 
    }
    
    void AllowToJump()              
    { 
         m_jump_allowed = true; 
    }
    
    void CannotJump()             
    {
         m_jump_allowed = false;
    }
    
    void Run()                      
    { 
         m_running_factor = 2.0; 
    }
    
    void StopRunning()              
    { 
         m_running_factor = 1.0; 
    }
    
    double GetRunningFactor() const 
    { 
           return m_running_factor; 
    }
    void Jump(double y_speed = DefaultYSpeed);
    
    void SetDefaultMovement()  
    {
        m_is_on_ground = m_jump_allowed = false;
        m_can_go_right = m_can_go_left = true;
    }
    
    void PlayerOnGround() 
    {
        m_is_on_ground = m_jump_allowed = true;
        SetYSpeed(0);
    }

    double GetNextXPosition(double dt) const 
    { 
           return GetX() + GetXSpeed() * dt * GetRunningFactor(); 
    }
    
    Aabb GetBasicAabb() const { return Aabb(.1, 0, .7, .9);  }

//obs³uga kolizji dla ka¿dej strony
    void CollisionOnRight(CreaturePtr creature);
    void CollisionOnLeft(CreaturePtr creature);
    void CollisionOverPlayer(CreaturePtr creature);
    void CollisionUnderPlayer(CreaturePtr creature);

    void AddScores(int scores) 
    { 
         m_total_scores += scores; 
    }
    
    int  GetScores() const     
    {
         return m_total_scores; 
    }

    // wystrzel pocisk
    void FireBullet();

// liczba ¿yc bohatera
    int GetLifesCount() const 
    { 
        return m_lifes; 
    }

// gdy gracz straci ¿ycie
    void LooseLife();
// metoda wywo³ywana gdy gracz ukonczy poziom
    void LevelCompleted();

    bool HasCompletedCurrentLevel() const 
    {
        return m_is_level_completed;
    }

    bool IsImmortal() const 
    { 
         return m_is_immortal; 
    }

    void EnableDoubleShot()         
    { 
         m_double_shot_enabled = true; 
    }
    
    void DisableDoubleShot()         
    {
          m_double_shot_enabled = false; 
    }
    
    bool IsDoubleShotEnabled() const 
    { 
         return m_double_shot_enabled; 
    }

    bool MoveMap() 
    {
        const size_t screen_tiles_count = Engine::Get().GetVisualization()->GetHorizontalTilesOnScreenCount();
        const size_t half_screen_tiles_count = screen_tiles_count/2;
        
        return 
        GetX() >= m_max_x_position && GetX() < (m_level_width - 1) - half_screen_tiles_count;
    }

    void NewLevelReset(LevelPtr level) 
    {
        SetDefaultMovement();
        LevelCreatureData player_data = level->GetPlayerData();
        SetPosition(player_data.x, player_data.y);
        SetSpeed(0, 0);
        m_max_x_position = 9;
        m_level_width = level->GetWidth();
        m_is_level_completed = false;
        m_state = PS::Stand;
        m_running_factor = 1;
    }

private:
    enum { DefaultXSpeed = 4, DefaultYSpeed = 20,
           DefaultXAcceleration = 0, DefaultYAcceleration = -60,
           DefaultLifesCount = 2 };
    
    PS::PlayerState m_state;  //stan postaci
    double m_running_factor;  //wspó³czynnik biegu
    bool m_jump_allowed;      
    size_t m_level_width;     

    int m_total_scores;       
    bool m_is_immortal;       
    double m_immortal_duration;  //czas nieœmiertelnoœci postaci
    int m_lifes;                

    bool m_double_shot_enabled;    

    bool m_is_level_completed; 

    double m_max_x_position;
};

typedef boost::shared_ptr<Player> PlayerPtr;

#endif
