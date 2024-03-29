#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "phenotype.hpp"
#include "point.hpp"
#include "lander.hpp"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>

#include <optional>

namespace sf { class RenderWindow; }

class Simulator
{
public:
    enum class Status
    {
        IDLE,
        RUNNING,
        FINISHED
    };

public:
    Simulator();
    virtual ~Simulator();

    void run(const Point2d& position, const Point2d& velocity, int fuel, int angle, int thrust, const Polyline& surfacePoints);
    void update(sf::Time dt);
    void render(sf::RenderWindow& window);
    void clear();
    const std::size_t numberOfIterations() const noexcept;
    Status status() const noexcept;

private:
    void geneticIteration();
    std::vector<Phenotype> generateInitialPopulation(std::size_t geneLength);
    Phenotype chooseParent();
    Phenotype arithmeticCrossover(const Phenotype& parent1, const Phenotype& parent2);
    void mutate(Phenotype& phenotype);
    std::optional<Point2d> hasCrossedSurface(const Polyline& line) const;

private:
    static size_t s_populationSize;
    static size_t s_geneLength;
    static sf::Time s_deltaUpdateTime;
    static double s_crossoverRate;
    static double s_mutationRate;
    
    std::vector<Phenotype> m_population;
    std::vector<sf::VertexArray> m_trajectories;
    Lander m_lander;
    Polyline m_solution;
    Polyline m_surfacePoints;
    Polyline m_landingLine;
    sf::Time m_updateTime;
    std::size_t m_numberOfIterations;
    Status m_status;
};

#endif
