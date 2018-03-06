# Nachenblaster
UCLA CS32 Project 3
Space Invaders inspired videogame

Instructions:
Use the arrows keys to navigate
Use space to shoot cabbages
Use tab to shoot torpedoes

How to play:
Clone Repository

macOS: You'll need to have XQuartz and freeGLUT installed to run the program.

Install XQuartz:
Click the download link, open the downloaded .dmg file, double-click on XQuartz.pkg, and follow the installation instructions.
Log out and log back in again.
To verify the installation, open a Terminal window and run the command echo $DISPLAY. That should produce one line of output that ends with org.macosforge.xquartz:0.

Install freeGLUT:
Install the homebrew package manager.
Open a Terminal window and run the command brew install freeglut.

Notes for Xcode users:

You might get a message at startup relating to the Assets directory. If you want to fix the issue just for this project, select File / Project Settings... and for Derived Data Location, select Project-relative Location. If instead you want to fix it for every project you create with Xcode, select Xcode / Preferences... and for the Derived Data, select Relative.
If your version of Xcode is too old, you'll get a message about the xcodeproj file being an incompatible project version.
