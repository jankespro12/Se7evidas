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

#include "includes.h"
#include "weap_data.h"
#include "weapon_stuff.h"

const string DummyWeapons [] = {
    s"S7_NullWeapon",
    SPRINTWEAPON,
    QUICKMELEEWEAPON
};

int GetWeaponName () {
    for (int x = 0; x < ArraySize (DummyWeapons); x++)
        if (CheckWeapon (DummyWeapons [x])) 
            return -1;
    
    for (int y = 0; y < ArraySize (WeaponNames); y++)
        if (CheckWeapon (WeaponNames [y]))
            return y;
    
    return -1;
}

void ChangeLastWeapon (bool mode, PlayerData_t *player) {
    if (!player) {
        Log ("\CgFunction ChangeLastWeapon: Fatal error: Invalid or NULL player struct");
        return;
    }
    
    int weaponNumber = 0;
    if (mode) {
        weaponNumber = player->scriptData.lastWeapon;
        if (weaponNumber < 0 || weaponNumber > ArraySize (WeaponNames) - 1)
            return;
        SetWeapon (WeaponNames [weaponNumber]);
    } else {
        weaponNumber = GetWeaponName ();
        if (weaponNumber < 0 || weaponNumber > ArraySize (WeaponNames) - 1)
            return;
        else
            player->scriptData.lastWeapon = weaponNumber;
    }
}

void DisableWeapon (string meh, string blah, PlayerData_t *player) {
    if (!player) {
        Log ("\CgFunction DisableWeapon: Fatal error: Invalid or NULL player struct");
        return;
    }
    
    if (CheckWeapon (meh)) {
        TakeInventory (blah, 99999);
        ChangeLastWeapon (1, player);
        return;
    }
    GiveInventory (meh, 1);
    SetWeapon (meh);
    ChangeLastWeapon (0, player);
}

// Scripts
// SynthFire stuff
Script_C void S7_SynthFire () {
    while (TRUE) {
        if (!CheckInventory (s"S7_SynthFireActive"))
            return;
        
        if (KeyDown (BT_ATTACK) && !CheckInventory (s"S7_SynthFireLeft"))
            GiveInventory (s"S7_SynthFireLeft", 1);
        
        if (KeyDown (BT_ALTATTACK) && !CheckInventory (s"S7_SynthFireRight"))
            GiveInventory (s"S7_SynthFireRight", 1);
        
        Delay (1);
        
        if (KeyUp (BT_ATTACK) && CheckInventory (s"S7_SynthFireLeft"))
            TakeInventory (s"S7_SynthFireLeft", 1);
        
        if (KeyUp (BT_ALTATTACK) && CheckInventory (s"S7_SynthFireRight"))
            TakeInventory (s"S7_SynthFireRight", 1);
    }
}

Script_C int S7_SynthFireAllowChange () {
    if (!CheckInventory (s"S7_SynthFireRightReloading") || CheckInventory (s"S7_SynthFireLeftReloading"))
        return 1;
    else
        return 0;
}

/*
Script_C void S7_RecoilPitch (accum offset) { // Called like this in code: TNT1 A 0 ACS_NamedExecuteAlways ("S7_RecoilPitch", 0, 0.5 * 65535)
    accum oldPitch = GetActorPitch (0);
    accum scaledOffset = ScaleValueAccum (offset, -90.0k, 90.0k, -0.25k, 0.25k);
    accum newPitch = ClampAccum (oldPitch - scaledOffset, -0.25k, 0.25k);
    
    SetActorPitch (0, newPitch);
}
*/

void AmmoCountersScript (PlayerData_t *player) {
    if (!player)
        return;
    
    if (CheckInventory (s"S7_ShotgunMagCounter") != (CheckInventory (s"S7_ShotgunMag") + CheckInventory (s"S7_ShotgunLoaded")))
        SetInventory (s"S7_ShotgunMagCounter", CheckInventory (s"S7_ShotgunMag") + CheckInventory (s"S7_ShotgunLoaded"));
}

/*#define PSICGFIREBOOL s"S7_PSICG_CanFire"
Script_C void S7_PSICG_FireTest () {
    accum puffX, puffY, puffZ;
    accum shooterX, shooterY, shooterZ;
    accum diff;

    puffX = GetActorX (0);
    puffY = GetActorY (0);
    puffZ = GetActorZ (0);

    SetActivator (0, AAPTR_TARGET);

    shooterX = GetActorX (0);
    shooterY = GetActorY (0);
    shooterZ = GetActorZ (0);

    diff = Distance2 (puffX, puffY, puffZ,
                      shooterX, shooterY, shooterZ);

    if (abs (diff) > 192)
        GiveInventory (PSICGFIREBOOL, 1);
    else
        TakeInventory (PSICGFIREBOOL, 0x7FFFFFFF);
}*/

Script_C void S7_SLanceBeamGrab () {
    accum x1 = GetActorX (0), y1 = GetActorY (0), z1 = GetActorZ (0);
    x1 = GetActorX (0), y1 = GetActorY (0), z1 = GetActorZ (0);
    accum x2, y2, z2, xDiff, yDiff, zDiff, angel, bitch;
    int flickerDelay = Random (8, 35 * 2);
    bool justFlickered = FALSE;

    SetActivatorToTarget (0);

    while (TRUE) {
        if (!CheckInventory (s"S7_SoulLance_BeamGrabbed"))
            break;

        if (flickerDelay < 1) {
            GiveInventory (DISABLEHUDTOKEN, 1);
            flickerDelay = Random (8, 35 * 2);
            justFlickered = TRUE;
        } else {
            flickerDelay--;
        }

        x2 = GetActorX (0); y2 = GetActorY (0); z2 = GetActorZ (0);
        xDiff = x1 - x2; yDiff = y1 - y2; zDiff = z1 - z2;
        angel = VectorAngle (xDiff, yDiff);
        bitch = VectorAngle (angel, zDiff);
        bitch = ScaleValueAccum (bitch, -1.0k, 1.0k, -0.25k, 0.25k);
        if (bitch > 32768) bitch = 65536 - bitch;

        SetActorAngle (0, angel);
        SetActorPitch (0, -bitch);
        PrintBold ("%k", -bitch);

        Delay (1);

        SetActorAngle (0, angel);
        SetActorPitch (0, -bitch);

        if (justFlickered) {
            TakeInventory (DISABLEHUDTOKEN, 0x7FFFFFFF);
            justFlickered = FALSE;
        }
    }
}