#ifndef __LEVELCHOICE_H__
#define __LEVELCHOICE_H__

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "ApplicationState.h"
#include "Sprite.h"

class Player;
typedef boost::shared_ptr<Player> PlayerPtr;


class LevelChoice: public ApplicationState, public boost::enable_shared_from_this<LevelChoice> 
{
public:
    explicit LevelChoice(PlayerPtr player);
    virtual ~LevelChoice();
    void Initialization();
    void Start();

    void Draw();
    bool Update(double dt);
    void ProcessEvents(const SDL_Event& event);

    boost::shared_ptr<ApplicationState> NextApplicationState() const;

    void SetPlayer(PlayerPtr player);

private:

// kolejny weze³
    int NextNode(size_t node) const;
// nazwa poziomu
    std::string NodeToLevelName(int node);

    bool GoLeft();
    bool GoRight();
    bool GoUpward();
    bool GoDown();
    
    void RunLevelFromNode(); // uruchamianie poziomu na podstawie wez³a
    void DrawRoad(size_t from, size_t to) const;

    struct Point 
    {
        Point(double x, double y) : x(x), y(y) 
        {}
        
        double operator[](int idx) const
        { 
               return (idx==0?x:y); 
        }
        
        double& operator[](int idx) 
        { 
                return (idx==0?x:y); 
        }
        double x, y;
    };

    typedef std::vector<bool> BoolVector;
    typedef std::vector<int> IntVector;

    std::vector<Point> m_positions;                    // pozycje punktów
    std::vector<IntVector> m_connections;              // po³¹czenia wez³ów
    std::map<int, std::string> m_node_to_level_name;   // nazwa poziomu

    Point m_face_pos;               // po³ozenie gracza na ekranie
    int m_current_from_node;        // numer wez³a pocz¹tkowego
    int m_current_to_node;          // numer wez³a docelowego

    SpritePtr m_horizontal_road_sprite;  // animacja drogi w poziomie
    SpritePtr m_vertical_road_sprite;    // animacja drogi w pionie
    SpritePtr m_entry_enabled_sprite;    // animacja aktywnego wejœcia
    SpritePtr m_entry_disabled_sprite;   // animacja nieaktywnego wejœcia
    SpritePtr m_face_sprite;             // animacja sprite'a bohatera

    double m_tile_width;             
    double m_tile_height;         

    PlayerPtr m_player;             // wskaŸnik na gracza
    boost::shared_ptr<ApplicationState> m_next_application_state;
};
#endif
