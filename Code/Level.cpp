#include <fstream>
#include <iostream> 
#include <sstream> 
#include <algorithm>

#include "Engine.h"
#include "Level.h"


Level::Level() : m_width(0),m_height(0),m_loaded(false) 
{
}

void Level::LoadFromFile(const std::string& filename) 
{
    std::ifstream lvl(filename.c_str());
    if (!lvl) 
    {
        std::cerr << "Nie za³adowano danych z pliku, b³¹d dzia³ania " << filename << "\n";
        m_loaded = false;
        return;
    }

    lvl >> m_width >> m_height;
    m_data.resize(m_height);
    for (size_t y = 0; y < m_height; ++y) 
    {
        m_data.at(y).resize(m_width);
    }

    int tmp;
    for (size_t y = 0; y < m_height; ++y) 
    {
        for (size_t x = 0; x < m_width; ++x) 
        {
            lvl >> tmp;
            m_data.at(y).at(x) = FT::FieldType(tmp);
        }
    }

    m_loaded = true;
}


void Level::LoadCreaturesFromFile(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file) 
    {
        std::cerr << "Nie za³adowano danych z pliku, b³¹d dzia³ania " << filename << "\n";
        return;
    }

// wczytywanie
    const int buffer_size = 1024;
    char buffer[buffer_size];
    while (file) 
    {
        file.getline(buffer, buffer_size);
        std::string line(buffer);
        if (line.length() < 5 || line.at(0) == '#')
            continue;
        std::istringstream iss(line);
        LevelCreatureData data;
        iss >> data.name;
        iss >> data.x;
        iss >> data.y;
        if (data.name == "player") 
        {
            m_player_data = data;
        } else 
        {
            m_creatures_to_create.push_back(data);
        }
    }

// sorotwanie wczytanych recordów
    m_creatures_to_create.sort(LevelCreatureData());
}


FT::FieldType Level::Field(size_t x, size_t y) const 
{
    if (x >= m_width || y >= m_height) 
    {
        return FT::None;
    }
    return m_data.at(y).at(x);
}


Aabb Level::GetFieldAabb(size_t x, size_t y) const 
{
    VisualizationPtr visualization = Engine::Get().GetVisualization();
    const size_t v_tiles_count = visualization->GetVerticalTilesOnScreenCount();
    y = v_tiles_count - y;  // odbicie w pionie

    Aabb box = Aabb(x, y-1, x + 1, y);
    return box;
}
