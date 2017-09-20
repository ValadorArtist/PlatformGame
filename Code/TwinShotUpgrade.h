#ifndef __TWIN_SHOT_UPGRADE_H__
#define __TWIN_SHOT_UPGRADE_H__
#include "Creature.h"

class TwinShotUpgrade : public Creature 
{
public:
    TwinShotUpgrade(double x, double y)
        : Creature(x,y, 0, 0) 
        {
                        }

    CR::CreatureType GetType() const 
    { 
                     return CR::TwinShot; 
                     }
};

#endif
