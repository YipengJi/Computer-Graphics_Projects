#include "StaticMeshActor.h" 
#include <iostream>

StaticMeshActor::StaticMeshActor()
{}

StaticMeshActor::StaticMeshActor(std::shared_ptr<Mesh> new_mesh)
: mesh(new_mesh)
, location(0, 0, 0, 1)
, angle_(0)
, scale(1.0)
, model_matrix(mat4::identity())
{}


void StaticMeshActor::draw(GLenum mode)
{
	if (!mesh) throw std::runtime_error("StaticMeshActor::draw: mesh is NULL");
	mesh->draw(mode);
}

void StaticMeshActor::updateModelMatrix() {
	model_matrix = 
		mat4::translate(location)
		* 
		mat4::rotate_y(angle_) * mat4::scale(scale);
}
