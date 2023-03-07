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
#define GENE_LENGTH 160
#define DELTA_UPDATE_TIME sf::seconds(0.06f)

Simulator::Simulator()
    : m_trajectories(sf::LineStrip)
    , m_landingLine(2)
    , m_numberOfIterations(0)
    , m_status(Status::IDLE)
{

}

Simulator::~Simulator()
{

}

void Simulator::run(const Point2d& position, const Point2d& velocity, int fuel, int angle, int thrust, const Polyline& surfacePoints)
{
    clear();

    m_lander = Lander(position, velocity, fuel, angle, thrust);
	m_population = generateInitialPopulation(GENE_LENGTH);
    m_status = Status::RUNNING;

    m_surfacePoints = surfacePoints;
    auto hasSameYCoordinate = [] (const Point2d& p, const Point2d& q) { return p.y == q.y; };
    auto iter = std::adjacent_find(surfacePoints.begin(), surfacePoints.end(), hasSameYCoordinate);
    assert(iter != surfacePoints.end());

    std::size_t index = std::distance(surfacePoints.begin(), iter);
    m_landingLine[0] = surfacePoints[index];
    m_landingLine[1] = surfacePoints[index + 1];
}

void Simulator::geneticIteration()
{
    std::vector<Phenotype> newPopulation;

    for (Phenotype& phenotype : m_population)
    {
        Lander lander = m_lander;
        sf::VertexArray vertices(sf::LineStrip);
        vertices.append(sf::Vertex(sf::Vector2f(lander.position().x, lander.position().y)));

        for (std::size_t i = 0; i < phenotype.size(); ++i)
        {
            lander.simulationStep(phenotype.gene(i).angle, phenotype.gene(i).thrust);
            
            if (auto intersection = hasCrossedSurface(lander.trajectoryLine()); intersection)
            {
                vertices.append(sf::Vertex(sf::Vector2f(intersection.value().x, intersection.value().y)));

                if (intersection.value().x >= m_landingLine[0].x &&
                    intersection.value().x <= m_landingLine[1].x &&
                    lander.hasSafelyLanded())
                {
                    m_trajectories.clear();

                    for (std::size_t vP = 0; vP < vertices.getVertexCount(); ++vP)
                    {
                        m_solution.push_back({vertices[vP].position.x, vertices[vP].position.y});
                        vertices[vP].color = sf::Color(0, 255, 0, 100);
                    }
                    std::reverse(m_solution.begin(), m_solution.end());
                    m_trajectories.push_back(vertices);
                    m_updateTime = sf::Time::Zero;
                    m_status = Status::FINISHED;
                    return;
                }
                break;
            }
            else
            {
                vertices.append(sf::Vertex(sf::Vector2f(lander.position().x, lander.position().y)));
            }
        }

        m_trajectories.push_back(vertices);
        phenotype.computeScore(lander, m_landingLine);
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

std::vector<Phenotype> Simulator::generateInitialPopulation(std::size_t geneLength)
{
    std::vector<Phenotype> population;
    population.reserve(POPULATION_SIZE);

    for (std::size_t i = 0; i < POPULATION_SIZE; ++i)
    {
        population.emplace_back(geneLength);
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
    const double mutationProbability = 0.03;

    for (std::size_t i = 0; i < phenotype.size(); ++i)
    {
        const double probability = utils::uniform(0., 1.);
        if (probability < mutationProbability)
        {
            phenotype.gene(i).angle = utils::uniform(-90, 90);
            phenotype.gene(i).thrust = utils::uniform(-1, 1);
        }
    }
}

std::optional<Point2d> Simulator::hasCrossedSurface(const Polyline& line) const
{
    assert(2 == line.size());
    std::optional<Point2d> result = std::nullopt;

    for (std::size_t i = 0; i < m_surfacePoints.size() - 1; ++i)
    {
        if (utils::doIntersect(m_surfacePoints[i], m_surfacePoints[i + 1], line[0], line[1]))
        {
            result = utils::lineLineIntersection(line[0], line[1], m_surfacePoints[i], m_surfacePoints[i + 1]);
            break;
        }
    }

    return result;
}

void Simulator::update(sf::Time dt)
{
	m_updateTime += dt;
    
    if (m_status == Status::RUNNING && m_updateTime > DELTA_UPDATE_TIME)
    {
        m_updateTime -= DELTA_UPDATE_TIME;
        m_trajectories.clear();
        geneticIteration();
        m_numberOfIterations++;
    }
    else if (m_status == Status::FINISHED)
    {
        if (m_solution.size() > 1)
        {
            const std::size_t n = m_solution.size();
            const Point2d newPosition = utils::lerp(m_solution[n-1], m_solution[n-2], m_updateTime.asSeconds() * 10);
            m_lander.setPosition(newPosition.x, newPosition.y);

            if (m_updateTime > sf::seconds(0.1f))
            {
                m_solution.pop_back();
                m_updateTime -= sf::seconds(0.1f);
            }
        }
        else
        {
            m_status = Status::IDLE;
        }
    }
}

void Simulator::render(sf::RenderWindow& window)
{
    for (const auto& trajectory : m_trajectories)
    {
        window.draw(trajectory, utils::scaledScreenTransform());
    }

    window.draw(m_lander);
}

Simulator::Status Simulator::status() const noexcept
{
    return m_status;
}

void Simulator::clear()
{
    m_trajectories.clear();
    m_updateTime = DELTA_UPDATE_TIME;
    m_numberOfIterations = 0;
    m_solution.clear();

    m_lander.setPosition(-50.f, -50.f); // hide the lander
}

const std::size_t Simulator::numberOfIterations() const noexcept
{
    return m_numberOfIterations;
}
