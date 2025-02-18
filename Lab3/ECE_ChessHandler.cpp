/**
 * @file ECE_ChessHandler.cpp
 * @author Sanjana Ganesh Nayak
 * @brief Class to handle the game mapping and status
 * @version 0.1
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "chessCommon.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


/**
 * @class ECE_ChessHandler
 * @brief Class to handle the game mapping and the status
 * 
 */
class ECE_ChessHandler {
public:
    int user_turn;                  // whose turn? 0=WHITE, 1=BLACK
    piece board[8][8];              // the piece on each square on the board
    std::string moveHistory;        // to maintain the history of the moves played in sequence

    bool inCheck;                   // is the user in Check?
    bool inCheckMate;               // is the user in checkmate?

    // glm::vec3 capturedPieces;       // position for the captured pieces

    /**
     * @brief Construct a new ECE_CHessHandler object
     * 
     */
    ECE_ChessHandler() {
        user_turn = 0;              // set to WHITE as initial player
        moveHistory = "";
        inCheck = false;
        inCheckMate = false;
        // capturedPieces = {          // initalise positions for captured pieces ???
        //     5*CHESS_BOX_SIZE,
        //     3.5*CHESS_BOX_SIZE,
        //     PHEIGHT
        // };
    }

    /**
     * @brief Initialize the ModelMap with the chess piece informtion
     *        Initialize the board[][] with the start setting
     * 
     * @param cTModelMap 
     */
    void setupChessBoard(tModelMap& cTModelMap)
    {
        cTModelMap = {
            // Chess board
            {EMPTY, {0.f, {1, 0, 0}, glm::vec3(CBSCALE), {0.f, 0.f, PHEIGHT}, true, "12951_Stone_Chess_Board", -1}},
            
            // White pieces
            {WHITE_PAWN_1,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-3.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE13", 0}},
            {WHITE_PAWN_2,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-2.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE13", 0}},
            {WHITE_PAWN_3,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-1.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true,  "PEDONE13", 0}},
            {WHITE_PAWN_4,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-0.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE13", 0}},
            {WHITE_PAWN_5,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 0.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE13", 0}},
            {WHITE_PAWN_6,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 1.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE13", 0}},
            {WHITE_PAWN_7,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 2.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE13", 0}},
            {WHITE_PAWN_8,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 3.5*CHESS_BOX_SIZE, -2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE13", 0}},
            {WHITE_ROOK_1,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-3.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "TORRE3", 0}},
            {WHITE_ROOK_2,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 3.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "TORRE3", 0}},
            {WHITE_KNIGHT_1, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-2.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "Object3", 0}},
            {WHITE_KNIGHT_2, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 2.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "Object3", 0}},
            {WHITE_BISHOP_1, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-1.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "ALFIERE3", 0}},
            {WHITE_BISHOP_2, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 1.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "ALFIERE3", 0}},
            {WHITE_QUEEN,    {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-0.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "REGINA2", 0}},
            {WHITE_KING,     {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 0.5*CHESS_BOX_SIZE, -3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "RE2", 0}},

            // Black pieces
            {BLACK_PAWN_1,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-3.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_PAWN_2,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-2.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_PAWN_3,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-1.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_PAWN_4,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-0.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_PAWN_5,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 0.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_PAWN_6,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 1.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_PAWN_7,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 2.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_PAWN_8,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 3.5*CHESS_BOX_SIZE,  2.5*CHESS_BOX_SIZE, PHEIGHT}, true, "PEDONE12", 1}},
            {BLACK_ROOK_1,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-3.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "TORRE02", 1}},
            {BLACK_ROOK_2,   {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 3.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "TORRE02", 1}},
            {BLACK_KNIGHT_1, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-2.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "Object02", 1}},
            {BLACK_KNIGHT_2, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 2.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "Object02", 1}},
            {BLACK_BISHOP_1, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-1.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "ALFIERE02", 1}},
            {BLACK_BISHOP_2, {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 1.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "ALFIERE02", 1}},
            {BLACK_QUEEN,    {90.f, {1, 0, 0}, glm::vec3(CPSCALE), {-0.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "REGINA01", 1}},
            {BLACK_KING,     {90.f, {1, 0, 0}, glm::vec3(CPSCALE), { 0.5*CHESS_BOX_SIZE,  3.5*CHESS_BOX_SIZE, PHEIGHT}, true, "RE01", 1}},
        };


        // Clear board (set to EMPTY)
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                board[i][j] = EMPTY;

        // White pieces
        // Rank 1
        board[0][0] = WHITE_ROOK_1;   
        board[0][1] = WHITE_KNIGHT_1; 
        board[0][2] = WHITE_BISHOP_1; 
        board[0][3] = WHITE_QUEEN;  
        board[0][4] = WHITE_KING;   
        board[0][5] = WHITE_BISHOP_2; 
        board[0][6] = WHITE_KNIGHT_2; 
        board[0][7] = WHITE_ROOK_2;   
        // Rank 2
        board[1][0] = WHITE_PAWN_1;
        board[1][1] = WHITE_PAWN_2;
        board[1][2] = WHITE_PAWN_3;
        board[1][3] = WHITE_PAWN_4;
        board[1][4] = WHITE_PAWN_5;
        board[1][5] = WHITE_PAWN_6;
        board[1][6] = WHITE_PAWN_7;
        board[1][7] = WHITE_PAWN_8;

        // Black pieces
        // Rank 8
        board[7][0] = BLACK_ROOK_1;  
        board[7][1] = BLACK_KNIGHT_1; 
        board[7][2] = BLACK_BISHOP_1; 
        board[7][3] = BLACK_QUEEN;  
        board[7][4] = BLACK_KING;   
        board[7][5] = BLACK_BISHOP_2; 
        board[7][6] = BLACK_KNIGHT_2; 
        board[7][7] = BLACK_ROOK_2;  
        // Rank 7
        board[6][0] = BLACK_PAWN_1;
        board[6][1] = BLACK_PAWN_2;
        board[6][2] = BLACK_PAWN_3;
        board[6][3] = BLACK_PAWN_4;
        board[6][4] = BLACK_PAWN_5;
        board[6][5] = BLACK_PAWN_6;
        board[6][6] = BLACK_PAWN_7;
        board[6][7] = BLACK_PAWN_8;
    }


    // i need to check if my move checks the opponents king == if yes then only moves to save the king are valid, if no valid moves then checkmate
    // i need to check if my move leads to a check on my king == if yes then i cant do the move

    /**
     * @brief Checks if the King is in Check
     * 
     * @param kingToCheck 
     */
    void isInCheck(piece kingToCheck) {
        // variables to store the king and opponent piece coordinates
        int kingRow, kingCol;
        int opponentKingRow = -1, opponentKingCol = -1;
        int opponentQueenRow = -1, opponentQueenCol = -1;
        int opponentRook1Row = -1, opponentRook1Col = -1;
        int opponentRook2Row = -1, opponentRook2Col = -1;
        int opponentBishop1Row = -1, opponentBishop1Col = -1;
        int opponentBishop2Row = -1, opponentBishop2Col = -1;
        int opponentKnight1Row = -1, opponentKnight1Col = -1;
        int opponentKnight2Row = -1, opponentKnight2Col = -1;
        int opponentPawn1Row = -1, opponentPawn1Col = -1;
        int opponentPawn2Row = -1, opponentPawn2Col = -1;
        int opponentPawn3Row = -1, opponentPawn3Col = -1;
        int opponentPawn4Row = -1, opponentPawn4Col = -1;
        int opponentPawn5Row = -1, opponentPawn5Col = -1;
        int opponentPawn6Row = -1, opponentPawn6Col = -1;
        int opponentPawn7Row = -1, opponentPawn7Col = -1;
        int opponentPawn8Row = -1, opponentPawn8Col = -1;

        piece opponentPawn1 = kingToCheck == WHITE_KING ? BLACK_PAWN_1 : WHITE_PAWN_1;
        piece opponentPawn2 = kingToCheck == WHITE_KING ? BLACK_PAWN_2 : WHITE_PAWN_2;
        piece opponentPawn3 = kingToCheck == WHITE_KING ? BLACK_PAWN_3 : WHITE_PAWN_3;
        piece opponentPawn4 = kingToCheck == WHITE_KING ? BLACK_PAWN_4 : WHITE_PAWN_4;
        piece opponentPawn5 = kingToCheck == WHITE_KING ? BLACK_PAWN_5 : WHITE_PAWN_5;
        piece opponentPawn6 = kingToCheck == WHITE_KING ? BLACK_PAWN_6 : WHITE_PAWN_6;
        piece opponentPawn7 = kingToCheck == WHITE_KING ? BLACK_PAWN_7 : WHITE_PAWN_7;
        piece opponentPawn8 = kingToCheck == WHITE_KING ? BLACK_PAWN_8 : WHITE_PAWN_8;

        piece opponentRook1 = kingToCheck == WHITE_KING ? BLACK_ROOK_1 : WHITE_ROOK_1;
        piece opponentRook2 = kingToCheck == WHITE_KING ? BLACK_ROOK_2 : WHITE_ROOK_2;

        piece opponentKnight1 = kingToCheck == WHITE_KING ? BLACK_KNIGHT_1 : WHITE_KNIGHT_1;
        piece opponentKnight2 = kingToCheck == WHITE_KING ? BLACK_KNIGHT_2 : WHITE_KNIGHT_2;

        piece opponentBishop1 = kingToCheck == WHITE_KING ? BLACK_BISHOP_1 : WHITE_BISHOP_1;
        piece opponentBishop2 = kingToCheck == WHITE_KING ? BLACK_BISHOP_2 : WHITE_BISHOP_2;

        piece opponentQueen = kingToCheck == WHITE_KING ? BLACK_QUEEN : WHITE_QUEEN;
        piece opponentKing = kingToCheck == WHITE_KING ? BLACK_KING : WHITE_KING;

        for (int row = 0; row < 8; row++) 
        {
            for (int col = 0; col < 8; col++) 
            {
                // Find the King's position
                if (board[row][col] == kingToCheck) 
                {
                    kingRow = row;
                    kingCol = col;
                }
                // Find the position of the opponent's King
                else if (board[row][col] == opponentKing)
                {
                    opponentKingRow = row;
                    opponentKingCol = col;
                }
                // Find the position of the opponent's Queen
                else if (board[row][col] == opponentQueen) 
                {
                    opponentQueenRow = row;
                    opponentQueenCol = col;
                }
                // Find the position of the opponent's Rook1
                else if (board[row][col] == opponentRook1) 
                {
                    opponentRook1Row = row;
                    opponentRook1Col = col;
                }
                // Find the position of the opponent's Rook2
                else if (board[row][col] == opponentRook2) 
                {
                    opponentRook2Row = row;
                    opponentRook2Col = col;
                }
                // Find the position of the opponent's Bishop1
                else if (board[row][col] == opponentBishop1) 
                {
                    opponentBishop1Row = row;
                    opponentBishop1Col = col;
                }
                // Find the position of the opponent's Bishop2
                else if (board[row][col] == opponentBishop2) 
                {
                    opponentBishop2Row = row;
                    opponentBishop2Col = col;
                }
                // Find the position of the opponent's Knight1
                else if (board[row][col] == opponentKnight1) 
                {
                    opponentKnight1Row = row;
                    opponentKnight1Col = col;
                }
                // Find the position of the opponent's Knight2
                else if (board[row][col] == opponentKnight2) 
                {
                    opponentKnight2Row = row;
                    opponentKnight2Col = col;
                }
                // Find the position of the opponent's Pawns
                else if (board[row][col] == opponentPawn1) 
                {
                    opponentPawn1Row = row;
                    opponentPawn1Col = col;
                }
                else if (board[row][col] == opponentPawn2) 
                {
                    opponentPawn2Row = row;
                    opponentPawn2Col = col;
                }
                else if (board[row][col] == opponentPawn3) 
                {
                    opponentPawn3Row = row;
                    opponentPawn3Col = col;
                }
                else if (board[row][col] == opponentPawn4) 
                {
                    opponentPawn4Row = row;
                    opponentPawn4Col = col;
                }
                else if (board[row][col] == opponentPawn5) 
                {
                    opponentPawn5Row = row;
                    opponentPawn5Col = col;
                }
                else if (board[row][col] == opponentPawn6) 
                {
                    opponentPawn6Row = row;
                    opponentPawn6Col = col;
                }
                else if (board[row][col] == opponentPawn7) 
                {
                    opponentPawn7Row = row;
                    opponentPawn7Col = col;
                }
                else if (board[row][col] == opponentPawn8) 
                {
                    opponentPawn8Row = row;
                    opponentPawn8Col = col;
                }
            }
        }

        // Initialize check to false
        inCheck = false;


        // Check if the king is checked by opponent rook
        if (kingRow == opponentRook1Row) 
        { 
            inCheck = true;
            // Is there any piece blocking the Rook's path?
            int j = (kingCol > opponentRook1Col) ? 1 : -1; 
            for (int col = opponentRook1Col + j; col != kingCol; col += j) 
            {
                if (board[opponentRook1Row][col] != EMPTY) 
                { 
                    // There is a piece blocking
                    inCheck = false;
                    break;
                }
            }
        }
        else if (kingCol == opponentRook1Col) 
        { 
            inCheck = true;
            // Is there any piece blocking the Rook's path?
            int i = (kingRow > opponentRook1Row) ? 1 : -1;
            for (int row = opponentRook1Col + i; row != kingRow; row += i) 
            {
                if (board[row][opponentRook1Col] != EMPTY) 
                { 
                    // There is a piece blocking
                    inCheck = false;
                    break;
                }
            }
        }
        
        else if (kingRow == opponentRook2Row) 
        {
            inCheck = true;
            // Ensure no pieces block the rook's path
            int j = (kingCol > opponentRook2Col) ? 1 : -1; 
            for (int col = opponentRook2Col + j; col != kingCol; col += j) 
            {
                if (board[opponentRook2Row][col] != EMPTY) 
                {  
                    // There is a piece blocking
                    inCheck = false;
                    break;
                }
            }
        }
        else if (kingCol == opponentRook2Col) 
        { 
            inCheck = true;
            // Ensure no pieces block the rook's path
            int i = (kingRow > opponentRook2Row) ? 1 : -1; 
            for (int row = opponentRook2Row + i; row != kingRow; row += i) 
            {
                if (board[row][opponentRook2Col] != EMPTY) 
                {  
                    // There is a piece blocking
                    inCheck = false;
                    break;
                }
            }
        }
        // Check if the king is checked by opponent knight
        else if ((abs(kingRow - opponentKnight1Row) == 2 && abs(kingCol - opponentKnight1Col) == 1) ||
                    abs(kingRow - opponentKnight1Row) == 1 && abs(kingCol - opponentKnight1Col) == 2) 
        {
            inCheck = true;
        }
        else if ((abs(kingRow - opponentKnight2Row) == 2 && abs(kingCol - opponentKnight2Col) == 1) ||
                    abs(kingRow - opponentKnight2Row) == 1 && abs(kingCol - opponentKnight2Col) == 2) 
        {
            inCheck = true;
        }
        // Check if the king is checked by opponent bishop
        else if (abs(kingRow - opponentBishop1Row) == abs(kingCol - opponentBishop1Col)) 
        {
            inCheck = true;
            // Ensure no pieces block the bishop's path
            int i = (kingRow > opponentBishop1Row) ? 1 : -1;
            int j = (kingCol > opponentBishop1Col) ? 1 : -1;
            for (int n = 1; n < abs(kingRow - opponentBishop1Row); n++) 
            {
                if (board[opponentBishop1Row + n * i][opponentBishop1Col + n * j] != EMPTY) 
                {
                    // There is a piece blocking
                    inCheck = false;
                    break;
                }
            }
        }
        else if (abs(kingRow - opponentBishop2Row) == abs(kingCol - opponentBishop2Col)) 
        {
            inCheck = true;
            // Ensure no pieces block the bishop's path
            int i = (kingRow > opponentBishop2Row) ? 1 : -1;
            int j = (kingCol > opponentBishop2Col) ? 1 : -1;
            for (int n = 1; n < abs(kingRow - opponentBishop2Row); n++) 
            {
                if (board[opponentBishop2Row + n * i][opponentBishop2Col + n * j] != EMPTY) 
                {
                    // There is a piece blocking
                    inCheck = false;
                    break;
                }
            }
        }
        // Check if the king is checked by opponent queen
        else if (kingRow == opponentQueenRow  || kingCol == opponentQueenCol
                     || abs(kingRow - opponentQueenRow) == abs(kingCol - opponentQueenCol)) 
        {
            inCheck = true;
            // Ensure no pieces block the queen's path
            int i = (kingRow != opponentQueenRow) ? ((kingRow > opponentQueenRow) ? 1 : -1) : 0;
            int j = (kingCol != opponentQueenCol) ? ((kingCol > opponentQueenCol) ? 1 : -1) : 0;
            for (int n = 1; n < std::max(abs(kingRow - opponentQueenRow), abs(kingCol - opponentQueenCol)); n++) {
                if (board[opponentQueenRow + n * i][opponentQueenCol + n * j] != EMPTY) {
                    // There is a piece blocking
                    inCheck = false;
                    // std::cout << "Safe!\n" << opponentQueenRow + n * i << opponentQueenCol + n * j << std::endl;
                    break;
                }
            }
        }
        // Check if the king is checked by opponent pawns
        else if (abs(kingCol - opponentPawn1Col) == 1 && kingRow == opponentPawn1Row + 1)
        {
            inCheck = true;
        }
        else if (abs(kingCol - opponentPawn2Col) == 1 && kingRow == opponentPawn2Row + 1)
        {
            inCheck = true;
        }
        else if (abs(kingCol - opponentPawn3Col) == 1 && kingRow == opponentPawn3Row + 1)
        {
            inCheck = true;
        }
        else if (abs(kingCol - opponentPawn4Col) == 1 && kingRow == opponentPawn4Row + 1)
        {
            inCheck = true;
        }
        else if (abs(kingCol - opponentPawn5Col) == 1 && kingRow == opponentPawn5Row + 1)
        {
            inCheck = true;
        }
        else if (abs(kingCol - opponentPawn6Col) == 1 && kingRow == opponentPawn6Row + 1)
        {
            inCheck = true;
        }
        else if (abs(kingCol - opponentPawn7Col) == 1 && kingRow == opponentPawn7Row + 1)
        {
            inCheck = true;
        }
        else if (abs(kingCol - opponentPawn8Col) == 1 && kingRow == opponentPawn8Row + 1)
        {
            inCheck = true;
        }
    }

    /**
     * @brief Checks if the player is under checkmate
     * 
     */
    void isKingCheckmate()
    {
        int kingRow, kingCol;
        piece kingToCheck = user_turn % 2 == 0 ? BLACK_KING : WHITE_KING; // if white moves, then check if that leads to check mate for black
        for (int row = 0; row < 8; row++) 
        {
            for (int col = 0; col < 8; col++) 
            {
                if (board[row][col] == kingToCheck) 
                {
                    kingRow = row;
                    kingCol = col;
                    break;
                }
            }
        }

        inCheck = false;

        // Check if the king is in check
        isInCheck(kingToCheck);
        if (!inCheck) 
        {
            inCheckMate = false;
            return;
        }


        // Check if the king has any valid moves to escape check
        const int kingMoves[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
        inCheckMate = false;
        for (auto move : kingMoves) {
            int newRow = kingRow + move[0];
            int newCol = kingCol + move[1];

            // Ensure the new position is within bounds
            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                piece originalPiece = board[newRow][newCol];

                bool valid = validMove(kingRow, kingCol, newRow, newCol);
                inCheck = false;
                if (valid)
                {
                    // Simulate moving the king to this position
                    board[kingRow][kingCol] = EMPTY; 
                    board[newRow][newCol] = kingToCheck;
                    isInCheck(kingToCheck);
                    if (inCheck == false)
                    {
                        inCheckMate = false;
                        // Restore the board state
                        board[kingRow][kingCol] = kingToCheck;
                        board[newRow][newCol] = originalPiece;
                        return;
                    }
                    // else {
                    //     std::cout << "In check for " << newCol << newRow << std::endl;
                    // }
                    // Restore the board state
                    board[kingRow][kingCol] = kingToCheck;
                    board[newRow][newCol] = originalPiece;
                }
                // else {
                //     std::cout << "Invalid move for " << newCol << newRow << std::endl;
                // }
                
                
            }
        }
        inCheckMate = true;
    }

        
    /**
     * @brief Checks if the move is valid
     * 
     * @param fromRow 
     * @param fromCol 
     * @param toRow 
     * @param toCol 
     * @return true 
     * @return false 
     */
    bool validMove(int fromRow, int fromCol, int toRow, int toCol) {
        // If the move puts the piece out of the board
        if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
            toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) 
        {
            return false;
        }

        // Get the piece at the source and destination
        piece fromPiece = board[fromRow][fromCol];
        piece toPiece = board[toRow][toCol];

        // Invalid if the initial square is empty
        if (fromPiece == EMPTY) 
        {
            return false;
        }

        bool isWhiteMoving = (fromPiece >= WHITE_PAWN_1 && fromPiece <= WHITE_KING);
        bool isBlackMoving = (fromPiece >= BLACK_PAWN_1 && fromPiece <= BLACK_KING);

        // Invalid if the wrong color is accessed
        // if ((user_turn % 2 == 0 && isBlackMoving) || (user_turn % 2 == 1 && isWhiteMoving))
        // {
        //     return false;
        // }

        bool isWhiteTarget = (toPiece >= WHITE_PAWN_1 && toPiece <= WHITE_KING);
        bool isBlackTarget = (toPiece >= BLACK_PAWN_1 && toPiece <= BLACK_KING);

        // Invalid if the same color exists in the final square
        if ((isWhiteMoving && isWhiteTarget) || (isBlackMoving && isBlackTarget)) 
        {
            return false;
        }

        bool isValid = false;
        // Verify the movement based on the piece type
        switch (fromPiece) 
        {
            // Pawn movement
            case WHITE_PAWN_1: 
            case WHITE_PAWN_2: 
            case WHITE_PAWN_3: 
            case WHITE_PAWN_4:
            case WHITE_PAWN_5: 
            case WHITE_PAWN_6: 
            case WHITE_PAWN_7: 
            case WHITE_PAWN_8:
                if (fromCol == toCol && toPiece == EMPTY && toRow == fromRow + 1) 
                {
                    isValid = true;
                }
                if (fromCol == toCol && toPiece == EMPTY && fromRow == 1 && toRow == 3) 
                {
                    isValid = true;
                }
                if (abs(toCol - fromCol) == 1 && toRow == fromRow + 1 && toPiece != EMPTY) 
                {
                    isValid = true;
                }
                break;

            case BLACK_PAWN_1: 
            case BLACK_PAWN_2: 
            case BLACK_PAWN_3: 
            case BLACK_PAWN_4:
            case BLACK_PAWN_5: 
            case BLACK_PAWN_6: 
            case BLACK_PAWN_7: 
            case BLACK_PAWN_8:
                if (fromCol == toCol && toPiece == EMPTY && toRow == fromRow - 1) 
                {
                    isValid = true;
                }
                if (fromCol == toCol && toPiece == EMPTY && fromRow == 6 && toRow == 4) 
                {
                    isValid = true;
                }
                if (abs(toCol - fromCol) == 1 && toRow == fromRow - 1 && toPiece != EMPTY) 
                {
                    isValid = true;
                }
                break;

            // Rook movement
            case WHITE_ROOK_1: 
            case WHITE_ROOK_2:
            case BLACK_ROOK_1: 
            case BLACK_ROOK_2:
                if (fromRow == toRow || fromCol == toCol) 
                {
                    isValid = true;
                    // Ensure no pieces block the rook's path
                    int i = (toRow != fromRow) ? ((toRow > fromRow) ? 1 : -1) : 0;
                    int j = (toCol != fromCol) ? ((toCol > fromCol) ? 1 : -1) : 0;
                    for (int n = 1; n < std::max(abs(toRow - fromRow), abs(toCol - fromCol)); n++) 
                    {
                        if (board[fromRow + n * i][fromCol + n * j] != EMPTY) 
                        {
                            // std::cout <<"Here\n";
                            isValid = false;
                            break;
                        }
                    }
                }
                break;

            // Knight movement
            case WHITE_KNIGHT_1: 
            case WHITE_KNIGHT_2:
            case BLACK_KNIGHT_1: 
            case BLACK_KNIGHT_2:
                if ((abs(toRow - fromRow) == 2 && abs(toCol - fromCol) == 1) ||
                    (abs(toRow - fromRow) == 1 && abs(toCol - fromCol) == 2)) 
                {
                    isValid = true;
                }
                break;

            // Bishop movement
            case WHITE_BISHOP_1: 
            case WHITE_BISHOP_2:
            case BLACK_BISHOP_1: 
            case BLACK_BISHOP_2:
                if (abs(toRow - fromRow) == abs(toCol - fromCol)) 
                {
                    isValid = true;
                    // Ensure no pieces block the bishop's path
                    int i = (toRow > fromRow) ? 1 : -1;
                    int j = (toCol > fromCol) ? 1 : -1;
                    for (int n = 1; n < abs(toRow - fromRow); n++) 
                    {
                        if (board[fromRow + n * i][fromCol + n * j] != EMPTY) 
                        {
                            isValid = false;
                            break;
                        }
                    }
                }
                break;

            // Queen movement
            case WHITE_QUEEN: 
            case BLACK_QUEEN:

                if (fromRow == toRow || fromCol == toCol || 
                    abs(toRow - fromRow) == abs(toCol - fromCol)) 
                {
                    isValid = true;
                    int i = (toRow != fromRow) ? ((toRow > fromRow) ? 1 : -1) : 0;
                    int j = (toCol != fromCol) ? ((toCol > fromCol) ? 1 : -1) : 0;
                    for (int n = 1; n < std::max(abs(toRow - fromRow), abs(toCol - fromCol)); n++) 
                    {
                        if (board[fromRow + n * i][fromCol + n * j] != EMPTY) 
                        {
                            isValid = false;
                            break;
                        }
                    }
                }
                break;

            // King movement
            case WHITE_KING: 
            case BLACK_KING:
                // std::cout <<"Here out\n";
                if (abs(toRow - fromRow) <= 1 && abs(toCol - fromCol) <= 1) 
                {
                    // std::cout <<"Here\n";
                    isValid = true;
                }
                break;
            
            default:
                break;
        }

        return isValid;
    }

    /**
     * @brief Move the pieces
     * 
     * @param move 
     * @param cModel 
     */
    void movePiece(std::string move, chessModel& cModel)
    {
        int fromCol = move[0] - 'a'; 
        int fromRow = move[1] - '1'; 
        int toCol = move[2] - 'a';
        int toRow = move[3] - '1';

        if(!validMove(fromRow, fromCol, toRow, toCol))
        {
            if (user_turn%2 == 0 && !inCheckMate)
                std::cout << "Invalid move!!" << std::endl;
            
            return;
        }

        piece fromPiece = board[fromRow][fromCol];
        piece originalTarget = board[toRow][toCol];
        board[toRow][toCol] = fromPiece;
        board[fromRow][fromCol] = EMPTY;

        piece kingToCheck = user_turn % 2 == 0 ? WHITE_KING : BLACK_KING; // if white moves a piece and does that lead to white king check
        isInCheck(kingToCheck);

        // Revert
        board[fromRow][fromCol] = fromPiece;
        board[toRow][toCol] = originalTarget;
    
        if (inCheck)
        {
            if (user_turn%2 == 0 && inCheck)
                std::cout << "Check!! Try again." << std::endl;
            return;
        }

        // Handle captured piece
        piece captured = board[toRow][toCol];
        if (captured != EMPTY) {
            std::cout << "Captured piece at " << move[2] << move[3] << std::endl;
            cModel.cTModelMap[captured].isAlive = false;
            // cModel.cTModelMap[captured].tPos = capturedPieces;
        }

        // Update the board
        board[toRow][toCol] = board[fromRow][fromCol];
        board[fromRow][fromCol] = EMPTY;
        piece p = board[toRow][toCol];
        cModel.cTModelMap[p].tPos.x = -3.5 * CHESS_BOX_SIZE + toCol * CHESS_BOX_SIZE;
        cModel.cTModelMap[p].tPos.y = -3.5 * CHESS_BOX_SIZE + toRow * CHESS_BOX_SIZE;
        // Check if it goes for check mate
        isKingCheckmate();
        user_turn++;                    // Update user turn
        moveHistory += move + " ";      // Update history

        // Update model parameters for animation
        cModel.isPieceMoving = true;
        cModel.pieceMoving = p;
        cModel.startPos = glm::vec3(-3.5 * CHESS_BOX_SIZE + fromCol * CHESS_BOX_SIZE, -3.5 * CHESS_BOX_SIZE + fromRow * CHESS_BOX_SIZE, PHEIGHT);
        cModel.endPos = cModel.cTModelMap[p].tPos;
        cModel.startTime = std::chrono::high_resolution_clock::now();
    }
};