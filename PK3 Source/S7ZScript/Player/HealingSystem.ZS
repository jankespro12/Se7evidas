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

class S7_HealingSystem : Inventory {
    const baseRegenAmount = 1.0;
    const baseRegenTime = 3;

    int regenPoints;
    protected int regenTimer;

    override void AttachToOwner (Actor other) {
        Super.AttachToOwner (other);

        regenPoints = 0;
    }

    virtual double GetBaseRegenTime (S7_BasePlayer pPawn) {
        if (!pPawn || !pPawn.xpSys)
            return baseRegenTime;

        double vitality = pPawn.xpSys.GetStat (S7Stat_Vitality);
        double logVit = (vitality >= 1) ? log (pPawn.xpSys.GetStat (S7Stat_Vitality) / 10.) : 0;

        return double (baseRegenTime) * clamp (1. - logVit, .25, 1.);
    }


    virtual double GetBaseRegenAmount (S7_BasePlayer pPawn) {
        if (!pPawn || !pPawn.xpSys)
            return baseRegenTime;

        double vitality = pPawn.xpSys.GetStat (S7Stat_Vitality);
        double logVit = (vitality >= 1) ? log (pPawn.xpSys.GetStat (S7Stat_Vitality) / 10.) : 0;

        return double (baseRegenAmount) * max (1. + logVit, 1.);
    }

    override void DoEffect () {
        if (!Owner || !(Owner is 'S7_BasePlayer'))
            return;

        let pPawn = S7_BasePlayer (Owner);
        if (!pPawn || !pPawn.xpSys)
            return;

        if (health <= 0 || pPawn.bKilled) {
            regenTimer = 0;
            regenPoints = 0;
            return;
        }

        if (regenTimer > 0 && regenPoints > 0)
            regenTimer--;

        if (regenTimer == 0 && regenPoints > 0) {
            double regenRateAmount, regenRateTime;

            [regenRateTime, regenRateAmount] = pPawn.xpSys.GetStatMod (S7Stat_Special, specialName: "regenRate");
            regenRateAmount += GetBaseRegenAmount (pPawn);

            int giveCount = int (floor (clamp (regenRateAmount, 1, regenPoints)));

            pPawn.GiveBody (giveCount);
            regenPoints -= giveCount;

            regenTimer = int (GetBaseRegenTime (pPawn) * regenRateTime);
        } else if (regenPoints < 0) {
            Console.PrintF ("S7 Error: What the hell? S7_HealingSystem.regenPoints became negative!");
            regenPoints = 0;
            regenTimer = 0;
        } else if (regenPoints == 0 && regenTimer > 0)
            regenTimer = 0;
    }
}