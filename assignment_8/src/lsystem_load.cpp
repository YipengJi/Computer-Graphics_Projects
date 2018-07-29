
#include "lsystem.h"
#include <memory>
#include <iostream>
#include <Jzon.h>

LindenmayerSystem::LindenmayerSystem()
	: name("none")
	, rotation_angle_deg(45)
	, initial_state("F")
	, num_iters(1)
{}

std::shared_ptr<LindenmayerSystem> LindenmayerSystem::load_lsystem_from_file(std::string const in_file_path) {
	// Parse JSON
	Jzon::Node spec;

	{
		Jzon::Parser p;
		spec = p.parseFile(in_file_path);
		if(p.getError().length() > 0) {
			std::cout << "Error parsing " << in_file_path << ": " << p.getError() << std::endl;
			return NULL;
		}
		if(!spec.isValid()) {
			std::cout << "Error loading " << in_file_path << ": probably wrong file path" << std::endl;
			return NULL;
		}
	}

	// Extract the information about our system
	auto const name = spec.get("name").toString("");
	bool const is_stochastic = spec.get("stochastic").toBool(false);
	double const rot_angle_deg = spec.get("angle_deg").toDouble(0.0);
	auto const initial_seq = spec.get("initial").toString("");
	int const num_iters = spec.get("num_iters").toInt(1);
	Jzon::Node const rs = spec.get("rules");

	std::cout
		<< "System:	" << name
		<< "\n	stochastic:	" << (is_stochastic?"true":"false")
		<< "\n	angle:		" << rot_angle_deg << " deg"
		<< "\n	iters:		" << num_iters
		<< "\n	initial:	" << initial_seq
		<< std::endl;
		
	// Allocate the l-system on heap to use polymorphism
	std::shared_ptr<LindenmayerSystem> lsys;

	std::cout << "Rules:\n";
	if(!is_stochastic) {
		std::shared_ptr<LindenmayerSystemDeterministic> lsys_det(new LindenmayerSystemDeterministic);
		lsys = lsys_det;

		// Iterate over rule dictionary, key=symbol, value= expansion of symbol

		for(Jzon::NamedNode const& rule_keyval : rs) {
			unsigned char const sym = rule_keyval.first[0];
			std::string const expansion = rule_keyval.second.toString("");
			std::cout << "	" << sym << " -> " << expansion << std::endl;

			lsys_det->addRuleDeterministic(sym, expansion);
		}
	} else {
		std::shared_ptr<LindenmayerSystemStochastic> lsys_sto(new LindenmayerSystemStochastic);
		lsys = lsys_sto;

		// Iterate over rule dictionary, key=symbol, value= list of alternatives with probabilities
		for(Jzon::NamedNode const& rule_keyval : rs) {
			unsigned char const sym = rule_keyval.first[0];

			if( rule_keyval.second.isString() ) {
				// If its a string, its just one alternative without
				std::string const expansion = rule_keyval.second.toString("");
				lsys_sto->addRuleStochastic(sym, {{1.0, expansion}});

				std::cout << "	" << sym << " -> " << expansion << std::endl;

			} else if (rule_keyval.second.isArray()) {

				std::vector<StochasticRule> alternatives;
				double total_p = 0.;
				for(Jzon::NamedNode const& alternative_keyval : rule_keyval.second) {
					double const p = alternative_keyval.second.get("p").toDouble(0);
					std::string const expansion = alternative_keyval.second.get("e").toString("");
					total_p += p;
					alternatives.push_back({p, expansion});

					std::cout << "	" << sym << " (p=" << p << ")-> " << expansion << std::endl;
				}

				if( total_p < 1.0 ) {
					std::cout << "Probabilities sum to " << total_p << ", should be 1.0" << std::endl;
				} else {
					lsys_sto->addRuleStochastic(sym, alternatives);
				}

			} else {
				std::cout << "Invalid rule for symbol " << sym << std::endl;
			}
		}
	}
	
	lsys->name = name;
	lsys->rotation_angle_deg = rot_angle_deg;
	lsys->initial_state = initial_seq;
	lsys->num_iters = num_iters;
	
	return lsys;
}
