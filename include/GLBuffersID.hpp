/**
 * @author obiwan138
 * @class GLBuffersID
 * @brief This structure stores all the required OpenGL buffers ID
 * 
 * @note OpenGL stores the important resources (VAO : Vertex Array Objects, VBO : Vertex Buffer Objects, Textures, etc.) as buffers in the GPU memory.
 * These buffers are not directly callable, instead we have handles (e.g. names or ID) mapping to the actual buffers in the GPU memory, allowing to operate on the 
 * buffers using the openGL functions. These handles are of type GLuint, which are unsigned int. Then, it is not necessary to use pointers to share textures or VAO/VBOs
 * as long as only one owner creates/modifies/delete the data and all the other owners only read it or apply it to the shaders. Of course, we (the developper) have 
 * to make sure that the data still exists before using it, that's why the chess objects (users) are members of the SceneManager class (main owner).
 */

#pragma once

// External libraries
#include <GL/glew.h>              // OpenGL Library
#include <glm/glm.hpp>            // OpenGL Mathematics

// Headers to include
#include "VertexData.hpp"

class GLBuffersID
{
    private :
        // VAO containing and mapping the different data buffers for the shader program
        GLuint vao;               

        // Data to store
        GLuint vertexVBO;           // Vertex GL buffer object (VBO)
        GLuint uvVBO;               // UV coordinates GL buffer object (VBO)
        GLuint normalVBO;           // Normal vectors GL buffer object (VBO)
        GLuint ebo;                 // Index GL buffer object (IBO or EBO)
        unsigned short numIndices;  // Number of indices

    public :

        // Explicit custom constructor (a GLBuffer object can only be created from a VertexData structure)
        explicit GLBuffersID(VertexData vertexStruct);

        // Get a pointer to the VAO
        const GLuint getVaoID() const;

        // Get the number of indices
        unsigned short getNumIndices() const;

        // Delete the buffers
        void deleteBuffers();

        // Destructor
        ~GLBuffersID();
};


