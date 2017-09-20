#ifndef ENGINE_H_
#define ENGINE_H_

#include "Configurator.h"
#include "Visualization.h"
#include "CreatureFactory.h"
#include "Sound.h"
#include "Window.h"

class Engine 
{
public:
       
//metody klasy Engine
    static Engine& Get() 
    {
        static Engine engine;
        return engine;
    }

    void Load() 
    {
        m_configurator.reset(new Configurator());
        m_creature_factory.reset(new CreatureFactory());
        m_window.reset(new Window());
        m_visualization.reset(new Visualization());
        m_sound.reset(new Sound());
       
    }

    ConfiguratorPtr GetConfigurator()   
    {
      return m_configurator; 
    }
    
    CreatureFactoryPtr GetCreatureFactory() 
    { 
      return m_creature_factory; 
    }
    
    WindowPtr GetWindow() 
    { 
      return m_window; 
    }
    
    VisualizationPtr GetVisualization()           
    { 
      return m_visualization; 
    }
    
    SoundPtr GetSound() 
    { 
      return m_sound; 
    }

private:
// pola klasy Engine
    CreatureFactoryPtr m_creature_factory;
    ConfiguratorPtr m_configurator;
    VisualizationPtr m_visualization;
    SoundPtr m_sound;
    WindowPtr m_window;
};
#endif
