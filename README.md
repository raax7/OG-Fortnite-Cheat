<!-- LOGO -->
<p align="center">
	<img align="center" src="https://i.imgur.com/LbbSQUq.png" alt="Logo" width="235" height="140">
</p>
<p align="center">Raax-OG-FN, a universal, internal Fortnite cheat.</p>
<p align="center">If you found this useful, join the Discord and star the project!</p>
<p align="center">
	<a href="https://discord.gg/Hg5dTFP7jy">Discord Server</a> |
	<a href="https://github.com/raax7/OG-Fortnite-Cheat/issues">Report an Issue</a>
</p>
<p align="center">
    <img alt="Release" src="https://img.shields.io/github/v/release/raax7/OG-Fortnite-Cheat?color=blue&style=for-the-badge">
    <img alt="Stars" src="https://img.shields.io/github/stars/raax7/OG-Fortnite-Cheat?color=blue&style=for-the-badge">
</p>


<!-- ABOUT somethingidk-FN -->
## DEVELOPERS READ ME FIRST!
I know the code base for this project is very big, so I've tried to document eveything well in the files,
however it still won't be perfect so here are 2 things you NEED to know before you start developing with this source.

### Namespaces
There are 3 main namespaces, SDK (you can guess what that is), Actors, Game and Features. Here is the purpose of each:
- SDK: The SDK
- Actors: Cached info's on various actors, as well as the loops for them (AFortPawn, AFortPickup etc)
- Game: Simple functions for drawing menu and ESP, current game frame, screen dimensions etc
- Features: Now I know alot of people clown for using this the nesting can become annoying, but I think it's important for clarity. If you go to Game/Features you will see it simply contains all things like AFortPawn bone IDs, targetting system, exploits (vehicles, pickups etc etc)

### Wrappers
I take advantage of wrappers and abstraction alot here, so most of the time like with K2_Project for example, you will not need to get the canvas and call it, there is a wrapper simply called Project in the SDK namespace. I advise you to go to Game/SDK/SDK/Engine_classes.h and look at all the wrappers at the bottom to avoid redundunt code

##
Overall, just explore the project. There is already tons of code using everything how I intended so you can simply follow that style
##


<!-- TABLE OF CONTENTS -->
## Table of Contents

<ol>
    <li><a href="#about-somethingidk-fn">About Raax-OG-FN</a></li>
    <li><a href="#how-it-works">How it works</a></li>
    <li><a href="#instructions">Instructions</a></li>
    <li><a href="#license">License</a></li>
</ol>



<!-- ABOUT somethingidk-FN -->
## About somethingidk-FN

Out the box, somethingidk-FN comes with some stand out features that make it the perfect base.
- Automatic Offset and VTable updating
- Return address spoofing (don't know who made it)
- Compile time string encryption ([skCrypter](https://github.com/skadro-official/skCrypter))
- Windows API function hiding ([LazyImporter](https://github.com/JustasMasiulis/lazy_importer))



<!-- HOW IT WORKS -->
## How it works

Depending on the build mode different things are enabled and disabled by default. Release is the safest to use with the least potential traces out of the box.

#### At Compilation (release)
- Release mode is stripped of all debug info
- A post-build powershell script is run to clear any remaining path strings

#### On Injection
1. A thread is made using CreateThread (may be a detection vector depending on project), as to not crash by the process being suspended for too long
2. Beep is called so the user knows the DLL was successfully injected
3. GObjects is initalized using one of 2 signatures
4. FName::AppendString...



<!-- INSTRUCTIONS -->
## Instructions



<!-- LICENSE -->
## License

Do literally whatever you want. (aside from reuploading the source and claiming it as your own)