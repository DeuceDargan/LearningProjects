#include <iostream>

//-------------------------------------------------------------------------------//
//                           constant variables                                  //
//-------------------------------------------------------------------------------//
#define boardVar board[MAX_BOARD_COL]

const int MIN_PLAYERS_NUM = 2;
const int MIN_BOARD_ROW = 3;
const int MIN_BOARD_COL = 3;
const int MAX_PLAYERS_NUM = 7;
const int MAX_BOARD_ROW = 12;
const int MAX_BOARD_COL = 14;
const int WIN_SIZE = 3;

struct Player
{
  std::string firstName;
  std::string fullName;
  int wins = 0;
  int loss = 0;
  int draw = 0;
  char mark;
};

//-------------------------------------------------------------------------------//
//                        utility functions prototypes                           //
//-------------------------------------------------------------------------------//
std::string nameCase (std::string);
std::string spaces (int);
char* clearBoard (char[][MAX_BOARD_COL]);
int advTurn (int,int);
int strToInt (std::string);
int digitCount (int);
bool checkNum (std::string);
bool ynChoice (std::string);

//-------------------------------------------------------------------------------//
//                        pregame functions prototypes                           //
//-------------------------------------------------------------------------------//
bool checkName (std::string);
std::string setPlayerFull (int);
std::string setPlayerFirst (std::string);

//-------------------------------------------------------------------------------//
//                       playgame functions prototypes                           //
//-------------------------------------------------------------------------------//
void displayBoard (char[][MAX_BOARD_COL], int, int);
char* setPiece (char[][MAX_BOARD_COL], Player, int, int);
bool checkWinner (char[][MAX_BOARD_COL], int, int);
bool checkWinnerHorizontal (char[][MAX_BOARD_COL], int, int);
bool checkWinnerVertical (char[][MAX_BOARD_COL], int, int);
bool checkWinnerIncline (char[][MAX_BOARD_COL], int, int);
bool checkWinnerDecline (char[][MAX_BOARD_COL], int, int);

//-------------------------------------------------------------------------------//
//                        endgame functions prototypes                           //
//-------------------------------------------------------------------------------//
void showScores (Player[],int);
Player* updateScores (Player[],int,int,int);

//-------------------------------------------------------------------------------//
//                                main function                                  //
//-------------------------------------------------------------------------------//
int main () {
  
  Player players[MAX_PLAYERS_NUM];
  Player* playerptr;
  std::string tempstr;
  std::string input;
  std::string playerOrder[MAX_PLAYERS_NUM];
  int playerNum;
  int rows;
  int cols;
  int count;
  int winner;
  int gamesPlayed = 0;
  int turn = 1;
  int starter = 1;
  char (*boardptr)[MAX_BOARD_COL] = new char[MAX_BOARD_ROW][MAX_BOARD_COL];
  bool orderCheck = false;
  bool end = false;
  bool valid;
  bool changeBoard = true;

  std::cout << "GAME START";
  std::cout << std::endl << std::endl;

  do
  {
    std::cout << "Enter the number of players -> ";
    std::getline(std::cin, input);
    valid = checkNum(input) && 
            strToInt(input) >= MIN_PLAYERS_NUM && 
            strToInt(input) <= MAX_PLAYERS_NUM;

    if (!valid)
    {
      std::cout << "Enter a NUMBER between " << MIN_PLAYERS_NUM << " to " << MAX_PLAYERS_NUM << " players\n\n";
    }
  } while (!valid);
  
  playerNum = strToInt(input);
  
  for (int x = 0; x < playerNum; x++)
  {
    players[x].fullName = setPlayerFull(x);
    players[x].firstName = setPlayerFirst(players[x].fullName);
    players[x].mark = 'a' + x;
    playerOrder[x] = players[x].firstName;
  } 
  do
  {
    if (changeBoard)
    {
      do
      {
        std::cout << "Enter the number board rows    -> ";
        std::getline(std::cin, input);
        valid = checkNum(input) && 
                strToInt(input) >= MIN_BOARD_ROW && 
                strToInt(input) <= MAX_BOARD_ROW;
    
        if (!valid)
        {
          std::cout << "Enter a NUMBER between " << MIN_BOARD_ROW << " to " << MAX_BOARD_ROW << " rows\n\n";
        }
      } while (!valid);
      
      rows = strToInt(input);
    
      do
      {
        std::cout << "Enter the number board columns -> ";
        std::getline(std::cin, input);
        valid = checkNum(input) && 
                strToInt(input) >= MIN_BOARD_COL && 
                strToInt(input) <= MAX_BOARD_COL;
    
        if (!valid)
        {
          std::cout << "Enter a NUMBER between " << MIN_BOARD_COL << " to " << MAX_BOARD_COL << " columns\n\n";
        }
      } while (!valid);
      
      cols = strToInt(input);
    } 
    playerptr = players;
    clearBoard(boardptr);
    winner = -1;
    count = 0;
    
    do
    {
      std::cout << std::endl << std::endl;
      setPiece(boardptr,players[turn], rows, cols);
      if (checkWinner(boardptr, rows, cols))
      {
        winner = turn;
      }
      turn = advTurn(turn, playerNum);
      count++;
      
    } while (winner == -1 && count < rows*cols);

    gamesPlayed++;
    std::cout << std::endl << std::endl;
    displayBoard(boardptr,rows,cols);
    std::cout << std::endl;
    updateScores(playerptr, playerNum, winner, gamesPlayed);
    std::cout << std::endl;
    showScores(playerptr, playerNum);

    if (winner == -1)
    {
      turn = advTurn(starter, playerNum);
    }
    starter = turn;
    end = gamesPlayed < 1000 && !(ynChoice("Play again?"));

    if (!end)
    {
      changeBoard = ynChoice("Do you want to change board size?");
    }   
  } while (!end);

  if (gamesPlayed >= 1000)
  {
    std::cout << "Wow you actually played 1000 games, touch grass\n";
  }
  std::cout << "\nGAME OVER";
}



//-------------------------------------------------------------------------------//
//                     check if string is all numbers                            //
//-------------------------------------------------------------------------------//
bool checkNum (std::string num)
{
  for (int x = 0; x < num.length(); x++)
  {
    if (num.at(x) < '0' || num.at(x) > '9')
    {
      return false;
    }
  }
  return true;
}

//-------------------------------------------------------------------------------//
//                      print number of spaces requested                         //
//-------------------------------------------------------------------------------//
std::string spaces (int num)
{
  std::string output = "";
  
  for (int x = 0; x < num; x++)
  {
    output += " ";
  }
  return output;
}

//-------------------------------------------------------------------------------//
//                      counts how many digits in number                         //
//-------------------------------------------------------------------------------//
int digitCount (int num)
{
  int output = 1;
  
  for (int x = 0; x > 9; x /= 10)
  {
    output++;
  }
  return output;
}

//-------------------------------------------------------------------------------//
//                           ask a yes no question                               //
//-------------------------------------------------------------------------------//
bool ynChoice (std::string question)
{
  bool valid;
  std::string input;
  
  do
  {
    std::cout << question << " (y/n) -> ";
    std::getline(std::cin, input);
    input = nameCase(input);
    valid = input == "Y" ||
            input == "Yes" ||
            input == "N" ||
            input == "No";
    if (!valid)
    {
      std::cout << "Invalid input, enter yes/no or y/n\n";
    }
  } while (!valid);
      
  return input == "Y" || input == "Yes";
}
//-------------------------------------------------------------------------------//
//                     turn a string of ints to a int                            //
//-------------------------------------------------------------------------------//
int strToInt (std::string str)
{
  int num = 0;
  
  for (int x = 0; x < str.length(); x++)
  {
    num *= 10;
    num += str.at(x) - '0';
  }
  return num;
}


//-------------------------------------------------------------------------------//
//                         Change string to namecase                             //
//-------------------------------------------------------------------------------//
std::string nameCase (std::string input)
{
  std::string output = "";
  
  for (int x = 0; x < input.length(); x++)
  {
    if (x == 0 || input.at(x-1) == ' ')
    {
      if (input.at(x) > 'Z')
      {
        output += input.at(x)-32;
      }
      else
      {
        output += input.at(x);
      }
    }
    else
    {
      if (input.at(x) < 'a' && input.at(x) != ' ')
      {
        output += input.at(x)+32;
      }
      else
      {
        output += input.at(x);
      }
    }
  }
  return output;
}


//-------------------------------------------------------------------------------//
//                               Resets the board                                //
//-------------------------------------------------------------------------------//
char * clearBoard (char board[][MAX_BOARD_COL])
{
  for (int x = 0; x < MAX_BOARD_ROW; x++)
  {
    for (int y = 0; y < MAX_BOARD_COL; y++)
    {
      board[x][y] = ' ';
    }   
  }
  return boardVar;
}


//-------------------------------------------------------------------------------//
//                          Sets turn to next player                             //
//-------------------------------------------------------------------------------//
int advTurn (int turn, int playerNum)
{
  if (turn == playerNum-1)
  {
    turn = 0;
  }
  else
  {
    turn++;
  }
  return turn;
}


//-------------------------------------------------------------------------------//
//                         check if string in a name                             //
//-------------------------------------------------------------------------------//
bool checkName (std::string name)
{
  bool multiName = false;
  int spaceCount = 0;
  
  for (int x = 0; x < name.length(); x++)
  {
    if (!((name.at(x) >= 'a' && name.at(x) <= 'z') || 
          (name.at(x) >= 'A' && name.at(x) <= 'Z') || 
           name.at(x) == ' '))
    {
      return false;
    }
    if (name.at(x) == ' ')
    {
      multiName = true;
      spaceCount++;
      
      if (x == 0 || name.at(x - 1) == ' ' || name.at(x + 1) == ' ')
      {
        return false;
      }
    }
  }
  return multiName && (spaceCount == 1);
}


//-------------------------------------------------------------------------------//
//                           set players full name                               //
//-------------------------------------------------------------------------------//
std::string setPlayerFull (int player)
{
  std::string input;
  std::string output;
  
  do
  {
    std::cout << "Enter player " << player + 1 << "'s fullname -> ";
    std::getline(std::cin, input);
    
    if (!checkName(input))
    {
      std::cout << "Please enter a valid full name with spaces inbetween only letters\n\n";
    }
  } while (!checkName(input));

  return nameCase(input);
}


//-------------------------------------------------------------------------------//
//                  takes the first name out the full name                       //
//-------------------------------------------------------------------------------//
std::string setPlayerFirst (std::string input)
{
  return input.substr(0, input.find_first_of(' '));
}


//-------------------------------------------------------------------------------//
//                            display game board                                 //
//-------------------------------------------------------------------------------//
void displayBoard (char board[][MAX_BOARD_COL], int rows, int cols)
{
  std::cout << "    ";
  
  for (int x = 1; x <= cols; x++)
  {
    if (x >= 10)
    {
      std::cout << x << "  ";
    }
    else
    {
      std::cout << x << "   ";      
    }
  }
  std::cout << "\n   ";
  
  for (int x = 0; x < cols; x++)
  {
    std::cout << "--- ";
  }
  std::cout << std::endl;

  for (int x = 0; x < rows; x++)
  {
    std::cout << char('A'+x) << " | ";
    
    for (int y = 0; y < cols; y++)
    {
      std::cout << board[x][y] << " | ";
    }
    
    std::cout << char('A'+x) << std::endl;
    std::cout << "   ";
    
    for (int x = 0; x < cols; x++)
    {
      std::cout << "--- ";
    }
    
    std::cout << std::endl;
  }
  
  std::cout << "    ";
  
  for (int x = 1; x <= cols; x++)
  {
    if (x >= 10)
    {
      std::cout << x << "  ";
    }
    else
    {
      std::cout << x << "   ";      
    }
  }
  std::cout << std::endl;
}


//-------------------------------------------------------------------------------//
//                           place mark on board                                 //
//-------------------------------------------------------------------------------//
char * setPiece (char board[][MAX_BOARD_COL], Player player, int rows, int cols)
{
  int rowInput;
  int colInput;
  bool valid;
  std::string input;
  std::cout << player.firstName << "'s Turn [" << player.mark << "]" << std::endl;
  displayBoard(board, rows, cols);
  
  do
  {
    std::cout << "Select a coordinate, row then column (example:A1), to place [" << player.mark << "] -> ";
    std::getline(std::cin, input);

    valid = input.length() > 1 && input.length() <= 3;

    if (valid)
    {
      rowInput = input.at(0);
      rowInput -= 32 * (input.at(0) > 'Z');
      rowInput -= 'A';

      if (input.length() == 2)
      {
        colInput = input.at(1) - '1';
      }
      else
      {
        colInput = strToInt(input.substr(1,2)) - 1;
      }

      valid = rowInput >= 0 &&
              rowInput < rows &&
              colInput >= 0 &&
              colInput < cols &&
              board[rowInput][colInput] == ' ';
    }
    
    if (!valid)
    {
      std::cout << "Invalid input\n";
    }
    else
    {
      board[rowInput][colInput] = player.mark;
    }
  } while (!valid);
  
  return boardVar;
}


//-------------------------------------------------------------------------------//
//                          check if theres a winner                             //
//-------------------------------------------------------------------------------//
bool checkWinner (char board[][MAX_BOARD_COL], int row, int col)
{
  bool winner1 = checkWinnerHorizontal(board,row,col);
  bool winner2 = checkWinnerVertical(board,row,col);
  bool winner3 = checkWinnerIncline(board,row,col);
  bool winner4 = checkWinnerDecline(board,row,col);
  
  return winner1 || winner2 || winner3 || winner4;
}


//-------------------------------------------------------------------------------//
//                      check win horizontal combinations                        //
//-------------------------------------------------------------------------------//
bool checkWinnerHorizontal (char board[][MAX_BOARD_COL], int rows, int cols)
{
  bool win = false;
  bool rowWin;
  
  for (int x = 0; x < rows; x++)
  {
    for (int y = 0; y < cols - (WIN_SIZE-1); y++)
    {
      rowWin = true;
      
      for (int z = 1; z < WIN_SIZE; z++)
      {
        if (!((board[x][y] == board[x][y + z] ||
           board[x][y] == board[x][y + z] - 32 ||
           board[x][y] == board[x][y + z] + 32) &&
           board[x][y] != ' '))
        {
          rowWin = false;
        }
      }
      if (rowWin)
      {
        win = true;
        
        for (int z = 0; z < WIN_SIZE; z++)
        {
          if (board[x][y + z] > 'Z')
          {
            board[x][y + z] = char(board[x][y + z] - 32);
            win = true;
          }
        }
      }
    }
  }
  return win;
}


//-------------------------------------------------------------------------------//
//                       check win vertical combinations                         //
//-------------------------------------------------------------------------------//
bool checkWinnerVertical (char board[][MAX_BOARD_COL], int rows, int cols)
{
  bool win = false;
  bool rowWin;
  
  for (int x = 0; x < rows - (WIN_SIZE-1); x++)
  {
    for (int y = 0; y < cols; y++)
    {
      rowWin = true;
      
      for (int z = 1; z < WIN_SIZE; z++)
      {
        if (!((board[x][y] == board[x + z][y] ||
           board[x][y] == board[x + z][y] - 32 ||
           board[x][y] == board[x + z][y] + 32) &&
           board[x][y] != ' '))
        {
          rowWin = false;
        }
      }
      if (rowWin)
      {
        win = true;
        
        for (int z = 0; z < WIN_SIZE; z++)
        {
          if (board[x + z][y] > 'Z')
          {
            board[x + z][y] = char(board[x + z][y] - 32);
            win = true;
          }
        }
      }
    }
  }
  return win;
}


//-------------------------------------------------------------------------------//
//                    check win incline diagnal combinations                     //
//-------------------------------------------------------------------------------//
bool checkWinnerIncline (char board[][MAX_BOARD_COL], int rows, int cols)
{
  bool win = false;
  bool rowWin;
  
  for (int x = (WIN_SIZE-1); x < rows; x++)
  {
    for (int y = 0; y < cols - (WIN_SIZE-1); y++)
    {
      rowWin = true;
      
      for (int z = 1; z < WIN_SIZE; z++)
      {
        if (!((board[x][y] == board[x - z][y + z] ||
           board[x][y] == board[x - z][y + z] - 32 ||
           board[x][y] == board[x - z][y + z] + 32) &&
           board[x][y] != ' '))
        {
          rowWin = false;
        }
      }
      if (rowWin)
      {
        win = true;
        for (int z = 0; z < WIN_SIZE; z++)
        {
          if (board[x - z][y + z] > 'Z')
          {
            board[x - z][y + z] = char(board[x - z][y + z] - 32);
            win = true;
          }
        }
      }
    }
  }
  return win;
}


//-------------------------------------------------------------------------------//
//                    check win decline diagnal combinations                     //
//-------------------------------------------------------------------------------//
bool checkWinnerDecline (char board[][MAX_BOARD_COL], int rows, int cols)
{
  bool win = false;
  bool rowWin;
  
  for (int x = 0; x < rows - (WIN_SIZE-1); x++)
  {
    for (int y = 0; y < cols - (WIN_SIZE-1); y++)
    {
      rowWin = true;
      for (int z = 1; z < WIN_SIZE; z++)
      {
        if (!((board[x][y] == board[x + z][y + z] ||
           board[x][y] == board[x + z][y + z] - 32 ||
           board[x][y] == board[x + z][y + z] + 32) &&
           board[x][y] != ' '))
        {
          rowWin = false;
        }
      }
      if (rowWin)
      {
        win = true;
        for (int z = 0; z < WIN_SIZE; z++)
        {
          if (board[x + z][y + z] > 'Z')
          {
            board[x + z][y + z] = char(board[x + z][y + z] - 32);
            win = true;
          }
        }
      }
    }
  }
  return win;
}


//-------------------------------------------------------------------------------//
//                           show the score board                                //
//-------------------------------------------------------------------------------//
void showScores (Player player[], int playerNum)
{
  int highestNameLength = 0;

  for (int x = 0; x < playerNum; x++)
  {
    if (highestNameLength < player[x].fullName.length())
    {
      highestNameLength = player[x].fullName.length();
    }
  }
  
  std::cout << spaces(highestNameLength) << "  ------ ------ ------\n";
  std::cout << spaces(highestNameLength) << " |  WIN | LOSS | DRAW |\n";
  std::cout << spaces(highestNameLength) << "  ------ ------ ------\n";

  for (int x = 0; x < playerNum; x++)
  {
    std::cout << spaces(highestNameLength - player[x].fullName.length()) << player[x].fullName;
    std::cout << " | " << spaces(4 - digitCount(player[x].wins)) << player[x].wins;
    std::cout << " | " << spaces(4 - digitCount(player[x].loss)) << player[x].loss;
    std::cout << " | " << spaces(4 - digitCount(player[x].draw)) << player[x].draw << " |\n";
    std::cout << spaces(highestNameLength) << "  ------ ------ ------\n";
  }
}


//-------------------------------------------------------------------------------//
//                       update points on scoreboard                             //
//-------------------------------------------------------------------------------//
Player * updateScores (Player players[],int playerNum,int winner,int gamesPlayed)
{
  if (winner == -1)
  {
    std::cout << "***DRAW***" << std::endl;
    std::cout << "Games Played: " << gamesPlayed << std::endl;
    for(int x = 0; x < playerNum; x++)
    {
      players[x].draw++;
    }
  }
  else
  {
    std::cout << "***" << players[winner].firstName << " WINS***" << std::endl;
    std::cout << "Games Played: " << gamesPlayed << std::endl;
    for (int x = 0; x < playerNum; x++)
    {
      if (winner == x)
      {
         players[x].wins++;
      }
      else
      {
         players[x].loss++;
      }
    }
  }
  return players;
}
