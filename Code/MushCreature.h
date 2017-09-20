#ifndef __MUSH_CREATURE_H__
#define __MUSH_CREATURE_H__

#include <iostream>
#include "Types.h"
#include "Creature.h"


class MushCreature : public Creature
{
    enum 
    {
        DefaultXSpeed = 2, 
        DefaultYSpeed = 0, 
        DefaultXAcceleration = 0, 
        DefaultYAcceleration = -60
    };

public:
    MushCreature(double x, double y) :
        Creature(x, y, DefaultXSpeed, DefaultYSpeed, DefaultXAcceleration, DefaultYAcceleration) 
        {
                  }   

    CR::CreatureType GetType() const   
    { 
        return CR::Mush; 
    }
    int WhenKilledAddScore() const  
    { 
        return 100; 
    }

    Aabb GetBasicAabb() const 
    {
        return Aabb(.05, 0, .9, .9);
    }

    void CheckCollisionsWithLevel(double dt, LevelPtr level) 
    {
        if (GetXSpeed() == 0) 
        {
            GoLeft();
        }
        
        if (IsFieldBelowCreature(dt, level)) 
        {
            CreatureOnGround();
        }
        
        if (DoFieldsEndOnLeft(dt, level) || IsFieldOnLeft(dt, level)) 
        {
            StopMove();
            GoRight();
        }

        if (DoFieldsEndOnRight(dt, level) || IsFieldOnRight(dt, level)) 
        {
            StopMove();
            GoLeft();
        }
    }
};
typedef boost::shared_ptr<MushCreature> MushCreaturePtr;

#endif
