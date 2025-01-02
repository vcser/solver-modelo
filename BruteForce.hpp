#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include <vector>
#include <cstring>
#include <string>
#include "GreedyBaseline.hpp"
#include "Parametros.hpp"
#include "Logging.hpp"

class BruteForce : public GreedyBaseline{
private:
	std::vector<int> _permutacionAsignaciones;
	bool next_assignation();

public:
	using GreedyBaseline::GreedyBaseline;
	void solve() override;
	~BruteForce(){};

	double getFitness() override{
        return fitness;
    };
};

#endif
