#include <iostream>

#include "Engine.h"
#include "MushCreature.h"
#include "BulletEntity.h"
#include "TwinShotUpgrade.h"
#include "CreatureFactory.h"


CreatureFactory::CreatureFactory() 
{
}

SpritePtr GetSpriteByName(const std::string& name) 
{
    return SpritePtr(new Sprite(Engine::Get().GetConfigurator()->Get(name)));
}

CreaturePtr CreatureFactory::CreateCreature(CR::CreatureType type, double x, double y) 
{
    const double eps = 0.0001;
    x += eps;  y += eps;

    CreaturePtr ptr;
    if(type==CR::Mush) 
    {
        ptr.reset(new MushCreature(x, y));
        ptr->SetSprites(GetSpriteByName("mush_left"), GetSpriteByName("mush_right"), GetSpriteByName("mush_stop") );
    } else 
    if (type == CR::PlayerBullet) 
    {
        ptr.reset(new BulletEntity(x, y));
        SpritePtr bullet = GetSpriteByName("player_bullet");
        ptr->SetSprites(bullet, bullet, bullet);
    } else 
    if (type == CR::TwinShot) 
    {
        ptr.reset(new TwinShotUpgrade(x, y));
        SpritePtr bullet = GetSpriteByName("twinshot_upgrade");
        ptr->SetSprites(bullet, bullet, bullet);
    }

    if (!ptr) 
    {
        std::cerr << "Fabryka nie potrafi stworzyc obiektu: " << type 
        << ", " << x << ", " << y << std::endl;
    }
    return ptr;
}


CreaturePtr CreatureFactory::CreateCreature(const std::string& name, double x, double y)
{
    if (name == "mush") 
    {
        return CreateCreature(CR::Mush, x, y);
    } else 
    if (name == "bullet") 
    {
        return CreateCreature(CR::PlayerBullet, x, y);
    } else 
    if(name=="twinshot_upgrade") 
    {
        return CreateCreature(CR::TwinShot, x, y);
    }
    std::cerr << "Fabryka nie potrafi stworzyc obiektu: " << name << std::endl;
    return CreaturePtr();
}
