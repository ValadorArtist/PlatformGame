#include <cassert>
#include <cmath>
#include <iostream>

#include "Game.h"
#include "Engine.h"
#include "CreatureFactory.h"
#include "Text.h"
#include "WallOfGlory.h"
#include "AddScore.h"
#include "LevelChoice.h"
#include "Creator.h"


void Game::ProcessEvents(const SDL_Event& event) 
{
    if (IsDone()) 
    {
        return;
    }

//przegl¹danie zdarzeñ
    if (event.type == SDL_QUIT) 
    {
        SetDone();
    } else 
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) 
    {
        m_next_application_state = m_level_choice;
        m_next_application_state->Initialization();
        SetDone();
    } else 
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d) 
    {
        m_player->Run();
    } else 
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_d) 
    {
        m_player->StopRunning();
    } else 
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) 
    {
        m_player->FireBullet();
    } else 
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) 
    {
        m_player->Jump();
    } else
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) 
    {
        m_player->GoLeft();
    } else 
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) 
    {
        m_player->GoRight();
    } else 
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT) 
    {
        m_player->StopLeft();
    } else 
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT) 
    {
        m_player->StopRight();
    }
}

void Game::Start() 
{
}

void Game::Initialization() 
{
    Engine& engine = Engine::Get();

// ³adowanie poziomu i spritów na plansze
    m_level.reset(new Level());
    m_level->LoadFromFile("data/" + m_level_name + ".lvl");
    if (!m_level->GetLoaded())
    {
        m_level_name = "1";
        m_level->LoadFromFile("data/" + m_level_name + ".lvl");
    }

// ³adowanie jendostek na plansze
    m_level->LoadCreaturesFromFile("data/" + m_level_name + ".ents");
    m_creatures_to_create = m_level->GetAllCreaturesToCreate();

    m_level_view.StoreSprite(FT::PlatformLeftEnd, SpritePtr(new Sprite(engine.GetConfigurator()->Get("platform_left"))));
    m_level_view.StoreSprite(FT::PlatformMidPart, SpritePtr(new Sprite(engine.GetConfigurator()->Get("platform_mid"))));
    m_level_view.StoreSprite(FT::PlatformRightEnd, SpritePtr(new Sprite(engine.GetConfigurator()->Get("platform_right"))));
    m_level_view.StoreSprite(FT::EndOfLevel, SpritePtr(new Sprite(engine.GetConfigurator()->Get("end_of_level"))));

// tworzenie postaci
    const LevelCreatureData player_data = m_level->GetPlayerData();
    if(!m_player) 
    {
        if (player_data.name == "player") 
        {
            m_player.reset(new Player(player_data.x, player_data.y, m_level->GetWidth(),3,0));
            m_player->SetSprites(SpritePtr(new Sprite(engine.GetConfigurator()->Get("player_left"))),
                                 SpritePtr(new Sprite(engine.GetConfigurator()->Get("player_right"))),
                                 SpritePtr(new Sprite(engine.GetConfigurator()->Get("player_stop"))) );
        } else 
        {
            std::cerr << "Brak informacji o postaci w pliku z poziomem" << std::endl;
        }
    }
    else 
    {
        m_stored_player_position_x = 9;
        m_player->NewLevelReset(m_level);
    }
}

void Game::CheckPlayerAndCreaturesCollisions(double dt) 
{
// poziomy i pionowy prostok¹t gracza w nastepnym kroku
    Aabb player_box_x = m_player->GetNextHorizontalAabb(dt);
    Aabb player_box_y = m_player->GetNextVerticalAabb(dt);

    for (std::vector<CreaturePtr>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) 
    {
        CreaturePtr creature = *it;
        const CR::CreatureType creature_type = creature->GetType();

        if (creature_type == CR::PlayerBullet) 
        {
// brak kolizji pocisku z graczem
            continue;
        } else 
        if (creature_type == CR::TwinShot) 
        {
// czy otrzymano bonus??
            if (m_player->GetAabb().Collides(creature->GetAabb())) 
            {
                m_player->EnableDoubleShot();
                creature->SetIsDead(true);
            }
            continue;
        }

        if (m_player->IsImmortal()) 
        {
            continue;
        }

        creature->SetDefaultMove();
        Aabb creature_box = creature->GetAabb();

// sprawdzanie czy zasz³a kolizja
        if (player_box_y.IsOver(creature_box)) 
        {
// naskoczenie na jednostke
            m_player->CollisionUnderPlayer(creature);
            player_box_y = m_player->GetNextVerticalAabb(dt);
        }
        else 
        if (player_box_x.IsOnLeftOf(creature_box)) 
        {
            m_player->CannotGoRight();
            m_player->CollisionOnRight(creature);
            player_box_x = m_player->GetNextHorizontalAabb(dt);
        }
        else 
        if (player_box_x.IsOnRightOf(creature_box)) 
        {
            m_player->CannotGoLeft();
            m_player->CollisionOnLeft(creature);
            player_box_x = m_player->GetNextHorizontalAabb(dt);
        }
        else 
        if (player_box_y.IsUnder(creature_box)) 
        {
            m_player->Fall();
            m_player->CollisionOverPlayer(creature);
            player_box_y = m_player->GetNextVerticalAabb(dt);
        }
    }
}

void Game::CheckCollisionOfOnePair(CreaturePtr fst_creature, CR::CreatureType fst_type,
                                   CreaturePtr snd_creature, CR::CreatureType snd_type,
                                   double dt) 
    {
    if (fst_creature->GetNextAabb(dt).Collides(snd_creature->GetNextAabb(dt)) == false) 
    {
        return;
    }

    // makro SWAP_IF zezwala na zamiane znaczenia argumentów fst_* z snd_*
    // dzieki temu otrzymujemy wskazniki kolizji w dobrej kolejnoœci
    
#define SWAP_IF( type_a, type_b )  \
    if (fst_type == type_a && snd_type == type_b) {  \
    	std::swap(fst_creature, snd_creature);   \
        std::swap(fst_type, snd_type);   \
    }

    SWAP_IF( CR::PlayerBullet, CR::Mush )

    /*je¿eli nast¹pi³a kolizja Mush z Bullet, to jednostka
     Mush bedzie pod fst_creature a Bullet pod snd_creature */

    if (fst_type == CR::Mush && snd_type == CR::PlayerBullet) 
    {
        snd_creature->SetIsDead(true);
        m_player->AddScores(fst_creature->WhenKilledAddScore());
        fst_creature->KilledbyBullet();
    }

    if (fst_type == CR::Mush && snd_type == CR::Mush) 
    {
        fst_creature->ReverseXSpeed();
        snd_creature->ReverseXSpeed();
    }

#undef SWAP_IF
}

void Game::CheckCreatureAndCreatureCollisions(double dt) 
{
// sprawdzenie ka¿dej pary jednokrotnie - O(n^2)
    std::vector<CreaturePtr>::iterator outer, inner;
    CreaturePtr out_creature, inn_creature;
    CR::CreatureType out_type, inn_type;
    for (outer = m_creatures.begin(); outer != m_creatures.end(); ++outer) 
    {
        out_creature = *outer;
        out_type = out_creature->GetType();
        if (out_creature->IsDead()) 
        {
            continue;
        }
        inner = outer;
        ++inner;
        for ( ; inner != m_creatures.end(); ++inner) 
        {
            inn_creature = *inner;
            inn_type = inn_creature->GetType();
            if (inn_creature->IsDead()) 
            {
                continue;
            }
            CheckCollisionOfOnePair(inn_creature, inn_type, out_creature, out_type, dt);
        }
    }
}

void Game::ExecuteCreators() 
{
    std::list<CreatorPtr> creators;

// pobierz kreatory od gracza
    std::list<CreatorPtr> plr_c = m_player->GetCreators();
    creators.splice(creators.end(), plr_c);
    m_player->DropAllCreators();

// pobierz kreatory z jednostek
    for (std::vector<CreaturePtr>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) 
    {
        CreaturePtr e = *it;
        if (!e->IsDead()) 
        {
            std::list<CreatorPtr> ent_c = e->GetCreators();
            creators.splice(creators.end(), ent_c);
            e->DropAllCreators();
        }
    }

// uruchamianie wszystkich kreatorów
    for (std::list<CreatorPtr>::iterator it = creators.begin(); it != creators.end(); ++it) 
    {
        (*it)->Create(*this);
    }
}

void Game::SeeAndAddCreatures(double /* dt */) 
{
// oznaczanie jednostek za lew¹ krawedzi¹ ekranu jako martwe
    const double distance_of_deletion = Engine::Get().GetVisualization()->GetHorizontalTilesOnScreenCount();
    for (std::vector<CreaturePtr>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) 
    {
        CreaturePtr e = *it;
        if (e->GetX() + distance_of_deletion < m_player->GetX()) 
        {
            e->SetIsDead(true);
        }
    }

// usuwanie martwych jednostek
    for (size_t i = 0; i < m_creatures.size(); ++i) 
    {
        if (m_creatures.at(i)->IsDead()) 
        {
            for (size_t c = i; c < m_creatures.size() - 1; ++c) 
            {
                m_creatures.at(c) = m_creatures.at(c + 1);
            }
            m_creatures.resize(m_creatures.size() - 1);
        }
    }

// dodawanie kolejnych jednostek
    const double distance_of_creation = Engine::Get().GetVisualization()->GetHorizontalTilesOnScreenCount();
    while (m_creatures_to_create.empty() == false) 
    {
        if (m_creatures_to_create.front().x - m_player->GetX() < distance_of_creation) 
        {
            LevelCreatureData data = m_creatures_to_create.front();
            m_creatures_to_create.pop_front();
            CreaturePtr e = Engine::Get().GetCreatureFactory()->CreateCreature(data.name, data.x, data.y);
            m_creatures.push_back(e);
        } else 
        {
            break;
        }
    }
}

void Game::BindLevelChoice(const boost::shared_ptr<LevelChoice>& screen) 
{
    m_level_choice = screen;
}

bool Game::Update(double dt) 
{
// czy gracz zakoñczy³ poziom??
    if (m_player->HasCompletedCurrentLevel()) 
    {
        m_level_choice->SetPlayer(m_player);
        m_next_application_state = m_level_choice;

        SetDone();
        return IsDone();
    }
    else 
    if (m_player->GetLifesCount() < 1) 
    {
        size_t score = m_player->GetScores();
        if (score != 0) 
        {
            m_next_application_state.reset(new AddScore(score));
        }
        else 
        {
            m_next_application_state.reset(new WallOfGlory());
        }
        SetDone();
    }

// zbierz nowe z ju¿ istniej¹cych obiektów
    ExecuteCreators();
    
// ustawianie domyœlnego ruch graczowi
    m_player->SetDefaultMove();

//sprawdzanie kolizji gracz-istota a tak¿e istota-istota
    CheckPlayerAndCreaturesCollisions(dt);
    CheckCreatureAndCreatureCollisions(dt);

// aktualizacja obiektu gracz
    m_player->Update(dt, m_level);

// aktualizacja stanu istot
    for (std::vector<CreaturePtr>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) 
    {
        CreaturePtr e = *it;
        if (e->IsAlive()) 
        {
            e->Update(dt, m_level);
        }
    }

//usuwanie i dodawanie nowych istot
    SeeAndAddCreatures(dt);

    return !IsDone();
}

void Game::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

// rysowanie mapy
    if (m_player->MoveMap()) 
    {
        m_stored_player_position_x = m_player->GetX();
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    {
        glTranslatef(-(m_stored_player_position_x * Engine::Get().GetVisualization()->GetTileWidth() - 0.45), 0, 0);
        glMatrixMode(GL_MODELVIEW);
    
        m_level_view.SetLevel(m_level, m_stored_player_position_x);
        m_level_view.Draw(m_stored_player_position_x);
        
// rysowanie postaci gracza
        m_player->Draw();

// rysowanie pozosta³ych obiektów
        for (std::vector<CreaturePtr>::const_iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) 
        {
            const CreaturePtr e = *it;
            if (e->IsAlive()) 
            {
                e->Draw();
            }
        }
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    SDL_GL_SwapBuffers();
}
