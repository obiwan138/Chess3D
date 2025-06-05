/**
 * @author obiwan138
 * @class ChessObject
 * @brief Define what is an OpenGL "mesh based" object equiped with a texture (abstract class) - Header file
 */

#pragma once

// Include standard headers
#include <memory>

// Include GLEW - OpenGL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include the object type
#include "ViewController.hpp"
#include "Shader.hpp"
#include "enumerations/MeshTypes.hpp"

class ChessObject
{
    protected :
        // Main member variables
        GLuint vao;                 // Pointer to the associated Vertex Array Object (VAO)
        GLuint texture;             // Pointer to the associated texture
        unsigned short numIndices;  // Number of verticies in the mesh

        glm::mat4 modelMatrix;      // Model's 4x4 transformation matrix
        
    public : 

        // Default ChessObject
        ChessObject();

        // Custom Constructor
        ChessObject(GLuint vaoPtrIn, GLuint texturePtrIn, unsigned short numIndicesIn);

        // operator =
        ChessObject& operator=(const ChessObject& other);

        // Draw the object
        void render(Shader* shaderPtr, ViewController* viewControllerPtr); 

        // Destructor
        ~ChessObject();
};