/**
 * @author obiwan138
 * @file ViewController.cpp
 * @brief this file implements the ViewController class methods
 */

#include "ViewController.hpp"

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Default Constructor
 * @details This constructor initializes the camera at the corresponding default position
 * - The camera is placed at a distance of 10 units from the origin
 * - The camera is placed at an elevation of 45 degrees from the horizontal plane
 * - The camera is placed at an azimuth of 90 degrees from the x-axis to the z-axis
 */
ViewController::ViewController(){
	// Define the initial spherical coordinates
    this->radius = 20.f;
    this->theta = glm::radians(45.f);
    this->phi = glm::radians(90.f);

    // Compute the view matrix to look at the origin
	this->viewMatrix = glm::lookAt(
								this->getCartesianCoord(),  // Camera is here
								glm::vec3(0,0,0),           // and looks here : origin
								glm::vec3(0,1,0)            // Head is up (set to 0,-1,0 to look upside-down)
						    );

    // Compute the projection matrix : 45 deg Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	this->projectionMatrix = glm::perspective(glm::radians(this->fov), 4.0f / 3.0f, 0.1f, 100.0f);
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Custom Constructor
 * 
 * @param sphericalCoord0: Initial spherical coordinates
 * @param radialSpeed: Radial speed
 * @param angularSpeed: Angular speed
 * @param fov: Field of view
 */
ViewController::ViewController(const float radius, const float elevationDegrees, const float azimutDegrees)
{
    // Define the initial spherical coordinates
    this->radius = radius;
    this->theta = glm::radians(elevationDegrees);
    this->phi = glm::radians(azimutDegrees);

    // Compute the view matrix to look at the origin
	this->viewMatrix = glm::lookAt(
								this->getCartesianCoord(),  // Camera is here
								glm::vec3(0,0,0),           // and looks here : origin
								glm::vec3(0,1,0)            // Head is up (set to 0,-1,0 to look upside-down)
						    );

    // Compute the projection matrix : 45 deg Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	this->projectionMatrix = glm::perspective(glm::radians(this->fov), 4.0f / 3.0f, 0.1f, 100.0f);
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Compute the view and the projection matrices from the user input
 */
void ViewController::updateMatrices()
{
	// Time difference between current and last frame
	float dt = (this->clock.restart()).asSeconds();

    // Move radially closer to the origin
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ 		// Key Up in AZERY keyboard config
		this->radius -= dt * this->radialSpeed;

        // Limit the radius to a minimum value
        if(this->radius < this->minRadius) 
        {
            this->radius = this->minRadius;
        }
	}
    // Move radially closer to the origin
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){ // Key Down for AZERTY keyboard
		this->radius += dt * this->radialSpeed;
	}
    // Rotate the camera to the left at constant radius pointing to the origin
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){    // Key Q AZERTY keyboard
		this->phi += dt * this->angularSpeed;
	}
    // Rotate the camera to the right at constant radius pointing to the origin
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){    // Key D for AZERTY keyboard
		this->phi -= dt * this->angularSpeed;
	}
	// Move Up around origin
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){   // Key Z for AZERTY keyboard
		this->theta += dt * this->angularSpeed;

        // Limit the elevation to a minimum value
        if(this->theta > this->maxElvation) 
        {
            this->theta = this->maxElvation;
        }
	}
	// Move down around origin
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){   // Key S for AZERTY keyboard
		this->theta -= dt * this->angularSpeed;

        // Limit the elevation to a minimum value
        if(this->theta < this->minElvation) 
        {
            this->theta = this->minElvation;
        }
	}

	// Finally, update the Camera matrix
	this->viewMatrix = glm::lookAt(
		this->getCartesianCoord(),  // Camera is here
		glm::vec3(0,0,0),           // and looks here : origin
		glm::vec3(0,1,0)            // Head is up (set to 0,-1,0 to look upside-down)
	);
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Convert spherical coordinates to cartesian coordinates
 * @note Convention : OpenGL uses a right-handed coordinate system (x-right, y-up, z-back)
 * - theta angle is the elevation angle from horizotal plane (-pi/2 <= theta <= pi/2)
 * - phi angle is the azimuth angle from the x-axis to z-axis (0 <= phi <= 2*pi)
 * @return glm::vec3
 */
glm::vec3 ViewController::getCartesianCoord() const
{
	return glm::vec3(
		this->radius * cos(this->theta) * cos(this->phi),   // RIGHT AXIS : x = r * cos(theta) * cos(phi)
		this->radius * sin(this->theta),					// UP AXIS 	  : y = r * sin(theta)
		this->radius * cos(this->theta) * sin(this->phi)	// BACK AXIS  : z = r * cos(theta) * sin(phi)
	);
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the projection matrix
 * @return glm::mat4
 */
glm::mat4 ViewController::getProjectionMatrix() const
{
	return this->projectionMatrix;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the view matrix
 * @return glm::mat4
 */
glm::mat4 ViewController::getViewMatrix() const
{
	return this->viewMatrix;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * @brief Destructor
 */
ViewController::~ViewController()
{
	// Nothing to do
}