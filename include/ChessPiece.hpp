/**
 * @author obiwan138
 * @class ChessPiece.hpp
 * @brief Define what is a Chess Piece (abstract class) - Header file
 */

#pragma once

#include "ChessObject.hpp" 
#include "enumerations/MeshTypes.hpp"
#include "enumerations/Team.hpp"

class ChessPiece : public ChessObject{

    // Member variables
    private :
        MeshTypes type;     // Type of the piece
        Team team;          // Team of the piece (BLACK, WHITE)
        bool alive;         // Is the piece alive or not
    
    public :
        // Default constructor
        ChessPiece();

        // Custom Constructor
        ChessPiece(MeshTypes typeIn, Team teamIn, GLuint vaoID, GLuint textureID, unsigned short numIndicesIn);

        // Operator =
        ChessPiece& operator=(const ChessPiece& other);

        // Getters
        MeshTypes getType() const;
        Team getTeam() const;
        bool getAlive() const;

        // Setters
        void setType(MeshTypes typeIn);
        void setTeam(Team teamIn);
        void setAlive(bool aliveIn);

        // Destructor
        ~ChessPiece();
};


