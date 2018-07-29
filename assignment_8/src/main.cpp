#include "lsystem.h"
#include <iostream>
#include <vector>
#include "utils/render_svg.h"

int main(int arg_count, char *arg_values[]) {
	if (arg_count < 2 || arg_count > 3) {
        std::cout << "usage: lsys in_file_path [out_file_path]" << std::endl;
		return 1;
	}

	std::string const in_file_path(arg_values[1]);
	std::string const out_file_path = (arg_count == 3) ? arg_values[2] : "";

	auto lsys = LindenmayerSystem::load_lsystem_from_file(in_file_path);

    if (!lsys) {
		std::cout << "Failed to load system\n\n";
		return 1;
    }

    if (!out_file_path.empty()) {
		// If the name of the output file is specified, we write the expanded sequence and render to a file.

		std::string const sym_sequence_expanded = lsys->expand();
        write_string_to_file(sym_sequence_expanded, out_file_path + ".txt");

		// Build lines according to symbols
		std::vector<Segment> segments = lsys->draw(sym_sequence_expanded);

		// Output segments to an SVG file
		std::string const svg = draw_segments_to_svg_file(segments);
		write_string_to_file(svg, out_file_path + ".svg");

	} else {
		// If an output name wasn't specified, we run the interactive mode.
		std::cout << "- Interactive mode -" << std::endl;
		render_glfw(*lsys);
    }

    std::cout << std::endl;

    return 0;
}
