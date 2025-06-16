/**
 * @author obiwan
 * @file Square.cpp
 * @brief Implementation of the square class
 */

 #include "Square.hpp"

/////////////////////////////////////////////////////////////////// 
/**
 * @brief Default constructor
 */

Square::Square():notation(""), position(glm::vec3(0,0,0)), piecePtr(nullptr){}


///////////////////////////////////////////////////////////////////
/**
 * @brief Custom constructor
 * @param notationIn A string value encoding the position (like "e5")
 * @param positionIn A 3D vector locating the center of the square
 */

 Square::Square(const std::string& notationIn, const glm::vec3& positionIn)
                :notation(notationIn), position(positionIn), piecePtr(nullptr){}

///////////////////////////////////////////////////////////////////
/**
 * @brief Get if the square is occupied
 * @return bool
 */
bool Square::isOccupied() const{
    return !(this->piecePtr == nullptr);
}


///////////////////////////////////////////////////////////////////
/**
 * @brief render piece
 */

void Square::renderPiece(Shader* shaderPtr, ViewController* viewControllerPtr){
    if(this->isOccupied()){
        this->piecePtr->render(shaderPtr, viewControllerPtr);
    }
}

///////////////////////////////////////////////////////////////////
/**
 * @brief Get the notation of the square
 * @return A string representing the notation
 */
std::string Square::getNotation() const {
    return this->notation;
}

///////////////////////////////////////////////////////////////////
/**
 * @brief Get the position of the square
 * @return A 3D vector representing the position
 */
glm::vec3 Square::getPosition() const {
    return this->position;
}

///////////////////////////////////////////////////////////////////
/**
 * @brief Set notation
 * @param notationIn the string we want as a notation
 */

void Square::setNotation(const std::string& notationIn){
    this->notation = notationIn;
}

///////////////////////////////////////////////////////////////////
/**
 * @brief Set position
 * @param positionIn the vector we want to locate at
 */

 void Square::setPosition(const glm::vec3& positionIn){
    this->position = positionIn;
}

///////////////////////////////////////////////////////////////////
/**
 * @brief Set piece
 * @param piece The ChessPiece to set
 */

 void Square::setPiece(ChessPiece* pieceInPtr)
 {
    this->piecePtr = pieceInPtr;
 }