#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "chess_game.h"

using namespace std;
vector<pair<int, int>> calculateValidMoves(ChessPiece *selectedPiece, const vector<ChessPiece> &pieces, vector<vector<int>> &map)
{
  vector<pair<int, int>> validMoves;
  if (!selectedPiece)
    return validMoves;

  switch (selectedPiece->type)
  {
  case PAWN:
  {
    // xac dinh huong di chuyen dua tren vi tri ban dau
    int direction = selectedPiece->initialY < 3 ? 1 : -1;
    int newY = selectedPiece->y + direction;

    // kiem tra khong cho phep di nguoc
    if (selectedPiece->initialY < 3) // neu quan o nua tren
    {
      if (direction < 0) // khong cho phep di len
      {
        break;
      }
    }
    else // neu quan o nua duoi
    {
      if (direction > 0) // khong cho phep di xuong
      {
        break;
      }
    }

    // kiem tra nuoc di hop le
    if (newY >= 0 && newY < 5)
    {
      if (map[newY][selectedPiece->x] == 1 &&
          !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x, newY))
      {
        validMoves.push_back({selectedPiece->x, newY});
      }
    }
    break;
  }
  case BISHOP:
    // Tượng di chuyển theo đường chéo
    for (int i = 1; i < 5; ++i)
    {
      // cheo phai duoi
      if (selectedPiece->x + i < 5 && selectedPiece->y + i < 5)
      {
        if (map[selectedPiece->y + i][selectedPiece->x + i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x + i, selectedPiece->y + i))
        {
          validMoves.push_back({selectedPiece->x + i, selectedPiece->y + i});
        }
      }
      // cheo trai duoi
      if (selectedPiece->x - i >= 0 && selectedPiece->y + i < 5)
      {
        if (map[selectedPiece->y + i][selectedPiece->x - i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x - i, selectedPiece->y + i))
        {
          validMoves.push_back({selectedPiece->x - i, selectedPiece->y + i});
        }
      }
      // cheo phai tren
      if (selectedPiece->x + i < 5 && selectedPiece->y - i >= 0)
      {
        if (map[selectedPiece->y - i][selectedPiece->x + i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x + i, selectedPiece->y - i))
        {
          validMoves.push_back({selectedPiece->x + i, selectedPiece->y - i});
        }
      }
      // cheo trai tren
      if (selectedPiece->x - i >= 0 && selectedPiece->y - i >= 0)
      {
        if (map[selectedPiece->y - i][selectedPiece->x - i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x - i, selectedPiece->y - i))
        {
          validMoves.push_back({selectedPiece->x - i, selectedPiece->y - i});
        }
      }
    }
    break;

  case ROOK:
    // Xe di chuyển theo đường thẳng (hàng hoặc cột)
    for (int i = 1; i < 5; ++i)
    {
      // phai
      if (selectedPiece->x + i < 5)
      {
        if (map[selectedPiece->y][selectedPiece->x + i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x + i, selectedPiece->y))
        {
          validMoves.push_back({selectedPiece->x + i, selectedPiece->y});
        }
      }
      // trai
      if (selectedPiece->x - i >= 0)
      {
        if (map[selectedPiece->y][selectedPiece->x - i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x - i, selectedPiece->y))
        {
          validMoves.push_back({selectedPiece->x - i, selectedPiece->y});
        }
      }
      // xuong
      if (selectedPiece->y + i < 5)
      {
        if (map[selectedPiece->y + i][selectedPiece->x] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x, selectedPiece->y + i))
        {
          validMoves.push_back({selectedPiece->x, selectedPiece->y + i});
        }
      }
      // Len
      if (selectedPiece->y - i >= 0)
      {
        if (map[selectedPiece->y - i][selectedPiece->x] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x, selectedPiece->y - i))
        {
          validMoves.push_back({selectedPiece->x, selectedPiece->y - i});
        }
      }
    }
    break;

  case QUEEN:
    // Hậu có thể di chuyển như xe và tượng cộng lại
    for (int i = 1; i < 5; ++i)
    {
      // che phai duoi
      if (selectedPiece->x + i < 5 && selectedPiece->y + i < 5)
      {
        if (map[selectedPiece->y + i][selectedPiece->x + i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x + i, selectedPiece->y + i))
        {
          validMoves.push_back({selectedPiece->x + i, selectedPiece->y + i});
        }
      }
      // cheo trai duoi
      if (selectedPiece->x - i >= 0 && selectedPiece->y + i < 5)
      {
        if (map[selectedPiece->y + i][selectedPiece->x - i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x - i, selectedPiece->y + i))
        {
          validMoves.push_back({selectedPiece->x - i, selectedPiece->y + i});
        }
      }
      // cheo phai tren
      if (selectedPiece->x + i < 5 && selectedPiece->y - i >= 0)
      {
        if (map[selectedPiece->y - i][selectedPiece->x + i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x + i, selectedPiece->y - i))
        {
          validMoves.push_back({selectedPiece->x + i, selectedPiece->y - i});
        }
      }
      // cheo trai tren
      if (selectedPiece->x - i >= 0 && selectedPiece->y - i >= 0)
      {
        if (map[selectedPiece->y - i][selectedPiece->x - i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y,
                           selectedPiece->x - i, selectedPiece->y - i))
        {
          validMoves.push_back({selectedPiece->x - i, selectedPiece->y - i});
        }
      }
      // Đường thẳng
      if (selectedPiece->x + i < 5) // Phải
      {
        if (map[selectedPiece->y][selectedPiece->x + i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x + i, selectedPiece->y))
        {
          validMoves.push_back({selectedPiece->x + i, selectedPiece->y});
        }
      }
      if (selectedPiece->x - i >= 0) // Trái
      {
        if (map[selectedPiece->y][selectedPiece->x - i] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x - i, selectedPiece->y))
        {
          validMoves.push_back({selectedPiece->x - i, selectedPiece->y});
        }
      }
      if (selectedPiece->y + i < 5) // Xuong
      {
        if (map[selectedPiece->y + i][selectedPiece->x] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x, selectedPiece->y + i))
        {
          validMoves.push_back({selectedPiece->x, selectedPiece->y + i});
        }
      }
      if (selectedPiece->y - i >= 0) // Len
      {
        if (map[selectedPiece->y - i][selectedPiece->x] == 1 &&
            !isPathBlocked(pieces, selectedPiece->x, selectedPiece->y, selectedPiece->x, selectedPiece->y - i))
        {
          validMoves.push_back({selectedPiece->x, selectedPiece->y - i});
        }
      }
    }
    break;
  }
  return validMoves;
}

// Ham kiem tra xem vi tri co hop le khong
bool isValidMove(ChessPiece &piece, int newX, int newY, vector<ChessPiece> &pieces)
{
  if (piece.isWhite)
  {
    return false;
  }
  // doc ma tran tu file
  vector<vector<int>> map = loadMapFromFile("map2.txt");

  // Kiem tra xem nuoc đi co nam trong gioi han cua ban co hay khong
  if (newX < 0 || newX >= cols || newY < 0 || newY >= rows)
  {
    return false;
  }

  // Kiem tra o dich phai la o co gia tri 1
  if (map[newY][newX] != 1)
  {
    return false;
  }

  // kiem tra xem co quan nao o vi tri dich khong
  for (const auto &p : pieces)
  {
    if (p.x == newX && p.y == newY)
    {
      int dx = newX - piece.x;
      int dy = newY - piece.y;
      // tim vi tri moi cho quan bi day
      int pushX = newX + (dx != 0 ? dx / abs(dx) : 0);
      int pushY = newY + (dy != 0 ? dy / abs(dy) : 0);

      // Kiem tra vi tri day hop le
      if (pushX >= 0 && pushX < cols && pushY >= 0 && pushY < rows &&
          map[pushY][pushX] == 1)
      {
        // Kiem tra khong co quan o vi tri day
        bool canPush = true;
        for (const auto &other : pieces)
        {
          if (other.x == pushX && other.y == pushY)
          {
            canPush = false;
            break;
          }
        }

        // Kiem tra luat di chuyen cua tung loai quan
        switch (piece.type)
        {
        case PAWN:
          return canPush && (piece.x == newX) &&
                 (newY == piece.y + (piece.initialY < 3 ? 1 : -1));
        case ROOK:
          return canPush && (piece.x == newX || piece.y == newY) &&
                 !isPathBlocked(pieces, piece.x, piece.y, newX, newY);
        case BISHOP:
          return canPush && (abs(piece.x - newX) == abs(piece.y - newY)) &&
                 !isPathBlocked(pieces, piece.x, piece.y, newX, newY);
        case QUEEN:
          return canPush && ((piece.x == newX || piece.y == newY) || (abs(piece.x - newX) == abs(piece.y - newY))) &&
                 !isPathBlocked(pieces, piece.x, piece.y, newX, newY);
        default:
          return false;
        }
      }
      return false; // Khong the day
    }
  }

  // Neu khong co quan can, kiem tra luat di chuyen binh thuong
  switch (piece.type)
  {
  case PAWN:
  {
    int direction = piece.initialY < 3 ? 1 : -1;
    return (piece.x == newX && newY == piece.y + direction);
  }
  case ROOK:
    return (piece.x == newX || piece.y == newY) &&
           !isPathBlocked(pieces, piece.x, piece.y, newX, newY);
  case BISHOP:
    return (abs(piece.x - newX) == abs(piece.y - newY)) &&
           !isPathBlocked(pieces, piece.x, piece.y, newX, newY);
  case QUEEN:
    return ((piece.x == newX || piece.y == newY) ||
            (abs(piece.x - newX) == abs(piece.y - newY))) &&
           !isPathBlocked(pieces, piece.x, piece.y, newX, newY);
  default:
    return false;
  }
}

// hàm ăn quân
bool capturePiece(vector<ChessPiece> &pieces, ChessPiece &currentPiece, int targetX, int targetY)
{
  if (currentPiece.isWhite)
  {
    return false;
  }
  // kiem tra gioi han ban co
  if (targetX < 0 || targetX >= cols || targetY < 0 || targetY >= rows)
  {
    return false;
  }

  // tim quan co o vi tri dich
  auto targetIt = find_if(pieces.begin(), pieces.end(), [targetX, targetY](const ChessPiece &p)
                          { return p.x == targetX && p.y == targetY; });

  // kiem tra quan địch ở vị trí đích không
  if (targetIt != pieces.end() && targetIt->isWhite != currentPiece.isWhite)
  {
    // kiem tra dduong di co bi chan khong
    if (!isPathBlocked(pieces, currentPiece.x, currentPiece.y, targetX, targetY))
    {
      // kiem tra huong di chuyen hop le cho tung loai quan
      bool validCapture = false;
      switch (currentPiece.type)
      {
      case PAWN:
      {
        int direction = currentPiece.initialY < 3 ? 1 : -1;
        validCapture = (targetY == currentPiece.y + direction) && (abs(targetX - currentPiece.x) == 1);
        break;
      }
      case BISHOP:
        validCapture = abs(targetX - currentPiece.x) == abs(targetY - currentPiece.y);
        break;
      case ROOK:
        validCapture = (targetX == currentPiece.x || targetY == currentPiece.y);
        break;
      case QUEEN:
        validCapture = (targetX == currentPiece.x || targetY == currentPiece.y) || (abs(targetX - currentPiece.x) == abs(targetY - currentPiece.y));
        break;
      default:
        validCapture = false;
      }

      if (validCapture)
      {
        // Di chuyển quân hiện tại đến vị trí mới
        currentPiece.x = targetX;
        currentPiece.y = targetY;

        // Xóa quân bị ăn
        pieces.erase(targetIt);
        return true;
      }
    }
  }

  return false;
}

// ham kiem tra xem co bi chan khong
bool isPathBlocked(const vector<ChessPiece> &pieces, int startX, int startY, int endX, int endY)
{
  vector<vector<int>> map = loadMapFromFile("map2.txt");
  int dx = (endX > startX) ? 1 : (endX < startX) ? -1
                                                 : 0;
  int dy = (endY > startY) ? 1 : (endY < startY) ? -1
                                                 : 0;

  int x = startX + dx;
  int y = startY + dy;

  while (x != endX || y != endY && x >= 0 && x < map[0].size() && y >= 0 && y < map.size())
  {
    if (map[y][x] != 1)
    {
      return true;
    }

    // Check for blocking pieces
    for (const auto &piece : pieces)
    {
      if (piece.x == x && piece.y == y)
      {
        return true; // Path is blocked
      }
    }
    x += dx;
    y += dy;
  }
  // kiem tra neu ra khoi ban co
  if (x < 0 || x >= map[0].size() || y < 0 || y >= map.size())
  {
    return true; // duong di khong hop le
  }
  return false;
}

// hàm xử lý đẩy quân
bool checkAndPushPiece(vector<ChessPiece> &pieces, int startX, int startY, int endX, int endY)
// startX, startY la vi tri ban dau (quan day), endX, endY la vi tri dich (quan bi day)
{
  vector<vector<int>> map = loadMapFromFile("map2.txt");

  // khoang cach theo chieu ngang va chieu doc de co the di chuyen quan
  int dx = endX - startX;
  int dy = endY - startY;

  // tim quan co dang di chuyen
  for (auto &piece : pieces)
  {
    if (piece.x == endX && piece.y == endY)
    {
      // tinh toan gia tri moi sau khi day (vi tri moi cua quan bi day)
      int newX = endX + dx;
      int newY = endY + dy;
      // kiem tra gioi han ban co
      if (newX >= 0 && newX < map[0].size() && newY >= 0 && newY < map.size())
      {
        // kiem  tra o dich co hop le khong
        if (map[newY][newX] == 1)
        {
          // kiem tra co quan co nao o vi tri moi khong
          bool isOccupied = false;
          for (const auto &p : pieces)
          {
            if (p.x == newX && p.y == newY)
            {
              isOccupied = true;
              break;
            }
          }

          if (!isOccupied)
          {
            piece.x = newX;
            piece.y = newY;
            return true;
          }
        }
      }
      return false;
    }
  }
  return false; // Khong co quan nao chan duong
}
