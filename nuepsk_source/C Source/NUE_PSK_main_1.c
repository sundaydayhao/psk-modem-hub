/*======================================================================

		NUE_PSK main

		Milton E. Cram	W8NUE
		Began On:		09/24/2006
		Current Date:	9/10/2009
		NUE_PSK Version 2.33

======================================================================*/
/*======================================================================
version 1.00 12/2/07
1) Removed Quadrature Encoder processing from ReceiveData.
2) Integrated ReceiveData into TIM1_isr.
3) TIM1_isr modified to keep track of QE time--provides 50ms timing, and sets ProcQEI flag (in Status).
4) main.c modified to check ProcQEI flag and call Proc_QEI as needed.
5) Proc_QEI modified to include check for TAP or HOLD of QE pushbutton.  If pushbutton is pressed for 
less than .5 sec, the TAP flag will be set.  If pressed for more than .5 sec, the HOLD flag will be set.  
These flags will be cleared by whatever code processes these flags.  It is intended that TAP will be 
used to initiate frequency lock when in Receive mode, and to make menu selections when in Setup mode.  
HOLD will be used to enter Setup mode whe in Receive Mode, and to exit Setup mode when in Setup mode.

Tuning is now smooth with both the encoder and the keyboard.  Potential interrupt timing conflicts 
eliminated since the processing is done as time is available, and can be interrupted by the critical 
acquisition timer.

...LCD_single_1.c contains the changes for LCD_E, LCD_DI, LCD_RW, LCD_CS1, and LCD_CS2.
Backlight control changed to C14.
Piezo output is on D1.
Voltage monitoring of the battery voltage and TX Audio pot are not yet implemented.
I'm working on the menu control using the encoder and pushbutton.

12/11/07
Voltage monitoring of battery voltage and TX Audio pot now working.  Added 10k ohm resistor from 
DAC to TX Audio pot to reduce audio level to TX.  This allows the pot to be nearer to mid range 
with the Yaesu FT series XCVRs.  Changed the conversion factor in the code so that TX Audio level 
reads close to 100% when the pot is at max.

Fixed a bug in the submenu that prevented correct operation of the rotary encoder during CCW rotation.  
Problem was due to modulus function with negative input value.  Wrote my own function to correct problem.

12/12/07
Fixed bug in PgUp/PgDn control of PGA Gain.
Fixed bug in Ctrl-L control of Text Screen Erase.

12/13/07
Disabled data acqusition/processing during tuning.  This should eliminate the occasional jump in 
the frequency during tuning. (Tuning is checked every 50ms.  If the RX processing, including spectral 
display, takes more than 50 ms, it could affect tuning.)

version 1.12 1/3/08
Cleaned up a lot.  Deleted un-needed files.  Combined a couple of the table files to reduce the total.  
Added includes for a couple of standard 'C' files to eliminate "implicit declaration" messages during build.

version 1.12b 3/9/08 
Corrected CWId bug.  Test for prior write of Callsign info to EEPROM was not working.  Corrected.  
(Primarily due to “” instead of ‘’).

3/11/08
1) In Write_Display_Data function—added test for data>0x7F.  If test positive, replace data with 0.  
This eliminates some “gibberish” from being displayed in RX.
2) “Tap” was not working in RX (to initiate Acquire function).  A line had been commented out in main.c.  Corrected.

Version 1.12c 3/18/08
Added additional delay in LCD routines to make sure requirement on high and low states of LCD_E are met 
(>450nsec in each state).

3/22/08
Modified Play_Macro routine to allow entering a control character at start of macros to automatically 
switch to TX.  Use Ctrl-S to enter the control character.  When the macro is "played", while in RX, 
the system will be toggled to TX.  Useful for calling CQ without having to first press F12 (Toggle RX/TX).

3/24/08

Added additional delay in Write_lcd_cntrl_regs routine to assure low state on LCD_E >450nsec.  
(Missed this one on 3/18)
Modified Ctrl-Q function to allow write of TX_CNTRL_AUTOSTOP to TxQue.  This enables the modem to 
terminate transmission when the buffer reaches the AutoStop character.  Pressing Ctrl-Q while sending, 
places the AutoStop character in the buffer.  Keys pressed after, pressing Ctrl-Q will be ignored. 

Version 1.20 4/28/08
Modified handling of backspace character in TX and Macro Recording.  The display is now properly 
updated when a backspace is entered (the last character is blanked, and the cursor is moved back).
Added ability to save modem state (PGA Gain, BackLight Status, AFC Status, SquelchThreshold, 
CWId status, and operating mode to EEPROM.  The last saved configuration is automatically restored 
upon power-up.

F10 Menus deleted.  F10 is now RX/TX toggle (replacing F12).
Rapid tuning using Alt + arrow keys.  (5X with Alt + right/left keys, 10X with Alt + up/down keys.)

Version 1.20A 5/2/08
Minor bug fix to properly restore PGA Gain state.

Version 1.20B 5/4/08
Modified menu handling to reduce looping.  This eliminates display flicker.  Modified the logic 
for handling button press and encoder changes.  Added a 250ms delay when new menu is displayed.  
These changes prevent missed menu options when rotating the encoder knob.  

Version 1.20C 5/4/08
Minor change in display of battery voltage and TX Audio level as a result of the logic changes above. 

Version 1.20D 5/7/08
Added Display of Configuration Variables--F12 to display, Tap Select to clear. 

Version 1.20E 5/9/08
Added simple IIR filter to Battery Voltage and TX Audio level monitors.

Version 1.21 6/12/08
Added Annunciator for Logging to right side of text display.

Version 1.22 6/17/08
Added drivers for UART output of Logging data.  For use with Viniculum VDIP1, to connect
a Flash Drive to the modem.

Version 1.23 6/19/08
Completed USB interface (VDIP1) for logging QSOs to Flash Drive.

Version 1.23A 6/22/08
Added detection for absent VDIP1 and/or flash drive. Audible alarm signifies error.

Version 1.23B 6/23/08
Added code to insert File Name for recording file.
Added code to insert Text into recording file.
Added Timer4 to drive Piezo.
None of this is tested yet.  3pm.

Version 1.24 6/30/08
Modified Text Entry.  Now have On Screen display of text entry for
Macros, CallSigns, FileName.

Version 1.25 7/4/08
Clean up.  Text Entry working fine.  Timer4 can now be used for an error alarm.
If you attempt to turn on USB recording, and there is no VDIP attached, the Piezo will
beep for 1/2 second.  If the VDIP is attached, but without a flash drive, there will 
be a delay, and then the Piezo will beep. (Beep routine)

Version 1.26 7/28/08
Minor bug fixes

Version 1.27 7/31/08
Added CRLFs to Recording file.

Version 1.28 8/1/08
Added CRLFs to Recording file when calling Toggle_RnT.

Version 1.29 8/14/08
Added default filename (NUE_PSK.txt) for recording file.

Version 1.29a 9/14/08
Added Firmware_Upgrade hooked to Shift F9.
User ID bits have been commented out.

Version 1.30 10/16/08
Added dynamic memory allocation for ADC_Buffer and Buffer
for flash upgrades.

Version 1.31 10/21/08
Revised bootloader2 to load memory image from flash drive.

Version 1.31a 11/17/08
Modifications to utilize VNC1L firmware with prompts, drive change notifications, 
LED power-on flashing disabled. (To simplify modem code.

Version 1.32 11/29/08
Now includes ability to reflash dsPIC Program Memory from flash drive.
Added initialization of Callsign. Added 25ms delay before testing
Select button state in bootloader.

Version 1.33 11/29/08
Now includes ability to Bootload from PC through USB card. Exchanged DTR/DSR control
lines for the NUE-USB adapter.

Version 1.34a 1/19/09
Added support for beacon mode.

Version 1.34b 1/22/09
Removed Malloc calls from lower bootloader.

Version 1.34c 1/22/09
Placed lower bootloader in separate project file.

Version 1.34d 1/27/09
Re-arranged linker scripts. Power-On Reset Delay changed to 128ms (was 32).

Version 1.34e 2/7/09
Put linker scripts back to 1.33 status. Altered OSC startup.  Now starts with fast
RC oscillator and then switches to xtal oscillator.
The reset vector has been changed to 0x402 for this upgrade.
The lower bootloader has been modified so that the reset vector can be modified
with a mem.dat file (Flash Bootload).
When the upgrade to 1.34E is done, it may be desirable to change the reset vector
back to 0x15000.  Modem1_34E(M1).hex may be used to produce a mem.dat file that 
puts the reset vector back to 0x15000.

Version 1.34f 2/22/09
Modified PC Bootload to correct problem with using Serial Adapter to load hex file.

Version 2.00 4/22/09
Initial work on RTTY implementation

Version 2.01 7/20/2009
Added display of L (letters) or F (figures).  Toggled by Ctrl-C
Added display of R (reverse) or N (normal).  Toggled by Ctrl-R  This interchanges
MARK and SPACE frequencies.  Fixed bug with display of mode when in RTTY.
RTTY TX Code added.

Version 2.10 7/28/2009
Debugged TX Code and changed treatment of space, CR, and LF.  These are now treated
as LTRS to eliminate transmitting FIGS/LTRS pair of characters for each space, LF, or
CR. Fixed RTTY Cursor updates and removed central cursor. Added test for zero signal
in ProcRTTYDet. Unshift On Space added 8/2.  Fixed Reverse error on TX 8/10.

Version 2.10g 8/10/2009
Fix for incorrect processing of keyboard entered Figures in TX

Version 2.10h 8/16/2009
Fix for bug in keyboard routine causing RTTY mode to be set.

Version 2.11 9/10/2009
Added counter to Beacon mode.

Version 2.12 9/24/2009
Added Weighted Overlap and Add FFT.

Version 2.20 12/4/2009
Added Config Upload/Download

Version 2.30b 1/14/2010
Revised Sine Tables

Version 2.30c 1/15/2010
Corrected Config Download problem.  Added autoreset after download.

Version 2.30d 1/21/2010
Revised Tables for PSK envelope (to free up memory)

Version 2.31 1/25/2010
Added delay to flash drive writes.

Version 2.32 1/26/2010
Added delay to flash drive writes.

Version 2.33a 1/29/2010
Added CTS flow control for flash drive writes.  Note that this change requires a wire 
to be added from the modem card 'pad b' to the CTS pad on the bottom of the USB card.
However this hardware mod is totally optional, needed IF AND ONLY IF the "Upload Config"
is to be used.

Version 2.33b 2/9/2010
Fixed Save Config in the CONFIG menu select area (Subroutines)

Version 2.33c 2/19/2010
Fixed keyboard freeze when running QPSK and QPSK/R from code commented out
in PSKmod.c (QPSKdet) and PSKdet.c (ViterbiDecode)

======================================================================================*/

#include	"NUE_PSK_Header.h"

/*=====================================================================*/		
/* Macros for setting device configuration registers                   */
/*=====================================================================*/		

	_FBS		( RBS_NO_RAM & BSS_NO_FLASH & BWRP_WRPROTECT_OFF )
	_FSS		( RSS_NO_RAM & SSS_NO_FLASH & SWRP_WRPROTECT_OFF )
	_FGS		( GSS_OFF & GWRP_OFF )
	_FOSCSEL	( FNOSC_FRC ) /* startup with fast RC oscillator */
	_FOSC		( FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT ) /* allow osc switching */
	_FWDT		( FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768 )
	_FPOR		( PWMPIN_ON & HPOL_ON & LPOL_ON & FPWRT_PWR128 ) /* was 32ms */

/*	_FUID0		( 0x01 )
	_FUID1		( 0x23 )
	_FUID2		( 0x45 )
	_FUID3		( 0x67 ) */


/*=====================================================================*/
	
/*	unsigned int SFR_ARRAY[7] __attribute__ ((noload)) = {0}; */
	#define	AFC_ON 1

/*=====================================================================*/		

int main ( void ) {

	char	Vers_String[20] = "NUE-PSK Ver 2.33c";
	extern int count;
	extern Flag_type Flag;
	extern char CharBfr;
	extern int m_TXState, cursor_position, m_AFCmode;
	extern int *pADC_Buffer;
	int i;
	char tempch;
	extern double Old_Freq;
	char UartDat;
	extern unsigned int BeaconInterval;
	extern int RunBeacon;
	extern unsigned int Beacon_Count;
	extern int LastScanCode;
	extern int ProcRTTY;
	/* set oscillator configuration */	
	PLLFBDbits.PLLDIV	=	30;	/* M=32 */
	Nop();
	CLKDIVbits.PLLPOST=0;		/* N1=2 */
	Nop();
	CLKDIVbits.PLLPRE=0;		/* N2=2 */
	Nop();
	/* switch to xtal with PLL */
	__builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);
	while(OSCCONbits.COSC != 0b011);

	INTCON1bits.NSTDIS = 0;
	POSCNT = 100;
	Piezo_tris = 0;
	init_clock ();
	InitUart2 (FAST); 			/* moved to here from below */
	init_SPI ();
	init_ADC ();
	Init_QED ();
	init_I2C1 ();
	Init_LCD ();
	TestBit_tris = 0;
	CallSign[0] = '\0';
	BeaconInterval = 60;
	
/*  Display version screen */
	Vers_String[20] = '\0';
	Clear_Text_Display (); 
	Write_String_Positioned_LCD1_Display (4,0,Vers_String);
	Write_String_Positioned_LCD1_Display (5,0," Copyright 2007-2009 ");
	Write_String_Positioned_LCD1_Display (6,0," Milton Cram  W8NUE  ");
	Write_String_Positioned_LCD1_Display (7,0," George Heron N2APB ");
	for ( i=0 ; i< 30 ; i++ ) Display_Battery_Voltage (2,5);
	Write_String_Positioned_LCD1_Display (2,5,"              ");                        
	Clear_Text_Display ();

/*	Continue with initialization */
	pADC_Buffer = (int *)malloc(ADC_BUFFER_LENGTH2*sizeof(int)); /* allocate buffer */
	init_Key_isr ();
	Set_PGA1_Gain (2);
	PGA = 2;
	Init_Filter_Keyboard_Pointers ( );
	init_TIM1 ();
	TestBit = 0;
	m_AFCmode = AFC_ON;
	AFC = m_AFCmode;
	/* Initialize in PSK mode */
	Init_FIR_PSK ();
	ResetModem (BPSK_MODE);
	mMODE = BPSK_MODE;
	/* end of PSK initialization */

	if (I2C1_Byte_Read (0x0) == 's') Restore_Config ();
	tempch = I2C1_Byte_Read (0x20);
	if (tempch == 'd') Write_CWId ();
	Init_Filename (); 
	Output_DAC_1A (0x800);

	_TRISD7 = 1;		 		/* make USB_DTR an input */
	_TRISD0 = 0; 	 	 		/* make USB_DSR an output */
	_TRISD10 = 1;		 		/* make USB_CTS an input */

	CNPU2bits.CN16PUE = 1;  	/* enable pull-up on DTR */

	USB_DSR = 1; 				/* and initialize hi */

/*	InitUart2 (SLOW); */
	T4CONbits.T32 = 0; 			/* Timer4 as 16 bit timer */
	T4CONbits.TCKPS = 1; 		/* Set prescaler to 8 */
	PR4 = 2500;
	PR5 = 0;
	IFS1bits.T4IF = 0; 			/* Clear Timer4 IF */
	T4ON = 1; 					/* Turn on Timer 4 */
	TIMER4 = 1; 				/* Enable Timer4 Interrupt Service Routine */

/* 	Init Timer6/7 */
	T6CONbits.T32 = 1; 			/* Timer6/7 as 32 bit timer */
	T6CONbits.TCKPS = 3; 		/* Set prescaler to 128 */
	BeaconInterval = 60;  		/* max delay ~ 13700 sec */
	Beacon_Count = 1;
	PR7 = (unsigned int) ((float)BeaconInterval * 2.38419);
	PR6 = (unsigned int) ((unsigned long) (BeaconInterval*156250) - (unsigned long) (PR7 * 65536));
	T67IF = 0; 					/* Clear Timer7 IF */		
	T67ON = 0; 					/* Leave Timer 6/7 turned OFF */
	T67IE = 1; 					/* Enable Timer 6/7 Interrupt Service Routine */

	Rtty.UOS = 1;				/* Enable "Unshift on Space" */

/*=====================================================================*/		

while (1)	{

	if (Status.CONFIG) {
		if (Status.Fifty_ms) {
			Status.Fifty_ms = 0;
			Configure ();
			Status.CONFIG = 0;
		}
	} 
  	else {
			if (m_TXState == TX_OFF_STATE) {
				if (Flag.ProcPSK == 1) {
					ProcPSKDet (); 
					Flag.ProcPSK = 0; 
				}
				if (ProcRTTY == 1) {
					ProcRTTY = 0;
					ProcRTTYDet ();
				}
				if (Status.HOLD == 1) {
					Status.HOLD = 0;
					Status.CONFIG = 1;
				}
				if (Status.TAP == 1) {
					Status.TAP = 0;
					Acquire();
				}
			}
			if (Flag.TglRnT == TRUE) {
				Toggle_RnT ();
				Flag.TglRnT = FALSE;
			}
			if (Flag.TglTune == TRUE) {
				Toggle_Tune ();
				Flag.TglTune = FALSE;
			}
			if (Flag.DAV == TRUE) {
				Flag.DAV = FALSE;
				Buffered_LCD_Write (CharBfr); 
			}
			if (Flag.DispFreq == 1) {
				Flag.DispFreq = 0;
				Display_Freq();
			}
			if (Flag.RXTune ==1) {
				if (RXTuneCnt > 0x2000) {
					RXTuneCnt = 0;
					Flag.RXTune = 0;
					Acquire();
				}	
			}
			if (Flag.ProcFFT == 1) {
				Flag.ProcFFT = 0;
				Proc_FFT();
			}
			if (RunBeacon == 1) {
				RunBeacon = 0;
				++Beacon_Count;
				Display_Beacon_Count();
				LastScanCode = 0x83; 	/* scan code for F7 */
				Play_Macro();
			}
			if (Status.Fifty_ms == 1) {
				Status.Fifty_ms = 0;
				Proc_QEI();
			}
			/* refresh annunciators at least once per second */
			if (T67ON) Display_Bcn();
			if ((Status.LOGGING) & (Status.SECOND)) Display_Log ();
			if (abs(NCO_Frequency - Old_Freq) > 5.) {
				cursor_position = (int) ((NCO_Frequency)/DELTAFREQ +.5) -32;
				Clear_Cursor();
				if (m_PSKmode != RTTY_MODE) Display_Cursor (cursor_position);
				else Display_RTTYCursor ();
			}
			Old_Freq = NCO_Frequency;
			Process_Key_FIFO(); 
    	}
  	}
}
/*=====================================================================*/		

