/**
 * @file ECE_ChessHandler.cpp
 * @author Sanjana Ganesh Nayak
 * @brief To create a dynamic 3D graphics application using lighting, shading, model transformations, and keyboard inputs.
    Description :
        Uses the code from tutorial09_Assimp to create a C++ application that loads in the obj files 
        for the 3D chess pieces and the chess board.

        Obje files used - Lab3/Chess/chess.obj
                        - Lab3/Stone_Chess_Board/12951_Stone_Chess_Board_v1_L3.obj
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

#include "ECE_ChessEngine.cpp"
#include "ECE_ChessHandler.cpp"

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
// User supporting files
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
// Lab3 specific chess class
#include "chessComponent.h"
#include "chessCommon.h"

/**
 * @brief Enum for different typr of commands
 * 
 */
typedef enum commands {MOVE, CAMERA, LIGHT, POWER, QUIT, INVALID} commands;


/**
 * @brief Struct containing the information of the current action according to the user command
 * 
 */
typedef struct action {
    commands type;
    std::string move;
    std::vector<float> cameraAngle;
    std::vector<float> lightAngle;
    float power;
} action;


// Functions for parsing the command
std::string trim(const std::string& str);
void parseCommand(const std::string& input, action* a);


int main( void )
{
    // Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Game Of Chess 3D", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it is closer to the camera than the former one
    glDepthFunc(GL_LESS); 

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "StandardShading.vertexshader", "StandardShading.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Get a handle for our "lightToggleSwitch" uniform
    GLuint LightSwitchID = glGetUniformLocation(programID, "lightSwitch");

    // Create a vector of chess components class
    // Each component is fully self sufficient
    std::vector<chessComponent> gchessComponents;

    // Load the OBJ files
    bool cBoard = loadAssImpLab3("Lab3/Stone_Chess_Board/12951_Stone_Chess_Board_v1_L3.obj", gchessComponents);
    bool cComps = loadAssImpLab3("Lab3/Chess/chess-mod.obj", gchessComponents);

    // Proceed iff OBJ loading is successful
    if (!cBoard || !cComps)
    {
        // Quit the program (Failed OBJ loading)
        std::cout << "Program failed due to OBJ loading failure, please CHECK!" << std::endl;
        return -1;
    }

    // Load it into a VBO (One time activity)
    // Run through all the components for rendering
    for (auto cit = gchessComponents.begin(); cit != gchessComponents.end(); cit++)
    {
        // Setup VBO buffers
        cit->setupGLBuffers();
        // Setup Texture
        cit->setupTextureBuffers();
    }

    // Use our shader (Not changing the shader per chess component)
    glUseProgram(programID);

    // Get a handle for our "LightPosition" uniform
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // Get a handle for our "LightPower" uniform
    GLuint lightPowerLocation = glGetUniformLocation(programID, "LightPower");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    


    // Initialize the start up camera and light
    action a;
    a.cameraAngle = {10.0f, 270.0f, 40.0f};
    a.lightAngle = {0.0f, 0.0f, 15.0f};
    a.power = 400.0f;

    
    // Initialize the chess engine
    ECE_ChessEngine komodo;
    komodo.InitializeEngine();
    komodo.setOptions("Minimal Reporting value 5");


    // Setup the Chess board locations
    chessModel cModel;
    cModel.isPieceMoving = false;
    cModel.startTime = std::chrono::time_point<std::chrono::high_resolution_clock>::min();
    ECE_ChessHandler game;
    game.setupChessBoard(cModel.cTModelMap);

    std::string input;
    std::string command;
    
    do{
        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;

        // If last prinf() was more than 1sec ago
        if (currentTime - lastTime >= 1.0) 
        { // If last prinf() was more than 1sec ago
            // printf and reset
            // printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the VP matrix from keyboard and mouse input
        computeMatricesFromInputsFinal(a.cameraAngle[0], a.cameraAngle[1], a.cameraAngle[2]);
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

        // Get light switch State (It's a toggle!)
        bool lightSwitch = getLightSwitch();
        // Pass it to Fragment Shader
        glUniform1i(LightSwitchID, static_cast<int>(lightSwitch));

        // Update the position of any piece that is moving
        if (cModel.isPieceMoving)
        {
            // Get the elapsed timesince start of animation
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - cModel.startTime);

            if (elapsed < std::chrono::milliseconds(2000))
            {
                // Calculate interpolation factor 
                float t = std::fmin(1.0f, (float)elapsed.count() / 2000.0);

                // Interpolate position 
                cModel.cTModelMap[cModel.pieceMoving].tPos.x = cModel.startPos.x + t * (cModel.endPos.x - cModel.startPos.x);
                cModel.cTModelMap[cModel.pieceMoving].tPos.y = cModel.startPos.y + t * (cModel.endPos.y - cModel.startPos.y);
                // Make the knight jump
                if(cModel.pieceMoving == WHITE_KNIGHT_1 || cModel.pieceMoving == WHITE_KNIGHT_2 || cModel.pieceMoving == BLACK_KNIGHT_1 || cModel.pieceMoving == BLACK_KNIGHT_2)
                    cModel.cTModelMap[cModel.pieceMoving].tPos.z = 5.f;
            }
            else 
            {
                // Animation finished, set final position
                cModel.cTModelMap[cModel.pieceMoving].tPos = cModel.endPos;
                cModel.isPieceMoving = false;
                cModel.startTime = std::chrono::time_point<std::chrono::high_resolution_clock>::min();
            }
        }

        // Run through all the entries in cTModelMap for rendering
        for (auto& mapEntry : cModel.cTModelMap)
        {
            tPosition& cTPosition = mapEntry.second;
            if (cTPosition.isAlive) 
            {
                // Find a matching component in gchessComponents based on meshName
                auto cit = std::find_if(gchessComponents.begin(), gchessComponents.end(),[&](auto& component) { return component.getComponentID() == cTPosition.meshName; });

                // Ensure the component exists in gchessComponents before rendering
                if (cit != gchessComponents.end())
                {
                    // Pass it for Model matrix generation
                    glm::mat4 ModelMatrix = cit->genModelMatrix(cTPosition);

                    // Generate the MVP matrix
                    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

                    // Send our transformation to the currently bound shader,
                    // in the "MVP" uniform
                    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
                    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
                    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

                    // Update the light angle and power
                    // Convert to Cartesian co-ordinate system
                    float lightX = a.lightAngle[2] * sin(glm::radians(a.lightAngle[0])) * cos(glm::radians(a.lightAngle[1]));
                    float lightY = a.lightAngle[2] * sin(glm::radians(a.lightAngle[0])) * sin(glm::radians(a.lightAngle[1]));
                    float lightZ = a.lightAngle[2] * cos(glm::radians(a.lightAngle[0]));
                    glUniform3f(LightID, lightX, lightY, lightZ);
                    glUniform1f(lightPowerLocation, a.power);

                    // Bind our texture 
                    cit->setupTexture(TextureID);

                    // Render buffers
                    cit->renderMesh();
                }
            }
        }
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();


        // if animation is complete, go for the next move
        if (!cModel.isPieceMoving && !game.inCheckMate)
        {
            if (game.user_turn % 2 == 0) 
            {
                // Get the input from the user
                std::cout << "Please enter a command: ";
                std::getline(std::cin, input);

                // Parse the input into the action struct
                parseCommand(input, &a);
                if (a.type == QUIT) 
                {
                    komodo.sendMove("quit");
                    break;
                }
                else if (a.type == INVALID) 
                {
                    continue;
                }
                else if (a.type == MOVE) 
                {
                    // Validate and move the piece
                    game.movePiece(a.move, cModel);
                    if (game.inCheckMate) {
                        std::cout << "Checkmate!! You WON. Game over.\nClose the window.";
                    }
                }
            }
            else {    
                komodo.sendMove(game.moveHistory);

                // Read Komodo's output
                std::string komodoOutput;
                if(komodo.getResponseMove(komodoOutput))
                {
                    // Validate and move the piece
                    game.movePiece(komodoOutput, cModel);
                    if (game.inCheckMate) {
                        std::cout << "Checkmate!! You LOST. Game over.\nClose the window.\n";
                        //  break;
                    }
                }
            } 
        }
    // Check if the ESC key was pressed or the window was closed
    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
               glfwWindowShouldClose(window) == 0 );
    

    // Cleanup VBO, Texture (Done in class destructor) and shader 
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    // Shake hand for exit!
    return 0;
}



/**
 * @brief Trims leading and trailing whitespace from a string.
 * 
 * @param str 
 * @return std::string 
 */
std::string trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}


/**
 * @brief Parses the input and populates the action struct
 * 
 * @param input 
 * @param a 
 */
void parseCommand(const std::string& input, action *a) 
{
    // Remove leading and trailing spaces
    std::string command = trim(input); 

    // Find the command type
    size_t spacePos = command.find(' ');
    std::string commandType = command.substr(0, spacePos);

    // Find the arguments
    std::string arguments = (spacePos != std::string::npos) ? trim(command.substr(spacePos + 1)) : "";

    if (commandType == "move") 
    {
        // Ensure move has exactly 4 characters and follows chess format
        a->type = MOVE;
        if (arguments.length() == 4 &&
            arguments[0] >= 'a' && arguments[0] <= 'h' &&
            arguments[1] >= '1' && arguments[1] <= '8' &&
            arguments[2] >= 'a' && arguments[2] <= 'h' &&
            arguments[3] >= '1' && arguments[3] <= '8') 
        {
            a->move = arguments;
        } 
        else 
        {
            std::cout << "Invalid move command!" << std::endl;
            a->type = INVALID;
        }
    } 
    else if (commandType == "camera") 
    {
        a->type = CAMERA;
        std::istringstream ss(arguments);
        std::vector<float> values;
        float value;
        int index = 0;
        // Check if the angles are valid
        while (ss >> value) 
        {
            if(index == 0) 
            {
                if (value < 10 || value > 80) 
                { 
                    std::cerr << "Invalid theta value\n";
                }
                else 
                {
                    values.push_back(value);
                }
            }

            if(index == 1) 
            {
                if (value < 0 || value > 360) 
                { 
                    std::cerr << "Invalid phi value\n";
                }
                else 
                {
                    values.push_back(value);
                }
            }

            if(index == 2) 
            {
                if (value < 0) 
                {  
                    std::cerr << "Invalid radius value\n";
                }
                else 
                {
                    values.push_back(value);
                }
            }
            index++;
        }

        // If all 3 values are valid
        if (values.size() == 3) 
        {
            a->cameraAngle = values;
        } 
        else 
        {
            std::cout << "Invalid camera command!" << std::endl;
            a->type = INVALID;
        }
    }
    else if (commandType == "light") 
    {
        a->type = LIGHT;
        std::istringstream ss(arguments);
        std::vector<float> values;
        float value;
        int index = 0;
        // Check if the angles are valid
        while (ss >> value) 
        {
            if(index == 0) 
            {
                if (value < 10 || value > 80) 
                {  
                    std::cerr << "Invalid theta value\n";
                }
                else 
                {
                    values.push_back(value);
                }
            }

            if(index == 1) 
            {
                if (value < 0 || value > 360) 
                {  
                    std::cerr << "Invalid phi value\n";
                }
                else 
                {
                    values.push_back(value);
                }
            }

            if(index == 2) 
            {
                if (value < 0) 
                { 
                    std::cerr << "Invalid radius value\n";
                }
                else 
                {
                    values.push_back(value);
                }
            }
            index++;
        }

        // If all 3 values are valid
        if (values.size() == 3) 
        {
            a->lightAngle = values;
        } 
        else 
        {
            std::cout << "Invalid light command!" << std::endl;
            a->type = INVALID;
        }
    }
    else if (commandType == "power") 
    {
        a->type = POWER;
        std::istringstream ss(arguments);
        float value;
        ss >> value;
    
        if (value >= 0) 
        {
            a->power = value;
        }
        else 
        {
            std::cout << "Invalid power command!" << std::endl;
            a->type = INVALID; 
        }
    }
    else if (commandType == "quit") 
    {
        std::cout << "Thanks for playing!" << std::endl;
        a->type = QUIT;
    }
    else 
    {
        std::cout << "Invalid command!" << std::endl;
        a->type = INVALID;
    }
}


