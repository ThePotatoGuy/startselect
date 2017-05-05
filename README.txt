
START SELECT README
@author Andre Allan Ponce
@email andreponce@null.net

TO RUN (without controller input):

1:  Ensure SDL2.dll and SDL2_gfx.dll are in the same directory as 
    startselect.exe
2:  Run startselect.exe
3:  Press the H key while the main window is in focus to open the help menu.

TO RUN (with controller input):

1:  Install an Xinput API wrapper/driver for PS3 controllers. I recommend using
    SCP toolkit (https://github.com/nefarius/ScpToolkit)
2:  Connect a Sony Dualshock PS3 controller to the computer and let SCPtoolkit
    (or the wrapper of your choice) configure the driver.
3:  Ensure SDL2.dll and SDL2_gfx.dll are in the same directory as 
    startselect.exe
4:  Run startselect.exe
5:  Press the H key while the main window is in focus to open the help menu.

COMMANDS:
H       - opens the HELP menu
SPACE   - Starts/Stops recording input (The window title will have (RECORDING)
        when input is being recorded)
G       - Generate the image
C       - open color chooser dialog box
I       - open save file dialog box to save the image (recording must be
        stopped)
S       - open save file dialog box to save stats (recording must be stopped)
L       - open open file dialog box to load stats (recording must be stopped)
R       - clear stats data (a confirmation dialog box will appear)

TO BUILD:

1:  Install MingW64 I recommended using MSYS2 to setup the build environment. 
    (http://www.msys2.org)
2:  Install SDL2. This can be done through MSYS2
3:  Add "SDL2_gfx/include" to your include path and "SDL2_gfx/lib" to your 
    library path. You MUST use this version as the current official version 
    has a rendering bug.
4:  Ensure SDL2.dll and SDL2_gfx.dll are in the build directory.
5:  Run make


