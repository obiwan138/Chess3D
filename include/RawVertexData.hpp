/**
 * @author obiwan138
 * @struct RawVertexData
 * @brief This structure stores all the required data corresponding to a 3D object
 */

#pragma once

// Standard libraries
#include <vector>

// External libraries
#include <glm/glm.hpp>            // OpenGL Mathematics

struct RawVertexData
{
    std::vector<glm::vec3> verticies;       // Vector of Vertices (= 3D points)
    std::vector<glm::vec2> uvs;             // UV coordinates for the texture
    std::vector<glm::vec3> normals;         // Normal vectors to the surface at a vertex
    std::vector<unsigned short> indices;    // Indices of the vertices to form triangles
    int numIndices;                         // Number of indices (verticies)
};
