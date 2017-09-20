#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include <list>

#include "Aabb.h"
#include "Types.h"

/* Na podstawie danych z pliku tworzone s¹ jednostki*/

struct LevelCreatureData 
{
    LevelCreatureData() : name("[unknown]"), x(-1), y(-1) 
    {  
                        }

    LevelCreatureData(const std::string& name, double x, double y)
                      : name(name), x(x), y(y) 
    {   
                        }

    bool operator() (const LevelCreatureData& a, const LevelCreatureData& b) 
    {
        return a.x < b.x;
    }

    std::string name;  // nazwa jednostki
    double x;          // po³ozenie na osi X
    double y;          // po³o¿enie na osi Y
};


class Level 
{
public:
    explicit Level();

    void LoadFromFile(const std::string& filename);
    void LoadCreaturesFromFile(const std::string& filename);
    FT::FieldType Field(size_t x, size_t y) const;
    
    std::list<LevelCreatureData> GetAllCreaturesToCreate() const  
    { 
                      return m_creatures_to_create; 
    }
    
    LevelCreatureData GetPlayerData() const 
    { 
                      return m_player_data; 
    }

    
    size_t GetWidth() const   
    { 
           return m_width; 
    }
    
    size_t GetHeight() const  
    { 
           return m_height; 
    }
    
    Aabb GetFieldAabb(size_t x, size_t y) const;

    bool GetLoaded() const 
    {
        return m_loaded;
    }

    
private:
    size_t m_width;
    size_t m_height;
    std::vector<std::vector<FT::FieldType> > m_data;
    std::list<LevelCreatureData> m_creatures_to_create;
    LevelCreatureData m_player_data;
    bool m_loaded;

};
typedef boost::shared_ptr<Level> LevelPtr;

#endif
