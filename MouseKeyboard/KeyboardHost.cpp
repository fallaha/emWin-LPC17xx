


//#include "..\AF_LIB\AF_string.h"
#include "KeyboardHost.h"

/** LPCUSBlib HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Host_t Keyboard_HID_Interface =
	{
		.Config =
			{
				.DataINPipeNumber       = 1,
				.DataINPipeDoubleBank   = false,

				.DataOUTPipeNumber      = 2,
				.DataOUTPipeDoubleBank  = false,

				.HIDInterfaceProtocol   = HID_CSCP_MouseBootProtocol,
				.PortNumber = 0,
			},
	};


/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
void mouse_init(void)
{	
	SetupHardware();
//	sei();


}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if defined (__CC_ARM)  || defined(__ICCARM__) // FIXME KEIL related
  SystemInit();
#endif
	//bsp_init();

	USB_Init();

	/* Create a stdio stream for the serial port for stdin and stdout */
//	Serial_CreateStream(NULL);
}

/** Task to manage an enumerated USB keyboard once connected, to display key state
 *  data as it is received.
 */


void KeyboardHost_Task(char *x,char *y,char *but)
{
	if (USB_HostState[Keyboard_HID_Interface.Config.PortNumber] != HOST_STATE_Configured)
	  return;

	if (HID_Host_IsReportReceived(&Keyboard_HID_Interface))
	{

        USB_MouseReport_Data_t MouseReport;
        HID_Host_ReceiveReport(&Keyboard_HID_Interface, &MouseReport);

		
//============================================
	*x = MouseReport.X;
	//	if(*x > 128)
	//	*x=256-*x;
		
		*y = MouseReport.Y ;
//		if (*y >128)
//			*y=256-*y;
		*but=MouseReport.Button;
	}
}

/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached(const uint8_t corenum)
{
//	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached(const uint8_t corenum)
{
//	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete(const uint8_t corenum)
{
//	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);

	uint16_t ConfigDescriptorSize;
	uint8_t  ConfigDescriptorData[512];

	if (USB_Host_GetDeviceConfigDescriptor(corenum, 1, &ConfigDescriptorSize, ConfigDescriptorData,
	                                       sizeof(ConfigDescriptorData)) != HOST_GETCONFIG_Successful)
	{
	//	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	Keyboard_HID_Interface.Config.PortNumber = corenum;

	if (HID_Host_ConfigurePipes(&Keyboard_HID_Interface,
	                            ConfigDescriptorSize, ConfigDescriptorData) != HID_ENUMERROR_NoError)
	{
		//	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	if (USB_Host_SetDeviceConfiguration(Keyboard_HID_Interface.Config.PortNumber,1) != HOST_SENDCONTROL_Successful)
	{
	//		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	if (HID_Host_SetBootProtocol(&Keyboard_HID_Interface) != 0)
	{
		//	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		USB_Host_SetDeviceConfiguration(Keyboard_HID_Interface.Config.PortNumber,0);
		return;
	}

//	LEDs_SetAllLEDs(LEDMASK_USB_READY);
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError(const uint8_t corenum, const uint8_t ErrorCode)
{
	USB_Disable();


//	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	for(;;);
}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t corenum,
											const uint8_t ErrorCode,
                                            const uint8_t SubErrorCode)
{


//	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}

/** Dummy callback function for HID Parser
 */
bool CALLBACK_HIDParser_FilterHIDReportItem(HID_ReportItem_t* const CurrentItem)
{
	return true;
}
