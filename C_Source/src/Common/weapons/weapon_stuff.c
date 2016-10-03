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
#include "weapons/weap_data.h"
#include "weapons/weapon_stuff.h"

const string DummyWeapons [] = {
    s"S7_NullWeapon",
    SPRINTWEAPON,
    QUICKMELEEWEAPON,
};

int GetWeaponName () {
    for (int x = 0; x < ArraySize (DummyWeapons); x++)
        if (CheckWeapon (DummyWeapons [x]))
            return -1;

    for (int y = 0; y < WeaponNames_Length; y++)
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
        if (weaponNumber < 0 || weaponNumber > WeaponNames_Length - 1)
            return;
        SetWeapon (WeaponNames [weaponNumber]);
    } else {
        weaponNumber = GetWeaponName ();
        if (weaponNumber < 0 || weaponNumber > WeaponNames_Length - 1)
            return;
        player->scriptData.lastWeapon = weaponNumber;
    }
}

void DisableWeapon (string meh, string blah, PlayerData_t *player) {
    if (!player) {
        Log ("\CgFunction DisableWeapon: Fatal error: Invalid or NULL player struct");
        return;
    }

    if (CheckWeapon (meh)) {
        TakeInventory (blah, 0x7FFFFFFF);
        ChangeLastWeapon (1, player);
        return;
    }
    GiveInventory (meh, 1);
    SetWeapon (meh);
    ChangeLastWeapon (0, player);
}

// Scripts
// SynthFire stuff
Script_C void S7_SynthFire (int mode) {
    while (TRUE) {
        if (!PlayerInGame (PLN))
            return;

        if (!CheckInventory (s"S7_SynthFireActive"))
            return;

        if (!mode) {
            if (KeyDownMOD (BT_ATTACK))
                SetInventory (s"S7_SynthFireLeft", 1);

            if (KeyDownMOD (BT_ALTATTACK))
                SetInventory (s"S7_SynthFireRight", 1);
        } else {
            if (KeyPressedMOD (BT_ATTACK))
                SetInventory (s"S7_SynthFireLeft", 1);

            if (KeyPressedMOD (BT_ALTATTACK))
                SetInventory (s"S7_SynthFireRight", 1);
        }

        Delay (1);

        if (!mode) {
            if (KeyUpMOD (BT_ATTACK))
                SetInventory (s"S7_SynthFireLeft", 0);

            if (KeyUpMOD (BT_ALTATTACK))
                SetInventory (s"S7_SynthFireRight", 0);
        } else {
            if (!KeyPressedMOD (BT_ATTACK))
                SetInventory (s"S7_SynthFireLeft", 0);

            if (!KeyPressedMOD (BT_ALTATTACK))
                SetInventory (s"S7_SynthFireRight", 0);
        }
    }
}

Script_C int S7_SynthFireAllowChange () {
    if (!CheckInventory (s"S7_SynthFireRightReloading") || CheckInventory (s"S7_SynthFireLeftReloading"))
        return 1;
    else
        return 0;
}

/*
Script_C void S7_RecoilPitch (accum offset) { // Called like this in code: TNT1 A 0 ACS_NamedExecuteAlways ("S7_RecoilPitch", 0, 0.5 * 65536)
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

void HellbladeScript (PlayerData_t *player) {
    if (!player)
        return;

    int curLvl = CheckInventory (s"S7_HellwarriorBladeAffinity");
    int curExp = CheckInventory (s"S7_HellwarriorBladeExperience");
    if (curLvl < 10 && curExp >= 2500 + curLvl * 1250) {
        GiveInventory (s"S7_HellwarriorBladeAffinity", 1);
        TakeInventory (s"S7_HellwarriorBladeExperience", 0x7FFFFFFF);
    }
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

Script_C int S7_MeleeDamage (int baseDamage, int mul) {
    accum baseMul  = 1.0k;
    accum mulBonus = 0.0k;

    if (CheckInventory (BERSERKTOKEN)) {
        baseMul = 3.0k;
        mulBonus = RandomFixed (0.0k, 2.0k);
    }

    return RoundA ((baseDamage * baseMul + (0.5k * CheckInventory (XPS_STRENGTHTOKEN))) * (mul + mulBonus));
}

/*Script_C void S7_BerserkWeapToggle () {
    if (!PlayerInGame (PLN))
        return;
    
    DisableWeapon (s"S7_BerserkWeap", s"", &PlayerData [PLN]);
}*/

enum {
    HELLBLADE_FORM1 = 1,
    HELLBLADE_NULL = 32767,
};

Script_C int S7_HellBladeDMG (int form, int multiplier, int baseDMG) {
    int affinity = CheckInventory (s"S7_HellwarriorBladeAffinity");

    switch (form) {
        case HELLBLADE_FORM1: {
            int modDMG = (multiplier + RandomFixed (0.0k, 2.0k)) * baseDMG + (0.5k * CheckInventory (XPS_STRENGTHTOKEN)) * (CheckInventory (BERSERKTOKEN) ? 3.0k : 1.0k);
            return RoundA (modDMG + (affinity / 10 * (modDMG / 2)));
        }
        break;
    }
}