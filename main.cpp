#define SDL_MAIN_HANDLED 
#include <iostream>
#include <vector>
#include <SDL_image.h>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include "interface.h"
#include "chess_game.h"

using namespace std;
int rows;
int cols;

// Function declarations
bool init(SDL_Window *&window, SDL_Renderer *&renderer);
void drawBoard(SDL_Renderer *renderer);
void drawPieces(SDL_Renderer *renderer, const vector<ChessPiece> &pieces);
void drawValidMoves(SDL_Renderer *renderer, const vector<ChessPiece> &pieces, ChessPiece *selectedPiece);
void handleMouseEvent(SDL_Event &e, vector<ChessPiece> &pieces, ChessPiece *&selectedPiece);
void close(SDL_Window *&window, SDL_Renderer *&renderer);
// theo doi trang thai hien thi cua icon quan co
bool showChessPieces = true;
void renderInterface(SDL_Renderer *renderer, bool showIcons, bool showChessPieces)
{
  if (showIcons)
  {
    // Vẽ các icon sau khi game bắt đầu
    /*Image downHeadIcon("images/down_head_icon1.png", 60, 60, 50, 50, renderer);
    downHeadIcon.draw(renderer);*/

    Image returnIcon("images/return_icon1.png", 60, SCREEN_HEIGHT - 100, 40, 40, renderer);
    returnIcon.draw(renderer);
    if (showChessPieces)
    {
      Image xeIcon("images/white_rock.png", 60, 150, 50, 50, renderer);
      xeIcon.draw(renderer);

      Image tuongIcon("images/white_bishop.png", 60, 200, 50, 50, renderer);
      tuongIcon.draw(renderer);

      Image totIcon("images/white_pawn.png", 60, 250, 50, 50, renderer);
      totIcon.draw(renderer);
    }
  }
  else
  {
    // Vẽ giao diện chính (Start, Quit, Game)
    Image gameButton("images/Game.png", SCREEN_WIDTH / 2 - 100, 100, 200, 60, renderer);
    gameButton.draw(renderer);

    Image startButton("images/Start.png", SCREEN_WIDTH / 2 - 100, 300, 200, 60, renderer);
    startButton.draw(renderer);

    Image quitButton("images/Quit.png", SCREEN_WIDTH / 2 - 100, 400, 200, 60, renderer);
    quitButton.draw(renderer);
  }
}
// Khai báo hàm createNoiseTexture nếu không có tệp tiêu đề
SDL_Texture *createNoiseTexture(SDL_Renderer *renderer, int width, int height);
using namespace std;

// Hàm chính
int main()
{
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  // Khai báo texture noise
  SDL_Texture *noiseTexture = nullptr;
  if (!init(window, renderer))
  {
    return -1;
  }
  // Tạo texture noise
  noiseTexture = createNoiseTexture(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
  //
  bool showIcons = false;
  bool isGameStarted = false;
  bool isDown = false;
  bool quit = false;
  SDL_Event e;

  // Khởi tạo các quân cờ
  vector<ChessPiece> pieces;
  //{ROOK, 0, 0, 3, false},
  //{BISHOP, 2, 0,  false},
  //{QUEEN, 0, 0,  false},
  // {BISHOP, 5, 0, 2, false},
  //{ROOK, 1, 1, false},
  //{PAWN, 3,1, false, 1 < 3}};
  //{PAWN, 1, 1, 1, false},
  //{PAWN, 2, 1, 1, false},
  //{PAWN, 3, 1, 1, false},
  //{PAWN, 4, 1, 1, false},
  //{PAWN, 5, 1, 1, false},
  //{PAWN, 6, 1, 1, false},
  // {PAWN, 7, 1, 1, false},
  /*{ROOK, 0, 7, 3, true},
  {BISHOP, 2, 7, 2, true},
  {QUEEN, 3, 7, 4, true},
  {BISHOP, 5, 7, 2, true},
  {ROOK, 7, 7, 3, true},
  {PAWN, 1, 6, 1, true},
  {PAWN, 2, 6, 1, true},
  {PAWN, 3, 6, 1, true},
  {PAWN, 7, 6, 1, true}};*/

  // Các quân trắng
  pieces.push_back({PAWN, 2, 0, true,false,0});
  pieces.push_back({PAWN, 3,0, true,false,0});
  pieces.push_back({PAWN, 3, 2, true,false,2});
  pieces.push_back({PAWN, 0, 2, true, false, 2});
  // các quân den
  pieces.push_back({ROOK, 0, 4, false, false, 4}); // 0 la toa do x cot, 4 la toa do y hang, fasle trang thai false isWhite nghia la quan den, false la isSelected ban dau la chua duoc chon khi click chuot vao se la True duoc chon, 4 la toa do y ban dau initialY
  pieces.push_back({PAWN, 1, 4, false, false,4});

  ChessPiece *selectedPiece = nullptr;
  vector<vector<ChessPiece>> moveHistory;

  // Vòng lặp game
  while (!quit)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
      {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        // kiem tra nut quit

        Image quitButton("images/Quit.png", SCREEN_WIDTH / 2 - 100, 400, 200, 60, renderer);
        if (!isGameStarted && quitButton.isClicked(mouseX, mouseY))
        {
          quit = true;
        }

        // kiem tra nut start
        Image startButton("images/Start.png", SCREEN_WIDTH / 2 - 100, 300, 200, 60, renderer);
        if (!isGameStarted && startButton.isClicked(mouseX, mouseY))
        {
          isGameStarted = true;
          showIcons = true;
          showChessPieces = false;
        }

        // kiem tra click vao return
        Image returnIcon("images/return_icon1.png", 60, SCREEN_HEIGHT - 100, 40, 40, renderer);
        if (isGameStarted && returnIcon.isClicked(mouseX, mouseY) && !moveHistory.empty())
        {
          pieces = moveHistory.back();
          moveHistory.pop_back();
          selectedPiece = nullptr;
          continue;
        }

        // Xử lý downHeadButton ở cả hai trạng thái
        Image downHeadButton("images/down_head_icon1.png", 60, 60, 50, 50, renderer);
        if (downHeadButton.isClicked(mouseX, mouseY))
        {
          showChessPieces = !showChessPieces;
        }

        // luu trang thai va chuyen quan
        if (isGameStarted)
        {
          if (selectedPiece != nullptr)
          {
            moveHistory.push_back(pieces);
          }
          handleMouseEvent(e, pieces, selectedPiece);
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer); // Xóa màn hình trước khi vẽ lại noise

    // Vẽ noise
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < NOISE_DENSITY; ++i)
    {
      int x = std::rand() % SCREEN_WIDTH;
      int y = std::rand() % SCREEN_HEIGHT;
      SDL_RenderDrawPoint(renderer, x, y);
    }
    if (!isGameStarted)
    {
      renderInterface(renderer, showIcons, showChessPieces);
    }
    else
    {
      // Vẽ các thành phần của game
      // Clear screen
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      // Vẽ bàn cờ
      drawBoard(renderer);
      // Vẽ các quân cờ
      drawPieces(renderer, pieces);
      // Vẽ các nước đi hợp lệ
      drawValidMoves(renderer, pieces, selectedPiece);
      // Vẽ noise
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      for (int i = 0; i < NOISE_DENSITY; ++i)
      {
        int x = std::rand() % SCREEN_WIDTH;
        int y = std::rand() % SCREEN_HEIGHT;
        SDL_RenderDrawPoint(renderer, x, y);
      }
      renderInterface(renderer, showIcons, showChessPieces);
    }
    // Cập nhật màn hình
    SDL_RenderPresent(renderer);
  }
  close(window, renderer);
  return 0;
}