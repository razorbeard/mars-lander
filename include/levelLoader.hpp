#ifndef LEVEL_LOADER_HPP
#define LEVEL_LOADER_HPP

#include "point.hpp"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

struct LevelData
{
    Point2d position{0.0, 0.0};
    Point2d velocity{0.0, 0.0};
    int fuel{0};
    int angle{0};
    int thrust{0};
};

class LevelLoader
{
public:
    LevelLoader();
    virtual ~LevelLoader();
    
    void load(const std::string& levelName);
    void render(sf::RenderWindow& window);

    const Polyline& surfacePoints() noexcept;

private:
    const std::vector<std::string> splitText(const std::string& text);

private:
    LevelData       m_levelData;
    sf::VertexArray m_groundLines;
    Polyline        m_surfacePoints;
};

#endif
