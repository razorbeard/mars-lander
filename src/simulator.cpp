#include "simulator.hpp"
#include "utils.hpp"

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <iostream>


#define POPULATION_SIZE 80

Simulator::Simulator()
    : m_trajectories(sf::LineStrip), m_isRunning(false)
{

}

Simulator::~Simulator()
{

}

void Simulator::run(const Point2d& position, const Point2d& velocity, int fuel, int angle, int thrust, const Polyline& surfacePoints)
{
    m_lander = Lander(position, velocity, fuel, angle, thrust);
	m_population = generateInitialPopulation(angle, thrust);	
    m_isRunning = true;

    m_surfacePoints = surfacePoints;
    auto hasSameYCoordinate = [] (const Point2d& p, const Point2d& q) { return p.y == q.y; };
    auto iter = std::adjacent_find(surfacePoints.begin(), surfacePoints.end(), hasSameYCoordinate);
    assert(iter != surfacePoints.end());

    std::size_t index = std::distance(surfacePoints.begin(), iter);
    m_landingLine.push_back(surfacePoints[index]);
    m_landingLine.push_back(surfacePoints[index + 1]);

    // CONTINUE
}

void Simulator::iteration()
{
    m_trajectories.clear();
    std::vector<Phenotype> newPopulation;

    for (Phenotype& phenotype : m_population)
    {
        sf::VertexArray vertices(sf::LineStrip);
        Lander lander = m_lander;

        for (std::size_t i = 0; i < phenotype.size() && !hasCrossedSurface(lander.trajectoryLine()); ++i)
        {
            lander.update(phenotype.gene(i).angle, phenotype.gene(i).thrust, vertices);
        }

        phenotype.computeScore(lander, m_landingLine);
        m_trajectories.push_back(vertices);

        if (lander.hasSafelyLanded()) std::cerr << "SOLUTION FOUND \n";
    }

    std::sort(m_population.begin(), m_population.end(), 
              [](const Phenotype& lhs, const Phenotype& rhs) { return lhs.score() > rhs.score(); });

    // Elistism
    newPopulation.push_back(m_population[0]);
    newPopulation.push_back(m_population[1]);

    for (std::size_t k = 0; k < m_population.size() / 2 - 1; ++k)
    {
        // Crossover
        Phenotype parent1 = chooseParent(m_population);
        Phenotype parent2 = chooseParent(m_population);
        
        Phenotype child1 = arithmeticCrossover(parent1, parent2);
        Phenotype child2 = arithmeticCrossover(parent2, parent1);
        
        // Mutation
        mutation(child1);
        mutation(child2);
        newPopulation.push_back(child1);
        newPopulation.push_back(child2);
    }

    m_population = newPopulation;
}

std::vector<Phenotype> Simulator::generateInitialPopulation(int startAngle, int startThrust)
{
    std::vector<Phenotype> population;
    population.reserve(POPULATION_SIZE);

    for (std::size_t i = 0; i < POPULATION_SIZE; ++i)
    {
        population.emplace_back(startAngle, startThrust);
    }

    return population;
}

Phenotype Simulator::chooseParent(const std::vector<Phenotype>& population)
{
    // Tournament selection
    std::size_t bestIndex = utils::uniform(0, population.size() - 1);
    for (std::size_t i = 1; i < 3; ++i)
    {
        const std::size_t candidateIdx = utils::uniform(0, population.size() - 1);
        if (m_population[candidateIdx].score() > m_population[bestIndex].score())
        {
            bestIndex = candidateIdx;
        }
    }

    return m_population[bestIndex]; 
}

Phenotype Simulator::arithmeticCrossover(const Phenotype& parent1, const Phenotype& parent2)
{
    Phenotype child = parent1;
    const int leftIdx = utils::uniform(0, parent1.size() - 1);
    const int rightIdx = utils::uniform(leftIdx, parent1.size() - 1);
    assert(leftIdx != rightIdx);

    const double alpha = utils::uniform(0., 1.);
    for (std::size_t i = leftIdx; i < rightIdx; ++i)
    {
        child.gene(i).thrust = int(alpha * child.gene(i).thrust + (1. - alpha) * parent2.gene(i).thrust);
        child.gene(i).angle = int(alpha * child.gene(i).angle + (1. - alpha) * parent2.gene(i).angle);
    }

    return child;
}

void Simulator::mutation(Phenotype& phenotype)
{
    const double mutationProbability = 0.05;

    for (std::size_t i = 0; i < phenotype.size(); ++i)
    {
        const double probability = utils::uniform(0., 1.);
        if (probability < mutationProbability)
        {
            phenotype.gene(i).angle = utils::uniform(-90, 90);
            phenotype.gene(i).thrust = utils::uniform(0, 4);
        }
    }
}

bool Simulator::hasCrossedSurface(const Polyline& line) const
{
    assert(2 == line.size());
    for (std::size_t i = 0; i < m_surfacePoints.size() - 1; ++i)
    {
        if (utils::doIntersect(m_surfacePoints[i], m_surfacePoints[i + 1], line[0], line[1]))
        {
            return true;
        }
    }

    return false;
}

void Simulator::render(sf::RenderWindow& window)
{
    for (const auto& trajectory : m_trajectories)
    {
        window.draw(trajectory, utils::scaledScreenTransform());
    }
}

bool Simulator::isRunning()
{
	return m_isRunning;
}
