/**
 * @author obiwan138
 * @file SceneManager.cpp
 * @brief Implementation of the SceneManager class
 */

#include <iostream>	
#include <fstream>
#include <stdexcept>

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "SceneManager.hpp"

//////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Custom constructor 
 * @details This function loads the different objects and textures from the game
 * @note The constructor is private to ensure that the SceneManager is a singleton class
 */

SceneManager::SceneManager(){

    // Load the meshes
    std::cout << "Loading meshes and GL buffers ..." << std::endl;

    // Load the the vertices data corresponding to the board
    if(!this->loadBoard("../resources/Stone_Chess_Board/Chess_Board.obj")){
        std::cerr << "Error while loading the board. Check the correspond file and path" << std::endl;
        
    };

    // Load the the vertices data corresponding to the pieces
    if(!this->loadPieces("../resources/Chess_Pieces/Chess_Pieces.obj")){
        std::cerr << "Error while loading the pieces. Check the correspond file and path" << std::endl;
    };

    // Load the textures
    std::cout << "Loading textures ..." << std::endl;

    //Load the texture for the chessboard
    this->otherTexture.emplace(ObjectType::BOARD, this->loadTexture("../resources/Stone_Chess_Board/Stone_chessboard_diffuse_image.bmp"));

    // Load the texture corresponding to the white pieces
    this->whiteTexture.emplace(ObjectType::PAWN, this->loadTexture("../resources/Chess_Pieces/white_pawn.bmp"));
    this->whiteTexture.emplace(ObjectType::KNIGHT, this->loadTexture("../resources/Chess_Pieces/white_knight.bmp"));
    this->whiteTexture.emplace(ObjectType::BISHOP, this->loadTexture("../resources/Chess_Pieces/white_bishop.bmp"));
    this->whiteTexture.emplace(ObjectType::ROOK, this->loadTexture("../resources/Chess_Pieces/white_rook.bmp"));
    this->whiteTexture.emplace(ObjectType::QUEEN, this->loadTexture("../resources/Chess_Pieces/white_queen.bmp"));
    this->whiteTexture.emplace(ObjectType::KING, this->loadTexture("../resources/Chess_Pieces/white_king.bmp"));

    // Load the texture corresponding to the black pieces
    this->blackTexture.emplace(ObjectType::PAWN, this->loadTexture("../resources/Chess_Pieces/black_pawn.bmp"));
    this->blackTexture.emplace(ObjectType::KNIGHT, this->loadTexture("../resources/Chess_Pieces/black_knight.bmp"));
    this->blackTexture.emplace(ObjectType::BISHOP, this->loadTexture("../resources/Chess_Pieces/black_bishop.bmp"));
    this->blackTexture.emplace(ObjectType::ROOK, this->loadTexture("../resources/Chess_Pieces/black_rook.bmp"));
    this->blackTexture.emplace(ObjectType::QUEEN, this->loadTexture("../resources/Chess_Pieces/black_queen.bmp"));
    this->blackTexture.emplace(ObjectType::KING, this->loadTexture("../resources/Chess_Pieces/black_king.bmp"));

    // Create chessboard
    this->chessboard = Chessboard(this->getVaoID(ObjectType::BOARD), this->getTextureID(ObjectType::BOARD), this->objectBuffers.at(ObjectType::BOARD).getNumIndices());

    // Notify user
    std::cout << "Manager correctly created" << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the unique instance's reference of the SceneManager
 * @note This function is static and can be called without an instance of the class (Call SceneManager::getInstance())
 * @note The first time that the function is called, it creates a SceneManager instance initialized using the custom constructor. The susbsequent times,
 * it will return the unique instance, as a static variable can be instanced only once.
 * @return SceneManager& the unique instance's reference of the SceneManager
 */

// Static method
SceneManager& SceneManager::getInstance(){
    // Construct the SceneManager if it is the first time the instance is called
    static SceneManager instance;
    // Return the unique instance 
    return instance;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Load the chess board
 * @details This function uses the Assimp library to load the VertexData structure of the chess board and create the vertexData attribute of the SceneManager class
 * @param filePath : the path to the file containing the chess board
 * @return true if the loading is successful, false otherwise
 */

bool SceneManager::loadBoard(std::string filePath){
    
    // Load the file using AssImp
	Assimp::Importer importer;

    // Store the file data into aiScene object
	const aiScene* scene = importer.ReadFile(filePath, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	if( !scene) {
		fprintf( stderr, importer.GetErrorString());
		getchar();
		return false;
	}

    // Create a new VertexData object
    VertexData vertexStruct;
    
    // Get the main (and only) mesh
    const aiMesh* mesh = scene->mMeshes[0];
        
    // Fill vertices positions
    vertexStruct.verticies.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D pos = mesh->mVertices[i];
        vertexStruct.verticies.push_back(glm::vec3(pos.x, pos.y, pos.z));
    }

    // Fill vertices texture coordinates
    vertexStruct.uvs.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
        vertexStruct.uvs.push_back(glm::vec2(UVW.x, UVW.y));
    }

    // Fill vertices normals
    vertexStruct.normals.reserve(mesh->mNumVertices);
    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D n = mesh->mNormals[i];
        vertexStruct.normals.push_back(glm::vec3(n.x, n.y, n.z));
    }

    // Fill face indices
    vertexStruct.indices.reserve(3*mesh->mNumFaces);
    for (unsigned int i=0; i<mesh->mNumFaces; i++){
        // Assume the model has only triangles.
        vertexStruct.indices.push_back(mesh->mFaces[i].mIndices[0]);
        vertexStruct.indices.push_back(mesh->mFaces[i].mIndices[1]);
        vertexStruct.indices.push_back(mesh->mFaces[i].mIndices[2]);
    }

    // Compute the number of indices
    vertexStruct.numIndices = vertexStruct.indices.size();

    // Find the horizontal projection of the center of mass of the current mesh
    float sumX = 0.f;
    float sumZ = 0.f;
    for(auto vertex : vertexStruct.verticies){
        sumX += vertex.x;
        sumZ += vertex.z;
    }
    glm::vec3 center(sumX/vertexStruct.numIndices, 0.f, sumZ/vertexStruct.numIndices);

    // Center the mesh on the origin of the horizontal plane (vertices are captured by reference to enable modification)
    for(auto& vertex : vertexStruct.verticies){
        vertex -= center;
    }

    // Build the openGL buffers from the VertexData structure
    this->objectBuffers.emplace(ObjectType::BOARD, GLBuffersID(vertexStruct));
    
    // The "scene" pointer will be deleted automatically by "importer"   
    // If we end up here, the loading step is sucessful
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Load the pieces all at once (from the same file)
 * @details This function uses the Assimp library to load the VertexData structures of the different meshes among the file and create the vertexData attribute of the SceneManager class
 * @param filePath : the path to the file containing the objects
 * @return true if the loading is successful, false otherwise
 */

bool SceneManager::loadPieces(std::string filePath){
    
    // Load the file using AssImp
	Assimp::Importer importer;

    // Store the file data into aiScene object
	const aiScene* scene = importer.ReadFile(filePath, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	if( !scene) {
		fprintf( stderr, importer.GetErrorString());
		getchar();
		return false;
	}

    // Map the index of the meshes to the object types
    std::map<ObjectType,int> meshIdx({
        {ObjectType::PAWN, 5},
        {ObjectType::KNIGHT, 3},
        {ObjectType::BISHOP, 1},
        {ObjectType::ROOK, 11},
        {ObjectType::QUEEN, 9},
        {ObjectType::KING, 7}
    });

    // Loop over the different meshes in the scene and store their vertices data
    for(auto it=meshIdx.begin(); it!=meshIdx.end(); it++){
        
        // Create a new VertexData object
        VertexData vertexStruct;
        
        // Get the mesh
        const aiMesh* mesh = scene->mMeshes[it->second];
        
        // Fill vertices positions
        vertexStruct.verticies.reserve(mesh->mNumVertices);
        for(unsigned int i=0; i<mesh->mNumVertices; i++){
            aiVector3D pos = mesh->mVertices[i];
            vertexStruct.verticies.push_back(glm::vec3(pos.x, pos.y, pos.z));
        }

        // Fill vertices texture coordinates
        vertexStruct.uvs.reserve(mesh->mNumVertices);
        for(unsigned int i=0; i<mesh->mNumVertices; i++){
            aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1
            vertexStruct.uvs.push_back(glm::vec2(UVW.x, UVW.y));
        }

        // Fill vertices normals
        vertexStruct.normals.reserve(mesh->mNumVertices);
        for(unsigned int i=0; i<mesh->mNumVertices; i++){
            aiVector3D n = mesh->mNormals[i];
            vertexStruct.normals.push_back(glm::vec3(n.x, n.y, n.z));
        }

        // Fill face indices
        vertexStruct.indices.reserve(3*mesh->mNumFaces);
        for (unsigned int i=0; i<mesh->mNumFaces; i++){
            // Assume the model has only triangles.
            vertexStruct.indices.push_back(mesh->mFaces[i].mIndices[0]);
            vertexStruct.indices.push_back(mesh->mFaces[i].mIndices[1]);
            vertexStruct.indices.push_back(mesh->mFaces[i].mIndices[2]);
        }

        // Compute the number of indices
        vertexStruct.numIndices = vertexStruct.indices.size();

        // Find the horizontal projection of the center of mass of the current mesh
        float sumX = 0.f;
        float sumZ = 0.f;
        for(auto vertex : vertexStruct.verticies){
            sumX += vertex.x;
            sumZ += vertex.z;
        }
        glm::vec3 center(sumX/vertexStruct.verticies.size(), 0.f, sumZ/vertexStruct.verticies.size());

        // Center the mesh on the origin of the horizontal plane
        for(auto& vertex : vertexStruct.verticies){
            vertex -= center;
        }

        // Build the openGL buffers from the VertexData structure
        this->objectBuffers.emplace(it->first, GLBuffersID(vertexStruct));
    }
    // The "scene" pointer will be deleted automatically by "importer"   
    // If we end up here, the loading step is sucessful
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Load a texture
 * @details This function reads a BMP texture to fill the textureData attribute of the SceneManager class
 * @param filePath : the path to the file containing the texture
 * @return true if the loading is successful, false otherwise
 */

GLuint SceneManager::loadTexture(const std::string imagePath) {

    // Try to read the image and fill the textureData attribute
    try {
        // Open file
        std::ifstream file(imagePath, std::ios::binary);

        // Check if the file is open
        if (!file) {
            throw std::runtime_error("Could not open file: " + imagePath);
        }

        // Read header
        std::vector<char> header(54);
        if (!file.read(header.data(), header.size())) {
            throw std::runtime_error("Failed to read BMP header");
        }

        // Validate BMP format
        if (header[0] != 'B' || header[1] != 'M') {
            throw std::runtime_error("Not a valid BMP file");
        }

        // Validate color depth
        if (*reinterpret_cast<int*>(&header[0x1E]) != 0 ||
            *reinterpret_cast<int*>(&header[0x1C]) != 24) {
            throw std::runtime_error("Not a 24-bit BMP file");
        }

        // Get image information
        uint32_t dataPos = *reinterpret_cast<int*>(&header[0x0A]);
        uint32_t imageSize = *reinterpret_cast<int*>(&header[0x22]);
        uint32_t width = *reinterpret_cast<int*>(&header[0x12]);
        uint32_t height = *reinterpret_cast<int*>(&header[0x16]);

        // Handle misformatted files
        if (imageSize == 0) imageSize = width * height * 3;
        if (dataPos == 0) dataPos = 54;

        // Read pixel data
        std::vector<unsigned char> data(imageSize);
        file.seekg(dataPos, std::ios::beg);
        if (!file.read(reinterpret_cast<char*>(data.data()), imageSize)) {
            throw std::runtime_error("Failed to read image data");
        }

        // Create OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, 
                    GL_UNSIGNED_BYTE, data.data());

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        return textureID;

    // Catch any error that could happen as described in the try block
    } catch (const std::exception& e) {
        std::cerr << "Error loading texture: " << e.what() << std::endl;
        return 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set up the board
 * @details This function sets up the board by placing the pieces on the grid
 */

void SceneManager::setUpBoard(){

    // Allow the setup if it is not already done
    if(this->chessboard.getSetUpState()){
        // Place the pawns
        for(int i=0; i<8; i++){
            // White pawns on row 1 (from 0)
            this->chessboard.grid[1][i].setPiece(ChessPiece(ObjectType::PAWN, Team::WHITE, this->getVaoID(ObjectType::PAWN), this->getTextureID(ObjectType::PAWN, Team::WHITE), this->objectBuffers.at(ObjectType::PAWN).getNumIndices()));
            // Black pawns on row 6 (from 0)
            this->chessboard.grid[6][i].setPiece(ChessPiece(ObjectType::PAWN, Team::BLACK, this->getVaoID(ObjectType::PAWN), this->getTextureID(ObjectType::PAWN, Team::BLACK), this->objectBuffers.at(ObjectType::PAWN).getNumIndices()));
        }

        // Now the board is set up
        this->chessboard.setSetUpState();
    }
    
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Render the scene
 * @details This function renders the scene by calling the render function of the chessboard
 * @param shaderPtr : Pointer to the shader to use
 * @param viewController+tr : Pointer to the view controller to use
 */
void SceneManager::render(Shader* shaderPtr, ViewController* viewControllerPtr){
    this->chessboard.render(shaderPtr, viewControllerPtr);
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get a vao pointer
 * @details This function returns a pointer to the VAO of one GLBuffersID object
 * @param type : the type of the object
 * @return GLuint* the VAO pointer
 */

const GLuint SceneManager::getVaoID(ObjectType type) const{
    return this->objectBuffers.at(type).getVaoID();
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get a texture pointer
 * @details This function returns a pointer to the texture of one object
 * @param type : the type of the object
 * @return GLuint* the texture pointer
 */

const GLuint SceneManager::getTextureID(ObjectType type) const{
    switch (type){
        case ObjectType::BOARD:
            return static_cast<const GLuint>(this->otherTexture.at(type));
            break;
        
        default:
            std::cerr << "Error: the object type is not a supported for now. If the type is a chess piece, please mention the team" << std::endl;
            return 0; 
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get a texture pointer (override for the team)
 * @details This function returns a pointer to the texture of one object
 * @param type : the type of the object
 * @param team : the team of the object
 * @return GLuint* the texture pointer
 */

const GLuint SceneManager::getTextureID(ObjectType type, Team team) const{
    switch (team){
        case Team::WHITE:
            return static_cast<const GLuint>(this->whiteTexture.at(type));
            break;
        
        case Team::BLACK:
            return static_cast<const GLuint>(this->blackTexture.at(type));
            break;

        default:
            std::cerr << "Error: the team is not a supported for now" << std::endl;
            return 0;
            break;
    }
}

/**
 * @brief Destructor
 * @details GLBuffersID object are automatically deleted by the map container, its destructor is called automatically and deletes the VAO and VBOs
 * Then it is still required to delete the textures
 */
SceneManager::~SceneManager(){

    // Delete the textures for white pieces
    for(auto it=this->whiteTexture.begin(); it!=this->whiteTexture.end(); it++){
        glDeleteTextures(1, &(it->second));
        std::cout << "Deleted white texures" << std::endl;
    }

    // Delete the textures for black pieces
    for(auto it=this->blackTexture.begin(); it!=this->blackTexture.end(); it++){
        glDeleteTextures(1, &(it->second));
        std::cout << "Deleted black texures"<< std::endl;
    }

    // Delete the textures for other objetcs
    for(auto it=this->otherTexture.begin(); it!=this->otherTexture.end(); it++){
        glDeleteTextures(1, &(it->second));
        std::cout << "Deleted other texures"<< std::endl;
    }

    // Delete the GL buffers for each object (vbos, ebo, vao)
    for(auto it=this->objectBuffers.begin(); it!=this->objectBuffers.end(); it++){
        it->second.deleteBuffers();
        std::cout << "Deleted buffers for object type: " << static_cast<int>(it->first) << std::endl;
    }
}	