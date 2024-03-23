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
/*   Copyright (C) 1996-2023 by Marcel de Kogel and the M2000 team.           */
/*                                                                            */
/*   See the file "LICENSE" for information on usage and redistribution of    */
/*   this file, and for a DISCLAIMER OF ALL WARRANTIES.                       */
/******************************************************************************/

// This file contains the system-independent part of the screen refresh drivers

#include <stdio.h>
#include <string.h>

#define SPACE 32 // space character
#define M2000_VERSION "0.9-SNAPSHOT"

void PutChar(int x, int y, int c, int fg, int bg, int si);
void PutImage(void);

// when doblank is 1, flashing characters are not displayed this refresh
static int doblank=1;

/****************************************************************************/
/*** Refresh screen (P2000T model)                                        ***/
/****************************************************************************/
void RefreshScreen_T(void)
{
  byte *S;
  int fg, bg, si, gr, fl, cg, FG, BG, conceal;
  int hg, hg_active, hg_c, hg_fg, hg_cg, hg_conceal;
  int x, y;
  int c;
  int lastcolor;
  int eor;
  int found_si;

  S = VRAM + ScrollReg;
  found_si = 0; // init to no double height codes found

  for (y = 0; y < 24; ++y)
  {
    /* Initial values:
       foreground=7 (white)
       background=0 (black)
       normal height
       graphics off
       flashing off
       contiguous graphics
       hold graphics off
       reveal display */
    fg = 7;
    bg = 0;
    si = 0;
    gr = 0;
    fl = 0;
    cg = 1;
    hg = 0;
    conceal = 0;
    hg_active = hg; // init the HG mode settings
    hg_c = SPACE;
    hg_cg = cg;
    hg_fg = fg;
    hg_conceal = conceal;
    lastcolor = fg;
    for (x = 0; x < 40; ++x)
    {
      /* Get character */
      c = S[x] & 0x7f;
      /* If bit 7 is set, invert the colours */
      eor = S[x] & 0x80;
      if (!(c & 0x60))
      {
        /* Control code found. Parse it */
        switch (c & 0x1f)
        {
        /* New text colour () */
        case 0x01: // red
        case 0x02: // green
        case 0x03: // yellow
        case 0x04: // blue
        case 0x05: // magenta
        case 0x06: // cyan
        case 0x07: // white
          fg = lastcolor = c & 0x0f;
          gr = conceal = hg = 0;
          break;
        /* New graphics colour */
        case 0x11: // red
        case 0x12: // green
        case 0x13: // yellow
        case 0x14: // blue
        case 0x15: // magenta
        case 0x16: // cyan
        case 0x17: // white
          fg = lastcolor = c & 0x0f;
          gr = 1;
          conceal = 0;
          break;
        /* Flash */
        case 0x08:
          fl = 1;
          break;
        /* Steady */
        case 0x09:
          fl = 0;
          break;
        /* End box (?) */
        case 0x0a:
          break;
        /* Start box (?) */
        case 0x0b:
          break;
        /* Normal height */
        case 0x0c:
          if (si)
            hg = 0;
          si = 0;
          break;
        /* Double height */
        case 0x0d:
          if (!si)
            hg = 0;
          si = 1;
          if (!found_si)
            found_si = 1;
          break;
        /* reserved for compatability reasons; these are still graphic mode */
        case 0x00:
        case 0x0e:
        case 0x0f:
        case 0x10:
        case 0x1b:
          break;
        /* conceal display */
        case 0x18:
          conceal = 1;
          break;
        /* contiguous graphics */
        case 0x19:
          cg = 1;
          break;
        /* separated graphics */
        case 0x1a:
          cg = 0;
          break;
        /* black background */
        case 0x1c:
          bg = 0;
          break;
        /* new background */
        case 0x1d:
          bg = lastcolor;
          break;
        /* hold graphics */
        case 0x1e:
          if (!hg)
          {
            hg = 1;
            if (gr) 
            {
              hg_active = 1;
              hg_fg = fg;
            }
          }
          break;
        /* release graphics */
        case 0x1f:
          hg = 0;
          break;
        }
        c = SPACE; // control chars are displayed as space by default
      }
      else 
      {
        hg_c = c;
        hg_cg = cg; // hold display of seperated/contiguous mode
        hg_conceal = conceal;
      }

      if (hg_active)
        c = hg_c;

      /* Check for flashing characters and concealed display */
      if ((fl && doblank) || (hg_active ? hg_conceal : conceal))
        c = SPACE;

      /* Check if graphics are on */
      if ((gr || hg_active) && (c & 0x20)) // c from 32..63
      {
        c += (c & 0x40) ? 64 : 96;
        if (!(hg_active ? hg_cg : cg))
          c += 64;
      }
      /* If double height code on previous line and double height
         is not set, display a space character */
      if (found_si == 2 && !si)
        c = SPACE;

      /* Get the foreground and background colours */
      FG = (hg_active ? hg_fg : fg);
      BG = bg;
      if (eor)
      {
        FG = FG ^ 7;
        BG = BG ^ 7;
      }
      /* Put the character in the screen buffer */
      PutChar(x, y, c - 32, FG, BG, (si ? found_si : 0));

      // update HG mode
      hg_active = (hg && gr);
      if (gr)
      {
        hg_fg = fg;
        hg_conceal = conceal;
      }
    }

    /* Update the double height state
       If there was a double height code on this line, do not
       update the character pointer. If there was one on the
       previous line, add two lines to the character pointer */
    if (found_si)
    {
      if (++found_si == 3)
      {
        S += 160;
        found_si = 0;
      }
    }
    else
      S += 80; // move to next line in VRAM
  }
}

/****************************************************************************/
/*** Refresh screen. This function updates the blanking state and then    ***/
/*** calls RefreshScreen_T() and finally it calls PutImage() to copy the  ***/
/*** off-screen buffer to the actual display                              ***/
/****************************************************************************/
void RefreshScreen(void)
{
  static int BCount = 0;
  // Update blanking count
  // flashing is on for 48 cycles and off for 16 cycles (64-48)
  BCount++;
  if (BCount == 48 / UPeriod) doblank = 1;
  if (BCount == 64 / UPeriod) doblank = BCount = 0;
  // Update the screen buffer
  RefreshScreen_T();
  // Put the image on the screen
  PutImage();
}
