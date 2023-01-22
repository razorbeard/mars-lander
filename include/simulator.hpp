#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "phenotype.hpp"
#include "point.hpp"
#include "lander.hpp"

#include <SFML/Graphics/VertexArray.hpp>
#include <optional>

namespace sf { class RenderWindow; }

class Simulator
{
public:
    Simulator();
    virtual ~Simulator();

    void run(const Point2d& position, const Point2d& velocity, int fuel, int angle, int thrust, const Polyline& surfacePoints);
    void iteration();
    void render(sf::RenderWindow& window);
    bool isRunning();
    void clearTrajectories();

private:
    std::vector<Phenotype> generateInitialPopulation(int startAngle, int startThrust);
    Phenotype chooseParent(const std::vector<Phenotype>& population);
    Phenotype arithmeticCrossover(const Phenotype& parent1, const Phenotype& parent2);
    void mutation(Phenotype& phenotype);
    std::optional<Point2d> hasCrossedSurface(const Polyline& line) const;

private:
	std::vector<Phenotype> m_population;
    std::vector<sf::VertexArray> m_trajectories;
	Lander m_lander;
    Polyline m_surfacePoints;
    Polyline m_landingLine;
    bool m_isRunning;
};

#endif
