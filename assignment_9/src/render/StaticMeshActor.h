#pragma once

#include "../utils/vec.h"
#include "gl.h"
#include <vector>
#include <string>
#include "Mesh.h"
#include "shader.h"
#include <memory>

class StaticMeshActor {
	
public:
	std::shared_ptr<Mesh> mesh;

	// current position
	vec4 location;

	// current y-axis angle
	float angle_;

	float scale;
	
	StaticMeshActor();
	StaticMeshActor(std::shared_ptr<Mesh> new_mesh);

	mat4 model_matrix;
	
	void updateModelMatrix();
	
	void draw(GLenum mode = GL_TRIANGLES);
};
