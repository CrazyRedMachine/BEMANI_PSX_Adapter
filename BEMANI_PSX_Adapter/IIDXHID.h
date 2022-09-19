#include "HID.h"

#define NUMBER_OF_LEDS 11
#define NUMBER_OF_BUTTONS 12
#define ENCODER_PPR 255

#define EPTYPE_DESCRIPTOR_SIZE uint8_t
#define STRING_ID_Base 4

#define DPAD_DOWN 0x04
#define DPAD_UP 0x00
#define DPAD_NEUTRAL 0x08

class IIDXHID_ : public PluggableUSBModule {
  public:
    IIDXHID_();
    void write_lights(uint32_t button_state, bool hid, bool reactive);
    int send_state(uint32_t button_state, int32_t turntable_state);
    unsigned long getLastHidUpdate();

  protected:
    EPTYPE_DESCRIPTOR_SIZE epType[1];
    unsigned long lastHidUpdate = 0;

    int getInterface(uint8_t* interface_count);
    int getDescriptor(USBSetup& setup);
    bool setup(USBSetup& setup);
};

extern IIDXHID_ IIDXHID;
