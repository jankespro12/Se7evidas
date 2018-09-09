/*
 *  Se7evidas - A GZDoom mod
 *  Copyright (C) 2018 Chronos "phantombeta" Ouroboros
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

//-------------------------------------------------------------------------------------------
//
// Green Armor spawner
//
//-------------------------------------------------------------------------------------------
class S7_SecVestSpawner : RandomSpawner {
    default {
        DropItem "S7_CCSecVest",   256, 2;
        //DropItem "S7_SBTKSecVest", 256, 1;
    }
}
class S7_GreenArmorSpawner : RandomSpawner replaces GreenArmor {
    default {
        DropItem "S7_Nothing",        256, 2;
        DropItem "S7_SecVestSpawner", 256, 1;
    }
}

//-------------------------------------------------------------------------------------------
//
// Blue Armor spawner
//
//-------------------------------------------------------------------------------------------
class S7_CombatArmorSpawner : RandomSpawner {
    default {
        DropItem "S7_CCCombatArmor",   256, 2;
        //DropItem "S7_SBTKCombatArmor", 256, 1;
    }
}
class S7_BlueArmorSpawner : RandomSpawner replaces BlueArmor {
    default {
        DropItem "S7_Nothing",            256, 2;
        DropItem "S7_SecVestSpawner",     256, 2;
        DropItem "S7_CombatArmorSpawner", 256, 1;
    }
}