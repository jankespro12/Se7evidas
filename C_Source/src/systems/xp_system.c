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

#include "includes.h"
#include "systems/xp_system.h"

#define XPSYSBASEXPGOAL (2500)

void LevelUp (int level, int attrPoints, bool log, string message) {
    SetFont (s"DBIGFONT");

    if (GetUserCVar (PLN, s"S7_MsgsOn")) {
        SetFont (s"DBIGFONT");
        HudMessage (HUDMSG_FADEINOUT | HUDMSG_LAYER_OVERHUD | HUDMSG_LOG * log, 16000, CR_UNTRANSLATED, 0.5k, 0.5k, 3.0k, 0.3k, 0.3k, 0.0k, "Level up!");
        SetFont (s"SMALLFNT");
        if (message == NULL)
            HudMessage_Str (HUDMSG_FADEINOUT | HUDMSG_LAYER_OVERHUD | HUDMSG_LOG * log, 16001, CR_UNTRANSLATED, 0.5k, 0.55k, 3.0k, 0.3k, 0.3k, 0.0k, s"You've reached level %d.\nYou have gained %d attribute points.", level, attrPoints);
        else
            HudMessage_Str (HUDMSG_FADEINOUT | HUDMSG_LAYER_OVERHUD | HUDMSG_LOG * log, 16001, CR_UNTRANSLATED, 0.5k, 0.55k, 3.0k, 0.3k, 0.3k, 0.0k, message);
    }

    PlaySound (0, s"Player/LevelUp", CHAN_UI);
}

void UpdateXPSysInfo (PlayerData_t *player) {
    player->xpSystem.level       = CheckInventory (XPS_LEVELTOKEN);
    player->xpSystem.experience  = CheckInventory (XPS_EXPTOKEN);
    player->xpSystem.attrPoints  = CheckInventory (XPS_ATTRPOINTSTOKEN);
    player->xpSystem.strengthLVL = CheckInventory (XPS_STRENGTHTOKEN);
    player->xpSystem.agilityLVL  = CheckInventory (XPS_AGILITYTOKEN);
    player->xpSystem.vitalityLVL = CheckInventory (XPS_VITALITYTOKEN);
    player->xpSystem.defenseLVL  = CheckInventory (XPS_DEFENSETOKEN);
    player->xpSystem.willLVL     = CheckInventory (XPS_WILLTOKEN);
    player->xpSystem.magicLVL    = CheckInventory (XPS_MAGICTOKEN);
    player->xpSystem.techLVL     = CheckInventory (XPS_TECHTOKEN);
}

void XPSys_UpdateLevel (PlayerData_t *player) {
    UpdateXPSysInfo (player);

    bool logMessages = GetUserCVar (PLN, s"S7_LogLVLUpMsgs");
    int  reqXP = XPSYSBASEXPGOAL * (1.0k + 2k * player->xpSystem.level);
    int  nextLevel = player->xpSystem.level + 1;
    int  attrPoints = 4;
    int  xp = player->xpSystem.experience;

    if (xp >= reqXP && player->xpSystem.level < ServerData.maxLevel) {
        switch (nextLevel) {
            default:
                LevelUp (nextLevel, attrPoints, logMessages, NULL);
                SetInventory (XPS_LEVELTOKEN, nextLevel);
                GiveInventory (XPS_ATTRPOINTSTOKEN, attrPoints);
                TakeInventory (XPS_EXPTOKEN, reqXP);
                break;
        }
    }

    UpdateXPSysInfo (player);
}

void XPSys_EnforceStats (PlayerData_t *player) {
    int baseHealth;
    int playerClass = GetPlayerInfo (PLN, PLAYERINFO_PLAYERCLASS);

    switch (playerClass) {
        case PlayerClass_Scout:
            baseHealth = 75;
            break;

        default:
            baseHealth = 100;
            break;
    }

    // Agility
    SetAmmoCapacity (STAMINATOKEN, GetMaxStamina (player));
    // Vitality
    SetActorProperty (0, APROP_SpawnHealth, (int) (baseHealth * (1.0k + 0.06k * player->xpSystem.vitalityLVL)));
    // Defense
    SetActorPropertyFixed (0, APROP_DamageFactor, 1.0k - 0.01k * player->xpSystem.defenseLVL);
    // Magic
    SetAmmoCapacity (MANATOKEN, GetMaxMana (player));
}

void UpdateXPSystem (PlayerData_t *player) {
    if (!player)
        return;

    XPSys_UpdateLevel  (player); // Level stuff
    XPSys_EnforceStats (player); // Stats stuff
}