#ifndef __ADD_SCORE_H__
#define __ADD_SCORE_H__

#include <utility>
#include <boost/shared_ptr.hpp>

#include "ApplicationState.h"


class AddScore : public ApplicationState 
{
private:
    struct Entry 
    {
        std::string name;
        size_t points;
    };

public:
    explicit AddScore(size_t points);

    void ProcessEvents(const SDL_Event& event);
    void Draw();
    bool Update(double dt);

    void Initialization();
    void Start();

    bool IsDone() const 
    { 
         return m_is_done; 
    }

    ApplicationStatePtr NextApplicationState() const;

private:
    std::pair<double, double> LetterPosition(char ch);
    void StoreInFile();

private:
    bool m_is_done;
    std::string m_player_nickname;
    size_t m_next_letter;
    size_t m_points;
    char m_highlighted_char;
    
};

typedef boost::shared_ptr<AddScore> AddScorePtr; 

#endif
