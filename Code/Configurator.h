#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

#include "Types.h"

struct DataConfigurator
{
     /* frame_duration - czas trwania klatki
     left, bottom, width oraz height to wspó³rzedne klatek
     layer - poziom, na którym wyœwietlane s¹ sprity */
     
    explicit DataConfigurator(DL::DisplayLayer layer, size_t frame_count, double frame_duration, double left,
                              double bottom, double width, double height, bool loop)
                               :layer(layer), frame_count(frame_count), 
                               frame_duration_time(frame_duration),left(left), 
                               bottom(bottom), width(width), height(height), 
                               loop(loop) 
    {
        
    }

    DL::DisplayLayer layer;       // warstwa rysowania sprita
    size_t frame_count;           // liczba klatek w animacji
    double frame_duration_time;   // czas trwania klatki
    double left;   
    double bottom; 
    double width;  
    double height; 
    bool loop;     ///czy zapetlac animacje??
};

class Configurator
{
public:
    explicit Configurator();
    DataConfigurator Get(const std::string& name) const;
    bool Contains(const std::string& name) const;

private:
    std::map<std::string, DataConfigurator> m_data;
    void Insert(const std::string& name, const DataConfigurator& data);
};

typedef boost::shared_ptr<Configurator> ConfiguratorPtr;

#endif
