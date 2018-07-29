
#include "lsystem.h"
#include <stack>
#include <memory>
#include <iostream>
#define PI 3.141592653589
/*
Provided utilities:

- Dice class (utils/misc.h)
	Produces random values uniformly distributed between 0 and 1
	Example:
		Dice d;
		double random_val = d.roll();

- write_string_to_file (utils/misc.h)
	Write string data into a text file.
	Example:
		write_string_to_file("ala ma kota!", "ala.txt");
*/

std::string LindenmayerSystemDeterministic::expandSymbol(unsigned char const& sym) {
	/*============================================================
		TODO 1.1
		For a given symbol in the sequence, what should it be replaced with after expansion?
	*/
	auto it = rules.find(sym);
	
	if (it != rules.end()) {
		return it->second;
	}


	return {char(sym)}; // this constructs string from char
	
	//============================================================

	/*
	You may find useful:
		map.find: Iterator to an element with key equivalent to key. If no such element is found, past-the-end (see end()) iterator is returned.
		http://en.cppreference.com/w/cpp/container/unordered_map/find
	*/
}

std::string LindenmayerSystem::expandOnce(std::string const& symbol_sequence) {
	/*============================================================
		TODO 1.2
		Perform one iteration of grammar expansion on `symbol_sequence`.
		Use the expandSymbol method
	*/
	
	std::string tmp_string;
	
	for ( std::string::const_iterator it = symbol_sequence.begin(); it!=symbol_sequence.end(); ++it) {
		std::string tmp = expandSymbol(*it);
		tmp_string += tmp;
	}
	
	return tmp_string;

	//============================================================
}

std::string LindenmayerSystem::expand(std::string const& initial, uint32_t num_iters) {
	/*============================================================
		TODO 1.3
		Perform `num_iters` iterations of grammar expansion (use expandOnce)
	*/
	
	std::string tmp_string = initial;
	
	for (int i=0; i<num_iters; i++) {
		tmp_string = expandOnce(tmp_string);
	}

	return tmp_string;
	
	//============================================================
}

struct State {
	vec2 dir;
	vec2 begin;
};

std::vector<Segment> LindenmayerSystem::draw(std::string const& symbols) {
	/*============================================================
		TODO 2.1
		Build line segments according to the sequence of symbols
		The initial position is (0, 0) and the initial direction is "up" (0, 1)
		Segment is std::pair<vec2, vec2>
	*/
	std::vector<Segment> returnVec;

	vec2 dir = vec2(0,1);
	
	vec2 begin_tmp = vec2(0,0);
	vec2 end_tmp = vec2(0,0);
	
	std::stack<State> states;
	State tmpState;
	double x,y;
		
	for ( std::string::const_iterator it = symbols.begin(); it!=symbols.end(); ++it) {
		switch(*it) {
			case '[':
				tmpState = {dir,begin_tmp};
				states.push(tmpState);
				break;
			case ']':
				tmpState = states.top();
				states.pop();
				dir = tmpState.dir;
				begin_tmp = tmpState.begin;
				break;
			//x' = cos(θ) * x - sin(θ) * y 
			//y' = sin(θ) * x + cos(θ) * y 
			case '+':
				x = cos(rotation_angle_deg*PI/180) * dir[0] - sin(rotation_angle_deg*PI/180) * dir[1];
				y = sin(rotation_angle_deg*PI/180) * dir[0] + cos(rotation_angle_deg*PI/180) * dir[1];
				dir = vec2(x,y);
				break;
			case '-':
				x = cos(rotation_angle_deg*PI/180) * dir[0] + sin(rotation_angle_deg*PI/180) * dir[1];
				y =-sin(rotation_angle_deg*PI/180) * dir[0] + cos(rotation_angle_deg*PI/180) * dir[1];
				dir = vec2(x,y);
				break;
			default:
				end_tmp = begin_tmp + dir;
				returnVec.push_back(std::pair<vec2,vec2>(begin_tmp,end_tmp));
				begin_tmp = end_tmp;
				break;	
		}
	}
	
	return returnVec;
	
	//============================================================
}

std::string LindenmayerSystemStochastic::expandSymbol(unsigned char const& sym) {
	/*============================================================
		TODO 4.1
		For a given symbol in the sequence, what should it be replaced with after expansion?
		(stochastic case)
	*/
	auto it_sym = rules.find(sym);
	
	if (it_sym != rules.end()) {
		std::vector<StochasticRule> rulesVector = it_sym->second;
		double randomVal = dice.roll();
		double probSum = 0.0;
		
		for (std::vector<StochasticRule>::const_iterator it = rulesVector.begin(); it!=rulesVector.end(); ++it) {
			probSum += it->probability;
			if (probSum > randomVal) {
				return it->expansion;
			}
		}		
	}

	return {char(sym)};

	//============================================================
}

void LindenmayerSystemDeterministic::addRuleDeterministic(unsigned char sym, std::string const& expansion) {
	rules[sym] = expansion;
}

void LindenmayerSystemStochastic::addRuleStochastic(unsigned char sym, std::vector<StochasticRule> expansions_with_ps) {
	rules[sym] = expansions_with_ps;
}
