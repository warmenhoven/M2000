#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro_native_dialog.h>
#include "Allegro.h"

#define FILE_EXIT_ID 1
#define FILE_INSERT_CASSETTE_ID 2
#define FILE_INSERTRUN_CASSETTE_ID 3
#define FILE_REMOVE_CASSETTE_ID 4
#define FILE_INSERT_CARTRIDGE_ID 5
#define FILE_REMOVE_CARTRIDGE_ID 6
#define FILE_RESET_ID 7
#define FILE_SAVE_SCREENSHOT_ID 8
#define FILE_LOAD_VIDEORAM_ID 9
#define FILE_SAVE_VIDEORAM_ID 10
#define FILE_SAVE_STATE_ID 11
#define FILE_LOAD_STATE_ID 12

#define DISPLAY_WINDOW_MENU 20
#define DISPLAY_WINDOW_640x480 21
#define DISPLAY_WINDOW_800x600 22
#define DISPLAY_WINDOW_960x720 23
#define DISPLAY_WINDOW_1280x960 24
#define DISPLAY_WINDOW_1440x1080 25
#define DISPLAY_WINDOW_1600x1200 26
#define DISPLAY_WINDOW_1920x1440 27
#define DISPLAY_SCANLINES 28
#define DISPLAY_FULLSCREEN 29
#define DISPLAY_SMOOTHING 19

#define KEYBOARD_SYMBOLIC_ID 30
#define KEYBOARD_POSITIONAL_ID 31
#define KEYBOARD_ZOEK_ID 32
#define KEYBOARD_START_ID 33
#define KEYBOARD_STOP_ID 34
#define KEYBOARD_CLEARCAS_ID 35

#define OPTIONS_SOUND_ID 40
#define OPTIONS_VOLUME_OFFSET 200
#define OPTIONS_VOLUME_HIGH_ID (10 + OPTIONS_VOLUME_OFFSET)
#define OPTIONS_VOLUME_MEDIUM_ID (4 + OPTIONS_VOLUME_OFFSET)
#define OPTIONS_VOLUME_LOW_ID (1 + OPTIONS_VOLUME_OFFSET)
#define OPTIONS_JOYSTICK_ID 44
#define OPTIONS_JOYSTICK_MAP_0_ID 45
#define OPTIONS_JOYSTICK_MAP_1_ID 46
#define OPTIONS_JOYSTICK_MAP 47

#define SPEED_SYNC 50
#define SPEED_PAUSE 51
#define FPS_50_ID 52
#define FPS_60_ID 53
#define SPEED_OFFSET 1000
#define SPEED_500_ID (500 + SPEED_OFFSET)
#define SPEED_200_ID (200 + SPEED_OFFSET)
#define SPEED_120_ID (120 + SPEED_OFFSET)
#define SPEED_100_ID (100 + SPEED_OFFSET)
#define SPEED_50_ID  (50 + SPEED_OFFSET)
#define SPEED_20_ID  (20 + SPEED_OFFSET)
#define SPEED_10_ID  (10 + SPEED_OFFSET)

#define HELP_ABOUT_ID 100

ALLEGRO_MENU *menu = NULL;

void UpdateViewMenu() {
  int i;
  for (i=1; i< sizeof(Displays)/sizeof(*Displays); i++)
    al_set_menu_item_flags(menu, i + DISPLAY_WINDOW_MENU, videomode == i ?  ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
}

void UpdateVolumeMenu () {
  al_set_menu_item_flags(menu, OPTIONS_VOLUME_HIGH_ID, (OPTIONS_VOLUME_HIGH_ID == mastervolume + OPTIONS_VOLUME_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, OPTIONS_VOLUME_MEDIUM_ID, (OPTIONS_VOLUME_MEDIUM_ID == mastervolume + OPTIONS_VOLUME_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, OPTIONS_VOLUME_LOW_ID, (OPTIONS_VOLUME_LOW_ID == mastervolume + OPTIONS_VOLUME_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
}

void UpdateCpuSpeedMenu () {
  al_set_menu_item_flags(menu, SPEED_500_ID, (SPEED_500_ID == CpuSpeed + SPEED_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, SPEED_200_ID, (SPEED_200_ID == CpuSpeed + SPEED_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, SPEED_120_ID, (SPEED_120_ID == CpuSpeed + SPEED_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, SPEED_100_ID, (SPEED_100_ID == CpuSpeed + SPEED_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, SPEED_50_ID, (SPEED_50_ID == CpuSpeed + SPEED_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, SPEED_20_ID, (SPEED_20_ID == CpuSpeed + SPEED_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
  al_set_menu_item_flags(menu, SPEED_10_ID, (SPEED_10_ID == CpuSpeed + SPEED_OFFSET) ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX);
}

void CreateEmulatorMenu() 
{
  ALLEGRO_MENU_INFO menu_info[] = {
    ALLEGRO_START_OF_MENU("File", 0),
      { "Insert Cassette...", FILE_INSERT_CASSETTE_ID, 0, NULL },
      { "Insert, Load and Run Cassette...", FILE_INSERTRUN_CASSETTE_ID, 0, NULL },
      { "Remove Cassette", FILE_REMOVE_CASSETTE_ID, 0, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Insert Cartridge...", FILE_INSERT_CARTRIDGE_ID, 0, NULL },
      { "Remove Cartridge...", FILE_REMOVE_CARTRIDGE_ID, 0, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Reset (F5)", FILE_RESET_ID, 0, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Save State... (F6)", FILE_SAVE_STATE_ID, 0, NULL },
      { "Load State... (Shift-F6)", FILE_LOAD_STATE_ID, 0, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Save Screenshot... (F7)", FILE_SAVE_SCREENSHOT_ID, 0, NULL },
      { "Load Video RAM...", FILE_LOAD_VIDEORAM_ID, 0, NULL },
      { "Save Video RAM... (F8)", FILE_SAVE_VIDEORAM_ID, 0, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Exit (Alt-F4 / Ctrl-Q)", FILE_EXIT_ID, 0, NULL },
      ALLEGRO_END_OF_MENU,

    ALLEGRO_START_OF_MENU("Display", DISPLAY_WINDOW_MENU),
      { "Scanlines On/Off", DISPLAY_SCANLINES, scanlines ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Smoothing On/Off", DISPLAY_SMOOTHING, smoothing ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "640 x 480", DISPLAY_WINDOW_640x480, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      { "800 x 600", DISPLAY_WINDOW_800x600, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      { "960 x 720", DISPLAY_WINDOW_960x720, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      { "1280 x 960", DISPLAY_WINDOW_1280x960, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      { "1440 x 1080", DISPLAY_WINDOW_1440x1080, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      { "1600 x 1200", DISPLAY_WINDOW_1600x1200, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      { "1920 x 1440", DISPLAY_WINDOW_1920x1440, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
#ifndef __linux__
      ALLEGRO_MENU_SEPARATOR,
#endif
#ifdef _WIN32
      { "Full Screen (F11)", DISPLAY_FULLSCREEN, 0, NULL },
#endif
#ifdef __APPLE__
      { "Full Screen (Shift-F11)", DISPLAY_FULLSCREEN, 0, NULL },
#endif
      ALLEGRO_END_OF_MENU,

    ALLEGRO_START_OF_MENU("Speed", 0),
      ALLEGRO_START_OF_MENU("CPU Speed", 0),
        { "500%", SPEED_500_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "200%", SPEED_200_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "120%", SPEED_120_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "100%", SPEED_100_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "50%",  SPEED_50_ID , ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "20%",  SPEED_20_ID , ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "10%",  SPEED_10_ID , ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        ALLEGRO_END_OF_MENU,
      ALLEGRO_MENU_SEPARATOR,
      { "50 Hz", FPS_50_ID, IFreq == 50 ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      { "60 Hz", FPS_60_ID, IFreq == 60 ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Sync On/Off", SPEED_SYNC, Sync ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      ALLEGRO_MENU_SEPARATOR,
      { "Pause Emulation (F9)", SPEED_PAUSE, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      ALLEGRO_END_OF_MENU,

    ALLEGRO_START_OF_MENU("Keyboard", 0),
      {"[ZOEK] - Show Cassette Index in BASIC (F1)", KEYBOARD_ZOEK_ID, 0, NULL },
      {"[START] - Start Loaded Program in BASIC (F2)", KEYBOARD_START_ID, 0, NULL },
      {"[STOP] - Pause/Halt Program in BASIC (Shift-F2)", KEYBOARD_STOP_ID, 0, NULL },
      {"[WIS] - Clear Cassette in BASIC", KEYBOARD_CLEARCAS_ID, 0, NULL },
      ALLEGRO_MENU_SEPARATOR,
      {"Symbolic Key Mapping", KEYBOARD_SYMBOLIC_ID, keyboardmap==1 ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      {"Positional Key Mapping", KEYBOARD_POSITIONAL_ID, keyboardmap==0 ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
      ALLEGRO_END_OF_MENU,

    ALLEGRO_START_OF_MENU("Options", 0),
      {soundmode ? "Sound On/Off (F10)" : "Sound Card Not Detected", OPTIONS_SOUND_ID, soundmode ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_DISABLED, NULL },
      ALLEGRO_START_OF_MENU("Sound Volume", 0),
        { "High", OPTIONS_VOLUME_HIGH_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "Medium", OPTIONS_VOLUME_MEDIUM_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        { "Low", OPTIONS_VOLUME_LOW_ID, ALLEGRO_MENU_ITEM_CHECKBOX, NULL },
        ALLEGRO_END_OF_MENU,
      ALLEGRO_MENU_SEPARATOR,
      {joymode ? "Joystick On/Off" : "Joystick Not Detected", OPTIONS_JOYSTICK_ID, joymode ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_DISABLED, NULL },
      ALLEGRO_START_OF_MENU("Joystick Mapping", OPTIONS_JOYSTICK_MAP),
        { "Emulate Cursorkeys + Spacebar", OPTIONS_JOYSTICK_MAP_0_ID, joymode ? (joymap==0 ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX) : ALLEGRO_MENU_ITEM_DISABLED, NULL },
        { "Fraxxon Mode (Left/Up/Spacebar)", OPTIONS_JOYSTICK_MAP_1_ID, joymode ? (joymap==1 ? ALLEGRO_MENU_ITEM_CHECKED : ALLEGRO_MENU_ITEM_CHECKBOX) : ALLEGRO_MENU_ITEM_DISABLED, NULL },
        ALLEGRO_END_OF_MENU,
      ALLEGRO_END_OF_MENU,

    ALLEGRO_START_OF_MENU("Help", 0),
      {"About M2000", HELP_ABOUT_ID, 0, NULL },
      ALLEGRO_END_OF_MENU,
    ALLEGRO_END_OF_MENU
  };

  menu = al_build_menu(menu_info);
  if (!joymode) al_remove_menu_item(menu, OPTIONS_JOYSTICK_MAP);
  UpdateVolumeMenu();
  UpdateCpuSpeedMenu();
  UpdateViewMenu();
  al_set_display_menu(display, menu);
}