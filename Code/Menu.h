#ifndef __MENU_H__
#define __MENU_H__

#include "ApplicationState.h"

class Menu : public ApplicationState 
{
private:
    struct Sel 
    {
        enum Selection 
        {
            None,
            NewGame,
            WallOfGlory,
            Quit
        };
    };

public:
    explicit Menu(): m_selection(Sel::NewGame) 
    {
    }

    void Initialization();
    void Start();

    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<ApplicationState> NextApplicationState() const;

private: 
    Sel::Selection m_selection;
    ApplicationStatePtr m_next_application_state;
};

#endif
