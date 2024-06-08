<!-- LOGO -->
<p align="center">
	<img align="center" src="https://i.imgur.com/LbbSQUq.png" alt="Logo" width="235" height="140">
</p>
<p align="center">Raax-OG-FN, a universal, internal Fortnite cheat.</p>
<p align="center">If you found this useful, join the Discord and star the project!</p>
<p align="center">THIS IS STILL UNDER DEVELOPMENT, LOTS OF THE CODE IS STILL SCUFFED AND WILL BE CHANGED LATER!</p>
<p align="center">
	<a href="https://discord.gg/Usbk5fjZmH">Discord Server</a> |
	<a href="https://github.com/raax7/OG-Fortnite-Cheat/issues">Report an Issue</a>
</p>
<p align="center">
    <img alt="Release" src="https://img.shields.io/github/v/release/raax7/OG-Fortnite-Cheat?color=blue&style=for-the-badge">
    <img alt="Stars" src="https://img.shields.io/github/stars/raax7/OG-Fortnite-Cheat?color=blue&style=for-the-badge">
</p>


<!-- VIDEO CLIP -->
## Video Clip

Clip from me on EZFN:
https://streamable.com/e/75g4ic

Clip from Threxo (disc id 695938625979940915) playing in Era tournament.
https://cdn.discordapp.com/attachments/1238298241834029107/1238986852934090832/EAR_YOURENY.mp4?ex=664147ed&is=663ff66d&hm=79eab5238897479c3214a83f1950a1d90c7d3844fb6e1194c01ed24de8774541&


<!-- TABLE OF CONTENTS -->
## Table of Contents

<ol>
    <li><a href="#about-raax-og-fn">About Raax-OG-FN</a></li>
    <li><a href="#how-it-works">How it works</a></li>
    <li><a href="#developers-read-me">Developers Read Me!</a></li>
    <li><a href="#license">License</a></li>
</ol>


<!-- ABOUT Raax-OG-FN -->
## About Raax-OG-FN

Out the box, Raax-OG-FN comes with some stand out features that make it the perfect base.
- Automatic offset and VFT index updating
- Compile time string encryption ([skCrypter](https://github.com/skadro-official/skCrypter))
- Windows API function hiding ([LazyImporter](https://github.com/JustasMasiulis/lazy_importer))



<!-- HOW IT WORKS -->
## How it works

This is quite a complicated question to answer on this project, as it has grown a lot bigger than I anticipated.
For now, until I finish writing this readme, you should look at the source on your own. Make sure to take a look at Globals.h and make sure everything matches your specific usage.



<!-- ABOUT Raax-OG-FN -->
## Developers Read Me
I know the code base for this project is quite big for a Fortnite cheat, so I've tried to document eveything well in the files, however it still won't be perfect so here are 2 things you NEED to know before you start developing with this source.

### Namespaces
There are 3 main namespaces, SDK (you can guess what that is), Actors, Game and Features. Here is the purpose of each:
- SDK: The SDK
- Actors: Cached info's on various actors, as well as the loops for them (AFortPawn, AFortPickup etc)
- Game: Simple functions for drawing menu and ESP, current game frame, screen dimensions etc
- Features: Now I know alot of people clown for using this the nesting can become annoying, but I think it's important for clarity. If you go to Game/Features you will see it simply contains all things like AFortPawn bone IDs, targetting system, exploits (vehicles, pickups etc etc)

### Wrappers
I take advantage of wrappers and abstraction alot here, so most of the time like with K2_Project for example, you will not need to get the canvas and call it, there is a wrapper simply called Project in the SDK namespace. I advise you to go to Game/SDK/SDK/Engine_classes.h and look at all the wrappers at the bottom to avoid redundunt code

### Overall
Overall, just explore the project. There is already tons of code using everything how I intended so you can simply follow that style



<!-- LICENSE -->
## License

We use the MIT license. For more information, please read the LICENSE file.
