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
#include "enumerations/MeshTypes.hpp"
#include "enumerations/Team.hpp"
#include "enumerations/TextureTypes.hpp"
#include "RawVertexData.hpp"
#include "RawTextureData.hpp"
#include "GLBuffersID.hpp"
#include "Shader.hpp"
#include "ViewController.hpp"
#include "Chessboard.hpp"

class SceneManager
{
    // Member variables
    private :      

        // OpenGL uffers ID owned by the SceneManager
        std::map<MeshTypes, GLBuffersID> objectBuffers;  

        // Textures owned by the SceneManager
        std::map<TextureTypes, GLuint> textures; 
        
        // Chessboard
        Chessboard chessboard;

        // Chess pieces
        std::map<TextureTypes, ChessPiece> chessPieces;

        // Private constructor (singleton)
        SceneManager();

    public :

        // Get the reference to a static instance of the scene manager existing in the function
        static SceneManager& getInstance();

        // Read texture data from file
        RawTextureData readTextureData(const std::string& filePath);

        // Send texture data to GPU
        GLuint sendTextureToGPU(const RawTextureData& textureData);

        // Load a set of texture files using OpenMP
        bool loadTextures(const std::vector<std::pair<TextureTypes, std::string>>& texturePaths);

        // Load the chess board
        bool loadBoard(const std::string& filePath);

        // Load the objects all at once (from the same file)
        bool loadPieces(const std::string& filePath);

        // Render the scene
        void render(Shader* shaderPtr, ViewController* viewControllerPtr);

        // Set up the board
        void setUpBoard();

        // Get a texture pointer
        const MeshTypes getMeshType(const TextureTypes& texture) const;

        // Get team
        const Team getTeam(const TextureTypes& texture) const;

        // Get a vao pointer
        const GLuint getVaoID(const MeshTypes& type) const;

        // Get a texture pointer (override for the team)
        const GLuint getTextureID(const TextureTypes& name) const;
        
        // Destructor
        ~SceneManager();
};