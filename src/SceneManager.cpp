/**
 * @author obiwan138
 * @file SceneManager.cpp
 * @brief Implementation of the SceneManager class
 */

#include <iostream>	
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <utility>
#include <omp.h> 

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

    // Path of the game textures
    std::vector<std::pair<TextureTypes, std::string>> texturePaths = 
    {
        {TextureTypes::BOARD, "../resources/Stone_Chess_Board/Stone_chessboard_diffuse_image.bmp"},
        {TextureTypes::WHITE_PAWN, "../resources/Chess_Pieces/white_pawn.bmp"},
        {TextureTypes::WHITE_ROOK, "../resources/Chess_Pieces/white_rook.bmp"},
        {TextureTypes::WHITE_KNIGHT, "../resources/Chess_Pieces/white_knight.bmp"},
        {TextureTypes::WHITE_BISHOP, "../resources/Chess_Pieces/white_bishop.bmp"},
        {TextureTypes::WHITE_QUEEN, "../resources/Chess_Pieces/white_queen.bmp"},
        {TextureTypes::WHITE_KING, "../resources/Chess_Pieces/white_king.bmp"},
        {TextureTypes::BLACK_PAWN, "../resources/Chess_Pieces/black_pawn.bmp"},
        {TextureTypes::BLACK_ROOK, "../resources/Chess_Pieces/black_rook.bmp"},
        {TextureTypes::BLACK_KNIGHT, "../resources/Chess_Pieces/black_knight.bmp"},
        {TextureTypes::BLACK_BISHOP, "../resources/Chess_Pieces/black_bishop.bmp"},
        {TextureTypes::BLACK_QUEEN, "../resources/Chess_Pieces/black_queen.bmp"},
        {TextureTypes::BLACK_KING, "../resources/Chess_Pieces/black_king.bmp"}
    };

    if(!this->loadTextures(texturePaths)){
        std::cerr << "Error while loading the textures. Check the correspond file and path" << std::endl;
    };

    // Create chessboard
    this->chessboard = Chessboard(this->getVaoID(MeshTypes::BOARD), this->getTextureID(MeshTypes::BOARD), this->objectBuffers.at(MeshTypes::BOARD).getNumIndices());

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
 * @details This function uses the Assimp library to load the RawVertexData structure of the chess board and create the RawVertexData attribute of the SceneManager class
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

    // Create a new RawVertexData object
    RawVertexData vertexStruct;
    
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

    // Build the openGL buffers from the RawVertexData structure
    this->objectBuffers.emplace(MeshTypes::BOARD, GLBuffersID(vertexStruct));
    
    // The "scene" pointer will be deleted automatically by "importer"   
    // If we end up here, the loading step is sucessful
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Load the pieces all at once (from the same file)
 * @details This function uses the Assimp library to load the RawVertexData structures of the different meshes among the file and create the RawVertexData attribute of the SceneManager class
 * @details The function uses OpenMP to speed up the loading process by parallelizing the loading of the different meshes
 * 
 * @param filePath : the path to the file containing the objects
 * 
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
    std::vector<std::pair<MeshTypes,int>> meshIdx = 
    {
        {MeshTypes::PAWN, 5},
        {MeshTypes::KNIGHT, 3},
        {MeshTypes::BISHOP, 1},
        {MeshTypes::ROOK, 11},
        {MeshTypes::QUEEN, 9},
        {MeshTypes::KING, 7}
    };

    // Create a map to store the mesh types and their corresponding indices
    std::map<MeshTypes, RawVertexData> meshData;

    // Loop over the different meshes in the scene and store their vertices data, speed up the process using OpenMP
    #pragma omp parallel for
    for(int i=0; i<meshIdx.size(); i++){
        
        // Create a new RawVertexData object
        RawVertexData vertexStruct;
        
        // Get the mesh
        const aiMesh* mesh = scene->mMeshes[meshIdx[i].second];
        
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

        // Save the mesh data in a thread-safe manner
        #pragma omp critical
        {
            // Add the mesh data to the map
            meshData.emplace(meshIdx[i].first, vertexStruct);
        }
    }
    // The "scene" pointer will be deleted automatically by "importer"   
    // If we end up here, the loading step is sucessful

    // Now load the mesh data into OpenGL
    for (const auto& pair : meshData) {
        
        // Load the Open GL buffers from the raw mesh data
        this->objectBuffers.emplace(pair.first, GLBuffersID(pair.second));
    }

    // If we end up here, the loading step is successful
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Read texture data from file
 * @details This function reads a BMP texture file and returns the raw texture data.
 * 
 * @param filePath : the path to the file containing the texture
 * 
 * @return RawTextureData The raw texture data read from the file
 */
RawTextureData SceneManager::readTextureData(const std::string& filePath) {

    // Open the file
    std::ifstream file(filePath, std::ios::binary);
    
    // Check if the file is open
    if (!file) {
        throw std::runtime_error("Could not open file: " + filePath);
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
    RawTextureData textureData;
    textureData.data.resize(imageSize);
    
    file.seekg(dataPos, std::ios::beg);
    
    if (!file.read(reinterpret_cast<char*>(textureData.data.data()), imageSize)) {
        throw std::runtime_error("Failed to read image data");
    }

    // Set texture properties
    textureData.width = width;
    textureData.height = height;

    return textureData;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Send texture data to GPU
 * @details This function uploads the raw texture data to the GPU and returns the OpenGL texture ID.
 * 
 * @param textureData : the raw texture data to upload
 * 
 * @return GLuint The OpenGL texture ID
 */
GLuint SceneManager::sendTextureToGPU(const RawTextureData& textureData) {

    // Generate a texture ID
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Upload the texture data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData.width, textureData.height, 0, GL_BGR, 
                 GL_UNSIGNED_BYTE, textureData.data.data());

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

///////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Read texture data from file and load it into OpenGL
 * @details Enhance the processing time by using OpenMP to read multiple textures in parallel.
 * 
 * @param texturePaths : the paths to the file containing the textures
 * 
 * @return true if the loading is successful, false otherwise
 */
bool SceneManager::loadTextures(const std::vector<std::pair<TextureTypes, std::string>>& texturePaths) {

    // Texture map to store the loaded textures
    std::map<TextureTypes, RawTextureData> rawTextures;

    // Read the texture files in parallel using OpenMP
    #pragma omp parallel for
    for (int i = 0; i < texturePaths.size(); ++i) 
    {
        // Read the texture data from the file
        RawTextureData data = readTextureData(texturePaths[i].second);

        // Add it to the map in a thread-safe manner
        #pragma omp critical
        {
            rawTextures.emplace(std::make_pair(texturePaths[i].first, data));
        }
    }

    // For each texture in the rawTextures map, send it to the GPU
    // Note: This is done sequentially as OpenGL calls are not thread-safe
    for (const auto& pair : rawTextures) {

        // Send the texture data to the GPU and register the texture ID
        this->textures.emplace(pair.first, sendTextureToGPU(pair.second));
    }

    // If we end up here, the loading step is successful
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set up the board
 * @details This function sets up the board by placing the pieces on the grid
 */
/*
void SceneManager::setUpBoard(){

    // Allow the setup if it is not already done
    if(this->chessboard.getSetUpState()){
        // Place the pawns
        for(int i=0; i<8; i++){
            // White pawns on row 1 (from 0)
            this->chessboard.grid[1][i].setPiece(ChessPiece(MeshTypes::PAWN, Team::WHITE, this->getVaoID(MeshTypes::PAWN), this->getTextureID(TextureTypes::PAWN, Team::WHITE), this->objectBuffers.at(MeshTypes::PAWN).getNumIndices()));
            // Black pawns on row 6 (from 0)
            this->chessboard.grid[6][i].setPiece(ChessPiece(MeshTypes::PAWN, Team::BLACK, this->getVaoID(MeshTypes::PAWN), this->getTextureID(MeshTypes::PAWN, Team::BLACK), this->objectBuffers.at(MeshTypes::PAWN).getNumIndices()));
        }

        // Now the board is set up
        this->chessboard.setSetUpState();
    }
    
}
    */

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

const GLuint SceneManager::getVaoID(MeshTypes type) const{
    return this->objectBuffers.at(type).getVaoID();
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get a texture pointer (override for the team)
 * @details This function returns a pointer to the texture of one object
 * @param type : the type of the object
 * @param team : the team of the object
 * @return GLuint* the texture pointer
 */

const GLuint SceneManager::getTextureID(TextureTypes name, Team team = Team::NONE) const{
    switch (team){
        case Team::WHITE:
            switch (name) 
            {
                case TextureTypes::WHITE_PAWN:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_PAWN));
                    break;
                case TextureTypes::WHITE_KNIGHT:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_KNIGHT));
                    break;
                case TextureTypes::WHITE_BISHOP:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_BISHOP));
                    break;
                case TextureTypes::WHITE_ROOK:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_ROOK));
                    break;
                case TextureTypes::WHITE_QUEEN:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_QUEEN));
                    break;
                case TextureTypes::WHITE_KING:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_KING));
                    break;
                default:
                    std::cerr << "Error: the texture type is not supported for now" << std::endl;
                    return 0;
                    break;
            }
            break;
        
        case Team::BLACK:
            switch (name) 
            {
                case TextureTypes::BLACK_PAWN:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_PAWN));
                    break;
                case TextureTypes::WHITE_KNIGHT:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_KNIGHT));
                    break;
                case TextureTypes::WHITE_BISHOP:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_BISHOP));
                    break;
                case TextureTypes::WHITE_ROOK:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_ROOK));
                    break;
                case TextureTypes::WHITE_QUEEN:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_QUEEN));
                    break;
                case TextureTypes::WHITE_KING:
                    return static_cast<const GLuint>(this->textures.at(TextureTypes::WHITE_KING));
                    break;
                default:
                    std::cerr << "Error: the texture type is not supported for now" << std::endl;
                    return 0;
                    break;
            }
            break;

        case Team::NONE:
            // If the team is not specified, return the texture for the board
            if(name == TextureTypes::BOARD){
                return static_cast<const GLuint>(this->textures.at(TextureTypes::BOARD));
            }
            else{
                std::cerr << "Error: the team is not specified. Please specify the team for the piece" << std::endl;
                return 0;
            }
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
    for(auto it=this->textures.begin(); it!=this->textures.end(); it++){
        glDeleteTextures(1, &(it->second));
        std::cout << "Deleted white texures" << std::endl;
    }

    // Delete the textures for black pieces
    for(auto it=this->textures.begin(); it!=this->textures.end(); it++){
        glDeleteTextures(1, &(it->second));
        std::cout << "Deleted black texures"<< std::endl;
    }

    // Delete the GL buffers for each object (vbos, ebo, vao)
    for(auto it=this->objectBuffers.begin(); it!=this->objectBuffers.end(); it++){
        it->second.deleteBuffers();
        std::cout << "Deleted buffers for object type: " << static_cast<int>(it->first) << std::endl;
    }
}	