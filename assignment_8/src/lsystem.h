
#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <math.h>
#include "utils/misc.h"
#include "utils/render_2d.h"

class LindenmayerSystem {

public:
	std::string name;
	double rotation_angle_deg;
	std::string initial_state;
	int num_iters;
	
	LindenmayerSystem();

	std::string expandOnce(std::string const& symbol_sequence);
	std::string expand(std::string const& initial, uint32_t num_iters);
	std::string expand() { return expand(initial_state, num_iters); }
	
	std::vector<Segment> draw(std::string const& symbols);
	
	static std::shared_ptr<LindenmayerSystem> load_lsystem_from_file(std::string const in_file_path);

    // Get drawing commands for the fully expanded sequence.
	std::vector<Segment> getSegments() { return draw(expand()); }

    virtual ~LindenmayerSystem() { }
	
protected:
	virtual std::string expandSymbol(unsigned char const& sym) = 0;
};

class LindenmayerSystemDeterministic : public LindenmayerSystem {

public:
	void addRuleDeterministic(unsigned char sym, std::string const& expansion);
	
protected:
	std::unordered_map< unsigned char , std::string > rules;


	virtual std::string expandSymbol(unsigned char const& sym) override;
};

struct StochasticRule {
	double probability;
	std::string expansion;

	StochasticRule(double const p, std::string const& e)
	: probability(p)
	, expansion(e)
	{}
};

class LindenmayerSystemStochastic : public LindenmayerSystem {

protected:
	std::unordered_map< unsigned char , std::vector<StochasticRule> > rules;

	// Source of uniformly distributed random real values between 0 and 1
	Dice dice;

public:

	void addRuleStochastic(unsigned char sym, std::vector<StochasticRule> expansions_with_ps);

protected:

	virtual std::string expandSymbol(unsigned char const& sym) override;
};

std::shared_ptr<LindenmayerSystem> load_lsystem(std::string const in_file_path);
void run_lsystem_pipeline(std::string const in_file_path, std::string & out_sequence, std::vector<Segment> & out_segments);
