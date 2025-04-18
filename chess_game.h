// FILE: chess_game.h
#ifndef CHESS_GAME_H
#define CHESS_GAME_H
#include <vector>
#include <SDL.h>
#include <fstream>
#include <string>

using namespace std;
// ban co
extern int rows;
extern int cols;

// Kích thước cửa sổ game
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
// Kích thước mỗi ô vuông trong bàn cờ
const int TILE_SIZE = 60;

// Enum để đại diện cho các loại quân
enum PieceType
{
  PAWN,
  BISHOP,
  KNIGHT,
  ROOK,
  QUEEN,
  KING
};

// dinh nghia stuct ChessPiece
struct ChessPiece
{
  PieceType type;  // loai quan co
  int x, y;        // vi tri x,y tren ban co
  bool isWhite;    // mau quan co
  bool isSelected; // trang thai duoc chon
  int initialY;   // vi tri y ban dau
};

void handleMouseEvent(SDL_Event &e, vector<ChessPiece> &pieces, ChessPiece *&selectedPiece);
// Hàm kiểm tra xem vị trí có hợp lệ không
bool isValidMove(ChessPiece &piece, int newX, int newY,  vector<ChessPiece> &pieces);

// Hàm kiểm tra xem đường đi có bị chặn không
bool isPathBlocked(const vector<ChessPiece> &pieces, int startX, int startY, int endX, int endY);

// Ham doc map tu File txt
vector<vector<int>> loadMapFromFile(const string &filename);

// ham tinh toan cac nuoc di hop le
vector<pair<int, int>> calculateValidMoves(ChessPiece *selectedPiece, const vector<ChessPiece> &pieces,  vector<vector<int>> &map);

//ham kiem tra day quan
bool checkAndPushPiece(vector<ChessPiece> &pieces, int startX, int startY, int endX, int endY);

//ham kiem tra an quan 
bool capturePiece(vector<ChessPiece> &pieces, ChessPiece &currentPiece, int targetX, int targetY);

// Khai báo các hàm từ interface_map.cpp
bool init(SDL_Window *&window, SDL_Renderer *&renderer);
void drawBoard(SDL_Renderer *renderer);
void drawPieces(SDL_Renderer *renderer, const vector<ChessPiece> &pieces);
void drawValidMoves(SDL_Renderer *renderer, const vector<ChessPiece> &pieces, ChessPiece *selectedPiece);
void close(SDL_Window *&window, SDL_Renderer *&renderer);

#endif // CHESS_GAME_H