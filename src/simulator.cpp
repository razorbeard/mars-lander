#include "simulator.hpp"
#include "utils.hpp"

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <iostream>

size_t Simulator::s_populationSize = 100;
size_t Simulator::s_geneLength = 160;
sf::Time Simulator::s_deltaUpdateTime = sf::seconds(0.06f);
double Simulator::s_crossoverRate = 0.95;
double Simulator::s_mutationRate = 0.03;

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
	m_population = generateInitialPopulation(s_geneLength);
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

    std::vector<Phenotype> newPopulation;
    for (std::size_t k = 0; k < m_population.size(); ++k)
    {
        Phenotype newPhenotype;
        const double crossoverProbability = utils::uniform(0., 1.);
        if (crossoverProbability < s_crossoverRate)
        {
            Phenotype parent1 = chooseParent();
            Phenotype parent2 = chooseParent();
            
            newPhenotype = arithmeticCrossover(parent1, parent2);
        }
        else
        {
            newPhenotype = chooseParent();
        }

        mutate(newPhenotype);
        newPopulation.push_back(newPhenotype);
    }

    m_population = newPopulation;
}

std::vector<Phenotype> Simulator::generateInitialPopulation(std::size_t geneLength)
{
    std::vector<Phenotype> population;
    population.reserve(s_populationSize);

    for (std::size_t i = 0; i < s_populationSize; ++i)
    {
        population.emplace_back(geneLength);
    }

    return population;
}

Phenotype Simulator::chooseParent()
{
    // Tournament selection
    std::size_t bestIndex = utils::uniform(0, m_population.size() - 1);
    for (std::size_t i = 1; i < 3; ++i)
    {
        const std::size_t candidateIdx = utils::uniform(0, m_population.size() - 1);
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
    for (std::size_t i = leftIdx; i <= rightIdx; ++i)
    {
        child.gene(i).thrust = std::round(alpha * child.gene(i).thrust + (1. - alpha) * parent2.gene(i).thrust);
        child.gene(i).angle = std::round(alpha * child.gene(i).angle + (1. - alpha) * parent2.gene(i).angle);
    }

    return child;
}

void Simulator::mutate(Phenotype& phenotype)
{
    for (std::size_t i = 0; i < phenotype.size(); ++i)
    {
        const double probability = utils::uniform(0., 1.);
        if (probability < s_mutationRate)
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
    
    if (m_status == Status::RUNNING && m_updateTime > s_deltaUpdateTime)
    {
        m_updateTime -= s_deltaUpdateTime;
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
    m_updateTime = s_deltaUpdateTime;
    m_numberOfIterations = 0;
    m_solution.clear();

    m_lander.setPosition(-50.f, -50.f); // hide the lander
}

const std::size_t Simulator::numberOfIterations() const noexcept
{
    return m_numberOfIterations;
}
