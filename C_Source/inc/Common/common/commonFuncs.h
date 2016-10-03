/*
**  Se7evidas - A GZDoom mod
**  Copyright (C) 2015-???  Chronos Ouroboros
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program; if not, write to the Free Software Foundation, Inc.,
**  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef COMMONFUNCS_H
#define COMMONFUNCS_H

#include <ACS_ZDoom.h>
#include "includes.h"

typedef char * cstr;

// Macros
#define atanA(x) VectorAngle (1.0k, x)
#define atan2A(x, y) VectorAngle (x, y)

#define TicsToSecs(tics) (TICUNITA * tics)

#define SetInventoryForced(name, amount) \
( \
 TakeInventory (name, 0x7FFFFFFF), \
 GiveInventory (name, amount) \
)

#define FormatStr(strIn, format, ...) \
( \
 vsprintf ((char *) strIn, (char const *) format, __VA_ARGS__) \
)

#define ArraySize(array) sizeof (array) / sizeof (*array)

// Externs
__addrdef extern __gbl_arr global_var;
__addrdef extern __mod_arr    map_var;
extern char CorruptionCharList [];

// Prototypes
/* Position */
vec3_k GetActorPositionVec (int tid);
Script_LS vec3_k GetActivatorPointerPos (int pointer);
/* Keys */
int KeyUp         (int key);
int KeyDown       (int key);
int KeyPressed    (int key);
int KeyPressed2   (int buttons, int oldbuttons, int key);
int KeyUpMOD      (int key);
int KeyDownMOD    (int key);
int KeyPressedMOD (int key);
/* Player info */
bool S7_PlayerNumEqualConsolePlayer (int playerNum);
int  GetMaxStamina (struct PlayerData_t *player);
int  GetMaxMana    (struct PlayerData_t *player);
/* Memory */
void *allocAndClear (size_t size);
/* Misc */
int   UnusedTID (int start, int end);
accum GetVelocity ();
bool  SetInventory (string name, int amount);

// ================ Text Manipulation ================
cstr CorruptText (cstr text);
cstr CorruptTextCase (cstr text);

// ================ Math ================
/* Clamping */
int   Clamp      (int x, int min, int max);
accum ClampAccum (accum x, accum min, accum max);
/* Scaling */
int   ScaleValue      (int   x,   int fromMin,   int fromMax,   int toMin,   int toMax);
accum ScaleValueAccum (accum x, accum fromMin, accum fromMax, accum toMin, accum toMax);
/* String to value conversion */
int   StrToInt  (string source);
bool *StrToBool (string source);
/* Trigonometry */
accum Distance2 (accum actor1X, accum actor1Y, accum actor1Z, // Distance between two XYZ coordinates
                 accum actor2X, accum actor2Y, accum actor2Z);
accum Distance2Vec (vec3_k vec1, vec3_k vec2);
vec3_k GetEulerAngles (vec3_k p1, vec3_k p2); // Euler angles as a vec3
/* Misc */
long accum LongFixedSqrt (long accum x);
int Random2 (int x, int y);

// This is just for zandro compat or something
int CustGetUserCVar (int num, string cvar);
accum CustGetUserCVarFixed (int num, string cvar);
string CustGetUserCVarString (int num, string cvar);

#endif