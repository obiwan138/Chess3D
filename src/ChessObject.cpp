/**
 * @author obiwan138
 * @file ChessObject.cpp
 * @brief Define what is an OpenGL "mesh based" object equiped with a texture.
 */

#include "ChessObject.hpp"

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Default Constructor
 * @details The default constructor assigns GLuint IDs 0 for VAO and texture, as well as the number of verticies
 */
ChessObject::ChessObject(){
    this->vao = static_cast<GLuint>(0);
    this->texture = static_cast<GLuint>(0);
    this->numIndices = 0;
    this->modelMatrix = glm::mat4(1.0f);
}
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Custom Constructor
 * @param vaoID : the ID of the Vertex Array Object
 * @param textureID : the ID of the texture
 * @param numIndicesIn : the number of indices in the mesh
 */

ChessObject::ChessObject(GLuint vaoID, GLuint textureID, unsigned short numIndicesIn){
    this->vao = vaoID;
    this->texture = textureID;
    this->numIndices = numIndicesIn;
    this->modelMatrix = glm::mat4(1.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief operator =
 * @param other : the other ChessObject to copy
 * @return ChessObject& : the reference to the new ChessObject
 */

ChessObject& ChessObject::operator=(const ChessObject& other){
    // Check if this and other are the same (self-check assignment)
    if(this==&other){
        return *this;
    }
    else{
        this->vao = other.vao;
        this->texture = other.texture;
        this->numIndices = other.numIndices;
        this->modelMatrix = other.modelMatrix;
        return *this;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Render a chess object
 * @param shaderPtr : the pointer to the shader program
 * @param viewControllerPtr : the pointer to the view controller
 */

 void ChessObject::render(Shader* shaderPtr, ViewController* viewControllerPtr){
    
    // Compute the MVP matrix
    glm::mat4 MVP = viewControllerPtr->getProjectionMatrix()
                    * viewControllerPtr->getViewMatrix()
                    * this->modelMatrix; 

    // Use the shader
    shaderPtr->use();
    
    // Send the corresponding matrices to the shader
    glUniformMatrix4fv(shaderPtr->getMvpMatrixID(), 1, GL_FALSE, &MVP[0][0]);                       // Send the MVP matrix to the shader
    glUniformMatrix4fv(shaderPtr->getModelMatrixID(), 1, GL_FALSE, &(this->modelMatrix[0][0]));     // Send the model matrix to the shader
    glUniform3f(shaderPtr->getLightID(),                                                            // Send the ligth position
                shaderPtr->getLightPosition().x, 
                shaderPtr->getLightPosition().y, 
                shaderPtr->getLightPosition().z);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);                   // Activate the texture unit
    glBindTexture(GL_TEXTURE_2D, this->texture);    // Bind our texture in Texture Unit 0
    glUniform1i(shaderPtr->getTextureID(), 0);        // Set the Texture shader to use Texture Unit 0

    // Index buffer
    glBindVertexArray(this->vao);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,      // mode
        this->numIndices,  // count
        GL_UNSIGNED_SHORT, // type
        (void*)0           // element array buffer offset
    );

    // Unbind VAO
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Default Destructor
 */
ChessObject::~ChessObject(){}