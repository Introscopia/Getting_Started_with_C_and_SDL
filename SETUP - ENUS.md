Quick guide for setting up MinGW and SDL for C development on windows.

# STEP 1 - Download MinGW
https://osdn.net/projects/mingw/
this is the most recent version (as of 2023, since 2017):
https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/
(download will start automatically)

# STEP 2 - Run mingw-get-setup.exe
-click install
-intallation directory should be c:\MinGW
-after "MinGW Installation Manager Setup Tool" is finished, the "MinGW Installation Manager" will show up, where you should click the checkboxes on:
	* mingw32-base-bin
	* mingw32-binutils-bin
	* Everything that says "The GNU C Compiler" on the description column ( mingw32-gcc-bin, ..., mingw32-gcc-man )
	* Everything that has a little star or seems important. Go with your gut feeling. Worst case scenario you run this thing again and add more stuff.
- On the menu at the top, click "Installation", and on the drop-down, "Apply Changes".
-wait for it to be done and close it.

# STEP 3 
- On the windows taskbar search box type "Environment Variables"
- Click the thing that says "Edit the system environment variables". It will open a little window,
- Click the button that says "Environment Variables..."
- on the lower list, "System variables", find a variable called "Path", select it and click "Edit..."
- Now on the "Edit environment variable" window, click "New" and add "C:\MinGW\bin" in there. (without the quotes)
- Done! Now we can compile from the command line. You can try it with the sample project in the 'Compilation Test, no SDL' folder.

# STEP 4 - SDL!
- go to https://github.com/libsdl-org/SDL/releases
- Download the Development Library, for MinGW
( link should look like this: SDL2-devel-2.28.2-mingw.zip )
-now go to https://www.libsdl.org/projects/
and, again, get the MinGW develpment libraries for whatever you think you need:
* SDL_image: https://github.com/libsdl-org/SDL_image/releases (loading/saving images)
* SDL_ttf: https://github.com/libsdl-org/SDL_ttf/releases (rendering text)
* SDL_net: https://github.com/libsdl-org/SDL_net/releases (networking)
etc.
- Now create an SDL folder in C drive. "C:\SDL"
- unzip all of your development libraries in there. Your base SDL directory should look like "C:\SDL\SDL2-2.28.2" (if that's the version you're on.)

YOU'RE DONE. Look in "WORKFLOW" to see what to do next.

