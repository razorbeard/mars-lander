#include "levelLoader.hpp"
#include "utils.hpp"

#include <iterator>
#include <fstream>
#include <sstream>

LevelLoader::LevelLoader() :
    m_levelData(),
    m_groundLines(),
    m_surfacePoints()
{

}

LevelLoader::~LevelLoader()
{

}

void LevelLoader::load(const std::string& levelName)
{
    m_surfacePoints.clear();
    m_groundLines.clear();

    std::ifstream file(levelName);
    std::string buffer;

    if (!file)
    {
       throw std::runtime_error("LevelLoader::load - Failed to load " + levelName);
    }

    // retrieve lander initial data
    std::getline(file, buffer);
    std::vector<std::string> values = splitText(buffer);
    m_levelData.position = {std::stof(values[0]), std::stof(values[1])};
    m_levelData.velocity = {std::stof(values[2]), std::stof(values[3])};
    m_levelData.fuel = std::stoi(values[4]);
    m_levelData.angle = std::stoi(values[5]);
    m_levelData.thrust = std::stoi(values[6]);

    // retrieve surface points
    while (std::getline(file, buffer))
    {
        values = splitText(buffer);

        sf::Vector2f p(std::stof(values[0]), std::stof(values[1]));
        sf::Vertex v({p.x, p.y});
        m_groundLines.append(v);
		m_surfacePoints.push_back({p.x, p.y});
    } 
}

void LevelLoader::render(sf::RenderWindow& window)
{
   window.draw(m_groundLines, utils::scaledScreenTransform()); 
}

const Polyline& LevelLoader::surfacePoints() noexcept
{
    return m_surfacePoints;
}

const std::vector<std::string> LevelLoader::splitText(const std::string& text)
{
    std::istringstream iss{text};
    std::vector<std::string> result(std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>());

    return result;
}
