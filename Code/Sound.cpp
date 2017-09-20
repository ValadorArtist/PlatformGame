#include <algorithm>
#include <iostream> 
#include <fstream>

#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

#include "Sound.h"


Sound::Sound() {
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
 
    if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) 
    {
        std::cout << "nie mo¿na zainicjalizowac muzyki: " << Mix_GetError() << "\n";
	exit(1);
    }
}

void Sound::LoadSounds() 
{
    std::ifstream settings("data/sounds.txt");
    
    std::string type;
    std::string name;
    std::string filename;
    while (settings) 
    {
        settings >> type >> name;
        std::getline(settings, filename);
        filename.erase(std::remove(filename.begin(), filename.end(), ' '), filename.end());
        if (type == "sfx") 
        {
            LoadSfx(name, filename);
        }
        else 
        {
            std::cout << "Nieznany format: '" << type << "'\n";
        }
    }
}

void Sound::LoadMusic(const std::string& name, const std::string& filename) 
{
    Mix_Music* m = Mix_LoadMUS(filename.c_str());
    if (m) 
    {
        m_music.insert(std::make_pair(name, m));
    }
    else 
    {
        std::cout << "Nie mo¿na wczytac pliku " << filename << "\n";
    }
}

void Sound::LoadSfx(const std::string& name, const std::string& filename) 
{
    Mix_Chunk* c = Mix_LoadWAV(filename.c_str());
    if (c) 
    {
        m_sfx.insert(std::make_pair(name, c));
    }
    else 
    {
        std::cout << "Nie mo¿na wczytac pliku sfx" << filename << "\n";
    }
}

void Sound::PlaySfx(const std::string& name) 
{
    if (m_sfx.find(name) == m_sfx.end()) 
    {
        std::cout << "Nieznany sfx '" << name << "'\n";
    }
    else 
    {
        if (Mix_PlayChannel(-1, m_sfx[name], 0) == -1) 
        { 
            std::cout <<"Nie mo¿na odtworzyc: '" << name << "'\n";
        }
    }    
}
