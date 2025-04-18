#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void saveToFile(int **board, int rows, int cols){
  string fileName;
  cout << "Nhap ten file de luu ma tran: ";
  cin >> fileName;

  ofstream outFile(fileName);
  if (outFile.is_open()){
    //Ghi kich thuoc ma tran vao dau file
    outFile << rows << " " << cols << endl;
    //ghi ma tran
    for (int i = 0; i < rows; i++){
      for (int j = 0; j < cols; j++){
        outFile << board[i][j] << " ";
      }
      outFile << endl;
    }
    outFile.close();
    cout << "Board pattern saved to file " << fileName << endl;
  }else{
    cout << "Unable to open file!" << endl; 
  }
}

void createChessBoard()
{
  int rows, cols;
  cout << "Nhap so hang:";
  cin >> rows;
  cout << "Nhap so cot:";
  cin >> cols;

  // cap phat dong mang 2 chieu
  int **board = new int *[rows];
  for (int i = 0; i < rows; i++)
  {
    board[i] = new int[cols];
  }

  cout << "Nhap ma tran (" << rows << "x" << cols << "):" << endl;
  cout << "0 = o trong, 1 = o hop le" << endl; 

  // Nhap gia tri cho ma tran
  cout << " ";
  for (int j = 0; j < cols; j++)
  {
    cout << j << " ";
  }
  cout << endl;

  //nhap gia tri cho ma tran hien thi toa do
  for (int i = 0; i < rows; i++)
  { 
    cout << i << ": "; //hien thi so hang
    for (int j = 0; j < cols; j++)
    {
      cin >> board[i][j];
      if (board[i][j] == 1){
        cout << "Da nhap 1 tai toa do [" << i << "," << j << "]" << endl;
      }
    }
  }
  //luu vao file
  saveToFile(board,rows, cols);

  //giai phong bo nho
  for (int i = 0; i < rows; i++)
  {
    delete[] board[i];
  }
  delete[] board;
}

int main()
{
  createChessBoard();
  return 0;
}