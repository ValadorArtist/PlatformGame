#ifndef __BULLET_CREATOR_H__
#define __BULLET_CREATOR_H__

#include "Creator.h"
#include "BulletEntity.h"
#include "Game.h"


class BulletCreator : public Creator 
{
public:
    explicit BulletCreator(double x, double y, double vx, double vy)
        : m_x(x), m_y(y), m_sx(vx), m_sy(vy)
    {
        }

    void Create(Game& game) 
    {
//tworzenie jednostki i nadanie jej pocisku i zwrotu zgodnego z graczem
        CreaturePtr creature = Engine::Get().GetCreatureFactory()->CreateCreature(CR::PlayerBullet, m_x, m_y);
        double x_speed = m_sx < 0 ? -creature->GetDefaultXSpeed() : creature->GetDefaultXSpeed();
        double y_speed = creature->GetDefaultYSpeed();
        
        creature->SetSpeed(x_speed + m_sx, y_speed + m_sy);

//dodawanie pociku i dzwieku lasera
        if (game.CanAddCreature(creature)) 
        {
            game.AddCreature(creature);
            Engine::Get().GetSound()->PlaySfx("bullet");
        }
    }

private:
    double m_x, m_y;    //po³o¿enie
    double m_sx, m_sy;  //predkosc
};

#endif
