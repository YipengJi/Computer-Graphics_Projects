//=============================================================================
//
//   Exercise code for "Introduction to Computer Graphics"
//     by Julian Panetta, EPFL
//
//=============================================================================
#ifndef QUAD_H
#define QUAD_H

#include "gl.h"
#include <array>

struct Quad {
	void initialize() {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(bufferObjects.size(), bufferObjects.data());

		std::array<GLfloat, 4 * 3> quad_positions = {
			-1.0f,  1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f
		};

		std::array<GLfloat, 4 * 2> quad_tex_coords = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f
		};

		std::array<GLfloat, 4 * 3> quad_normals = {
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f
		};

		// Attribute 0 -> Vertex positions
		glBindBuffer(
			GL_ARRAY_BUFFER,
			bufferObjects[VTX_BUFFER]); // Modify the vertex buffer object
		glBufferData(
			GL_ARRAY_BUFFER,		// Allocate buffer that is...
			4 * 3 * sizeof(float),	// large enough to fit 12 coordinates
			quad_positions.data(),	// initialized with our position data
			GL_STATIC_DRAW);		// unlikely to change (performance hint)

		glVertexAttribPointer(
			0,				// Point vertex attribute 0 at our buffer
			3, GL_FLOAT,	//     3 Floats per vertex
			GL_FALSE,		//     Don't normalize
			0, 0);			//     No gap between vertex data,

		//     No offset from array beginning.
		glEnableVertexAttribArray(0);

		// glBindBuffer(GL_ARRAY_BUFFER,
		//              bufferObjects[NORMAL_BUFFER]);
		// glBufferData(GL_ARRAY_BUFFER,
		//              4 * 3 * sizeof(float),
		//              quad_normals.data(),
		//              GL_STATIC_DRAW);
		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// glEnableVertexAttribArray(1);

		// Attribute 1 -> texture coordinates
		glBindBuffer(
			GL_ARRAY_BUFFER,
			bufferObjects[TEX_BUFFER]);

		glBufferData(
			GL_ARRAY_BUFFER,
			4 * 2 * sizeof(float),
			quad_tex_coords.data(),
			GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// Upload indices of the vertices to draw
		std::array<GLuint, 6> indices = { 0, 1, 2, 0, 2, 3 };
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}

	void draw() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}

	~Quad() {
		for (GLuint &b : bufferObjects) {
			if(b) glDeleteBuffers(1, &b);
			b = 0;
		}
		if (vao) glDeleteVertexArrays(1, &vao);
	}

private:
	enum { VTX_BUFFER = 0, NORMAL_BUFFER = 1, TEX_BUFFER = 2, INDEX_BUFFER = 3 };
	GLuint vao = 0;
	const size_t n_indices = 6;
	std::array<GLuint, 4> bufferObjects{{0, 0, 0, 0}};
};

#endif /* end of include guard: QUAD_H */
