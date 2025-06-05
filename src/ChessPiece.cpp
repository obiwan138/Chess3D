/**
 * @author obiwan138
 * @file ChessPiece.hpp
 * @brief Define the chess piece - Implementation file
 */

#include "ChessPiece.hpp"

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Default Constructor
 * @details Initialize an empty piece
 */

ChessPiece::ChessPiece():ChessObject(){
    this->type = ObjectType::BOARD;
    this->team = Team::NONE;
    this->alive = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Custom Constructor 
 * @param typeIn Type of the piece
 * @param teamIn Team of the piece
 * @param vaoPointerIn Pointer to the associated Vertex Array Object (VAO)
 * @param texturePointerIn Pointer to the associated texture
 */
ChessPiece::ChessPiece(ObjectType typeIn, Team teamIn, GLuint vaoID, GLuint textureID, unsigned short numIndicesIn):ChessObject(vaoID, textureID, numIndicesIn){
    this->type = typeIn;
    this->team = teamIn;
    this->alive = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Operator =
 * @param other ChessPiece to copy
 */
ChessPiece& ChessPiece::operator=(const ChessPiece& other){
    // Check if this and other are the same (self-check assignment)
    if(this==&other){
        return *this;
    }
    else{
        // Assign each member variable of other to this
        this->vao = other.vao;
        this->texture = other.texture;
        this->numIndices = other.numIndices;
        this->modelMatrix = other.modelMatrix;
        this->type = other.type;
        this->team = other.team;
        this->alive = other.alive;
        return *this;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the type of the piece
 * @return ObjectType of the piece
 */
ObjectType ChessPiece::getType() const{
    return this->type;
}


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the team of the piece
 * @return Team of the piece
 */
Team ChessPiece::getTeam() const{
    return this->team;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the state of the piece
 * @return Is the piece alive or not
 */
bool ChessPiece::getAlive() const{
    return this->alive;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set the team of the piece
 * @param teamIn Team of the piece
 */
void ChessPiece::setType(ObjectType typeIn){
    this->type = typeIn;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set the team of the piece
 * @param teamIn Team of the piece
 */
void ChessPiece::setTeam(Team teamIn){
    this->team = teamIn;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Set the state of the piece
 * @param aliveIn Is the piece alive or not
 */
void ChessPiece::setAlive(bool aliveIn){
    this->alive = aliveIn;
}

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Default Destructor
 */
ChessPiece::~ChessPiece(){}
