#ifndef __CREATURE_H__
#define __CREATURE_H__

#include <boost/shared_ptr.hpp> 

#include "Aabb.h"
#include "Sprite.h"
#include "Types.h"
#include "Engine.h"
#include "Level.h"
#include "CreatorBasicClass.h"


class Creature : public CreatorBasicClass
{
public:
    explicit Creature(double x, double y, double def_speed_x, double def_speed_y,
                    double def_acceleration_x = 0, double default_acceleration_y = 0);

    virtual CR::CreatureType GetType() const = 0;
    virtual int WhenKilledAddScore() const { return 0; }
    virtual void Update(double dt, LevelPtr level);
    virtual void Draw() const;
    virtual void CheckCollisionsWithLevel(double dt, LevelPtr level);
    void SetSprites(SpritePtr left, SpritePtr right, SpritePtr stop);
    
    bool IsDead() const                
    { 
         return m_is_dead; 
    }
    bool IsAlive() const               
    { 
         return !m_is_dead; 
    }
    
    void SetIsDead(bool is_dead=true)  
    { 
         m_is_dead = is_dead; 
    }
    
    void KilledByPlayer();
    void KilledbyBullet();

// metody dotycz¹ce po³o¿enia
     
    double GetX() const 
    { 
           return m_x; 
    }
    
    double GetY() const 
    { 
           return m_y; 
    }
    
    virtual double GetNextXPosition(double dt) const 
    { 
            return m_x + GetNextXSpeed(dt) * dt; 
    }
    
    virtual double GetNextYPosition(double dt) const 
    { 
            return m_y + GetNextYSpeed(dt) * dt; 
    }
    
    void SetX(double newx)                   
    { 
         m_x = newx; 
    }
    
    void SetY(double newy)                   
    { 
         m_y = newy; 
    }
    
    void SetPosition(double x, double y)     
    { 
         m_x = x; m_y = y;
    }

// metody dotycz¹ce predkoœci
    
    double GetXSpeed() const              
    { 
           return m_sx; 
    }
    
    double GetYSpeed() const              
    { 
           return m_sy; 
    }
    
    double GetNextXSpeed(double dt) const 
    { 
           return m_sx + m_ax * dt; 
    }
    
    double GetNextYSpeed(double dt) const 
    { 
           return m_sy + m_ay * dt; 
    }
    
    double GetDefaultXSpeed() const       
    { 
           return m_default_speed_x; 
    }
    
    double GetDefaultYSpeed() const      
    { 
           return m_default_speed_y; 
    }
    
    void  ReverseXSpeed()                  
    {
           m_sx = -m_sx;
    }
    
    void  ReverseYSpeed()                  
    { 
          m_sy = -m_sy; 
    }
    
    void ReverseSpeed()                   
    { 
         ReverseXSpeed(); 
         ReverseYSpeed(); 
    }
    
    void  SetXSpeed(double speed)      
    {
          m_sx = speed; 
    }
    
    void  SetYSpeed(double speed)      
    {
          m_sy = speed; 
    }
    
    void  SetSpeed(double sx, double sy)  
    {
           m_sx = sx; m_sy = sy; 
    }

// metody dotycz¹ce przyspieszenia

    double GetXAcceleration() const          
    { 
           return m_ax; 
    }
    
    double GetYAcceleration() const          
    { 
           return m_ay; 
    }
    
    double GetDefaultXAcceleration() const   
    { 
           return m_default_acceleration_x; 
    }
    
    double GetDefaultYAcceleration() const  
    { 
           return m_default_acceleration_y;
    }
    
    void  SetXAcceleration(double acceleration)     
    {
           m_ax = acceleration; 
    }
    
    void  SetYAcceleration(double acceleration)     
    { 
          m_ay = acceleration;
    }

//zmiana poruszania sie jednostki
    virtual void GoLeft()       
    { 
            m_sx -= GetDefaultXSpeed();
            m_state=ES::GoLeft; 
    }
    
    virtual void GoRight()      
    { 
            m_sx += GetDefaultXSpeed(); 
            m_state=ES::GoRight; 
    }
    
    virtual void StopLeft()     
    { 
            m_sx += GetDefaultXSpeed();
            m_state=ES::Stand;  
    }
    
    virtual void StopRight()    
    { 
            m_sx -= GetDefaultXSpeed();
            m_state=ES::Stand; 
    }
    
    virtual void StopMove() 
    { 
            m_sx = 0; 
            m_state = ES::Stand;
    }
    void CannotGoLeft()      
    { 
         m_can_go_left = false; 
    }
    
    void CannotGoRight()     
    {
         m_can_go_right = false;
    }
    
    void Fall()                 
    { 
         m_sy =  0.0;
         m_is_on_ground = false; 
    }
    
    virtual void SetDefaultMove()  
    {
             m_is_on_ground = false; 
             m_can_go_right = m_can_go_left = true; 
    }
             
    void CreatureOnGround() 
    {
        m_is_on_ground = true;
        m_sy = 0;
    }

// metody zbieraj¹ce informacje na temat pól wokó³ jednostki

   bool IsFieldOnLeft(double dt, LevelPtr level) const;
   bool IsFieldOnRight(double dt, LevelPtr level) const;
   bool DoFieldsEndOnLeft(double dt, LevelPtr level) const;
   bool DoFieldsEndOnRight(double dt, LevelPtr level) const;
   bool IsFieldBelowCreature(double dt, LevelPtr level) const;
   bool IsFieldAboveCreature(double dt, LevelPtr level) const;
    

// zapisywanie pod argumenty x i y numeru kafla

    void GetCurrentTile(size_t *x, size_t *y) const 
    {
        const size_t v_tiles_count = Engine::Get().GetVisualization()->GetVerticalTilesOnScreenCount();
        *y = v_tiles_count - (GetAabb().GetMinY() + GetAabb().GetMaxY())/2;
        *x = GetX() + GetBasicAabb().GetMaxX()/2;
    }

// prostok¹t otaczaj¹cy jednostke bez uwzgledniania pozycji jednostki

    virtual Aabb GetBasicAabb() const 
    { 
            return Aabb(0, 0, 1, 1);  
    }

    Aabb GetAabb() const 
    { 
         return GetBasicAabb().Move(m_x, m_y, m_x, m_y); 
    }

    Aabb GetNextAabb(double dt) const 
    {
        return GetBasicAabb().Move(GetNextXPosition(dt), GetNextYPosition(dt),
                                   GetNextXPosition(dt), GetNextYPosition(dt) );
    }
    
    Aabb GetNextHorizontalAabb(double dt) const 
    {
        return GetBasicAabb().Move(GetNextXPosition(dt), m_y, GetNextXPosition(dt), m_y);
    }

    Aabb GetNextVerticalAabb(double dt) const 
    {
        return GetBasicAabb().Move(m_x, GetNextYPosition(dt), m_x, GetNextYPosition(dt));
    }

    

private:
    double m_default_speed_x;     
    double m_default_speed_y;
    double m_default_acceleration_x;  
    double m_default_acceleration_y;

    bool m_is_dead;           

protected:
    ES::EntityState m_state;  
    
    bool m_is_on_ground;     
    bool m_can_go_left;       
    bool m_can_go_right; 
            
    double m_x;              
    double m_y;               
    double m_sx;             
    double m_sy;              
    double m_ax;           
    double m_ay;              
    
    SpritePtr m_left;         
    SpritePtr m_right;       
    SpritePtr m_stop;
};

typedef boost::shared_ptr<Creature> CreaturePtr;

#endif
