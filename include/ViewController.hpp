/**
 * @author obiwan138
 * @class ViewController
 * @brief This class is used to control the camera view in the scene
 * @details The camera is controlled by the user inputs (keyboard) and the camera is moved in a spherical coordinate system around the origin
 */

#pragma once

// Include SFML
#include <SFML/Graphics.hpp>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ViewController 
{
    private:

        // Camera coordinates
        float radius; // Distance from the origin [m]                       // >0
        float theta;  // Elevation angle form horizontal plane [rad]        // [-pi/2, pi/2]
        float phi;    // Azimut angle (sens x-axis to z-axis) plane [rad]   // [0, 2*pi]

        // Camera speed 
        const float angularSpeed = 1.f;     // Angular speed (rotation) [rad/s]
        const float radialSpeed = 5.f;            // raial speed (zoom) [m/s]

        // Camera Field of View
        const float fov = 45.f; // [deg]

        // Safety parameters
        const float minRadius = 0.1f; // Minimum radius [m]
        const float maxElvation = 0.9f*3.14f/2.f; // Minimum radius [m]
        const float minElvation = - 0.9f*3.14f/2.f; // Minimum radius [m]

        // Induced matrices
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        // Clock for the time difference between current and last frame
        sf::Clock clock;                
        
    public:

        // Default constructor 
        ViewController();

        // Custom constructor
        ViewController(const float radius, const float elevationDegrees, const float azimutDegrees);

        // Actualize the matrices from the user inputs
        void updateMatrices();

        // Convert spherical coordinates to cartesian coordinates
        glm::vec3 getCartesianCoord() const;

        // Get the view matrix
        glm::mat4 getViewMatrix() const;

        // Get the projection matrix
        glm::mat4 getProjectionMatrix() const;

        // Destructor
        ~ViewController();

};