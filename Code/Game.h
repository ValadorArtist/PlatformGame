#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <SDL/SDL.h>

#include "Convert.h"
#include "Player.h"
#include "Level.h"
#include "SpriteGrid.h"
#include "Creature.h"
#include "ApplicationState.h"

class LevelChoice;

class Game : public ApplicationState 
{
public:
    explicit Game(const std::string& level_name, PlayerPtr player) 
                  : m_player(player), m_stored_player_position_x(9.0), m_level_name(level_name)  
    {
    }

    void Start();
    void Initialization();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    void ExecuteCreators();
    bool CanAddCreature(CreaturePtr creature) const 
    {
        size_t current_tile_x, current_tile_y;
        creature->GetCurrentTile(&current_tile_x, &current_tile_y);
        if (m_level->Field(current_tile_x, current_tile_y) != FT::None) 
        {
            return false;
        }
        return true;
    }
    void AddCreature(CreaturePtr creature)
    {
        m_creatures.push_back(creature);
    }

    LevelPtr GetLevel() const 
    { 
             return m_level; 
    }

    ApplicationStatePtr NextApplicationState() const 
    {
        return m_next_application_state;
    }

    void BindLevelChoice(const boost::shared_ptr<LevelChoice>& screen);

private:
    void SeeAndAddCreatures(double dt);
    void CheckPlayerAndCreaturesCollisions(double dt);
    void CheckCreatureAndCreatureCollisions(double dt);
    void CheckCollisionOfOnePair(CreaturePtr fst_creature, CR::CreatureType fst_type,
                                 CreaturePtr snd_creature, CR::CreatureType snd_type, double dt);

    PlayerPtr m_player;
    std::vector<CreaturePtr> m_creatures;              
    std::list<LevelCreatureData> m_creatures_to_create;

    LevelPtr m_level;
    SpriteGrid m_level_view;
    double m_stored_player_position_x;

    std::string m_level_name;
    ApplicationStatePtr m_next_application_state;
    

    boost::shared_ptr<LevelChoice> m_level_choice;
};

typedef boost::shared_ptr<Game> GamePtr;

#endif
