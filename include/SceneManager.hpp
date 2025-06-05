/**
 * @author obiwan138
 * @class SceneManager.hpp
 * @brief This class owns the different 3D objects and textures from the game
 * @note This class is a singleton class and should be accessed through the getInstance() method
 */

#pragma once

// Standard libraries
#include <map>
#include <string>

// External libraries
#include <GL/glew.h>              // OpenGL Library
#include <glm/glm.hpp>            // OpenGL Mathematics

// Headers to include
#include "ObjectType.hpp"
#include "Team.hpp"
#include "VertexData.hpp"
#include "GLBuffersID.hpp"
#include "Shader.hpp"
#include "ViewController.hpp"
#include "Chessboard.hpp"

class SceneManager
{
    // Member variables
    private :      

        // Textures owned by the SceneManager
        std::map<ObjectType, GLuint> whiteTexture;         // Stores the textures of the white pieces
        std::map<ObjectType, GLuint> blackTexture;         // Stores the textures of the black pieces
        std::map<ObjectType, GLuint> otherTexture;         // Stores the textures of the other objects
        
        // OpenGL uffers ID owned by the SceneManager
        std::map<ObjectType, GLBuffersID> objectBuffers;  
        
        // Chessboard
        Chessboard chessboard;

        // Private constructor (singleton)
        SceneManager();

    public :

        // Get the reference to a static instance of the scene manager existing in the function
        static SceneManager& getInstance();

        // Load a texture
        GLuint loadTexture(const std::string imagePath);

        // Load the chess board
        bool loadBoard(const std::string filePath);

        // Load the objects all at once (from the same file)
        bool loadPieces(const std::string filePath);

        // Render the scene
        void render(Shader* shaderPtr, ViewController* viewControllerPtr);

        // Set up the board
        void setUpBoard();

        // Get a vao pointer
        const GLuint getVaoID(ObjectType type) const;

        // Get a texture pointer
        const GLuint getTextureID(ObjectType type) const;

        // Get a texture pointer (override for the team)
        const GLuint getTextureID(ObjectType type, Team team) const;
        
        // Destructor
        ~SceneManager();
};