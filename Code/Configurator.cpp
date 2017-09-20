#include <iostream>

#include "Configurator.h"
#include "Types.h"


Configurator::Configurator() 
{
    Insert("player_right", DataConfigurator(DL::Player, 4, 0.1, 0, 4 * 32, 32, 32, true));
    Insert("player_left",  DataConfigurator(DL::Player, 4, 0.1, 0, 5 * 32, 32, 32, true));
    Insert("player_stop",  DataConfigurator(DL::Player, 8, 0.1, 0, 6 * 32, 32, 32, true));

    Insert("platform_left",  DataConfigurator(DL::Foreground, 1, 1, 0, 1*32, 32, 32, true));
    Insert("platform_mid",   DataConfigurator(DL::Foreground, 1, 1, 0, 2*32, 32, 32, true));
    Insert("platform_right", DataConfigurator(DL::Foreground, 1, 1, 0, 3*32, 32, 32, true));
    Insert("end_of_level",   DataConfigurator(DL::Foreground, 1, 1, 32, 2*32, 32, 32, true));

    Insert("mush_right", DataConfigurator(DL::Entity, 4, 0.3,  0, 11 * 32, 32, 32, true));
    Insert("mush_left",  DataConfigurator(DL::Entity, 4, 0.3,  0, 11 * 32, 32, 32, true));
    Insert("mush_stop",  DataConfigurator(DL::Entity, 1, 0.3,  0, 12 * 32, 32, 32, true));

    Insert("player_bullet",  DataConfigurator(DL::Entity, 4, 0.3,  5*32, 11*32, 32, 32, true));

    Insert("twinshot_upgrade", DataConfigurator(DL::Entity, 2, 0.3,  0*32, 13*32, 32, 32, true));
}

DataConfigurator Configurator::Get(const std::string& name) const 
{
    if (Contains(name)) 
    {
        return m_data.find(name)->second;
    }
    std::cerr << "Nie zanleziono odpowiedniej konfiguracji: " << name << std::endl;
    throw("Nie znaleziono odpowiedniej konfiguracji: " + name);
}

bool Configurator::Contains(const std::string& name) const 
{
    return (m_data.find(name) != m_data.end());
}

void Configurator::Insert(const std::string& name, const DataConfigurator& data) 
{
    m_data.insert(std::make_pair(name, data));
}

