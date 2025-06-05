/**
 * @author obiwan138
 * @file Chessboard.cpp
 * @brief Implementation of the Chessboard class
 */ 

#include "Chessboard.hpp"
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Default constructor
 */

Chessboard::Chessboard():ChessObject(){
    this->setUpState = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Custom contructor
 * @param vaoID VAO associated to the chessboard
 * @param textureID Texture of the chessboard
 * @param numIndicesIn Number of indices in the mesh
 * @note The contructor does not set up the board with the pieces, this is done with the setUpBoard function in the SceneManager class
 */

Chessboard::Chessboard(GLuint vaoID, GLuint textureID, unsigned short numIndicesIn):ChessObject(vaoID, textureID, numIndicesIn){
    this->setUpState = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Init Grid
 * @details Init the grid squares without placing the pieces
 * @note The chessboard 3D mesh is such that it's centered on (0,0) and the squares are roughly 1 unit² surface 
 * Furthermore, the x axis points to the right for the white player and the z axis points from the black to the white player
 * @note The grid is a 2D array of squares, the row index maps to the numbers (1 to 8) and the column index maps to the letters (a to h)
 * @example: grid[0][0] is the square a1 located at (-3.5x, 0y, 3.5z), grid[7][7] is the square h8 located at (3.5x, 0y, -3.5z)
 */

void Chessboard::initGrid(){
    // First cell and cell size
    glm::vec3 a1(-3.5f, 0.f, +3.5f);
    char a1Letter = 'a';
    char a1Num = '1';
    float dpos = 1.f;

    // Go over the squares
    for(int i=0; i<grid.size(); i++){
        for(int j=0; j<grid[0].size(); j++){
            
            // Compute the currect square notation
            char letter = a1Letter + j;
            char num = a1Num + i;
            std::string notation = {letter,num};

            // Compute the position of the square
            glm::vec3 pos(a1.x + j*dpos, 0.f, a1.z - i*dpos);
            
            // Assign the Square parameters to the grid
            this->grid[i][j].setNotation(notation);
            this->grid[i][j].setPosition(pos);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Render the chessboard and the pieces
 * @param shaderPtr Pointer to the shader
 * @param viewControllerPtr Pointer to the view controller
 */

void Chessboard::render(Shader* shaderPtr, ViewController* viewControllerPtr){

    // Render the chessboard
    this->ChessObject::render(shaderPtr, viewControllerPtr);

    /* 
    // If the board is set up, render the pieces
    if(this->setUpState){

        // Go over the chessboard
        for(auto& row : this->grid){
            for(auto& square : row){

                // If the current cell is occupied by the piece
                if(square.isOccupied()){

                    // Render the piece
                    square.renderPiece(shaderPtr, viewControllerPtr);
                }
            }
        }
    }
        */
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Determine if the board is set up
 * @return boolean
 */

bool Chessboard::getSetUpState() const{
    return this->setUpState;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set the SetUpState variable to true
 * @param isSetUp
 */

 void Chessboard::setSetUpState(){
    this->setUpState = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set the SetUpState variable to true or false
 * @param isSetUp
 */

void Chessboard::setSetUpState(const bool isSetUp){
    this->setUpState = isSetUp;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Destructor
 */

Chessboard::~Chessboard(){}
