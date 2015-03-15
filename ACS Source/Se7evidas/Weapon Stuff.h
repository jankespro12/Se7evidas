#DEFINE S7_WEAPONMAX 4
str S7_WeaponName [S7_WEAPONMAX] = { "S7_Raptor", "S7_Shotgun", "S7_TEC9", "S7_Revolver" };
#DEFINE S7_DUMMYWEAPON_COUNT 3
str S7_DummyWeapons [S7_DUMMYWEAPON_COUNT] = { "S7_NullWeapon", "S7_SprintWeapon", "S7_QuickMelee" };
global int 2:S7_LastWeapon [];

function int getWeaponName (void) {
    for (int i = 0; i < S7_WEAPONMAX; i++) {
        if (checkWeapon (S7_DummyWeapons [i])) 
            return -1;
        
        if (checkWeapon (S7_WeaponName [i]))
            return i;
    }
    
    return -1;
}

function void lastWeapon (int mode) {
    int weaponNumber;
    if (mode != 0) {
        weaponNumber = S7_LastWeapon [playerNumber ()];
        setWeapon (S7_WeaponName [weaponNumber]);
    } else {
        weaponNumber = getWeaponName ();
        if (weaponNumber == -1)
            return;
        else
            S7_LastWeapon [playerNumber ()] = weaponNumber;
    }
}

function void disableWeapon (str meh, str blah) {
    if (meh == 0)
        meh = S7_DummyWeapons [0];
    
    if (checkWeapon (meh)) {
        if (blah != FALSE)
            takeInventory (blah, 99999);
        lastWeapon (1);
        return;
    }
    
    giveInventory (meh, 1);
    setWeapon (meh);
    lastWeapon (0);
}

// Scripts
// SynthFire stuff
#DEFINE S7_SynthFire 910
script S7_SynthFire (void) {
    while (TRUE) {
        if (!checkInventory ("SynthFireActive"))
            terminate;
        
        if (keyDown (BT_ATTACK) && !checkInventory ("SynthFireLeft"))
            giveInventory ("SynthFireLeft", 1);
        
        if (keyDown (BT_ALTATTACK) && !checkInventory ("SynthFireRight"))
            giveInventory ("SynthFireRight", 1);
        
        delay (1);
        
        if (!keyDown (BT_ATTACK) && checkInventory ("SynthFireLeft"))
            takeInventory ("SynthFireLeft", 1);
        
        if (!keyDown (BT_ALTATTACK) && checkInventory ("SynthFireRight"))
            takeInventory ("SynthFireRight", 1);
    }
}

#DEFINE S7_SynthFireAllowChange 911
script S7_SynthFireAllowChange (void) {
    if (!checkInventory ("SynthFireRightReloading") || checkInventory ("SynthFireLeftReloading"))
        setResultValue (1);
    else
        setResultValue (0);
}

#DEFINE S7_QuickMelee 919
script S7_QuickMelee (void) {
    disableWeapon ("S7_QuickMelee", 100000);
}