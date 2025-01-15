/*

Objective:
Game of Chess common file
*/

#ifndef COMMON_H
#define COMMON_H

#include <unordered_map>
// Include GLM
#include <glm/glm.hpp>
#include <string>
#include <chrono>

// Mesh properties has table
typedef struct 
{
    bool hasBones;
    bool hasFaces;
    bool hasNormals;
    bool hasPositions;
    bool hasTangentsAndBitangents;
    bool hasTextureCoords;
    bool hasVertexColors;
    unsigned int numOfUVChannels;
} meshPropsT;

/**
 * @brief Enum for the chess pieces
 * 
 */
typedef enum piece {
    // White Pieces
    WHITE_PAWN_1, WHITE_PAWN_2, WHITE_PAWN_3, WHITE_PAWN_4, WHITE_PAWN_5, WHITE_PAWN_6, WHITE_PAWN_7, WHITE_PAWN_8,
    WHITE_ROOK_1, WHITE_ROOK_2,
    WHITE_KNIGHT_1, WHITE_KNIGHT_2,
    WHITE_BISHOP_1, WHITE_BISHOP_2,
    WHITE_QUEEN,
    WHITE_KING,

    // Black Pieces
    BLACK_PAWN_1, BLACK_PAWN_2, BLACK_PAWN_3, BLACK_PAWN_4, BLACK_PAWN_5, BLACK_PAWN_6, BLACK_PAWN_7, BLACK_PAWN_8,
    BLACK_ROOK_1, BLACK_ROOK_2,
    BLACK_KNIGHT_1, BLACK_KNIGHT_2,
    BLACK_BISHOP_1, BLACK_BISHOP_2,
    BLACK_QUEEN,
    BLACK_KING,

    // Empty Placeholder
    EMPTY
} piece;


/**
 * @brief Structure to hold each target piece
 * 
 */
typedef struct
{
    float rAngle;         // Rotation angle
    glm::vec3 rAxis;      // Rotation axis
    glm::vec3 cScale;     // Scale of the piece
    glm::vec3 tPos;       // Postion
    bool isAlive;         // If the piece is alive
    std::string meshName; // Mesh name
    int user;             // white=0, black=1
} tPosition;


// Chess board scaling
const float CBSCALE = 0.6f;
// Chess board square box size (per side)
//const float CHESS_BOX_SIZE = 3.f;
const float CHESS_BOX_SIZE = (float)(CBSCALE * 5.4);
// Chess pieces scaling
const float CPSCALE = 0.015f;
// Platform height
const float PHEIGHT = -3.0f;
// Hash to hold the target Model matrix spec for each Chess component
typedef std::unordered_map <piece, tPosition> tModelMap;


/**
 * @brief Structure to track rendering and animation
 * 
 */
typedef struct {
    tModelMap cTModelMap;                                                       // Piece-wise rendering information
    bool isPieceMoving;                                                         // If animation is on
    piece pieceMoving;                                                          // The piece that is moving
    glm::vec3 startPos;                                                         // Starting pos of animation
    glm::vec3 endPos;                                                           // Ending pos of animation
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;      // Starting time of animation
}chessModel;

#endif
