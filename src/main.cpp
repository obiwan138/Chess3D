/**
 * @author obiwan138
 * @file main.cpp
 * @brief Main program running the 3D Chess game
 */

// Include GLEW
#include <GL/glew.h>						// Init Open GL states
#include <glm/glm.hpp>						// Open GL Math library
#include <glm/gtc/matrix_transform.hpp>

// Include SFML
#include <SFML/Graphics.hpp>				// SFML 2D rendering
#include <SFML/Window.hpp>					// SFML Window creation and management
#include <SFML/OpenGL.hpp>					// SFML OpenGL integration

// Include project header files
#include "Shader.hpp"
#include "SceneManager.hpp"
#include "ViewController.hpp"

int main(int argc, char* argv[])
{
	/********************************************************************
	 * Initialize the SFML Window with OPENGL settings
	 ********************************************************************/

	// Create the window with OpenGL settings
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	// Window creation
    sf::RenderWindow window(sf::VideoMode(1200, 800),	// Window size
							"3D Chess game", 			// Title of the window
							sf::Style::Default, 		// Default window style
							settings);					// OpenGL settings

	window.setVerticalSyncEnabled(true);
	window.setVisible(true);				// Make window visible
	window.setActive(true); 				// Create context for OpenGL

	// Hide the mouse cursor and set it to the center of the window
	window.setMouseCursorVisible(false);
	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

	/********************************************************************
	 * Initialize the OpenGL state machine
	 ********************************************************************/

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		return -1;
	}

	// Dark background
	glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	/********************************************************************
	 * Load the Scene manager
	 * - Managing the GLBuffers for the chess board and pieces
	 ********************************************************************/

	/**
	 * Load the chess object manager
	 */
	SceneManager sceneManager = SceneManager::getInstance();

	ViewController viewController;
	Shader shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

	/********************************************************************
	 * Main loop
	 ********************************************************************/
	
	// Boolean for the main loop
    bool running = true;

	// Main loop
    while (running)
    {
        /********************************************************************
	 	* Handle closing window event and escape key
	 	********************************************************************/
        sf::Event event;
        while (window.pollEvent(event))
        {
			// Check if the user pressed the escape key
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				// end the program
                running = false;	
			}
			// Check if the user closed the window
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
			// Check if the window was resized
            else if (event.type == sf::Event::Resized)
            {
                // Adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

		/********************************************************************
	 	* Actualize the scene
	 	********************************************************************/

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the view controller to update the view settins and matrices from user inputs
		viewController.updateMatrices();	

		// Render the scene
		sceneManager.render(&shader, &viewController);
		
		// End the current frame (internally swaps the front and back buffers of the window)
        window.display();
	}

	// Unbind Open GL states
	glBindVertexArray(0);	// Unbind the VAO
	glUseProgram(0);		// Unbind the shader program

	// The different OpenGL VAO, VBO and shaders are destroyed by the classes which own them

	return 0;
}


