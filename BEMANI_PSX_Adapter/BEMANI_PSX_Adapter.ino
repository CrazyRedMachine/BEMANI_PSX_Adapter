#include "IIDXHID.h"
#include "POPNHID.h"
#include "PsxControllerHwSpi.h"
#include <inttypes.h>

//use doublepress hotkeys (select is E1, start is E2, double press select E3, triple press select is E1+E3, double press start is E4, triple press start is E2+E4)
#define WITH_DOUBLECLICK 1

//delay in µs between HID reports (note: 1000 for jkoc/popn, will be *15 for uskoc)
#define REPORT_DELAY 1000

#define COOLDOWN 4

IIDXHID_ IIDXHID;
POPNHID_ POPNHID;

const byte PIN_PS2_ATT = A2;
PsxControllerHwSpi<PIN_PS2_ATT> psx;
bool haveController = false;

int32_t tt_pos = 0;

typedef enum
{
  TT_NEUTRAL = 0,
  TT_UP,
  TT_DOWN,
} tt_direction_t;

void update_encoder(tt_direction_t dir, uint8_t quantity) {
  static tt_direction_t last_state;
  static int cooldown = 0;

  if (cooldown > 0)
    cooldown--;

  if (dir != last_state || cooldown == 0)
  {
    if (dir == TT_UP) {
      tt_pos += quantity;
    } else if (dir == TT_DOWN) {
      tt_pos -= quantity;
    }
    cooldown = COOLDOWN;
  }

  last_state = dir;
}

bool g_popn = false;

void setup() {
  haveController = psx.begin();
  delay(300);
  psx.read();
  PsxButtons psxButtons = psx.getButtonWord();
  if ((psxButtons & PSB_PAD_LEFT) && (psxButtons & PSB_PAD_DOWN) && (psxButtons & PSB_PAD_RIGHT))
  {
    g_popn = true;
    PluggableUSB().plug(&POPNHID);
  } else {
    PluggableUSB().plug(&IIDXHID);
  }
}

void buttonRead(uint32_t *button_state, tt_direction_t *tt_dir)
{
  PsxButtons psxButtons = 0;
if (!haveController) {
    if (psx.begin ()) {
      delay (300);     
      haveController = true;
    }
  } else {
    if (!psx.read ()) {
      haveController = false;
    } else {
      psxButtons = psx.getButtonWord();
    }
  }

  uint32_t buttonsState = 0;

  if (g_popn)
  {
    buttonsState |= !!(psxButtons & PSB_TRIANGLE) << 0;
    buttonsState |= !!(psxButtons & PSB_CIRCLE) << 1;
    buttonsState |= !!(psxButtons & PSB_R1) << 2;
    buttonsState |= !!(psxButtons & PSB_CROSS) << 3;
    buttonsState |= !!(psxButtons & PSB_L1) << 4;
    buttonsState |= !!(psxButtons & PSB_SQUARE) << 5;
    buttonsState |= !!(psxButtons & PSB_R2) << 6;
    buttonsState |= !!(psxButtons & PSB_PAD_UP) << 7;
    buttonsState |= !!(psxButtons & PSB_L2) << 8;
    buttonsState |= !!(psxButtons & PSB_SELECT) << 9;
    buttonsState |= !!(psxButtons & PSB_START) << 10;

    *button_state = buttonsState;
    return;
  }
  /*
    1          square
    2          L1
    3          cross
    4          R1
    5          circle
    6          L2
    7          left
    sel        sel
    stt        start
    tt cw      up
    tt ccw     down
  */

  buttonsState |= !!(psxButtons & PSB_SQUARE) << 0;
  buttonsState |= !!(psxButtons & PSB_L1) << 1;
  buttonsState |= !!(psxButtons & PSB_CROSS) << 2;
  buttonsState |= !!(psxButtons & PSB_R1) << 3;
  buttonsState |= !!(psxButtons & PSB_CIRCLE) << 4;
  buttonsState |= !!(psxButtons & PSB_L2) << 5;
  buttonsState |= !!(psxButtons & PSB_PAD_LEFT) << 6;
  buttonsState |= !!(psxButtons & PSB_START) << 8; /* E1 */
  buttonsState |= !!(psxButtons & PSB_SELECT) << 9; /* E2 */

  if (psxButtons & PSB_PAD_UP)
  {
    *tt_dir = TT_DOWN;
    buttonsState |= ((uint32_t)DPAD_UP) << 16;
  }
  else if (psxButtons & PSB_PAD_DOWN)
  {
    *tt_dir = TT_UP;
    buttonsState |= ((uint32_t)DPAD_DOWN) << 16;
  }
  else
  {
    *tt_dir = TT_NEUTRAL;
    buttonsState |= ((uint32_t)DPAD_NEUTRAL) << 16;
  }

#if WITH_DOUBLECLICK == 1
  static uint16_t previous_select = 0;
  static bool select_double = false;
  static bool select_triple = false;
  if (buttonsState >> 9 & 1)
  {
    if (previous_select)
    {
      if (previous_select < 18)
      {
        if (select_double) select_triple = true;
        else select_double = true;
      }
    }
    previous_select = 18;
  }
  else
  {
    if (previous_select)
    {
      previous_select--;
      if (previous_select == 0)
      {
        select_double = false;
      }
    }
    if (select_triple)
    {
      select_double = false;
      select_triple = false;
    }
  }
  //change output based on double/triple click
  if (select_triple)
  {
    buttonsState |= (uint32_t)1 << 10; /* E3 */
  }
  else if (select_double)
  {
    buttonsState |= (uint32_t)1 << 10;
    buttonsState &= ~((uint32_t)1 << 9);
  }

  static uint16_t previous_start = 0;
  static bool start_double = false;
  static bool start_triple = false;
  if (buttonsState >> 8 & 1)
  {
    if (previous_start)
    {
      if (previous_start < 18)
      {
        if (start_double) start_triple = true;
        else start_double = true;
      }
    }
    previous_start = 18;
  }
  else
  {
    if (previous_start)
    {
      previous_start--;
      if (previous_start == 0)
      {
        start_double = false;
      }
    }
    if (start_triple)
    {
      start_double = false;
      start_triple = false;
    }
  }
  //change output based on double/triple click
  if (start_triple)
  {
    buttonsState |= (uint32_t)1 << 11; /* E4 */
  }
  else if (start_double)
  {
    buttonsState |= (uint32_t)1 << 11;
    buttonsState &= ~((uint32_t)1 << 8);
  }
#endif

  *button_state = buttonsState;

}

void loop() {
  static unsigned long lastReport = 0;
  static unsigned long lastPool = 0;
  static unsigned long currTime = 0;
  static int32_t saved_tt_stop = tt_pos;
static uint32_t curr_button_state;
static tt_direction_t curr_tt_dir;
static int curr_tt_pos;

  currTime = micros();

  if (g_popn)
  {
    if ( (currTime - lastReport) >= REPORT_DELAY )
    {
      uint32_t button_state = 0;
      buttonRead(&button_state, NULL);
      POPNHID.sendState(button_state);
      lastReport = currTime;
    }
    return;

  }

  /* IIDX MODE */
  // Limit the encoder from 0 to ENCODER_PPR
  if (tt_pos >= ENCODER_PPR) {
    tt_pos = 1;
  } else if (tt_pos <= 0) {
    tt_pos = ENCODER_PPR - 1;
  }

  if ( (currTime - lastPool) >= 15*REPORT_DELAY ) /* 15ms for USKOC compatibility */
  { 
    buttonRead(&curr_button_state, &curr_tt_dir);
    lastPool = currTime;
  }

  if ( (currTime - lastReport) >= REPORT_DELAY )
  {
    if ((curr_button_state >> 9) & 1 || (curr_button_state >> 10) & 1)
    {
      static uint8_t upd_sel_cooldown = 0;
      if (upd_sel_cooldown) upd_sel_cooldown--;

      if (upd_sel_cooldown == 0)
      {
        update_encoder(curr_tt_dir, 1);
        upd_sel_cooldown = 8;
      }
    } else {
      static uint8_t upd_cooldown = 2;
      if (upd_cooldown) upd_cooldown--;

      if (upd_cooldown == 0)
      {
        update_encoder(curr_tt_dir, 1);
        upd_cooldown = 2;
      }
    }

    IIDXHID.send_state(curr_button_state, tt_pos);
    lastReport = currTime;
  }
}
