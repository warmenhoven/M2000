# M2000 : Philips P2000 home computer emulator
Version 0.6
                                     
## Supported platforms

* MS-DOS
* Unix/X
* Windows - using the Allegro 5 libraries

## Files included in release packages
```
m2000          The emulator
M2000.txt      Readme file
Default.fnt    SAA5050 font
P2000ROM.bin   P2000 ROM image
BASIC.bin      BASIC cartridge ROM image (v. 1.1NL)
CWSDMI.ZIP     (MS-DOS version only) A DPMI server required by M2000 MS-DOS
```

## What's emulated

-  P2000T or P2000M model (P2000M emulation is buggy)
-  Support for 1 ROM cartridge
-  User-definable amount of RAM
-  One tape drive
-  Sound
-  SAA5050 character rounding emulated in high resolution mode

## Key Mappings

![keyboard mappings](/img/toetsenbord.png)

```
Cursor Keys, Alt/Ctrl -  Movement
Delete                -  <
Shift-Delete          -  >
` ~                   -  CODE
```

## Special Keys
```
F4           -  Toggle tracing on/off (Debugging version only)
F5           -  Toggle sound on/off
F11          -  Decrease sound volume
F12          -  Increase sound volume
F6           -  Change options
F7           -  Make screen shot (Not implemented in the Unix/X version)
F8           -  Pause & Blank screen
F9           -  Pause
ESC/F10      -  Quit emulator
```

## Command line options
```
-trap <address>        Trap execution when PC reaches specified address [-1]
                       (Debugging version only)
-help                  Print a help page describing all available command
                       line options
-verbose <level>       Select debugging messages [1]
                       0 - Silent           1 - Startup messages
                       4 - Tape
-ifreq <frequency>     Select interrupt frequency [50 Hz]
-cpuspeed <speed>      Set Z80 CPU speed [100%]
-sync <value>          Sync/Do not sync emulation [1]
                       0 - Do not sync   1 - Sync
                       Emulation is faster if sync is turned off
-ram <value>           Select amount of RAM installed [32KB]
-uperiod <period>      Number of interrupts per screen update [1]
                       Try -uperiod 2 or -uperiod 3 if emulation is a bit
                       slow
-t / -m                Select P2000 model [-t]
-video <mode>          Select video mode/window size [0]
                       0 - 500x300 (Unix/X)
                           320x240 (Linux/SVGALib, T-model emulation only)
                           256x240 (MS-DOS, T-model emulation only)
                       1 - 520x490 (Unix/X)
                           640x480 (Linux/SVGALib and MS-DOS)
-printer <filename>    Select file for printer output
                       Default is PRN for the MS-DOS version, stdout for
                       the Unix versions
-printertype <value>   Select printer type [0]
                       0 - Daisy wheel   1 - Matrix
-romfile <file>        Select P2000 ROM dump file [P2000ROM.bin]
-tape <filename>       Select tape image to use [P2000.cas]
-boot <value>          Allow/Don't allow BASIC to boot from tape [0]
                       0 - Don't allow booting
                       1 - Allow booting
-font <filename>       Select font to use [Default.fnt]
-sound <mode>          Select sound mode [255]
                       0 - No sound
                       1 - PC Speaker (MS-DOS) or /dev/dsp (Unix)
                       2 - SoundBlaster (MS-DOS)
                       255 - Detect
-volume <value>        Select initial volume
                       0 - Silent   15 - Maximum
-joystick <mode>       Select joystick mode [1]
                       0 - No joystick support
                       1 - Joystick support
                       When joystick support is on, moving the joystick
                       emulates cursorkey presses, pressing a joystick
                       button emulates pressing the spacebar
-shm <mode>            Use/Do not use MIT SHM extensions for X [1] (Unix/X
                       version only)
                       0 - Don't use SHM   1 - Use SHM
```

## Configuration files

The emulator loads two configuration files (if present) before it loads a cartridge ROM: 
* M2000.cfg located in the emulator's directory and
* CART.cfg (i.e. BASIC.cfg by default) located in the cartridge dump's directory.
  
These are plain text files containing optional command line options. \
Options can be separated with spaces, tabs or returns. \
Please note that for the Unix versions, the configuration files should be present in the current working directory.

## How to compile the sources yourself

TODO

## P2000 documentation and software

TODO


## History
```
0.6     Fixed several bugs in the Z80 emulation engine, fixed several
        compatibility problems, added high resolution and character
        rounding emulation support
0.5     Completely rewrote the Z80 emulation engine, fixed various minor
        bugs, fixed a major bug in the SoundBlaster detection routine
0.4.1   Fixed a major bug that caused bad compiling on high-endian
        machines
0.4     Fixed some minor bugs, added P2000M emulation, added Linux/SVGALib
        and Unix/X ports, speeded up screen refresh drivers (again)
0.3     Major speed increase in screen refresh drivers, added options
        dialogue
0.2     Major sound emulation improvements, fixed some bugs in video
        emulation, added -ram and -volume command line options
0.1     Initial release
```

## Credits

- Marcel de Kogel for creating the M2000 emulator, as found on his [M2000 distribution site](https://www.komkon.org/~dekogel/m2000.html)
- Stafano Bodrato (@zx70) for creating the Allegro version for M2000
- Hans Bus provided lots of technical information on the P2000
- Marat Fayzullin provided invaluable help improving the Unix/X version
- M2000 MS-DOS was compiled using DJ Delorie's DJGPP v2.0. DJGPP is a 32
  bit C compiler for MS-DOS. Source code and binaries of DJGPP are
  available at http://www.delorie.com

## License

Unknown, but probably GNU GPLv3, as most of the original source files contain the following header:
```
Copyright (C) Marcel de Kogel 1996,1997
You are not allowed to distribute this software commercially
Please, notify me, if you make any changes to this file
```
