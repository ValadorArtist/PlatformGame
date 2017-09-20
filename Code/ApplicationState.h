#ifndef __APPLICATION_STATE_H__
#define __APPLICATION_STATE_H__

#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>

class ApplicationState
{
public:
// metody klasy Application_State z przypisanymi wartosciami
    explicit ApplicationState() 
             : m_is_done(false) 
    {
    }
    
    virtual void Start() = 0;
    virtual void Initialization() = 0;
    

    virtual void Draw() = 0;
    virtual bool Update(double dt) = 0;
    virtual void ProcessEvents(const SDL_Event& event) = 0;

    virtual boost::shared_ptr<ApplicationState> NextApplicationState() const = 0;

    bool IsDone() const 
    {
        return m_is_done;
    }

    void SetDone(bool value=true) 
    {
        m_is_done = value;
    }

private:
//pole klasy Application_State
    bool m_is_done;
};

typedef boost::shared_ptr<ApplicationState> ApplicationStatePtr;

#endif
