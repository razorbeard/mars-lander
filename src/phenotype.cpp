#include "phenotype.hpp"
#include "utils.hpp"
#include "lander.hpp"

#include <algorithm>
#include <cassert>

Phenotype::Phenotype(std::size_t geneLength) :
    m_score{0.0}
{
    m_genes.reserve(geneLength);

    for (std::size_t i = 0; i < geneLength; ++i)
    {
        const int randomAngle = utils::uniform(-15, 15);
        const int randomThrust = utils::uniform(-1, 1);
        m_genes.push_back({randomAngle, randomThrust});
    }
}

Phenotype::~Phenotype()
{

}

void Phenotype::computeScore(const Lander& lander, const Polyline& landingLine)
{
    assert(2 == landingLine.size());

    const Polyline trajectoryLine = lander.trajectoryLine();
    const Point2d velocity = lander.velocity();

    if (!utils::doIntersect(trajectoryLine[0], trajectoryLine[1], landingLine[0], landingLine[1]))
    {
        const Point2d targetPoint = { (landingLine[0].x + landingLine[1].x) / 2, landingLine[0].y };
        const double distanceToTarget = utils::length(lander.position(), targetPoint);

        const double distancePenality = distanceToTarget / 100;
        const double velocityPenality = utils::length(velocity) / 175;

        m_score = 100.0 - distancePenality - velocityPenality;
    }
    else if (20 < std::abs(velocity.x) || 40 < std::abs(velocity.y))
    {
        const double penality = std::abs(velocity.x) / 250 + std::abs(velocity.y) / 250;

        m_score = 100.0 - penality;
    }
    else
    {
        m_score = 100.0;
    }
}

Gene& Phenotype::gene(std::size_t id)
{
    return m_genes[id];
}

const Gene& Phenotype::gene(std::size_t id) const noexcept
{
    return m_genes[id];
}

const std::size_t Phenotype::size() const noexcept
{
    return m_genes.size();
}

const double Phenotype::score() const noexcept
{
    return m_score;
}
