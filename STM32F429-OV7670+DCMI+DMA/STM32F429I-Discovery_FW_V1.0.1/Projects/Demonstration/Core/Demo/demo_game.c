/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : Reversi.c
Purpose     : Simple 'reversi' game

              With game engine 'SmartGecko' from
              Energy Micro AS (http://www.energymicro.com)
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    demo_game.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013 
  * @brief   Game module functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include <string.h>
#include <stdio.h>

#include "GUI.h"
#include "DIALOG.h"
#include "MENU.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define NUM_CELL_X        8
#define NUM_CELL_Y        8

#define AA_USE_HIRES      0
#define AA_FACTOR         1

#if AA_USE_HIRES
  #define AA_CALCFACTOR   AA_FACTOR
#else
  #define AA_CALCFACTOR   1
#endif

#define DESKTOP_COLOR     GUI_DARKGREEN
#define CLIENT_COLOR      GUI_DARKGRAY
#define GRID_COLOR        GUI_BROWN

#define ID_MENU_NEW       (GUI_ID_USER +  0)
#define ID_MENU_PASS      (GUI_ID_USER +  1)
#define ID_MENU_EXIT      (GUI_ID_USER +  2)
#define ID_MENU_SETTINGS  (GUI_ID_USER +  3)
#define ID_MENU_ABOUT     (GUI_ID_USER +  4)
#define ID_MENU_TEST      (GUI_ID_USER +  5)

//
// Defines, player AI 'SmartGecko'
//
#define DEPTH          4
#define END_GAME_DEPTH 9

#define INFINITY       1000000
#define WINNING_BONUS   100000

#define VALUE_OF_A_MOVE_POSSIBILITY 15
#define VALUE_OF_AN_UNSAFE_PIECE     8
#define VALUE_OF_A_SAFE_PIECE       20
#define VALUE_OF_A_CORNER         1000

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  U8  aCells[NUM_CELL_X][NUM_CELL_Y];
  U8  aMoves[NUM_CELL_X][NUM_CELL_Y];
  int ActPlayer;
} BOARD;

typedef char REVERSI_AI_Func(const BOARD * pBoard, int* px, int* py);

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// Global data
//
static WM_HWIN  _hFrame;

static REVERSI_AI_Func* _pPlayerAI[2];

static BOARD  _Board;
static int    _GameOver;
static int    _BoardX0;
static int    _BoardY0;
static int    _CellSize;

//
// Game settings
//
static int      _ShowPossibleMoves = 1;

//
// Static data, player AI 'SmartGecko'
//
static int * _px;
static int * _py;

static BOARD _aBoardStack[END_GAME_DEPTH + 1];

static const int _xs[60] = { 
  7, 7, 0, 0, 7, 7, 5, 5, 2, 2, 
  0, 0, 5, 5, 2, 2, 5, 5, 4, 4, 
  3, 3, 2, 2, 7, 7, 4, 4, 3, 3, 
  0, 0, 6, 6, 6, 6, 5, 5, 4, 4, 
  3, 3, 2, 2, 1, 1, 1, 1, 7, 7, 
  6, 6, 1, 1, 0, 0, 6, 6, 1, 1
};

static const int _ys[60] = { 
  7, 0, 7, 0, 5, 2, 7, 0, 7, 0, 
  5, 2, 5, 2, 5, 2, 4, 3, 5, 2, 
  5, 2, 4, 3, 4, 3, 7, 0, 7, 0, 
  4, 3, 5, 4, 3, 2, 6, 1, 6, 1, 
  6, 1, 6, 1, 5, 4, 3, 2, 6, 1, 
  7, 0, 7, 0, 6, 1, 6, 1, 6, 1 
};

static int _aaSafe[10][10];

static const I32 _aaValues[8][8] = { 
  { 1000, -100, 25,  0,  0, 25, -100, 1000 },
  { -100, -400, -5, -5, -5, -5, -400, -100 },
  {   25,   -5, 12,  2,  2, 12,   -5,   25 },
  {    0,   -5,  2,  2,  2,  2,   -5,    0 },
  {    0,   -5,  2,  2,  2,  2,   -5,    0 },
  {   25,   -5, 12,  2,  2, 12,   -5,   25 },
  { -100, -400, -5, -5, -5, -5, -400, -100 },
  { 1000, -100, 25,  0,  0, 25, -100, 1000 }
};

/*********************************************************************
*
*       Function declarations
*
**********************************************************************
*/
static void _StartNewGame(void);

/*********************************************************************
*
*       Static code, helper functions
*
**********************************************************************
*/
/*******************************************************************
*
*       _CalcBoardDimensions
*/
static void _CalcBoardDimensions(void) {
  GUI_RECT r;
  WM_GetClientRectEx(WM_GetClientWindow(_hFrame), &r);
  _CellSize  = ((r.x1 > r.y1) ? r.y1 : r.x1) >> 3;
  _BoardX0   = (r.x1 - (_CellSize << 3)) >> 1;
  _BoardY0   = (r.y1 - (_CellSize << 3)) >> 1;
}

/*******************************************************************
*
*       _InvalidateBoard
*/
static void _InvalidateBoard(void) {
  WM_InvalidateWindow(WM_GetClientWindow(_hFrame));
}

/*******************************************************************
*
*       _InvalidateCell
*/
static void _InvalidateCell(int x, int y) {
  GUI_RECT r;
  r.x0 = _BoardX0 + (x * _CellSize);
  r.y0 = _BoardY0 + (y * _CellSize);
  r.x1 = r.x0 + _CellSize - 1;
  r.y1 = r.y0 + _CellSize - 1;
  WM_InvalidateRect(WM_GetClientWindow(_hFrame), &r);
}

/*********************************************************************
*
*       _SetCapture
*/
static void _SetCapture(void) {
#if (GUI_SUPPORT_MOUSE & GUI_SUPPORT_CURSOR)
  WM_HWIN hWin;
  hWin = WM_GetClientWindow(_hFrame);
  if (WM_HasCaptured(hWin) == 0) {
    WM_SetCapture(hWin, 0);
  }
#endif
}

/*********************************************************************
*
*       _ReleaseCapture
*/
static void _ReleaseCapture(void) {
#if (GUI_SUPPORT_MOUSE & GUI_SUPPORT_CURSOR)
  WM_HWIN hWin;
  hWin = WM_GetClientWindow(_hFrame);
  if (WM_HasCaptured(hWin)) {
    WM_ReleaseCapture();
  }
#endif
}

/*********************************************************************
*
*       Static code, game API routines
*
**********************************************************************
*/
/*******************************************************************
*
*       _GetStone
*/
static char _GetStone(const BOARD * pBoard, int x, int y) {
  char r = 0;
  if ((x >= 0) && (y >= 0) && (x < NUM_CELL_X) && (y < NUM_CELL_Y)) {
    r = pBoard->aCells[x][y];
  }
  return r;
}

/*******************************************************************
*
*       _SetStone
*/
static void _SetStone(BOARD * pBoard, int x, int y) {
  if ((x >= 0) && (y >= 0) && (x < NUM_CELL_X) && (y < NUM_CELL_Y)) {
    pBoard->aCells[x][y] = pBoard->ActPlayer;
    _InvalidateCell(x, y);
  }
}

/*******************************************************************
*
*       _IsValidMove
*/
static char _IsValidMove(BOARD * pBoard, int x, int y) {
  char r = 0;
  if ((x >= 0) && (y >= 0) && (x < NUM_CELL_X) && (y < NUM_CELL_Y)) {
    r = ((pBoard->aMoves[x][y]) ? 1 : 0);
  }
  return r;
}


/*******************************************************************
*
*       _CheckDirection
*/
static char _CheckDirection(const BOARD * pBoard, int x, int y, int dx, int dy) {
  char Cell;
  x += dx;
  y += dy;
  Cell = _GetStone(pBoard, x, y);
  if ((Cell != pBoard->ActPlayer) && (Cell != 0)) {
    do {
      x += dx;
      y += dy;
      Cell = _GetStone(pBoard, x, y);
    } while ((Cell != pBoard->ActPlayer) && (Cell != 0));
    return ((Cell == pBoard->ActPlayer) ? 1 : 0);
  }
  return 0;
}

/*******************************************************************
*
*       _CalcValidMoves
*/
static int _CalcValidMoves(BOARD * pBoard) {
  int x, y, r = 0;
  U8 Valid;
  for (y = 0; y < NUM_CELL_Y; y++) {
    for (x = 0; x < NUM_CELL_X; x++) {
      Valid = 0;
      if (pBoard->aCells[x][y] == 0) {
        Valid |= _CheckDirection(pBoard, x, y, -1, -1) << 0;
        Valid |= _CheckDirection(pBoard, x, y,  0, -1) << 1;
        Valid |= _CheckDirection(pBoard, x, y,  1, -1) << 2;
        Valid |= _CheckDirection(pBoard, x, y,  1,  0) << 3;
        Valid |= _CheckDirection(pBoard, x, y,  1,  1) << 4;
        Valid |= _CheckDirection(pBoard, x, y,  0,  1) << 5;
        Valid |= _CheckDirection(pBoard, x, y, -1,  1) << 6;
        Valid |= _CheckDirection(pBoard, x, y, -1,  0) << 7;
        if (Valid) {
          r++;
        }
      }
      if (Valid != pBoard->aMoves[x][y]) {
        pBoard->aMoves[x][y] = Valid;
        _InvalidateCell(x, y);
      }
    }
  }
  return r;
}

/*******************************************************************
*
*       _DoDirection
*/
static void _DoDirection(BOARD * pBoard, int x, int y, int dx, int dy) {
  do {
    _SetStone(pBoard, x, y);
    x += dx;
    y += dy;
  } while (_GetStone(pBoard, x, y) != pBoard->ActPlayer);
}

/*******************************************************************
*
*       _MakeMove
*/
static void _MakeMove(BOARD * pBoard, int x, int y) {
  U8 Valid;
  _SetStone(pBoard, x, y);
  Valid = pBoard->aMoves[x][y];
  if (Valid & (U8)(1 << 0)) { _DoDirection(pBoard, x, y, -1, -1); }
  if (Valid & (U8)(1 << 1)) { _DoDirection(pBoard, x, y,  0, -1); }
  if (Valid & (U8)(1 << 2)) { _DoDirection(pBoard, x, y,  1, -1); }
  if (Valid & (U8)(1 << 3)) { _DoDirection(pBoard, x, y,  1,  0); }
  if (Valid & (U8)(1 << 4)) { _DoDirection(pBoard, x, y,  1,  1); }
  if (Valid & (U8)(1 << 5)) { _DoDirection(pBoard, x, y,  0,  1); }
  if (Valid & (U8)(1 << 6)) { _DoDirection(pBoard, x, y, -1,  1); }
  if (Valid & (U8)(1 << 7)) { _DoDirection(pBoard, x, y, -1,  0); }
}

/*******************************************************************
*
*       _CalcScore
*/
static int _CalcScore(const BOARD * pBoard) {
  int x, y, r = 0;
  char Cell;
  for (y = 0; y < NUM_CELL_Y; y++) {
    for (x = 0; x < NUM_CELL_X; x++) {
      Cell = pBoard->aCells[x][y];
      if (Cell) {
        r += (Cell == pBoard->ActPlayer) ? (1) : (-1);
      }
    }
  }
  return r;
}

/*********************************************************************
*
*       Static code, player AI 'SmartGecko'
*
**********************************************************************
*/
/*********************************************************************
*
*       _valuePieces
*
* Purpose:
*   Find the value of all the pieces belonging to the given player.
*   A positive value is good for this player.
*   The value can also be negative, if the player occupies tiles
*   next to a free corner, which makes it easier for the opponent
*   to get to this corner.
*/
static I32 _valuePieces(BOARD * pBoard, int player) {
  I32 sum;
  int x, y, corners, changed, s, xCorner, yCorner, numberOfSafe;
  
  sum = 0;
  for (y = 1; y <= 8; y++) {
    for (x = 1; x <= 8; x++) {
      _aaSafe[x][y] = 0;
    }
  }
  corners = 0;
  corners += pBoard->aCells[0][0] == player;
  corners += pBoard->aCells[7][0] == player;
  corners += pBoard->aCells[0][7] == player;
  corners += pBoard->aCells[7][7] == player;
  if (corners) {
    //
    // Corners are the most valuable asset of the position.
    //
    sum += corners * VALUE_OF_A_CORNER;
    //
    // Calculate how many pieces are safe,
    // meaning they can never be taken back
    // by the opponent.
    //
    while (1) {
      //
      // Repeatedly swipe the board looking for safe pieces,
      // until no more safe pieces are found.
      //
      changed = 0;
      numberOfSafe = 0;
      for (y = 1; y <= 8; y++) {
        for (x = 1; x <= 8; x++) {
          if (!_aaSafe[x][y] && pBoard->aCells[x - 1][y - 1] == player) {
            //
            // A piece can never be taken back if in each of the four directions,
            // one of the two neighboring tiles are safe.
            //
            s =  (_aaSafe[x - 1][y    ] || _aaSafe[x + 1][y    ])  // West  - East
              && (_aaSafe[x    ][y - 1] || _aaSafe[x    ][y + 1])  // North - South
              && (_aaSafe[x - 1][y - 1] || _aaSafe[x + 1][y + 1])  // NW    - SE
              && (_aaSafe[x - 1][y + 1] || _aaSafe[x + 1][y - 1]); // SW    - NE
            if (s) {
              _aaSafe[x][y] = 1;
              changed = 1;
              ++numberOfSafe;
            }
          }
        }
        sum += numberOfSafe * VALUE_OF_A_SAFE_PIECE;
      }
      if (!changed) {
        break;
      }
    }
  }
  //
  // Now add the value of the unsafe pieces.
  //
  for (y = 0; y < 8; y++) {
    yCorner = (y < 4) ? 0 : 7;
    for (x = 0; x < 8; x++) {
      if (pBoard->aCells[x][y] == player && !_aaSafe[x + 1][y + 1]) {
        xCorner = x<4 ? 0 : 7;
        if (pBoard->aCells[xCorner][yCorner]) {
          //
          // If the corner is taken, we value each position in
          // the quadrant the same.
          //
          sum += VALUE_OF_AN_UNSAFE_PIECE;
        } else {
          //
          // If the corner is still free, we use a lookup table
          // to find the value of each position.
          //
          sum += _aaValues[x][y];
        }
      }
    }
  }
  return sum;
}

/*********************************************************************
*
*       _Eval
*
* Purpose:
*   Evaluation function for a Reversi board.
*   Positive value means player 1 is in the lead,
*   negative value means player 2 is in the lead.
*/
static I32 _Eval(BOARD * pBoard) {
  int ActPlayer, movesA, movesB;
  I32 score, value;

  ActPlayer = pBoard->ActPlayer;
  pBoard->ActPlayer = 1;
  movesA = _CalcValidMoves(pBoard);
  pBoard->ActPlayer = 2;
  movesB = _CalcValidMoves(pBoard);
  pBoard->ActPlayer = ActPlayer;
  if (movesA == 0 && movesB == 0) {
    //
    // The game is over
    //
    pBoard->ActPlayer = 1;
    score = _CalcScore(pBoard);
    pBoard->ActPlayer = ActPlayer;
    if (score==0) {
      return 0;
    }
    if (score > 0) {
      return score + WINNING_BONUS;
    }
    if (score > 0) {
      return score - WINNING_BONUS;
    }
  }
  //
  // A high number of possible moves is very valuable
  //
  value  = VALUE_OF_A_MOVE_POSSIBILITY * (movesA - movesB);
  value += _valuePieces(pBoard, 1);
  value -= _valuePieces(pBoard, 2);
  return value;
}

/*********************************************************************
*
*       _Descend
*
* Purpose:
*   Minimax search for the best possible move with alpha-beta pruning
*/
static I32 _Descend(int depth, I32 alpha, I32 beta, int firstMove) {
  BOARD * pBoard;
  BOARD * nextBoard;
  int x, y, moves, i, alt, maximize;

  pBoard    = _aBoardStack + depth;
  nextBoard = _aBoardStack + depth - 1;
  if (depth == 0) {
    return _Eval(pBoard);
  }
  moves = _CalcValidMoves(pBoard);
  if (moves == 0) {
      //
      // The player has to pass
      //
      pBoard->ActPlayer = 3 - pBoard->ActPlayer;
      moves = _CalcValidMoves(pBoard);
      if (moves == 0) {
        //
        // The game is over
        //
        return _Eval(pBoard);
      }
  }
  maximize = pBoard->ActPlayer == 1;
  for (i = 0; i < 60; ++i) {
    //
    // Try the possible moves in order from most attractive to least attractive
    // position, to maximize the effect of the alpha-beta pruning.
    //
    x = _xs[i];
    y = _ys[i];
    if (pBoard->aMoves[x][y]) {
      *nextBoard = *pBoard;
      _MakeMove(nextBoard, x, y);
      nextBoard->ActPlayer = 3 - pBoard->ActPlayer;
      //
      // Recursively evaluate the board resulting from this move.
      //
      alt = _Descend(depth - 1, alpha, beta, 0);
      if (maximize) {
        if (alt > alpha) {
          alpha = alt;
          if (firstMove) {
            *_px = x;
            *_py = y;
          }
        }
      } else {
        if (alt < beta) {
          beta = alt;
          if (firstMove) {
            *_px = x;
            *_py = y;
          }
        }
      }
      if (beta <= alpha) {
        //
        // This is the alpha-beta pruning terminal condition.
        //
        goto end;
      }
    }
  }
end:
  return maximize ? alpha : beta;
}

/*********************************************************************
*
*       _PlayerAI_SmartGecko
*/
static char _PlayerAI_SmartGecko(const BOARD * pBoard, int * px, int * py) {
  int x, y, freeTiles, depth;

  //
  // Initialize the safe board to true to get the edges right
  //
  for (y = 0; y < 10; y++) {
    for (x = 0; x < 10; x++) {
      _aaSafe[x][y] = 1;
    }
  }
  *px = -1;
  *py = -1;
  _px = px;
  _py = py;
  freeTiles = 0;
  for (y = 0; y < 8; y++) {
    for (x = 0; x < 8; x++) {
      if (!pBoard->aCells[x][y]) {
        ++freeTiles;
      }
    }
  }
  depth = DEPTH;
  if (freeTiles <= END_GAME_DEPTH) {
    //
    // In the end game, we expand the search depth.
    //
    depth = freeTiles;
  }
  _aBoardStack[depth] = *pBoard;
  _Descend(depth, -INFINITY, INFINITY, 1);
  if (*px == -1) {
    return 0;
  }
  return 1;
}

/*********************************************************************
*
*       Static code, about box
*
**********************************************************************
*/

/*********************************************************************
*
*       Static code, message box
*
**********************************************************************
*/
/*******************************************************************
*
*       _cbMessageBox
*/
static void _cbMessageBox(WM_MESSAGE* pMsg) {
  WM_HWIN hWin;
  int Id;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
    if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) 
    {
      Id = WM_GetId(pMsg->hWinSrc);
      GUI_EndDialog(hWin, (Id == GUI_ID_OK) ? 1 : 0);
    }
    else if ( pMsg->Data.v == WM_NOTIFICATION_CHILD_DELETED)
    {
      WM_NotifyParent(WM_GetParent(pMsg->hWin), 0x500);
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _ShowMessageBox
*/
static int _ShowMessageBox(const char* pTitle, const char* pText, int YesNo) {
  WM_HWIN hFrame, hClient, hBut;
  int r = 0;
  //
  // Create framewin
  //
  hFrame = FRAMEWIN_CreateEx(65, 75, 190, 90, WM_GetDesktopWindowEx(1), WM_CF_SHOW, FRAMEWIN_CF_MOVEABLE, 0, pTitle, &_cbMessageBox);
  FRAMEWIN_SetClientColor   (hFrame, GUI_WHITE);
  FRAMEWIN_SetFont          (hFrame, &GUI_Font16B_ASCII);
  FRAMEWIN_SetTextAlign     (hFrame, GUI_TA_HCENTER);
  //
  // Create dialog items
  //
  hClient = WM_GetClientWindow(hFrame);
  TEXT_CreateEx(10, 7, 170, 30, hClient, WM_CF_SHOW, GUI_TA_HCENTER, 0, pText);
  if (YesNo) {
    hBut = BUTTON_CreateEx(97, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_CANCEL);
    BUTTON_SetText        (hBut, "No");
    hBut = BUTTON_CreateEx(32, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
    BUTTON_SetText        (hBut, "Yes");
  } else {
    hBut = BUTTON_CreateEx(64, 45, 55, 18, hClient, WM_CF_SHOW, 0, GUI_ID_OK);
    BUTTON_SetText        (hBut, "Ok");
  }
  
   //Exec modal dialog
  
  WM_SetFocus(hFrame);
  WM_MakeModal(hFrame);
  r = GUI_ExecCreatedDialog(hFrame);
  //WM_SetFocus(_hFrame);
  return r;
}

/*********************************************************************
*
*       Static code, dialog settings
*
**********************************************************************
*/

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*******************************************************************
*
*       _SetPlayer
*/
static void _SetPlayer(int Player) {
  int Score, ValidMoves, PossibleMoves;
  char ac[256];
  
  _Board.ActPlayer = Player;
  if (Player == 1) {
    FRAMEWIN_SetText(_hFrame, "Reversi - Player 1");
  } else {
    FRAMEWIN_SetText(_hFrame, "Reversi - Player 2");
  }
  FRAMEWIN_SetBarColor(_hFrame, 1, (Player == 1) ? GUI_RED : GUI_BLUE);
  PossibleMoves = _CalcValidMoves(&_Board);
  GUI_Exec();

  if (!PossibleMoves) {
    GUI_Exec();
    _Board.ActPlayer = 3 - Player;
    ValidMoves = _CalcValidMoves(&_Board);
    _Board.ActPlayer = Player;
    _CalcValidMoves(&_Board);
    if (ValidMoves) {   // No valid moves, player must pass
      if (_pPlayerAI[_Board.ActPlayer - 1] == NULL) {
        _ShowMessageBox("Reversi", "No possible moves.\nYou have to pass!", 0);
      } else {
        //
        // Pass ai player
        //
      }
      _SetPlayer(3 - _Board.ActPlayer);
    } else {            // No valid moves for all players, game is over
      _GameOver = 1;
      _Board.ActPlayer = 1;
      Score = _CalcScore(&_Board);
      if (Score > 0) {
        sprintf(ac, "Red wins by %d stones!\nDo you want to start a new game?", Score);
      } else if (Score) {
        sprintf(ac, "Blue wins by %d stones!\nDo you want to start a new game?", -Score);
      } else {
        strcpy(ac, "The game ends in a draw!\nDo you want to start a new game?");
      }
      if (_ShowMessageBox("Reversi", ac, 1)) {
        _StartNewGame();
      }
    }
  }
}

/*******************************************************************
*
*       _NextPlayer
*/
static void _NextPlayer(void) {
  int x, y;
  
  do {
    _SetPlayer(3 - _Board.ActPlayer);
    if (_pPlayerAI[_Board.ActPlayer - 1]&& !_GameOver) {
      char DoMove;
      DoMove = (*_pPlayerAI[_Board.ActPlayer - 1])(&_Board, &x, &y);
      if (DoMove) {
        _MakeMove(&_Board, x, y);
      }
    }
  } while (_pPlayerAI[_Board.ActPlayer - 1] && !_GameOver);
}

/*******************************************************************
*
*       _StartNewGame
*/
static void _StartNewGame(void) {
  memset(&_Board, 0, sizeof(BOARD));
  _Board.aCells[3][3] = 1;
  _Board.aCells[4][4] = 1;
  _Board.aCells[3][4] = 2;
  _Board.aCells[4][3] = 2;
  _GameOver    = 0;
  _SetPlayer(1);
  _InvalidateBoard();
}

/*******************************************************************
*
*       _HandlePID
*/
static void _HandlePID(int x, int y, int Pressed) {
  static int _IsInHandlePID;

  if (_IsInHandlePID++ == 0) {
    _CalcBoardDimensions();
    x -= _BoardX0;
    y -= _BoardY0;
    if ((x >= 0) && (y >= 0)) {
      x /= _CellSize;
      y /= _CellSize;
      if ((x < 8) && (y < 8)) {
        if (_IsValidMove(&_Board, x, y)) {
          if (Pressed == 1) {
            _ReleaseCapture();
            _MakeMove(&_Board, x, y);
            _NextPlayer();
          } else {
            _SetCapture();
          }
          goto EndHandlePID;
        }   
      }
    }
    _ReleaseCapture();
  }
EndHandlePID:
  _IsInHandlePID--;
}

/*********************************************************************
*
*       _OnTouch
*/
static void _OnTouch(WM_MESSAGE* pMsg) {
  const GUI_PID_STATE* pState = (const GUI_PID_STATE*)pMsg->Data.p;
  if (pState) {  // Something happened in our area (pressed or released)
    _HandlePID(pState->x, pState->y, pState->Pressed);
  }
}

/*********************************************************************
*
*       _OnMouseOver
*/
#if (GUI_SUPPORT_MOUSE & GUI_SUPPORT_CURSOR)
static void _OnMouseOver(WM_MESSAGE* pMsg) {
  const GUI_PID_STATE* pState = (const GUI_PID_STATE *)pMsg->Data.p;
  if (pState) {
    _HandlePID(pState->x, pState->y, -1);
  }
}
#endif

/*******************************************************************
*
*       _OnPaint
*/
static void _OnPaint(WM_HWIN hWin) {
  GUI_COLOR Color;
  GUI_RECT  r;
  int x, y, xPos, yPos;
  int CellSize, rStone, rMove;
  char Cell, IsValidMove;
  int xCircle, yCircle;
  
  _CalcBoardDimensions();
  GUI_AA_SetFactor(AA_FACTOR);
  #if AA_USE_HIRES
    GUI_AA_EnableHiRes();
  #endif
  LCD_SetBkColor(CLIENT_COLOR);
  WM_GetClientRectEx(hWin, &r);
  GUI_ClearRect(r.x0, r.y0, r.x1, _BoardY0 - 1);
  GUI_ClearRect(r.x0, _BoardY0, _BoardX0 - 1, _BoardY0 + (8 * _CellSize));
  GUI_ClearRect(_BoardX0 + (8 * _CellSize) + 1, _BoardY0, r.x1, _BoardY0 + (8 * _CellSize));
  GUI_ClearRect(r.x0, _BoardY0 + (8 * _CellSize) + 1, r.x1, r.y1);
  CellSize = _CellSize - 1;
  rStone = ((CellSize - 3) * AA_CALCFACTOR) >> 1;
  rMove  = ((CellSize - 2) * AA_CALCFACTOR) >> 3;
  if (rStone < AA_CALCFACTOR) {
    rStone = AA_CALCFACTOR;
  }
  if (rMove < (AA_CALCFACTOR >> AA_USE_HIRES)) {
    rMove = (AA_CALCFACTOR >> AA_USE_HIRES);
  }
  for (yPos = _BoardY0, y = 0; y < 8; y++) {
    for (xPos = _BoardX0, x = 0; x < 8; x++) {
      Color = ((x + (y & 1)) & 1) ? (GUI_LIGHTGRAY) : (GUI_WHITE);
      LCD_SetColor(Color);
      GUI_FillRect(xPos + 1, yPos + 1, xPos + CellSize, yPos + CellSize);
      Cell        = _GetStone(&_Board, x, y);
      IsValidMove = (_ShowPossibleMoves) ? _IsValidMove(&_Board, x, y) : 0;
      if (_pPlayerAI[_Board.ActPlayer - 1]) {
        IsValidMove = 0;
      }
      if (Cell || IsValidMove) {
        xCircle = (xPos + 1) * AA_CALCFACTOR + ((CellSize * AA_CALCFACTOR) >> 1);
        yCircle = (yPos + 1) * AA_CALCFACTOR + ((CellSize * AA_CALCFACTOR) >> 1);
        if (Cell) {
          Color = (Cell == 1) ? (GUI_RED) : (GUI_BLUE);
          LCD_SetColor(Color);
          #if (AA_FACTOR > 1)
            GUI_AA_FillCircle(xCircle, yCircle, rStone);
          #else
            GUI_FillCircle(xCircle, yCircle, rStone);
          #endif
        } else {
          LCD_SetColor(GUI_BLACK);
          #if (AA_FACTOR > 1)
            GUI_AA_FillCircle(xCircle, yCircle, rMove);
          #else
            GUI_FillCircle(xCircle, yCircle, rMove);
          #endif
        }
      }
      LCD_SetColor(GRID_COLOR);
      GUI_DrawVLine(xPos, yPos + 1, yPos + CellSize);
      xPos += _CellSize;
    }
    GUI_DrawVLine(xPos, yPos + 1, yPos + CellSize);
    GUI_DrawHLine(yPos, _BoardX0, _BoardX0 + (_CellSize << 3));
    yPos += _CellSize;
  }
  GUI_DrawHLine(yPos, _BoardX0, _BoardX0 + (_CellSize << 3));
}
/*******************************************************************
*
*       _cbReversiWin
*/
static void _cbReversiWin(WM_MESSAGE* pMsg) {
  WM_HWIN hWin = pMsg->hWin;
  
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint(hWin);
    break;
  case WM_TOUCH:
    _OnTouch(pMsg);
    break;
#if (GUI_SUPPORT_MOUSE & GUI_SUPPORT_CURSOR)
  case WM_MOUSEOVER:
    _OnMouseOver(pMsg);
    break;
#endif
  case WM_DELETE:
    _hFrame = 0;
    WM_NotifyParent(WM_GetParent(pMsg->hWin), 0x500);
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/*********************************************************************
*
*       _StartReversi
*/
static void _StartReversi(WM_HWIN hWin) {
  
  int xSize, ySize;
  
  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  
  _hFrame = FRAMEWIN_CreateEx(0, 0, xSize, ySize-24, hWin, WM_CF_SHOW, FRAMEWIN_CF_ACTIVE, 0, NULL, &_cbReversiWin);
  
  FRAMEWIN_SetClientColor(_hFrame, GUI_INVALID_COLOR);
  FRAMEWIN_SetFont       (_hFrame, GUI_FONT_13B_1);
  FRAMEWIN_SetTextAlign  (_hFrame, GUI_TA_HCENTER);
  FRAMEWIN_AddCloseButton(_hFrame, FRAMEWIN_BUTTON_RIGHT,  0);

  _StartNewGame();
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       MainTask
*/
void DEMO_Game(WM_HWIN hWin) 
{
  _pPlayerAI[1] = _PlayerAI_SmartGecko; 
  _StartReversi(hWin);
 
}

/*************************** End of file ****************************/
