/*
**  Se7evidas - A GZDoom mod
**  Copyright (C) 2015  Chronos Ouroboros
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

#ifndef SAVE_H
#define SAVE_H

#include <ACS_ZDoom.h>

// Defines
#define SAVESYS_SAVEVERSION 1

#define SD_INFO       s"S7_SaveData_Info"
#define SD_RPGSYSTEM  s"S7_SaveData_RPGSystem"
#define SD_SCRIPTDATA s"S7_SaveData_ScriptData"
#define SD_THUMPERDEF s"S7_SaveData_ThumperDef"
#define SD_IMPINV     s"S7_SaveData_ImportantInv"
#define SD_INV        s"S7_SaveData_Inv"

// Prototypes
bool        LoadSaveDataToPointer (int playerNum, SavedData_t *data);
SavedData_t LoadSaveData          (int playerNum);
bool        SaveSaveData          (int playerNum, SavedData_t *data);
int         SaveSys_ReadInt       (string s, int *offset, int length);
string      SaveSys_ReadStr       (string s, int *offset, int length);

#endif