#include "Engine.h"
#include "Sprite.h"

Sprite::Sprite(const DataConfigurator& data) 
                     :m_data(data), m_current_frame(0),m_current_frame_duration(0.0),
                      m_width_repeat(-1), m_height_repeat(-1) 
{
}

void Sprite::SetCurrentFrame(size_t frame_num) 
{
    m_current_frame = frame_num;
    m_current_frame_duration = 0.0;

}

void Sprite::Update(double dt) 
{
    m_current_frame_duration += dt;
    
    if (m_current_frame_duration >= m_data.frame_duration_time) 
    {
        m_current_frame++;
        m_current_frame_duration -= m_data.frame_duration_time;
    }
// sprawdzanie czy nast¹pi³ koniec animacji
    if (m_current_frame >= m_data.frame_count) 
    {
        if (m_data.loop) 
        {
            m_current_frame = 0;
        } else 
        {
            m_current_frame = m_data.frame_count - 1;
        }
    }
}

void Sprite::DrawCurrentFrame(double x, double y, double width, double height) 
{
// je¿eli nie jest aktywne powtarzanie to rysuje normalnie
    if (m_width_repeat < 0 && m_height_repeat < 0) 
    {
        Engine::Get().GetVisualization()->DrawSprite(
            m_data.left + m_data.width * m_current_frame, m_data.bottom,
            m_data.width, m_data.height, x, y, width, height, m_data.layer);
        return;
    }

//rysowanie z powtarzaniem

    const double rep_width = m_width_repeat;                           // rozmiary powtarzanego sprite'a
    const double rep_height = m_height_repeat;
    const int whole_in_x = static_cast<int>(width / rep_width);        // liczba ca³ych spritów dla osi
    const int whole_in_y = static_cast<int>(height / rep_height);
    const double scrap_width = width - whole_in_x * rep_width;         // rozmiar skrawka
    const double scrap_height = height - whole_in_y * rep_height;
    const double tex_x = m_data.left + m_data.width * m_current_frame; // po³o¿enie sprite'a w teksturze
    const double tex_y = m_data.bottom;

//rysowanie ca³ego sprit'a
    for (int ix=0; ix < whole_in_x; ++ix) 
    {
        for (int iy=0; iy < whole_in_y; ++iy) 
        {
            Engine::Get().GetVisualization()->DrawSprite(tex_x,tex_y,m_data.width,
                                                         m_data.height,                               
                                                         x + rep_width * ix,  y + rep_height * iy,                      
                                                         rep_width,  rep_height,                                     
                                                         m_data.layer);
        }
    }

//dla ka¿dego y-ka, rysuj kawa³ek x-ksa
    for (int iy=0; iy < whole_in_y && scrap_width > 0; ++iy) 
    {
        Engine::Get().GetVisualization()->DrawSprite(tex_x,  tex_y,                                                    
            m_data.width * (scrap_width/rep_width),  m_data.height,            
            x + rep_width * whole_in_x,   y + rep_height * iy,                 
            scrap_width,  std::max(scrap_height,  rep_height),                 
            m_data.layer);
    }

//dla ka¿dego x-ksa, rysuj kawa³ek y-ka
    for (int ix=0; ix < whole_in_x && scrap_height > 0; ++ix) 
    {
        Engine::Get().GetVisualization()->DrawSprite(
            tex_x,  tex_y,                                                     
            m_data.width,  m_data.height * (scrap_height/rep_height),          
            x + rep_width * ix,  y + rep_height * whole_in_y,                 
            std::max(scrap_width,  rep_width),  scrap_height,                 
            m_data.layer);
    }

//naro¿niki zaweiraj¹ kawa³ek x-ksa i y-ka
    if (scrap_height > 0 && scrap_width > 0) 
    {
        Engine::Get().GetVisualization()->DrawSprite(
            tex_x,  tex_y,                                                                       // pozycja textury
            m_data.width * (scrap_width/rep_width),  m_data.height * (scrap_height/rep_height),  // rozmiar textury
            x + rep_width * whole_in_x,  y + rep_height * whole_in_y,                            // pozycja ekranu
            scrap_width,  scrap_height,                                                          // rozmiar ekranu
            m_data.layer);
    }
}

void Sprite::SetRepeat(double width, double height) 
{
    m_width_repeat = width;
    m_height_repeat = height;
}

void Sprite::ResetRepeat() 
{
    m_height_repeat = m_width_repeat = -1;
}

