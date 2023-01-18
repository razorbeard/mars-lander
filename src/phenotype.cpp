#include "phenotype.hpp"
#include "utils.hpp"
#include "lander.hpp"

#include <algorithm>
#include <cassert>

Phenotype::Phenotype(int startAngle, int startThrust, std::size_t geneSize) :
    m_score{0.0}
{
    m_genes.reserve(geneSize);
    int clampedAngle = startAngle;

    for (std::size_t i = 0; i < geneSize; ++i)
    {
        const int randomAngle = std::clamp(utils::uniform(clampedAngle - 15, clampedAngle + 15), -90, 90);
        const int randomThrust = utils::uniform(0, 4);
        m_genes.push_back({randomAngle, randomThrust});

        clampedAngle = randomAngle;
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

    if (!utils::doIntersect(trajectoryLine.front(), trajectoryLine.back(), landingLine.front(), landingLine.back()))
    {
        const Point2d targetPoint = {(landingLine.front().x + landingLine.back().x) / 2, landingLine.front().y};
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

const double Phenotype::score() const noexcept
{
    return m_score;
}
