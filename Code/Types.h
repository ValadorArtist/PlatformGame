#ifndef __TYPES_H__
#define __TYPES_H__

namespace PS 
{
    enum PlayerState 
    {
        Stand,
        GoLeft,
        GoRight,
        TurnLeft,  
        TurnRight 
    };
}

namespace DL 
{
    enum DisplayLayer 
    {
        Foreground = 1,  // przedni plan
        Player = 2,      // plan z graczem
        Entity = 2,      // plan z jednostkami
        Background = 3   // t³o
    };
}

namespace FT 
{
    enum FieldType 
    {
        None = 0,
        PlatformLeftEnd = 1,
        PlatformMidPart = 2,
        PlatformRightEnd = 3,

        EndOfLevel = 4,

        COUNT
    };
}

// stan jednostki
namespace ES 
{
    enum EntityState 
    {
        Stand,
        GoLeft,
        GoRight
    };
}

// typ jednostki
namespace CR {
    enum CreatureType 
    {
        Mush,
        PlayerBullet,
        TwinShot
    };
}

#endif
