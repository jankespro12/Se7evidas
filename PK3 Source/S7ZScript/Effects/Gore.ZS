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

//-------------------------------------------------------------------------------------------
//
// Gore actors
//
//-------------------------------------------------------------------------------------------
class S7_GoreBase : S7_EffectsBase {
    default {
        Mass 1;
        Radius 3;
        Height 4;

        +DONTGIB +NOTELEPORT +NOBLOCKMAP +THRUSPECIES
        +CORPSE
    }

    states {
    Raise: // Juuuuust to make sure...
        stop;
    }
}

class S7_GibbingBlood : S7_Flying_Blood {
    int loopCount;

    states {
    Spawn:
        TNT1 A 0 noDelay {
            bMissile = true;
            loopCount = random [S7_GoreRNG] (3, 5);
        }
    Flight:
        TNT1 A 0 {
            if ((loopCount--) <= 0)
                return ResolveState ("Null");

            return ResolveState (null);
        }
        BLD1 CCCBBBAAA 1 A_SpawnItemEx ('S7_Flying_Blood_Trail', 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, S7_BloodSXFFlags);
        loop;

    Death:
        TNT1 A 1;
        stop;
    }
}

class S7_GoreBloodCloud : S7_GoreBase {
    default {
        RenderStyle "translucent";
        Scale 0.75;
        Alpha 0.65;

        +NOINTERACTION +THRUACTORS +NOGRAVITY
    }

    states {
    Spawn:
        BLD3 A 2;
        BLD3 A 1 A_FadeOut (0.025);
        wait;
    }
}

class S7_Gibs_Blood_Trail : S7_Flying_Blood_Trail {
    default {
        RenderStyle "translucent";
        Scale 0.5;
        Alpha 0.5;
    }
}
class S7_GenericGoreBits : S7_GoreBase {
    default {
        Scale 0.5;
        BounceType "Hexen";
        BounceFactor 0.4;
        BounceCount 7;
        BounceSound "Gore/Bounce";

        +MOVEWITHSECTOR +THRUACTORS +ROLLSPRITE +ROLLCENTER
        +USEBOUNCESTATE
        -SOLID
    }

    transient CVar stayTimeCVar, fadeSpeedCVar;
    int timer, rollDir, trailSpawnDuration;
    Vector3 oldCoords;

    states {
    Spawn:
        TNT1 A 0 noDelay {
            static const StateLabel stateNames [] = { "Frame1", "Frame2", "Frame3", "Frame4", "Frame5" };

            bMissile = true;
            bKilled = false;
            A_SetRoll (random [S7_GoreRNG] (0, 360));
            A_SetScale (abs (scale.x) * randomPick [S7_GoreRNG] (-1, 1), abs (scale.y) * randomPick [S7_GoreRNG] (-1, 1));

            rollDir = random [S7_GoreRNG] (0, 1);
            trailSpawnDuration = 25;
            oldCoords = pos;

            return ResolveState (stateNames [random [S7_GoreRNG] (0, stateNames.Size () - 1)]);
        }

    Frame1: BLD3 B 0; goto Flight;
    Frame2: BLD3 C 0; goto Flight;
    Frame3: BLD3 D 0; goto Flight;
    Frame4: BLD3 E 0 A_SetScale (scale.x * 1.5, scale.y * 1.5); goto Flight;
    Frame5: BLD3 F 0 A_SetScale (scale.x * 1.8, scale.y * 1.8); goto Flight;

    Flight:
        #### # 1 {
            if (!bNoBlood && (trailSpawnDuration--) > 0) {
                bool MUDA; Actor trail;
                [MUDA, trail] = A_SpawnItemEx ('S7_Gibs_Blood_Trail', 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, S7_BloodSXFFlags);
                trail.SetOrigin (oldCoords, false);
            }

            if (rollDir != -1 && vel.Length () > 0.5) {
                int rollAdd = random [S7_GoreRNG] (10, 20);
                A_SetRoll (roll + (rollDir ? rollAdd : -rollAdd));
            }

            oldCoords = pos;

            if (bKilled && pos.z <= floorZ)
                return ResolveState ("Death");

            return ResolveState (null);
        }
        wait;

    Bounce.Floor:
        #### # 0 { rollDir = -1; }
        goto Flight;

    Bounce.Wall:
        #### # 0 {
            if (rollDir != -1)
                rollDir = !!rollDir;
        }
        goto Flight;

    Death:
        #### # 0 {
            if (pos.z > floorZ) {
                bKilled = true;
                bNoBounceSound = true;
                return ResolveState ("Flight");
            }

            bUseBounceState = false;
            bNoInteraction = true;

            return ResolveState (null);
        }
    Waiting:
        #### # 1 {
            vel = (0, 0, 0);
            A_Stop ();

            if (!stayTimeCVar)
                stayTimeCVar = CVar.GetCVar ("S7_GibsStayTime", players [consolePlayer]);

            int stayTime = stayTimeCVar.GetInt ();

            if (stayTime > -1) {
                if (timer >= stayTime)
                    return ResolveState ("Fade");

                timer++;
            }

            return ResolveState (null);
        }
        wait;
    Fade:
        #### # 1 {
            if (!fadeSpeedCVar)
                fadeSpeedCVar = CVar.GetCVar ("S7_GibsFadeSpeed", players [consolePlayer]);

            A_FadeOut (fadeSpeedCVar.GetFloat ());
        }
        wait;
    }
}