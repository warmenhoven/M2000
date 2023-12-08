/******************************************************************************/
/*                             M2000 - the Philips                            */
/*                ||||||||||||||||||||||||||||||||||||||||||||                */
/*                ████████|████████|████████|████████|████████                */
/*                ███||███|███||███|███||███|███||███|███||███                */
/*                ███||███||||||███|███||███|███||███|███||███                */
/*                ████████|||||███||███||███|███||███|███||███                */
/*                ███|||||||||███|||███||███|███||███|███||███                */
/*                ███|||||||███|||||███||███|███||███|███||███                */
/*                ███||||||████████|████████|████████|████████                */
/*                ||||||||||||||||||||||||||||||||||||||||||||                */
/*                                  emulator                                  */
/*                                                                            */
/*   Copyright (C) 2013-2023 by the M2000 team.                               */
/*                                                                            */
/*   See the file "LICENSE" for information on usage and redistribution of    */
/*   this file, and for a DISCLAIMER OF ALL WARRANTIES.                       */
/******************************************************************************/

#include <stdio.h>
#include <string.h>

typedef struct {
  const int key;
  const char* value;
} LanguageEntry;

#define FILE_EXIT_ID                      1
#define FILE_INSERT_CASSETTE_ID           2
#define FILE_INSERTRUN_CASSETTE_ID        3
#define FILE_REMOVE_CASSETTE_ID           4
#define FILE_INSERT_CARTRIDGE_ID          5
#define FILE_REMOVE_CARTRIDGE_ID          6
#define FILE_RESET_ID                     7
#define FILE_SAVE_SCREENSHOT_ID           8
#define FILE_LOAD_VIDEORAM_ID             9
#define FILE_SAVE_VIDEORAM_ID             10
#define FILE_SAVE_STATE_ID                11
#define FILE_LOAD_STATE_ID                12
#define DISPLAY_WINDOW_MENU               20
#define DISPLAY_WINDOW_640x480            21
#define DISPLAY_WINDOW_800x600            22
#define DISPLAY_WINDOW_960x720            23
#define DISPLAY_WINDOW_1280x960           24
#define DISPLAY_WINDOW_1440x1080          25
#define DISPLAY_WINDOW_1600x1200          26
#define DISPLAY_WINDOW_1920x1440          27
#define DISPLAY_SCANLINES                 28
#define DISPLAY_FULLSCREEN                29
#define DISPLAY_FULLSCREEN_APPLE          107
#define DISPLAY_SMOOTHING                 19
#define KEYBOARD_SYMBOLIC_ID              30
#define KEYBOARD_POSITIONAL_ID            31
#define KEYBOARD_ZOEK_ID                  32
#define KEYBOARD_START_ID                 33
#define KEYBOARD_STOP_ID                  34
#define KEYBOARD_CLEARCAS_ID              35
#define OPTIONS_SOUND_ID                  40
#define OPTIONS_SOUND_NOT_DETECTED_ID     100
#define OPTIONS_VOLUME_MENU_ID            101
#define OPTIONS_VOLUME_HIGH_ID            41
#define OPTIONS_VOLUME_MEDIUM_ID          42
#define OPTIONS_VOLUME_LOW_ID             43
#define OPTIONS_JOYSTICK_ID               44
#define OPTIONS_JOYSTICK_NOT_DETECTED_ID  102
#define OPTIONS_JOYSTICK_MAP_0_ID         45
#define OPTIONS_JOYSTICK_MAP_1_ID         46
#define OPTIONS_JOYSTICK_MAP              47
#define OPTIONS_SAVE_PREFERENCES          48
#define OPTIONS_ENGLISH_ID                38
#define OPTIONS_NEDERLANDS_ID             39
#define SPEED_SYNC                        50
#define SPEED_PAUSE                       51
#define FPS_50_ID                         52
#define FPS_60_ID                         53
#define SPEED_10_ID                       54
#define SPEED_20_ID                       55
#define SPEED_50_ID                       56
#define SPEED_100_ID                      57
#define SPEED_120_ID                      58
#define SPEED_200_ID                      59
#define SPEED_500_ID                      60
#define HELP_ABOUT_ID                     61
#define FILE_MENU_ID                      62
#define HELP_MENU_ID                      63
#define HELP_ABOUT_MSG_ID                 64
#define OPTIONS_MENU_ID                   65
#define OPTIONS_LANGUAGE_MENU_ID          103
#define KEYBOARD_MENU_ID                  104
#define SPEED_MENU_ID                     105
#define SPEED_CPU_MENU_ID                 106

#define DIALOG_LOAD_CASSETTE              108
#define DIALOG_LOAD_CARTRIDGE             109
#define DIALOG_SAVE_SCREENSHOT            110
#define DIALOG_LOAD_VRAM                  111
#define DIALOG_SAVE_VRAM                  112
#define DIALOG_LOAD_STATE                 113
#define DIALOG_SAVE_STATE                 114


static LanguageEntry ENstrings[] = {
  { FILE_MENU_ID, "File->" },
  { FILE_INSERT_CASSETTE_ID, "Insert Cassette..." },
  { FILE_INSERTRUN_CASSETTE_ID, "Insert and Boot Cassette..." },
  { FILE_REMOVE_CASSETTE_ID, "Remove Cassette" },
  { FILE_INSERT_CARTRIDGE_ID, "Insert Cartridge..." },
  { FILE_REMOVE_CARTRIDGE_ID, "Remove Cartridge..." },
  { FILE_SAVE_STATE_ID, "Save State... (F6)" },
  { FILE_LOAD_STATE_ID, "Load State... (Shift-F6)" },
  { FILE_SAVE_SCREENSHOT_ID, "Save Screenshot... (F7)" },
  { FILE_LOAD_VIDEORAM_ID, "Load Video RAM..." },
  { FILE_SAVE_VIDEORAM_ID, "Save Video RAM... (F8)" },
  { FILE_EXIT_ID, "Exit (Alt-F4 / Ctrl-Q)" },
  { DISPLAY_WINDOW_MENU, "Display->" },
  { DISPLAY_SCANLINES, "Scanlines On/Off" },
  { DISPLAY_SMOOTHING, "Smoothing On/Off" },
  { DISPLAY_FULLSCREEN, "Full Screen (F11)" },
  { DISPLAY_FULLSCREEN_APPLE, "Full Screen (Shift-F11)" },
  { SPEED_MENU_ID, "Speed->" },
  { SPEED_CPU_MENU_ID, "CPU Speed->" },
  { SPEED_SYNC, "Sync On/Off" },
  { SPEED_PAUSE, "Pause Emulation (F9)" },
  { KEYBOARD_MENU_ID, "Keyboard->" },
  { KEYBOARD_ZOEK_ID, "[ZOEK] - Show Cassette Index (F1)" },
  { KEYBOARD_START_ID, "[START] - Start Loaded Program (F2)" },
  { KEYBOARD_STOP_ID, "[STOP] - Pause/Halt Program (Shift-F2)" },
  { KEYBOARD_CLEARCAS_ID, "[WIS] - Clear Cassette" },
  { KEYBOARD_SYMBOLIC_ID, "Symbolic Key Mapping" },
  { KEYBOARD_POSITIONAL_ID, "Positional Key Mapping" },
  { OPTIONS_MENU_ID, "Options->"},
  { OPTIONS_SOUND_ID, "Sound On/Off (F10)" },
  { OPTIONS_SOUND_NOT_DETECTED_ID, "Sound Card Not Detected" },
  { OPTIONS_VOLUME_MENU_ID, "Sound Volume->" },
  { OPTIONS_VOLUME_HIGH_ID, "High" },
  { OPTIONS_VOLUME_MEDIUM_ID, "Medium" },
  { OPTIONS_VOLUME_LOW_ID, "Low" },
  { OPTIONS_JOYSTICK_ID, "Joystick On/Off" },
  { OPTIONS_JOYSTICK_NOT_DETECTED_ID, "Joystick Not Detected" },
  { OPTIONS_JOYSTICK_MAP, "Joystick Mapping->" },
  { OPTIONS_JOYSTICK_MAP_0_ID, "Emulate Cursorkeys + Spacebar" },
  { OPTIONS_JOYSTICK_MAP_1_ID, "Fraxxon Mode (Left/Up/Spacebar)" },
  //{ OPTIONS_LANGUAGE_MENU_ID, "UI Language->" },
  { OPTIONS_SAVE_PREFERENCES, "Save Preferences" },
  { HELP_MENU_ID, "Info->" },
  { HELP_ABOUT_ID, "About M2000" },
  { HELP_ABOUT_MSG_ID, "Thanks to Marcel de Kogel for creating the core of this emulator back in 1996." },
  { DIALOG_LOAD_CASSETTE, "Select a .cas cassette file" },
  { DIALOG_LOAD_CARTRIDGE, "Select a .bin cartridge file" },
  { DIALOG_SAVE_SCREENSHOT, "Save as .png or .bmp file" },
  { DIALOG_LOAD_VRAM, "Select a .vram file" },
  { DIALOG_SAVE_VRAM, "Save as .vram file" },
  { DIALOG_LOAD_STATE, "Select a .dmp file" },
  { DIALOG_SAVE_STATE, "Save as .dmp file" },
  { 0, NULL },
};

static LanguageEntry NLstrings[] = {
  { FILE_MENU_ID, "Bestand->" },
  { FILE_INSERT_CASSETTE_ID, "Invoeren cassette..."},
  { FILE_INSERTRUN_CASSETTE_ID, "Invoeren en starten cassette..." },
  { FILE_REMOVE_CASSETTE_ID, "Verwijder cassette" },
  { FILE_INSERT_CARTRIDGE_ID, "Invoeren cartridge..." },
  { FILE_REMOVE_CARTRIDGE_ID, "Verwijder cartridge..." },
  { FILE_SAVE_STATE_ID, "Opslaan huidige toestand... (F6)" },
  { FILE_LOAD_STATE_ID, "Inlezen vorige toestand... (Shift-F6)" },
  { FILE_SAVE_SCREENSHOT_ID, "Opslaan schermafdruk... (F7)" },
  { FILE_LOAD_VIDEORAM_ID, "Inlezen Video RAM..." },
  { FILE_SAVE_VIDEORAM_ID, "Opslaan Video RAM... (F8)" },
  { FILE_EXIT_ID, "Afsluiten (Alt-F4 / Ctrl-Q)" },
  { DISPLAY_WINDOW_MENU, "Beeld->" },
  { DISPLAY_SCANLINES, "Scanlijnen aan/uit" },
  { DISPLAY_SMOOTHING, "Vloeiende letters aan/uit" },
  { DISPLAY_FULLSCREEN, "Volledig scherm (F11)" },
  { DISPLAY_FULLSCREEN_APPLE, "Volledig scherm (Shift-F11)" },
  { SPEED_MENU_ID, "Snelheid->" },
  { SPEED_CPU_MENU_ID, "CPU snelheid->" },
  { SPEED_SYNC, "Synchronisatie aan/uit" },
  { SPEED_PAUSE, "Pauzeer emulatie (F9)" },
  { KEYBOARD_MENU_ID, "Toetsenbord->" },
  { KEYBOARD_ZOEK_ID, "[ZOEK] - Toon cassette inhoud (F1)" },
  { KEYBOARD_START_ID, "[START] - Start ingeladen programma (F2)" },
  { KEYBOARD_STOP_ID, "[STOP] - Pauzeer/stop programma (Shift-F2)" },
  { KEYBOARD_CLEARCAS_ID, "[WIS] - Wis cassette" },
  { KEYBOARD_SYMBOLIC_ID, "Toetsindeling o.b.v. karakter" },
  { KEYBOARD_POSITIONAL_ID, "Toetsindeling o.b.v. positie" },
  { OPTIONS_MENU_ID, "Opties->" },
  { OPTIONS_SOUND_ID, "Geluid aan/uit (F10)" },
  { OPTIONS_SOUND_NOT_DETECTED_ID, "Geluidskaart niet gedetecteerd" },
  { OPTIONS_VOLUME_MENU_ID, "Geluidsvolume->" },
  { OPTIONS_VOLUME_HIGH_ID, "Hoog" },
  { OPTIONS_VOLUME_MEDIUM_ID, "Gemiddeld" },
  { OPTIONS_VOLUME_LOW_ID, "Laag" },
  { OPTIONS_JOYSTICK_ID, "Joystick aan/uit" },
  { OPTIONS_JOYSTICK_NOT_DETECTED_ID, "Joystick niet gedetecteerd" },
  { OPTIONS_JOYSTICK_MAP, "Joystick configuratie->" },
  { OPTIONS_JOYSTICK_MAP_0_ID, "Emuleer cursortoetsen + spatiebalk" },
  { OPTIONS_JOYSTICK_MAP_1_ID, "Fraxxon mode (links/omhoog/spatiebalk)" },
  //{ OPTIONS_LANGUAGE_MENU_ID, "Voorkeurstaal->" },
  { OPTIONS_SAVE_PREFERENCES, "Opslaan van voorkeuren" },
  { HELP_MENU_ID, "Info->" },
  { HELP_ABOUT_ID, "Over M2000" },
  { HELP_ABOUT_MSG_ID, "Met dank aan Marcel de Kogel voor het creëren van de basis voor deze emulator in 1996." },
  { DIALOG_LOAD_CASSETTE, "Selecteer een .cas cassette bestand" },
  { DIALOG_LOAD_CARTRIDGE, "Selecteer een .bin cartridge bestand" },
  { DIALOG_SAVE_SCREENSHOT, "Bewaar als .png of .bmp bestand" },
  { DIALOG_LOAD_VRAM, "Selecteer een .vram bestand" },
  { DIALOG_SAVE_VRAM, "Bewaar als .vram bestand" },
  { DIALOG_LOAD_STATE, "Selecteer een .dmp bestand" },
  { DIALOG_SAVE_STATE, "Bewaar als .dmp bestand" },
  { 0, NULL },
};

const char* _(int key) {
  LanguageEntry *UIstrings = uilanguage == 1 ? NLstrings : ENstrings;
  int i=0;
  for (i=0; UIstrings[i].key; i++) {
    if (key == UIstrings[i].key) 
      return UIstrings[i].value;
  }
  return "[KEY NOT FOUND]";
}
