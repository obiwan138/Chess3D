/**
 * @author obiwan138
 * @file GLBuffersID.cpp
 * @brief Implementation of the GLBuffersID class
 */

#include "GLBuffersID.hpp"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Custom constructor
 * @details This function generates the VAO and the VBOs for the vertices, UVs, normals and indices
 * @param vertexStruct : structure containing the vertices, UVs, normals and indices vectors
 */

GLBuffersID::GLBuffersID(VertexData vertexStruct){
  
    // Generate the vertex array object
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);

    /**
     * VAO attribute 0 : vertices buffer
     */
    // Generate a buffer for the vertices
    glGenBuffers(1, &(this->vertexVBO));				            // Generate the buffer
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);	                // Bind the VBO as the active GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, 								    // Load data in the active buffer
                vertexStruct.verticies.size() * sizeof(glm::vec3),  // Size of the data in bytes
                vertexStruct.verticies.data(), 					    // Pointer to the data
                GL_STATIC_DRAW);								    // Data is static (set once)
    
    // Allocate the required memory in the corresponding VAO attribute
    glVertexAttribPointer(
        0,                  // VAO attribute index is 0 (first)
        3,                  // element size is 3 (glm::vec3)
        GL_FLOAT,           // type of the element
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Enable the attribute 0 of VAO for the shader program
    glEnableVertexAttribArray(0); 

    /**
     * VAO attribute 1 : uv coordinates buffer
     */
    // Generate a buffer for the UVs
    glGenBuffers(1, &(this->uvVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->uvVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                vertexStruct.uvs.size() * sizeof(glm::vec2), 
                vertexStruct.uvs.data(), 
                GL_STATIC_DRAW);

    // Allocate the required memory in the corresponding VAO attribute
    glVertexAttribPointer(
        1,                                // VAO attribute index is 1 (second)
        2,                                // element size is 2 (glm::vec2)
        GL_FLOAT,                         // type of the element
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Enable the attribute 1 of VAO for the shader program
    glEnableVertexAttribArray(1);

    /**
     * VAO attribute 2 : normal vectors buffer
     */
    // Generate a buffer for the normals
    glGenBuffers(1, &(this->normalVBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->normalVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                vertexStruct.normals.size() * sizeof(glm::vec3), 
                vertexStruct.normals.data(), 
                GL_STATIC_DRAW);

    // Allocate the required memory in the corresponding VAO attribute
    glVertexAttribPointer(
        2,                                // VAO attribute index is 2 (third)
        3,                                // element size is 3 (glm::vec3)
        GL_FLOAT,                         // type of the element
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Enable the attribute 2 of VAO for the shader program
    glEnableVertexAttribArray(2);

    /**
     * VAO attribute 3 : index/element buffer (EBO)
     * This buffer has a different state "GL_ELEMENT_ARRAY_BUFFER" instead of GL_ARRAY_BUFFER
     * so it does not require to call glVertexAttribPointer and glEnableVertexAttribArray
     */

    // Generate a buffer for the indices as well
    glGenBuffers(1, &(this->ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                vertexStruct.indices.size() * sizeof(unsigned short), 
                vertexStruct.indices.data(), 
                GL_STATIC_DRAW);

    this->numIndices = vertexStruct.numIndices;

    // Unbind VAO
    glBindVertexArray(0);
}


/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the VAO
 * @details This function returns a pointer to the VAO of one GLBuffersID object
 * @return the VAO
 */
const GLuint GLBuffersID::getVaoID() const{
    return static_cast<const GLuint>(this->vao);
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the number of indices
 * @details This function returns the number of indices of one GLBuffersID object
 * @return the number of indices
 */
unsigned short GLBuffersID::getNumIndices() const{
    return this->numIndices;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief deleteBuffers
 * @details This function deletes the VBOs and the VAO when the a GLBuffersID object is destroyed
 */

void GLBuffersID::deleteBuffers(){
    // Cleanup VBOs
    if(this->vertexVBO != 0){
        glDeleteBuffers(1, &(this->vertexVBO));
        std::cout << "Deleted vertex VBO"<< std::endl;
    }
    if(this->uvVBO != 0){
        glDeleteBuffers(1, &(this->uvVBO));
        std::cout << "Deleted uv VBO"<< std::endl;
    }
    if(this->normalVBO != 0){
        glDeleteBuffers(1, &(this->normalVBO));
        std::cout << "Deleted normal VBO"<< std::endl;
    }
    if(this->ebo != 0){
        glDeleteBuffers(1, &(this->ebo));
        std::cout << "Deleted EBO"<< std::endl;
    }
    // Cleanup VAO
    if(this->vao != 0){
        glDeleteVertexArrays(1, &(this->vao));
        std::cout << "Deleted VAO"<< std::endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Destructor
 */

 GLBuffersID::~GLBuffersID(){}