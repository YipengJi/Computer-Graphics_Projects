//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

//== INCLUDES =================================================================

#include "Mesh.h"
#include <fstream>
#include <string>
#include <stdexcept>
#include <limits>
#include <algorithm>

//== IMPLEMENTATION ===========================================================

void Mesh::read(const std::string &_filename)
{
    // read a mesh in OFF format

    // open file
    std::ifstream ifs(_filename);
    if (!ifs) {
        throw std::runtime_error("Can't open " + _filename);
    }


    // read OFF header
    std::string s;
    unsigned int nV, nF, dummy, i;
    ifs >> s;
    if (s != "OFF") throw std::runtime_error("Not an OFF file");
    ifs >> nV >> nF >> dummy;
    std::cout << "\n  read " << _filename << ": " << nV << " vertices, " << nF << " triangles\n";


    // read vertices
    Vertex v;
    vertices_.clear();
    vertices_.reserve(nV);
    for (i=0; i<nV; ++i)
    {
        ifs >> v.position;
        vertices_.push_back(v);
    }


    // read triangles
    Triangle t;
    triangles_.clear();
    triangles_.reserve(nF);
    for (i=0; i<nF; ++i)
    {
        ifs >> dummy >> t.i0 >> t.i1 >> t.i2;
        triangles_.push_back(t);
    }


    // close file
    ifs.close();


    // compute face and vertex normals
    compute_normals();

    // compute bounding box
    compute_bounding_box();

    // copy this mesh's data to the GPU
    m_generate_vao();
}

// Copy this mesh's data to the GPU; must be done after OpenGL has been initialized!
void Mesh::m_generate_vao() {
    clean();
    // Generate vertex array object for the mesh
    //     (Stores pointers to buffers holding vertex data)
    glGenVertexArrays(1, &vao);
    glGenBuffers(bufferObjects.size(), bufferObjects.data());

    // Update the mesh geometry vertex array object
    glBindVertexArray(vao);

    // Pack the vertex coordinates into an array that we can pass to OpenGL
    const size_t nv = vertices_.size(),
                 nt = triangles_.size();
    std::vector<float> coords(nv * 3);
    for (size_t i = 0; i < nv; ++i) {
        coords[3 * i + 0] = vertices_[i].position[0];
        coords[3 * i + 1] = vertices_[i].position[1];
        coords[3 * i + 2] = vertices_[i].position[2];
    }

    // Allocate and fill buffer holding mesh vertex coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VTX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, // (re)allocate buffer to hold the mesh's vertex positions
                 sizeof(float) * coords.size(),
                 coords.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Pack the normal components into an array that we can pass to OpenGL
    for (size_t i = 0; i < nv; ++i) {
        coords[3 * i + 0] = vertices_[i].normal[0];
        coords[3 * i + 1] = vertices_[i].normal[1];
        coords[3 * i + 2] = vertices_[i].normal[2];
    }

    // Allocate and fill buffer holding mesh vertex normals
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, // (re)allocate buffer to hold the mesh's vertex positions
                 sizeof(float) * coords.size(),
                 coords.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    n_indices = 3 * nt;
    std::vector<GLuint> indices(n_indices);
    for (size_t i = 0; i < nt; ++i) {
        indices[3 * i + 0] = triangles_[i].i0;
        indices[3 * i + 1] = triangles_[i].i1;
        indices[3 * i + 2] = triangles_[i].i2;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}


//-----------------------------------------------------------------------------

// Determine the weights by which to scale triangle (p0, p1, p2)'s normal when
// accumulating the vertex normals for vertices 0, 1, and 2.
// (Recall, vertex normals are a weighted average of their incident triangles'
// normals, and in our raytracer we'll use the incident angles as weights.)
// \param[in] p0, p1, p2    triangle vertex positions
// \param[out] w0, w1, w2    weights to be used for vertices 0, 1, and 2
void angleWeights(const vec3 &p0, const vec3 &p1, const vec3 &p2,
                  double &w0, double &w1, double &w2) {
    // compute angle weights
    const vec3 e01 = normalize(p1-p0);
    const vec3 e12 = normalize(p2-p1);
    const vec3 e20 = normalize(p0-p2);
    w0 = acos( std::max(-1.0f, std::min<float>(1.0f, dot(e01, -e20) )));
    w1 = acos( std::max(-1.0f, std::min<float>(1.0f, dot(e12, -e01) )));
    w2 = acos( std::max(-1.0f, std::min<float>(1.0f, dot(e20, -e12) )));
}


//-----------------------------------------------------------------------------

void Mesh::compute_normals()
{
    // compute triangle normals
    for (Triangle& t: triangles_)
    {
        const vec3& p0 = vertices_[t.i0].position;
        const vec3& p1 = vertices_[t.i1].position;
        const vec3& p2 = vertices_[t.i2].position;
        t.normal = normalize(cross(p1-p0, p2-p0));
    }

    // initialize vertex normals to zero
    for (Vertex& v: vertices_)
    {
        v.normal = vec3(0,0,0);
    }

    // \todo Paste your assignment 3 vertex normal computation solution here.
    
 	for (Triangle& t: triangles_)
    {
    	double w0,w1,w2;
    	const vec3& p0 = vertices_[t.i0].position;
        const vec3& p1 = vertices_[t.i1].position;
        const vec3& p2 = vertices_[t.i2].position;
    	angleWeights(p0,p1,p2,w0,w1,w2);
    	
    	vertices_[t.i0].normal += w0 * t.normal;
    	vertices_[t.i1].normal += w1 * t.normal;
    	vertices_[t.i2].normal += w2 * t.normal;
    }

	for (Vertex& v: vertices_)
    {
        v.normal = normalize(v.normal);
    }
}


//-----------------------------------------------------------------------------


void Mesh::compute_bounding_box()
{
    bb_min_ = vec3(std::numeric_limits<double>::max());
    bb_max_ = vec3(std::numeric_limits<double>::lowest());

    for (Vertex v: vertices_)
    {
        bb_min_ = min(bb_min_, v.position);
        bb_max_ = max(bb_max_, v.position);
    }
}
