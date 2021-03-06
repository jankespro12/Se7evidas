/*
 *  Se7evidas - A GZDoom mod
 *  Copyright (C) 2018-2019 Chronos "phantombeta" Ouroboros
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

extend class S7_BasePlayer {
    static const string PermanentItems [] = {
        "S7_GrenadeWeap:S7_GrenadeCount",
        "S7_BerserkWeap:S7_Berserk",
        "S7_Thumper_PUnified",
        "S7_UnarmedWeap"
    };

    void Misc_PermanentItems () {
        if (player.cheats & CF_Predicting)
            return;

        for (int i = 0; i < PermanentItems.Size (); i++) {
            Array<String> parts; parts.Clear ();
            PermanentItems [i].Split (parts, ":");

            // If this happens someone's a fucking idiot.
            if (parts.Size () < 1 || parts.Size () > 2)
                continue;

            if ((parts.Size () == 1 || CheckInventory (parts [1], 1)) && !CheckInventory (parts [0], 1))
                GiveInventory (parts [0], 1);
        }
    }

    void Misc_StaminaRegen () {
        if (player.cheats & CF_Predicting)
            return;

        int curStamina = CountInv ('S7_Stamina');
        int maxStamina = GetMaxStamina ();
        bool regenBlocked = (sprint_Sprinting || teching_Active);

        if (health <= 0 || curStamina == maxStamina || regenBlocked) {
            playerData.staminaTics = 0;
            return;
        }

        bool berserkActive = CheckInventory ('S7_BerserkToken', 1);
        int count = 1, interval = 1;

        if (berserkActive)
            count = (!playerData.dying) ? 4 : 2;
        if (playerData.dying)
            interval = berserkActive ? 2 : 3;

        if (playerData.staminaTics >= interval) {
            GiveInventory ('S7_Stamina', count);
            playerData.staminaTics = 0;
        }

        if (curStamina != maxStamina)
            playerData.staminaTics++;
    }

    void Misc_BerserkSideEffects () {
        if (player.cheats & CF_Predicting)
            return;
        if (health <= 0 || bKilled)
            return;

        int bsrkCount = CountInv ('S7_BerserkToken');

        if (!playerData.bsrkAddiction)
            playerData.bsrkMaxAddiction = 0;
        else
            playerData.bsrkMaxAddiction = max (playerData.bsrkMaxAddiction, playerData.bsrkAddiction);

        if (bsrkCount > 0 || !playerData.bsrkAddiction)
            playerData.bsrkWithdrawalTics = 0;
        else if (!bsrkCount && playerData.bsrkAddiction > 0) {
            playerData.bsrkWithdrawalTics++;

            double shiverTime = floor ((35. * 5.) * clamp (1. - (.2 * (playerData.bsrkMaxAddiction - 1.)), 0., 1.));
            if (playerData.bsrkWithdrawalTics >= shiverTime) {
                double shiverMod = max (playerData.bsrkWithdrawalTics / max (shiverTime * .35, 5), 1.);

                A_SetAngle (angle + clamp (fRandom [berserkEffects] (-.01, .01) * shiverMod, -.5, .5));
                A_SetPitch (pitch + clamp (fRandom [berserkEffects] (-.01, .01) * shiverMod, -.5, .5));
            }

            double baseDetoxTime = ((playerData.bsrkMaxAddiction < 2) ? (35. * 20.) : (35. * 40.)) * clamp (1. - (.01 * (xpSys.GetStat (S7Stat_Will) / 3.)), .45, 1.);
            double detoxTime = floor (baseDetoxTime * (1. + .2 * (playerData.bsrkMaxAddiction - 1.)));
            if (playerData.bsrkWithdrawalTics >= detoxTime) {
                double additionalDetoxTime = detoxTime * clamp (1. - (.035 * (xpSys.GetStat (S7Stat_Will) / 3.)), .35, 1.);
                bool doDetox = ((playerData.bsrkWithdrawalTics % max (additionalDetoxTime, 5)) == 0);

                if (doDetox)
                    playerData.bsrkAddiction--;
            }
        }
    }

    transient CVar heartbeatsSoundCVar;
    void Misc_Heartbeats () {
        double strengthFactor = 0.;
        if (shaderData)
            strengthFactor = shaderData.lowHealthStrengthFactor * .95;

        if (health > 0) {
            if (!heartbeatsSoundCVar)
                heartbeatsSoundCVar = CVar.GetCVar ("S7_HeartbeatsOn", players [consoleplayer]);
            if (heartbeatsSoundCVar && heartbeatsSoundCVar.GetInt ()) {
                // I couldn't find a better way to do this... >.>
                if ((health <= 25 && health > 15 && playerData.heartbeatTics >= 35 * 1.50) ||
                    (health <= 15 && health > 10 && playerData.heartbeatTics >= 35 * 1.25) ||
                    (health <= 10 && health > 5  && playerData.heartbeatTics >= 35       ) ||
                    (health <= 5  && health > 2  && playerData.heartbeatTics >= 35 *  .75) ||
                    (health <= 2  &&                playerData.heartbeatTics >= 35 *  .50)
                ) {
                    A_StartSound ("Player/Heartbeat", slot: CHAN_Body, flags: CHANF_Local, volume: .85, attenuation: ATTN_None);
                    playerData.heartbeatTics = 0;
                    strengthFactor = 4.;
                } else if (health > 25) { // If health is greater than 25...
                    playerData.heartbeatTics = 0;
                    strengthFactor = 0;
                } else // If neither...
                    playerData.heartbeatTics++; // Add one to heartbeatTics
            }
        }

        if (shaderData) {
            shaderData.lowHealthOn = true;
            shaderData.lowHealthStrengthFactor = strengthFactor;
        }
    }

    void Misc_HellhunterBlade () {
        if (player.cheats & CF_Predicting)
            return;

        int curLvl = CountInv ('S7_HellhunterBladeAffinity');
        int curExp = CountInv ('S7_HellhunterBladeExperience');
        int nxtLvl = 2500 + curLvl * 1250;
        if (curLvl < 10 && curExp >= nxtLvl) {
            GiveInventory ('S7_HellhunterBladeAffinity', 1);
            TakeInventory ('S7_HellhunterBladeExperience', nxtLvl);
        }
    }
}