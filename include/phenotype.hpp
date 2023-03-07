#ifndef PHENOTYPE_HPP
#define PHENOTYPE_HPP

#include "point.hpp"

#include <vector>

class Lander;

struct Gene
{
    int angle;
    int thrust;
};

class Phenotype 
{
public:
    Phenotype(std::size_t geneLength = 160);
    virtual ~Phenotype();

    void computeScore(const Lander& lander, const Polyline& landingLine);
    Gene& gene(std::size_t id);
    const Gene& gene(std::size_t id) const noexcept;
    const std::size_t size() const noexcept;
    const double score() const noexcept;

private:
    std::vector<Gene> m_genes;
    double m_score;
};

#endif
