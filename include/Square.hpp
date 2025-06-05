/**
 * @author obiwan138
 * @class Square
 * @brief Class to represent a square on the chessboard
 */

#pragma once

// Standard libraries
#include <string>

// External libraries
#include <glm/glm.hpp>            // OpenGL Mathematics

// Project headers
#include "ChessPiece.hpp"

class Square{

    private :

        std::string notation;               // Notation of this square
        glm::vec3 position;                 // Position of the square
        ChessPiece piece;                   // Piece at this square (eventually none)

    public : 

        // Default constructor
        Square();

        // Custom constructor to init all the squares of the board
        Square(const std::string notationIn, const glm::vec3 position);

        // Get if the square is occupied by a piece
        bool isOccupied() const;

        // Render the piece
        void renderPiece(Shader* shaderPtr, ViewController* viewControllerPtr);

        // Get the notation of the square
        std::string getNotation() const;

        // Get the position of the square
        glm::vec3 getPosition() const;       
        
        // Set the notation
        void setNotation(const std::string notationIn);

        // Set position
        void setPosition(const glm::vec3);

        // Set piece
        void setPiece(const ChessPiece& pieceIn);
};