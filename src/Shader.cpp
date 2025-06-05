/**
 * @author obiwan138
 * @file Shader.hpp
 * @brief Implementation of the member functions for the Shader program class
 */

#include "Shader.hpp"

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Load the shaders
 * @param vertexPath : the path to the vertex shader file
 * @param fragmentPath : the path to the fragment shader file
 * @return GLuint the ID of the shader program
 */

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    // Load the GLSL shader files and compile the shader program
    this->programID = this->loadShaders(vertexPath, fragmentPath);

    // Get a handle for our uniform variables
    this->modelMatrixID = glGetUniformLocation(this->getID(), "M");
    this->viewMatrixID = glGetUniformLocation(this->getID(), "V");
    this->mvpMatrixID = glGetUniformLocation(this->getID(), "MVP");
    this->textureID = glGetUniformLocation(this->getID(), "ShaderTexture");
    this->lightID = glGetUniformLocation(this->getID(), "LightPosition_worldspace");
    
    // Set the light's position
    this->lightPosition = glm::vec3(0,15,0);
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Load the shaders
 * @param vertexPath : the path to the vertex shader file
 * @param fragmentPath : the path to the fragment shader file
 * @return GLuint the ID of the shader program
 */

 GLuint Shader::loadShaders(const char* vertexPath, const char* fragmentPath) {

    // Create a lambda function that opens file, return its content as a string and close it
    auto readFile = [](const char* filePath) -> std::string {
        std::ifstream file(filePath, std::ios::in);
        if (!file.is_open()) {
            std::cerr << "Failed to open " << filePath << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    };

    // Read the vertex and fragment shader files
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    // If one of them is empty, return 0
    if (vertexCode.empty() || fragmentCode.empty()) return 0;

    // Compile the shaders
    GLuint vertexShader = this->compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = this->compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    
    // Create the shader program id and associate the shaders programs to it
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    // Test if the shader program is successfully created
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetProgramInfoLog(program, length, nullptr, log.data());
        std::cerr << "Shader Program Linking Failed:\n" << log.data() << std::endl;
    }
    
    // Delete the compiled shaders, we only need the shader program id
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Return the program ID
    return program;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Compile a shader
 * @param source : the source code of the shader
 * @param type : the type of the shader (GL_VERTEX_SHADER or GL_VERTEX_FRAGMENT here)
 * @return GLuint the ID of the shader
 */
GLuint Shader::compileShader(const char* source, GLenum type) {

    // Create a GL shader program
    GLuint shader = glCreateShader(type);

    // Associate a source code
    glShaderSource(shader, 1, &source, nullptr);

    // Compile the shader
    glCompileShader(shader);

    // Test if compilation is successful
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> log(length);
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        std::cerr << "Shader Compilation Failed:\n" << log.data() << std::endl;
    }

    // Return the shader program
    return shader;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Use the shader program
 * @details This function activates the shader program using glUseProgram()
 */
void Shader::use() const {
    glUseProgram(this->programID);
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the ID of the shader program
 * @details This function returns a copy of the ID of the shader program. ProgramID being a GLuint (unsigned int)
 * It is not necessary to return a const reference.
 * 
 * @return GLuint the ID of the shader program
 */
GLuint Shader::getID() const {
    return this->programID;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the ID of the shader model matrix uniform variable
 * @details This function returns a copy of the ID of the shader model matrix uniform variable. modelMatrixID being a GLuint (unsigned int)
 * It is not necessary to return a const reference.
 * 
 * @return GLuint the ID of the shader model matrix uniform variable
 */
GLuint Shader::getModelMatrixID() const {
    return this->modelMatrixID;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the ID of the shader view matrix uniform variable
 * @details This function returns a copy of the ID of the shader view matrix uniform variable. viewMatrixID being a GLuint (unsigned int)
 * It is not necessary to return a const reference.
 * 
 * @return GLuint the ID of the shader view matrix uniform variable
 */
GLuint Shader::getViewMatrixID() const {
    return this->viewMatrixID;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the ID of the shader MVP matrix uniform variable
 * @details This function returns a copy of the ID of the shader MVP matrix uniform variable. mvpMatrixID being a GLuint (unsigned int)
 * It is not necessary to return a const reference.
 * 
 * @return GLuint the ID of the shader MVP matrix uniform variable
 */
GLuint Shader::getMvpMatrixID() const {
    return this->mvpMatrixID;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get the ID of the shader texture uniform variable
 * @details This function returns a copy of the ID of the shader texture uniform variable. textureID being a GLuint (unsigned int)
 * It is not necessary to return a const reference.
 * 
 * @return GLuint the ID of the shader texture uniform variable
 */
GLuint Shader::getTextureID() const {
    return this->textureID;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Destructor
 * @details This function deletes the shader program if the ID is valid (different from 0)
 */
Shader::~Shader() {
    if (this->programID) {
        glDeleteProgram(this->programID);
        std::cout << "Deleted shader program: " << this->programID << std::endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get Light ID
 * @return GLuint
 */
GLuint Shader::getLightID() const{
    return this->lightID;
}

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get Light position vector
 * @return glm::vec3
 */
glm::vec3 Shader::getLightPosition() const{
    return this->lightPosition;
}