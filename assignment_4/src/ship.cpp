#include "ship.h"
#include <fstream>
#include <string>

Ship::Ship()
    :pos_(0, 0, 0, 1), direction_(0, 0, 1, 0), speed_(0), angle_(0), angular_speed_(0), n_indices_(0), radius_(0.002)
{}

Ship::~Ship()
{
    if (vbo_)  glDeleteBuffers(1, &vbo_);
    if (nbo_)  glDeleteBuffers(1, &nbo_);
    if (tbo_)  glDeleteBuffers(1, &tbo_);
    if (ibo_)  glDeleteBuffers(1, &ibo_);
    if (vao_)  glDeleteVertexArrays(1, &vao_);
}

bool Ship::load_model(const char* _filename)
{

    // implements a simple OFF reader

    // open file
    std::ifstream ifs(_filename);
    if (!ifs)
    {
        std::cerr << "Can't open " << _filename << "\n";
        return false;
    }

    // read OFF header
    std::string s;
    ifs >> s;
    if (s != "OFF")
    {
        std::cerr << "No OFF file\n";
        return false;
    }

    unsigned int nV, nF, dummy, i, j;
    ifs >> nV >> nF >> dummy;
    std::cout << _filename << ": " << nV << " vertices, " << nF << " triangles\n";

    vertices_.resize(nV);
    // read vertices
    for (i = 0; i<nV; ++i)
    {
        ifs >> vertices_[i];
    }

    indices_.resize(nF * 3);
    // read faces
    for (i = 0; i<nF; ++i)
    {
        ifs >> dummy;
        if (dummy == 3)
        {
            for (j = 0; j < 3; j++)
            {
                ifs >> indices_[3 * i + j];
            }
        }
        else
        {
            std::cerr << "No triangle mesh\n";
            return false;
        }
    }

    // close file
    ifs.close();
    compute_normals();
    initialize_buffers();

    return true;
}

void Ship::compute_normals()
{
    int nF = indices_.size()/3;
    int nV = vertices_.size();

    vertex_normals_.resize(nV, vec3(0, 0, 0));
    for (int i = 0; i<nF; i++)
    {
        vec3 a, b;
        a = vertices_[indices_[3 * i + 1]] - vertices_[indices_[3 * i + 0]];
        b = vertices_[indices_[3 * i + 2]] - vertices_[indices_[3 * i + 0]];
        face_normals_.push_back(normalize(cross(a, b)));

        vertex_normals_[indices_[3 * i + 0]] += face_normals_[i];
        vertex_normals_[indices_[3 * i + 1]] += face_normals_[i];
        vertex_normals_[indices_[3 * i + 2]] += face_normals_[i];
    }

    for (int i = 0; i<nV; i++)
    {
        vertex_normals_[i] = normalize(vertex_normals_[i]);
    }
}

void Ship::accelerate(float speedup)
{
    speed_ += speedup;
}

void Ship::accelerate_angular(float angular_speedup)
{
    angular_speed_ += angular_speedup;
}

void Ship::update_ship()
{
    angle_ += angular_speed_;
    mat4 rot = mat4::rotate_y(angle_);
    direction_ = rot*vec4(0,0,1,0);
    pos_ += speed_*direction_;
}

void Ship::draw(GLenum mode)
{
    if (n_indices_ == 0) initialize_buffers();

    glBindVertexArray(vao_);
    glDrawElements(mode, n_indices_, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void Ship::initialize_buffers()
{
    const unsigned int n_vertices = vertices_.size();
    const unsigned int n_triangles = indices_.size()/3;

    std::vector<GLfloat> positions(3 * n_vertices);
    std::vector<GLfloat>   normals(3 * n_vertices);
    std::vector<GLuint >   indices(3 * n_triangles);
    std::vector<GLfloat> texcoords(4 * n_vertices);

    unsigned int p(0), n(0), t(0), i(0);

    // generate vertices
    for (unsigned int v = 0; v<n_vertices; ++v)
    {
            positions[p++] = vertices_[v][0];
            positions[p++] = vertices_[v][1];
            positions[p++] = vertices_[v][2];

            normals[n++] = vertex_normals_[v][0];
            normals[n++] = vertex_normals_[v][1];
            normals[n++] = vertex_normals_[v][2];

            texcoords[t++] = 0.5;
            texcoords[t++] = 0.5;
    }


    // generate triangles
    for (unsigned int f = 0; f<n_triangles*3 ; f++)
    {
        indices[f] = indices_[f];
    }
    n_indices_ = 3 * n_triangles;


    // generate vertex array object
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);


    // vertex positions -> attribute 0
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * n_vertices*sizeof(float), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // normal vectors -> attribute 1
    glGenBuffers(1, &nbo_);
    glBindBuffer(GL_ARRAY_BUFFER, nbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * n_vertices*sizeof(float), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // texture coordinates -> attribute 2
    glGenBuffers(1, &tbo_);
    glBindBuffer(GL_ARRAY_BUFFER, tbo_);
    glBufferData(GL_ARRAY_BUFFER, 2 * n_vertices*sizeof(float), &texcoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // triangle indices
    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * n_triangles*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}
