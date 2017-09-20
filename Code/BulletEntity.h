#ifndef __BULLET_ENTITY_H__
#define __BULLET_ENTITY_H__

#include "Creature.h"
#include <iostream>


class BulletEntity : public Creature 
{
    enum 
    {
        DefaultXSpeed = 6, DefaultYSpeed = -2,
        DefaultXAcceleration = 0, DefaultYAcceleration = 0,
        DefaultTimeToLive = 2
    };

public:
    BulletEntity(double x, double y) :
        Creature(x, y, DefaultXSpeed, DefaultYSpeed, DefaultXAcceleration, DefaultYAcceleration),
        m_time_to_live(DefaultTimeToLive) 
    {
        }

    virtual CR::CreatureType GetType() const 
    { 
            return CR::PlayerBullet; 
            }
            
    virtual Aabb GetBasicAabb() const 
    {
        return Aabb(0, 0, .4, .4);
    }

    void SetDefaultMove() 
    {
    }

    void Update(double dt, LevelPtr level) 
    {
        m_right->Update(dt);

//usuwanie obiektu po pewnym czasie
        m_time_to_live -= dt;
        if (m_time_to_live < 0) 
        {
            SetIsDead(true);
            return;
        }
//sprawdzanie kolizji
        CheckCollisionsWithLevel(dt, level);
        SetPosition(GetNextXPosition(dt), GetNextYPosition(dt));
    }

    virtual void CheckCollisionsWithLevel(double dt, LevelPtr level) 
    {
    //sprawdzanie czy s¹ jakieœ kolizje z jakiejœ strony
        if (IsFieldBelowCreature(dt, level) || IsFieldAboveCreature(dt, level)) {
            ReverseYSpeed();
        }

        if (IsFieldOnLeft(dt, level)||IsFieldOnRight(dt, level)) {
            ReverseXSpeed();
        }
    }

private:
    double m_time_to_live;   // czas do samozniszczenia
};
typedef boost::shared_ptr<BulletEntity> BulletEntityPtr;

#endif
