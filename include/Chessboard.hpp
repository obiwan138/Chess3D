/**
 * @author obiwan138
 * @class Chessboard
 * @brief Class to represent the chessboard
 */ 

#pragma once

// Standard libraries
#include <array>

// Project headers
#include "ChessObject.hpp"
#include "Square.hpp"

class Chessboard : public ChessObject{

    private :

        bool setUpState;  // is the board set up with the pieces ?

    public :

        //2D array of squares representing the chessboard
        std::array<std::array<Square,8>,8> grid;

        // Default constructor
        Chessboard();

        // Custom constructor 
        Chessboard(GLuint vaoID, GLuint textureID, unsigned short numIndicesIn);

        // Init the chessboard grid
        void initGrid();

        // Render the chessboard
        void render(Shader* shaderPtr, ViewController* viewControllerPtr);

        // Get setUp
        bool getSetUpState() const;

        // Set the SetUpState variable to true
        void setSetUpState();

        // Set the SetUpState variable to true or false
        void setSetUpState(const bool isSetUp);

        // Destructor 
        ~Chessboard();

};