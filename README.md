releasing soon...



# OG-Fortnite-Cheat
OG Fortnite cheat that works on all major OG Fortnite versions

# Info
This is the first fully open-source, auto-offset updating OG-FN cheat that works for all builds s0-s15!<br>
A star would be appreciated as this is a big thing to release for free.<br>
This cheat works very similarly to 1hack. (fuck 1hack)<br>

# Injection Methods
### Self-Injection method
1.

### Nova's awful anti-cheat bypass
1. Open Nova and hold alt-tab
2. When the admin request appears, ignore for now
3. When in the lobby, use any injector
4. After injecting, press YES on the admin request from Nova's anti-cheat
<br>
(dont use an injector like processhacker, that will get auto detected and force you to close your game by fortnite. any free github loadlibrary injector will work tho, just nothing fortnite itself detects by window name)

# How It Works?
The idea behind this is to use the way Unreal-Engine is built to create a cheat that is fully functional for all builds, automatically updatitng everything and using just 1 signature and a couple static offsets
### The flow of initalization
1. At the point of injection, the cheat uses one of the GObjects signatures to find the TUObjectArray. From here we loop through the objects untill we find the UKismetSystemLibrary then
