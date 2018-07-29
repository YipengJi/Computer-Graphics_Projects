
#pragma once
#include <random>
#include "vec.h"

/*
	Produces random values uniformly distributed between 0 and 1
	Example:
		Dice d;
		double random_val = d.roll();
*/
class Dice {

protected:
	std::default_random_engine engine;
    std::uniform_real_distribution<double> distribution;

public:
	Dice()
	: distribution(0., 1.)
	{
		seed(std::random_device()());
	}

	double roll() {
		return distribution(engine);
	}

	void seed(int val) {
		engine.seed(val);
	}
};

/*
	Write string data into a text file.
	Example:
		write_string_to_file("ala ma kota!", "ala.txt");
*/
void write_string_to_file(std::string const& data, std::string const& file_path);


Segment segment_group_bounding_box(std::vector<Segment> const& segments);
