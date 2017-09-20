#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include "Configurator.h"

class Sprite 
{
public:
    explicit Sprite(const DataConfigurator& data);

    void Update(double dt);
    void SetCurrentFrame(size_t frame_num);
    void DrawCurrentFrame(double x, double y, double width, double height);

    void SetRepeat(double width, double height);

//Sprite rozci¹gany a nie powtarzany
    void ResetRepeat();

private:
    DataConfigurator m_data;
    size_t m_current_frame;           // numer aktualnej klatki
    double m_current_frame_duration;  // czas trwania aktualnej klatki

    double m_width_repeat;            // szerokosc powy¿ej której sprite jest powtarzany
    double m_height_repeat;           // wysokosc powy¿ej której sprite jest powtarzany
};

typedef boost::shared_ptr<Sprite> SpritePtr;

#endif
