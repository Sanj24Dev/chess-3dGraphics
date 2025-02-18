# chess-3dGraphics
3D chess game using OpenGL, Assimp, and Komodo chess engine

## How to play
1. Clone the repository: ```git clone https://github.com/opengl-tutorials/ogl.git```
    <br> This install OpenGL, Assimp libraries
2. Do the following
    ```cd ogl```
    ```git clone https://github.com/Sanj24Dev/chess-3dGraphics.git```
3. The folder heirarchy looks like this:
    ogl<br>
    |-build
    |-common (one provided in chess-3dGraphics repository)
    |-external
    |-Lab3
    |-CMakeLists.txt (one provided in chess-3dGraphics repository)
4. Clear the build folder
5. Generate the executable using CMake
6. Run ./launch-Lab3.sh from the build folder

## Features
- 3D Rendering: Chessboard and pieces are rendered in 3D using OpenGL and ASSIMP.
- Chess Engine Integration: Moves are sent to a chess engine (e.g., Komodo) using the UCI protocol, and the response moves are captured and executed.
- Smooth Movement: Pieces slide across the chessboard, with knights moving through the air.
- Piece Removal: When pieces are captured, they are removed from the game scene.
- UCI Move Input: Users input chess moves in UCI format (e.g., "e2e4") through the command window.

## Dependencies
- OpenGL: For rendering the 3D chessboard and pieces.
- ASSIMP: For importing and loading 3D models of chess pieces.
- Chess Engine (e.g., Komodo): For game logic and move processing via UCI format.
- GLFW: For window creation and handling input.
