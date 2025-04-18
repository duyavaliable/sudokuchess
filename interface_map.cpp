#include <iostream>
#include <SDL_image.h>
#include <SDL.h>
#include <vector>
#include <ctime>
#include <fstream>
#include "chess_game.h"

using namespace std;

// Texture variables
SDL_Texture *whitePawnTexture;
SDL_Texture *whiteRockTexture;
SDL_Texture *whiteBishopTexture;
SDL_Texture *whiteQueenTexture;
SDL_Texture *blackPawnTexture;
SDL_Texture *blackRockTexture;
SDL_Texture *blackBishopTexture;
SDL_Texture *blackQueenTexture;

// Icon coordinates
SDL_Point threeLinesIconPos = {13 * TILE_SIZE, 1 * TILE_SIZE};
SDL_Point downHeadIconPos = {1 * TILE_SIZE, 1 * TILE_SIZE};
SDL_Point refreshIconPos = {13 * TILE_SIZE, 13 * TILE_SIZE};
SDL_Point returnIconPos = {1 * TILE_SIZE, 13 * TILE_SIZE};

// Function to load a texture
SDL_Texture *LoadTexture(const char *file, SDL_Renderer *ren)
{
  SDL_Surface *tempSurface = IMG_Load(file);
  if (!tempSurface)
  {
    cerr << "Failed to load image: " << file << " SDL_image Error: " << IMG_GetError() << endl;
    return nullptr;
  }
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tempSurface);
  SDL_FreeSurface(tempSurface);
  return tex;
}
// ham doc ma tran tu file
vector<vector<int>> loadMapFromFile(const string &filename)
{
  vector<vector<int>> map; // Khai bao vector map
  ifstream file(filename); // Mo file

  if (!file.is_open())
  {
    cerr << "Cannot open file: " << filename << endl;
    return map;
  }
  file >> rows >> cols;

  map.resize(rows, vector<int>(cols));
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      file >> map[i][j];
    }
  }
  file.close();
  return map;
}

// Hàm khởi tạo SDL2 và tạo cửa sổ game
bool init(SDL_Window *&window, SDL_Renderer *&renderer)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    cerr << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << endl;
    return false;
  }

  window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == nullptr)
  {
    cerr << "Không thể tạo cửa sổ! SDL_Error: " << SDL_GetError() << endl;
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
  {
    cerr << "Không thể tạo renderer! SDL_Error: " << SDL_GetError() << endl;
    return false;
  }

  // Initialize SDL_image
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
  {
    cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
    return false;
  }

  // Load textures
  whitePawnTexture = LoadTexture("images/white_pawn.png", renderer);
  // whiteRockTexture = LoadTexture("images/white_rock.png", renderer);
  // whiteBishopTexture = LoadTexture("images/white_bishop.png", renderer);
  // whiteQueenTexture = LoadTexture("images/white_queen.png", renderer);
  blackRockTexture = LoadTexture("images/black_rock.png", renderer);
  blackPawnTexture = LoadTexture("images/black_pawn.png", renderer);
  blackBishopTexture = LoadTexture("images/black_bishop.png", renderer);
  blackQueenTexture = LoadTexture("images/black_queen.png", renderer);
  return true;
}

// Hàm tạo texture noise
SDL_Texture *createNoiseTexture(SDL_Renderer *renderer, int width, int height)
{
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
  SDL_SetRenderTarget(renderer, texture);

  srand(static_cast<unsigned>(time(0)));
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      Uint8 r = rand() % 256;
      Uint8 g = rand() % 256;
      Uint8 b = rand() % 256;
      SDL_SetRenderDrawColor(renderer, r, g, b, 255);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }

  SDL_SetRenderTarget(renderer, nullptr);
  return texture;
}

// Hàm vẽ bàn cờ
void drawBoard(SDL_Renderer *renderer)
{
  // Render the background texture
  // SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
  auto map = loadMapFromFile("map2.txt");

  int offsetX = (WINDOW_WIDTH - cols * TILE_SIZE) / 2;
  int offsetY = (WINDOW_HEIGHT - rows * TILE_SIZE) / 2;

  // Vẽ các ô bàn cờ
  for (int row = 0; row < rows; ++row)
  {
    for (int col = 0; col < cols; ++col)
    {
      if (map[row][col] == 1) // chi ve o trang khi gia tri la 1
      {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu trắng
        SDL_Rect rect = {
            offsetX + col * TILE_SIZE,
            offsetY + row * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE};
        SDL_RenderFillRect(renderer, &rect); // Vẽ ô vuông thu vien de ve va to mau hinh chu nhat dac
        // ve vien den quanh o
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu đen
        SDL_RenderDrawRect(renderer, &rect);
      }
    }
  }
}

// Hàm vẽ quân cờ
void drawPieces(SDL_Renderer *renderer, const vector<ChessPiece> &pieces)
{
  for (const ChessPiece &piece : pieces)
  {
    // Tính vị trí quân cờ trên màn hình
    int offsetX = (WINDOW_WIDTH - cols * TILE_SIZE) / 2;
    int offsetY = (WINDOW_HEIGHT - rows * TILE_SIZE) / 2;

    int xPos = offsetX + piece.x * TILE_SIZE;
    int yPos = offsetY + piece.y * TILE_SIZE;

    SDL_Texture *texture = nullptr;
    switch (piece.type)
    {
    case PAWN:
      texture = piece.isWhite ? whitePawnTexture : blackPawnTexture;
      break;
    case ROOK:
      texture = piece.isWhite ? whiteRockTexture : blackRockTexture;
      break;
    case BISHOP:
      texture = piece.isWhite ? whiteBishopTexture : blackBishopTexture;
      break;
    case QUEEN:
      texture = piece.isWhite ? whiteQueenTexture : blackQueenTexture;
      break;
      // Add cases for other piece types and set the appropriate texture
    }

    if (texture)
    {
      SDL_Rect rect = {xPos, yPos, TILE_SIZE, TILE_SIZE};
      SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
  }
}

// ham khoi tao quan co tu map
void initializePieces(const vector<vector<int>> &map, vector<ChessPiece> &pieces)
{
  pieces.clear();

  // vi tri dat cac quan co theo thu tu uu tien
  vector<PieceType> pieceOrder = {QUEEN, BISHOP, ROOK, PAWN};
  int pieceIndex = 0;

  // duyet qua ma tran
  for (int row = 0; row < rows; ++row)
  {
    for (int col = 0; col < cols; ++col)
    {
      if (map[row][col] == 1 && pieceIndex < pieceOrder.size())
      {
        // tao qua co moi co tai vi tri co gia tri la 1
        ChessPiece piece;
        piece.x = col;
        piece.y = row;
        piece.type = pieceOrder[pieceIndex];
        piece.isWhite = false;
        piece.isSelected = false;
        piece.initialY = row; // luu vi tri y ban dau
        pieces.push_back(piece);
        pieceIndex++;
      }
    }
  }
}

// Ham xu ly su kien chuot
void handleMouseEvent(SDL_Event &e, vector<ChessPiece> &pieces, ChessPiece *&selectedPiece)
{
  if (e.type == SDL_MOUSEBUTTONDOWN)
  {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    cout << "Mouse Clicked at: (" << mouseX << ", " << mouseY << ")" << endl;

    int offsetX = (WINDOW_WIDTH - cols * TILE_SIZE) / 2;
    int offsetY = (WINDOW_HEIGHT - rows * TILE_SIZE) / 2;

    // tinh toan o duoc click
    int boardx = (mouseX - offsetX) / TILE_SIZE;
    int boardy = (mouseY - offsetY) / TILE_SIZE;

    // in toa do tren ban co
    cout << "Board position: (" << boardx << ", " << boardy << ")" << endl;

    // kiem tra xem click co nam trong ban co hay k
    if (boardx >= 0 && boardx < cols && boardy >= 0 && boardy < rows)
    {
      if (selectedPiece == nullptr)
      {
        // Chon quan co
        for (auto &piece : pieces)
        {
          if (piece.x == boardx && piece.y == boardy)
          {
            selectedPiece = &piece;
            cout << "Selected piece at: (" << piece.x << ", " << piece.y << ")" << endl;
            break;
          }
        }
      }
      else
      {
        //neu click vao chinh quan co thi huy chon
        if (selectedPiece->x == boardx && selectedPiece->y == boardy)
        {
          cout << "Deselected piece at: (" << selectedPiece->x << ", " << selectedPiece->y << ")" << endl;
          
          selectedPiece = nullptr;
        }
        else
        {
          // kiem tra xem co the an quan khong
          if (capturePiece(pieces, *selectedPiece, boardx, boardy))
          {
            cout << "Piece captured!" << endl;
            selectedPiece = nullptr;
          }

          // Di chuyen quan co
          else if (isValidMove(*selectedPiece, boardx, boardy, pieces))
          {
            // luu vi tri cu cua quan day
            int oldX = selectedPiece->x;
            int oldY = selectedPiece->y;

            // kiem tra thuc hien viec day quan co
            bool pushed = checkAndPushPiece(pieces, selectedPiece->x, selectedPiece->y, boardx, boardy);
            if (pushed)
            { // neu day thanh cong di chuyen quan day-> vi tri quan bi day
              selectedPiece->x = boardx;
              selectedPiece->y = boardy;
            }
            else
            { // neu khong day duoc di chuyen binh thuong
              selectedPiece->x = boardx;
              selectedPiece->y = boardy;
            }
            selectedPiece = nullptr;
          }
        }
      }
    }
  }
}

// Hàm vẽ các nước đi hợp lệ cho quân cờ đã chọn
void drawValidMoves(SDL_Renderer *renderer, const vector<ChessPiece> &pieces, ChessPiece *selectedPiece)
{
  if (selectedPiece == nullptr)
    return; // Không có quân nào được chọn

  vector<vector<int>> map = loadMapFromFile("map2.txt");

  int offsetX = (WINDOW_WIDTH - cols * TILE_SIZE) / 2;
  int offsetY = (WINDOW_HEIGHT - rows * TILE_SIZE) / 2;

  // Tạo vector lưu các nước đi hợp lệ
  vector<pair<int, int>> validMoves = calculateValidMoves(selectedPiece, pieces, map);

  // Vẽ các nước đi hợp lệ
  for (const auto &move : validMoves)
  {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
    SDL_Rect rect = {
        offsetX + move.first * TILE_SIZE,
        offsetY + move.second * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE};
    SDL_RenderFillRect(renderer, &rect);
  }
}

// Hàm dọn dẹp SDL
void close(SDL_Window *&window, SDL_Renderer *&renderer)
{
  // SDL_DestroyTexture(whiteRockTexture);
  SDL_DestroyTexture(whitePawnTexture);
  // SDL_DestroyTexture(whiteBishopTexture);*/
  SDL_DestroyTexture(blackQueenTexture);
  SDL_DestroyTexture(blackPawnTexture);
  SDL_DestroyTexture(blackRockTexture);
  SDL_DestroyTexture(blackBishopTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
}
