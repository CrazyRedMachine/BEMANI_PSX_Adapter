#include "HID.h"

#define USB_EP_BINTERVAL 1
#define EPTYPE_DESCRIPTOR_SIZE uint8_t

#define DM_NUMBER_OF_BUTTONS 8
#define DM_REPORT_ID 6

class DMHID_ : public PluggableUSBModule {
  public:
    DMHID_();
    int sendState(uint32_t buttonsState);

  protected:
    EPTYPE_DESCRIPTOR_SIZE epType[1];
    uint8_t protocol;
    uint8_t idle;

    int getInterface(uint8_t* interfaceCount);
    int getDescriptor(USBSetup& setup);
    bool setup(USBSetup& setup);
};

extern DMHID_ DMHID;
