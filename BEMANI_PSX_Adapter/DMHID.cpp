#include "DMHID.h"
#include <string.h>

/* HID string and device descriptor */
const DeviceDescriptor PROGMEM DM_USB_DeviceDescriptor =
  D_DEVICE(USB_DEVICE_CLASS_HUMAN_INTERFACE, 0, 0, 64, 0x1ccf, 0x1002, 0x100, IMANUFACTURER, IPRODUCT, ISERIAL, 1);

static bool SendControl(uint8_t d) {
  return USB_SendControl(0, &d, 1) == 1;
}

static bool USB_SendStringDescriptor(const char* string_P, uint8_t string_len, uint8_t flags) {
  SendControl(2 + string_len * 2);
  SendControl(3);
  bool pgm = flags & TRANSFER_PGM;
  for (uint8_t i = 0; i < string_len; i++) {
    bool r = SendControl(pgm ? pgm_read_byte(&string_P[i]) : string_P[i]);
    r &= SendControl(0);
    if (!r) {
      return false;
    }
  }
  return true;
}

static const uint8_t PROGMEM _hidReportDM[] = {
  0x05, 0x01,                    /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x05,                    /* USAGE (Game Pad) */
  0xa1, 0x01,                    /* COLLECTION (Application) */

  0x85, DM_REPORT_ID,            /*   REPORT_ID (6) */
  0x05, 0x09,                    /*   USAGE_PAGE (Button) */
  0x19, 0x01,                    /*   USAGE_MINIMUM (Button 1) */
  0x29, DM_NUMBER_OF_BUTTONS,    /*   USAGE_MAXIMUM (Button 8) */
  0x15, 0x00,                    /*   LOGICAL_MINIMUM (0) */
  0x25, 0x01,                    /*   LOGICAL_MAXIMUM (1) */
  0x75, 0x01,                    /*   REPORT_SIZE (1) */
  0x95, DM_NUMBER_OF_BUTTONS,    /*   REPORT_COUNT (8) */
  0x81, 0x02,                    /*   INPUT (Data,Var,Abs) */

  0xc0                           /* END_COLLECTION */
};

static const char* const PROGMEM DM_String_Manufacturer = "Konami Computer Entertainment Japan, Inc.";
static const char* const PROGMEM DM_String_Product = "drummania Dedicated Controller";
static const char* const PROGMEM DM_String_Serial = "DMHID";

DMHID_::DMHID_() : PluggableUSBModule(1, 1, epType) {
  epType[0] = EP_TYPE_INTERRUPT_IN;
}

int DMHID_::getInterface(uint8_t* interfaceCount) {
  *interfaceCount += 1;
  HIDDescriptor hidInterface = {
    D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
    D_HIDREPORT(sizeof(_hidReportDM)),
    D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, USB_EP_BINTERVAL)
  };
  return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int DMHID_::getDescriptor(USBSetup& setup) {
  if (setup.wValueH == USB_DEVICE_DESCRIPTOR_TYPE) {
    return USB_SendControl(TRANSFER_PGM, (const uint8_t*)&DM_USB_DeviceDescriptor, sizeof(DM_USB_DeviceDescriptor));
  }
  if (setup.wValueH == USB_STRING_DESCRIPTOR_TYPE) {
    if (setup.wValueL == IPRODUCT) {
      return USB_SendStringDescriptor(DM_String_Product, strlen(DM_String_Product), 0);
    }
    else if (setup.wValueL == IMANUFACTURER) {
      return USB_SendStringDescriptor(DM_String_Manufacturer, strlen(DM_String_Manufacturer), 0);
    }
    else if (setup.wValueL == ISERIAL) {
      return USB_SendStringDescriptor(DM_String_Serial, strlen(DM_String_Serial), 0);
    }
  }

  if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) {
    return 0;
  }

  if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) {
    return 0;
  }

  if (setup.wIndex != pluggedInterface) {
    return 0;
  }

  return USB_SendControl(TRANSFER_PGM, _hidReportDM, sizeof(_hidReportDM));
}

bool DMHID_::setup(USBSetup& setup) {
  if (pluggedInterface != setup.wIndex) {
    return false;
  }

  uint8_t request = setup.bRequest;
  uint8_t requestType = setup.bmRequestType;

  if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
    return true;
  }

  if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
    if (request == HID_SET_PROTOCOL) {
      protocol = setup.wValueL;
      return true;
    }
    if (request == HID_SET_IDLE) {
      idle = setup.wValueL;
      return true;
    }
  }

  return false;
}

int DMHID_::sendState(uint32_t buttonsState) {
  uint8_t data[2];
  data[0] = DM_REPORT_ID;
  data[1] = (uint8_t)(buttonsState & 0xFF);
  return USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, 2);
}
