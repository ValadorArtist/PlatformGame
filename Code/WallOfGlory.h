#ifndef __WALL_OF_GLORY_H__
#define __WALL_OF_GLORY_H__

#include <string>
#include <vector> 
#include <boost/shared_ptr.hpp>

#include "ApplicationState.h"


class WallOfGlory : public ApplicationState
{
private:
    struct Entry 
    {
        std::string name;
        size_t points;
    };

public:
    explicit WallOfGlory();

    void Start();
    void Initialization();
    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);
    
    bool IsDone() const 
    { 
         return m_is_done; 
    }

    ApplicationStatePtr NextApplicationState() const;

private:
    void LoadFromFile();

private:
    bool m_is_done;
    std::vector<Entry> m_entries;
};

#endif
