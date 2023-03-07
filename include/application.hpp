#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"
#include "container.hpp"
#include "levelLoader.hpp"
#include "simulator.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
public:
    Application();
    virtual ~Application();

    void run();

private:
    void createButtons();
    void processInput();
    void update(sf::Time dt);
    void render();

private:
    static const sf::Time s_timePerFrame;
    
    sf::RenderWindow m_window;
    sf::Text m_statisticsText;
    TextureHolder m_textures;
    FontHolder m_fonts;
    Container m_container;
    LevelLoader m_levelLoader;
    Simulator m_simulator;
};

#endif
