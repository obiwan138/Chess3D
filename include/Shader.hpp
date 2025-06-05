/**
 * @author obiwan138
 * @class Shader
 * @brief Class to load and manage shader programs (vertex and fragment shaders) and the corresponding uniform variables
 */

#pragma once

// Include standard librairies
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// OpenGL librairies
#include <GL/glew.h>
#include <glm/glm.hpp>            // OpenGL Mathematics


class Shader {

    private:

        // ID of the shader program
        GLuint programID;

        // GLSL Uniform variables
        GLuint modelMatrixID;   // ID of the model matrix uniform variable
        GLuint viewMatrixID;    // ID of the view matrix uniform variable
        GLuint mvpMatrixID;     // ID of the MVP (Model-View-Projection) matrix uniform variable
        GLuint textureID;       // ID of the texture uniform variable
        GLuint lightID;         // ID of the Light uniform variable

        // Light position
        glm::vec3 lightPosition;

        // Load the shaders
        GLuint loadShaders(const char* vertexPath, const char* fragmentPath);

        // Compile a shader
        GLuint compileShader(const char* source, GLenum type);
    
    public:

        // Constructor
        explicit Shader(const char* vertexPath, const char* fragmentPath);

        // Use the shader program
        void use() const;

        // Get the ID of the shader program
        GLuint getID() const;

        // Get the ID of the shader texture uniform variable
        GLuint getTextureID() const;

        // Get the ID of the shader model matrix uniform variable
        GLuint getModelMatrixID() const;

        // Get the ID of the shader view matrix uniform variable
        GLuint getViewMatrixID() const;

        // Get the ID of the shader MVP matrix uniform variable
        GLuint getMvpMatrixID() const;

        // Get the Ligth ID
        GLuint getLightID() const;

        // Get the light position
        glm::vec3 getLightPosition() const;

        // Destructor
        ~Shader();
    
};
