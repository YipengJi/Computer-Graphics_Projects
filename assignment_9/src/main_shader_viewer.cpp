
#include "render/ShaderViewer.h"
#include <vector>
#include <string>

std::string const shader_path(SHADER_PATH);

int main(int arg_count, char *arg_values[]) {
	
	if(arg_count < 2) {
		std::cerr << "Usage\n	./shvi my_shader.frag output.png" << std::endl;
		return 1;
	}

	std::string out_file_path;
	if(arg_count >= 3) {
		// 2nd argument is the file to write
		out_file_path = arg_values[2];
	}
	
	std::vector<std::string> my_frag_shaders = {
		shader_path + "/noise.frag",
		shader_path + "/" + arg_values[1]
	};

	ShaderViewer shvi;
	shvi.setShaderFiles({shader_path + "/display.vert"}, my_frag_shaders);
	//shvi.run(out_file_path);
	shvi.run([&]() {
		if(!out_file_path.empty()) {
			shvi.saveScreenshot(out_file_path);
		}
		return true;
	});
}
