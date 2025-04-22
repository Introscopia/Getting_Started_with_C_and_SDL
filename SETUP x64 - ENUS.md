# Quick Guide to Install MinGW and SDL for C Development on Windows

# STEP 1 - Install the Compiler:  
**Mingw-w64**: https://www.mingw-w64.org/  
This is our compiler.  
- Create a folder named `W64` in your `C:` drive.  
- Go to https://github.com/skeeto/w64devkit/releases  
- Download the latest version, x64, without the `.sig` file. Currently, it is: `w64devkit-x64-2.1.0.exe`
- Open your `C:\W64\` folder.  
- If the download is an `.exe` file (like the current v2.1.0), run it. It will extract the devkit. Place it here in the `W64` folder.  

# STEP 2 - Install SDL  
- Create a folder named "SDL" directly in your `C:` drive, `C:\SDL`.  
  When downloading the following files, save them in this folder.  
- Open the link: https://github.com/libsdl-org/SDL/releases  
- Download the latest version, the development package ("devel") for MinGW.  
  (It should look something like this: `SDL3-devel-3.2.10-mingw.tar.gz`)  
Then do the same for the modules you plan to use, for example:  
* **SDL_image**: https://github.com/libsdl-org/SDL_image/releases (load and save images)  
* **SDL_ttf**: https://github.com/libsdl-org/SDL_ttf/releases (render text from TTF fonts)  
* **SDL_net**: https://github.com/libsdl-org/SDL_net/releases (networking)  

- Extract the contents of all these compressed files into your `SDL` folder.  
**NOTE!** These `.tar.gz` files have two 'layers' of compression!  
If you just select "Extract All" or "Extract Here," you will still have a compressed file!  
The folder we want in `C:\SDL` at the end should look like `\SDL3-3.2.10\`.  

# STEP 3 - Test  
- Download the repository https://github.com/Introscopia/Getting_Started_with_C_and_SDL  
  by clicking the green `<> Code` button and selecting the `Download ZIP` option.  
- Extract the `.zip` file and navigate to the extracted folder.  
- Go to the "Pong 64" folder: `...\Getting_Started_with_C_and_SDL\Pong 64`  
- Right-click on `#MAKE.bat` and select "Edit."  
- On the line that says `echo cd "C:\path\to\Pong 64"`,  
  you need to change `"path\to"` to the actual address of your `Pong 64` folder.  
  - Tip: In the file explorer, you can click on the address bar (outside the directory names) to select the full path, then copy it with `CTRL+C`.  
- Save and close `#MAKE.bat`, then double-click it to compile!  
- A black terminal window will appear.  
  Once it's done working, the `Pong.exe` file will appear in the folder.
- This Pong project is not only an example, but can also be used as a template for your future projects!