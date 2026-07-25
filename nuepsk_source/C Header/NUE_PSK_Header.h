/*======================================================================

		NUE_PSK_Header
		
		Milton E. Cram	W8NUE
		Version 		1.07
		Began On:		09/24/2006
		Current Date:	7/1/2008
		Based on original by John H. Fisher	K5JHF


======================================================================*/
 
#include <p33FJ128MC706.h>
#include <dsp.h>
#include <stdio.h>
#include <stdlib.h>

/*======================================================================*/
/* Global Types */
#define PI2 6.2831853071795864765
#define KCONV 10430.37835              /* 		4096*16/PI2 			*/
#define KF 159.1549
#define TIMER4 IEC1bits.T4IE	/* for control of Timer 4 */
#define T4ON T4CONbits.TON
#define T67IF IFS3bits.T7IF /* Timer 6/7 Interrupt flag */
#define T67IE IEC3bits.T7IE	/* for control of Timer 6/7 */
#define	T67ON T6CONbits.TON
#define BLOCKSIZE 128
#define BLOCKSIZE2 (BLOCKSIZE*2)
#define ADC_BUFFER_LENGTH 1024
#define ADC_BUFFER_LENGTH2 2*ADC_BUFFER_LENGTH
#define	ENCODER_TIME 400 /* 50 mSec */
#define FFT_BLOCK_LENGTH 512
#define LOG2_BLOCK_LENGTH 9
#define SAMPLING_RATE 8000
#define DELTAFREQ 15.625
#define FFTTWIDCOEFFS_IN_PROGMEM
#define RX 0
#define TX 1
#define REC_MACRO 2
#define SETUP 3
#define MAXROW 32
#define MAXCOL 18
#define MAXMENU 15

#define BPSK_MODE 0
#define QPSKU_MODE 1
#define QPSKL_MODE 2
#define RTTY_MODE 6
#define PSK63_MODE (1<<3)

#define RTTY_STOP_BITS 2

#define TX_OFF_STATE 0			/*TX is off */
#define TX_SENDING_STATE 1		/*TX is sending text */
#define TX_PAUSED_STATE 2		/*TX is paused */
#define TX_PREAMBLE_STATE 3		/*TX sending starting preamble */
#define TX_POSTAMBLE_STATE 4	/*TX sending ending posteamble */
#define TX_CWID_STATE 5			/*TX sending CW ID */
#define TX_TUNE_STATE 6			/*TX is tuning mode */
#define TX_SETUP 7				/* Setup state */
#define TX_MACRO_REC 8			/* Macro Recording State */
#define TX_FILE_INPUT 9			/* Input to Recording File */

#define TX_CNTRL_AUTOSTOP 4
#define TX_CNTRL_ADDCWID 5
#define TX_CNTRL_NOSQTAIL 6

#define TX_BUF_SIZE 256
#define BACK_SPACE_CODE 0x08

#define TXOFF_CODE 2		/* control codes that can be placed in the input */
#define TXON_CODE 3		/* queue for various control functions */
#define TXTOG_CODE 1

#define GET_MYCALL 0x0E
#define GET_THEIRCALL 0x0F
#define GET_BEACON_COUNT 0x10

#define KB .00484 /* Battery voltage conversion factor */
#define KV .5595 /* TX Audio Level conversion factor */

#define SLOW 0
#define FAST 1

#define NCHK 0
#define CHK	1

typedef struct 	{
        unsigned F0_char		:1;     
        unsigned E0_char		:1;     
        unsigned Shift_key		:1;         
        unsigned Alt_key		:1;     
        unsigned Ctrl_key		:1;     
        unsigned Bailout		:1;     
        unsigned RXTune			:1;
        unsigned DispFreq		:1;
        unsigned E1_char		:1;
        unsigned ProcFFT		:1;
        unsigned TglRnT			:1;
        unsigned ProcPSK		:1;
		unsigned TglTune		:1;
		unsigned DAV			:1;
		unsigned Setup			:1;
		unsigned MacRec			:1;
		} Flag_type;

typedef struct	{
		unsigned QED1A			:1;
		unsigned QED1B			:1;
		unsigned QED1PB			:1;
		unsigned PRESS			:1;
		unsigned TAP			:1;
		unsigned HOLD			:1;
		unsigned CONFIG			:1;
		unsigned Fifty_ms		:1;
		unsigned UP				:1;
		unsigned DOWN			:1;
		unsigned LCD_CTRLR		:1;
		unsigned DISP_CFG		:1;
		unsigned LOGGING		:1;
		unsigned SECOND			:1;
		} Status_type;

typedef struct {
		unsigned SHIFT			:2;	/* 00>170Hz, 01>200Hz, 10>850Hz */
		unsigned FIGURES		:1;
		unsigned REVERSE		:1; /* invert Mark/Space */
		unsigned DUM1			:1;
		unsigned DUM2			:1;
		unsigned UOS			:1; /* true if Unshift on Space selected */
		unsigned DAV			:1;
		} RTTY_type;						

typedef union {
		double f;
		char c[4];
		} u_val;

typedef unsigned int uint;
typedef unsigned char uchar;

/*======================================================================*/
/*	Global Enumerations		*/

enum { x1, x2, x4, x5, x8, x10, x16, x32 };

/*======================================================================*/
/*	Global Defines	*/

#define	MCP6S21_CS1_tris	TRISFbits.TRISF0	

#define MCP4922_CS1_tris	TRISDbits.TRISD11

#define	MCP6S21_CS1			LATFbits.LATF0	

#define MCP4922_CS1			LATDbits.LATD11

#define	PTT_tris			TRISFbits.TRISF1	
#define	PTT_On				LATFbits.LATF1

#define RXD_tris			TRISFbits.TRISF4
#define RXD					PORTFbits.RF4

#define	Key_0_tris			TRISBbits.TRISB8
#define	Key_1_tris			TRISBbits.TRISB9
#define	Key_2_tris			TRISBbits.TRISB10
#define	Key_3_tris			TRISBbits.TRISB11
#define	Key_4_tris			TRISBbits.TRISB12
#define	Key_5_tris			TRISBbits.TRISB13
#define	Key_6_tris			TRISBbits.TRISB14
#define	Key_7_tris			TRISBbits.TRISB15	


#define	QED_1A_tris			TRISBbits.TRISB4
#define	QED_1B_tris			TRISBbits.TRISB5
#define	Button1_tris		TRISGbits.TRISG9

#define	QED_1A				PORTBbits.RB4
#define	QED_1B				PORTBbits.RB5
#define	Button1				PORTGbits.RG9

#define	BackLight_tris		TRISCbits.TRISC14
#define	Piezo_tris			TRISDbits.TRISD1
#define	TestBit_tris		TRISDbits.TRISD0

#define	BackLight			LATCbits.LATC14
#define	Piezo				LATDbits.LATD1
#define	TestBit				LATDbits.LATD0

#define TRUE 1
#define FALSE 0

#define Max_FIR 512

#define	EEPROM_Write_Address	0b10100000

#define	EEPROM_Read_Address		0b10100001

/* #define USB_DTR	 		_RD0
#define USB_DSR	 		LATDbits.LATD7 */

#define USB_DTR	 		_RD7
#define USB_DSR	 		LATDbits.LATD0 
#define USB_CTS	 		_RD10

/*======================================================================*/
/*	Global	Variables	*/

extern	fractional Filter_delay_I1 [ 35 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_I1")));

extern	fractional Filter_delay_I2 [ 35 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_I2")));

extern	fractional Filter_delay_Q1 [ 35 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_Q1")));

extern	fractional Filter_delay_Q2 [ 35 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_Q2")));

extern	fractional Filter_delay_I3 [ 65 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_I3")));

extern	fractional Filter_delay_Q3 [ 65 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_Q3")));

extern	fractional Filter_delay_I4 [ 65 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_I4")));

extern	fractional Filter_delay_Q4 [ 65 ]

		__attribute__ (( space ( ymemory), aligned ( 2  ), section ("Filter_delay_Q4")));

extern	const fractional __attribute__ (( space (psv))) Dec4LPCoef[ 35 ];

extern	const fractional __attribute__ (( space (psv))) FIRBitCoef[ 65 ];

extern	const fractional __attribute__ (( space (psv))) FIRFreqCoef[ 65 ];

extern	const fractional __attribute__ (( space (psv))) RTTY1Coef[ 65 ];

extern	const fractional __attribute__ (( space (psv))) RTTY2Coef[ 65 ];

extern	const unsigned int	Scan_table [ 0x84 ][ 5 ];

extern 	const unsigned char  RTTY_table [ 0x20 ][ 2 ];

extern  int    INT_Count, MAX_Cnt;

extern  Status_type	Status;

extern  RTTY_type Rtty;

extern 	fractcomplex FFT_buffer []
	__attribute__ ((space ( ymemory), aligned (FFT_BLOCK_LENGTH * 2 * 2),section ("FFT_buffer")));

extern	int Key_FIFO_In, Key_FIFO_Out;

extern	int	Key_FIFO [ 64 ];

extern	int Temp_FIFO_In, Temp_FIFO_Out;

extern	int	Temp_FIFO [ 64 ];

extern 	int	Record_Address;
extern	int Last_Address;

extern 	int m_NoSquelchTail; 
extern 	int m_NeedCWid; 
extern 	int m_NeedShutoff; 
extern 	int m_TempNoSquelchTail;
extern 	int m_TempNeedCWid; 
extern 	int m_TempNeedShutoff;

extern 	int m_pTail;
extern 	int m_pHead;
extern 	char GetTxChar();
extern 	int m_TXState;
extern 	int m_PSKmode;
extern 	int m_CWState;
extern 	int m_CWtimer;
extern 	int m_CWSetSpeed;
extern 	char XmitBuffer[TX_BUF_SIZE];

extern 	char CFG1;
extern 	char CFG2;

extern	char	AUTO;
extern	char	PGA;
extern	char	mMODE;
extern	char	BL;
extern	char	SQTH;
extern	char	AFC;
extern	char	CWID;

/* RTTY related variables */
extern uchar	RegCount;

/* PSK31 and CW modulator private functions */
extern 	char GetChar( );



extern	void	(*Alt_A_Pointer)();
extern	void	(*Alt_K_Pointer)();
extern 	void	(*Ctrl_A_Pointer)();
extern 	void	(*Ctrl_B_Pointer)();
extern 	void	(*Ctrl_C_Pointer)();
extern 	void	(*Ctrl_F_Pointer)();
extern 	void	(*Ctrl_I_Pointer)();
extern 	void	(*Ctrl_K_Pointer)();
extern	void	(*Ctrl_L_Pointer)();
extern  void	(*Ctrl_M_Pointer)();
extern  void	(*Ctrl_N_Pointer)();
extern  void	(*Ctrl_O_Pointer)();
extern  void	(*Ctrl_P_Pointer)();
extern	void	(*Ctrl_Q_Pointer)();
extern	void	(*Ctrl_R_Pointer)();
extern	void	(*Ctrl_S_Pointer)();
extern	void	(*Ctrl_T_Pointer)();
extern	void	(*Ctrl_U_Pointer)();
extern	void	(*Ctl_Tb_Pointer)();
extern	void 	(*ESC_Pointer)();
extern	void	(*F1_Pointer)();
extern	void	(*F2_Pointer)();
extern	void	(*F3_Pointer)();
extern	void	(*F4_Pointer)();
extern	void	(*F5_Pointer)();
extern	void	(*F6_Pointer)();
extern	void	(*F7_Pointer)();
extern	void	(*F8_Pointer)();
extern	void	(*Alt_F1_Pointer)();
extern	void	(*Alt_F2_Pointer)();
extern	void	(*Alt_F3_Pointer)();
extern	void	(*Alt_F4_Pointer)();
extern	void	(*Alt_F5_Pointer)();
extern	void	(*Alt_F6_Pointer)();
extern	void	(*Alt_F7_Pointer)();
extern	void	(*Alt_F8_Pointer)();
extern	void	(*Ctl_F1_Pointer)();
extern	void	(*Ctl_F2_Pointer)();
extern	void	(*Ctl_F3_Pointer)();
extern	void	(*Ctl_F4_Pointer)();
extern	void	(*Ctl_F5_Pointer)();
extern	void	(*Ctl_F6_Pointer)();
extern	void	(*Ctl_F7_Pointer)();
extern	void	(*Ctl_F8_Pointer)();
extern	void	(*F9_Pointer)();
extern	void	(*S_F9_Pointer)();
extern 	void	(*Alt_F9_Pointer) ();
extern	void	(*F10_Pointer)();
extern	void	(*F11_Pointer)();
extern	void	(*F12_Pointer)();
extern	void	(*Up_Pointer)();
extern	void	(*Down_Pointer)();
extern	void	(*Right_Pointer)();
extern	void	(*Left_Pointer)();
extern	void	(*AUp_Pointer)();
extern	void	(*ADown_Pointer)();
extern	void	(*ARight_Pointer)();
extern	void	(*ALeft_Pointer)();
extern	void	(*Pg_Up_Pointer)();
extern	void	(*Pg_Dn_Pointer)();
extern	void	(*Home_Pointer)();
extern	void	(*End_Pointer)();
extern	void	(*Tab_Pointer)();
extern	void	(*S_Tab_Pointer)();
extern	void	(*kp_0_Pointer)();
extern	void	(*kp_1_Pointer)();
extern	void	(*kp_2_Pointer)();
extern	void	(*kp_3_Pointer)();
extern	void	(*kp_4_Pointer)();
extern	void	(*kp_5_Pointer)();
extern	void	(*kp_6_Pointer)();
extern	void	(*kp_7_Pointer)();
extern	void	(*kp_8_Pointer)();
extern	void	(*kp_9_Pointer)();
extern	void	(*slash_Pointer)();
extern	void	(*star_Pointer)();
extern	void	(*minus_Pointer)();
extern	void	(*plus_Pointer)();
extern	void	(*Enter_Pointer)();
extern	void	(*dot_Pointer)();

					/* FIR_I, FIR_Q, */
extern FIRStruct	Filter_I1,
					Filter_I2,
					Filter_Q1,
					Filter_Q2,
					Filter_I3,
					Filter_Q3,
					Filter_I4,
					Filter_Q4;

extern	int		RXTuneCnt;

extern 	long	NCO_phz;
extern	float	Lower_Frequency,
				Upper_Frequency,
				Sample_Frequency,
				Scale_Factor;
extern 	double	NCO_Frequency,
				freqerr,
				m_TxFreq,
				tempPHZ;
extern 	char	CharBfr;
extern 	char	CallSign[0x10];
extern 	char	FileName[0x10];
extern	const int Sine_table [ 1025 ];
extern 	const int PSKPhaseLookupTable[6][5]; 
// extern	const float PSKShapeTbl[2049]; 

extern	int 	QED_PB_Select;

extern unsigned char LCD_Addr[4];


/*======================================================================*/
/*	Global Prototypes	*/

void	init_TIM1 ( void );

void	Init_LCD ( void );

void	Init_QED ( void );

void	Push_Key_FIFO ( int Data );

int		Pull_Key_FIFO ( void );

void	Clear_Key_FIFO ( void );

void	Push_Temp_FIFO ( int Data );

int		Pull_Temp_FIFO ( void );

void	Clear_Temp_FIFO ( void );

void	Clear_Display ( uchar StartLine );

void	Write_Display_Data ( uchar data, uchar rev );

void	Write_lcd_cntrl_regs ( uchar regs, uchar data );

void	Clear_Text_Display ( void);

void	Wait_Display_Busy ( void );

void	Test_Left_Busy ( void );

void	Test_Right_Busy ( void );

void	Test_Both_Busy ( void );

void	Buffered_LCD_Write ( unsigned char ch );

void	Buffered_Text_Capture ( uchar ch );

void 	PutUartChar(char Char, char Check_Count);

void	Reset_LCD_buffer ( void );

unsigned char Bit_Reverse_Nibble ( unsigned char byte );

void	Bailout ( void );

void	Beep ( void );

void	Init_Filename ( void );

void	End_Transmission ( void );

void	Save_TheirCall ( void );

void	Save_Freq ( void );

void	Toggle_File_Input ( void );

void	Retrieve_Freq ( void );

void	Paste_Kybd_to_XmitBfr ( void );

void	Insert_TheirCall ( void );

void	Insert_MyCall ( void );
	
void	Insert_Beacon_Count ( void );

void	Save_MyCall ( void );

void	Save_FileName ( void );

void	Write_CWId ( void );

void	Put_My_Call ( void );

void	Put_Their_Call ( void );

void	Put_Beacon_Count ( void );

void	Set_RnT_Flag ( void );

void	Set_Tune_Flag ( void );

void	Proc_QEI ( void );

int		Sine_lookup ( int phase );

float	PSKEnv (int i, int n );

void	Dummy_Function (  );

void	Ctrl_A_Function ( );
void	Ctrl_B_Function ( );
void	Ctrl_C_Function ( );
void	Ctrl_F_Function ( );
void	Ctrl_I_Function ( );
void	Ctrl_K_Function ( );
void	Ctrl_M_Function ( );
void	Ctrl_N_Function ( );
void	Ctrl_L_Function ( );
void	Ctrl_O_Function ( );
void	Ctrl_P_Function ( );
void	Ctrl_Q_Function ( );
void	Ctrl_R_Function ( );
void	Ctrl_S_Function ( );
void	Ctrl_T_Function ( );
void	Ctrl_U_Function ( );
void	Ctl_Tb_Function ( );
void	EnableAFC ( void );
void	DisableAFC ( void );
void	Incr_NCO_Small ( void );
void	Incr_NCO_Big ( void );
void	Decr_NCO_Small ( void );
void	Decr_NCO_Big ( void );
void	Incr_NCO_SmallP ( void );
void	Incr_NCO_BigP ( void );
void	Decr_NCO_SmallP ( void );
void	Decr_NCO_BigP ( void );
void	Tune_NCO (int change);
void	Acquire ( void );
void	Alt_A_Function ( );
void	Alt_F_Function ( );
void	Alt_K_Function ( );
void	Alt_M_Function ( );
void	Alt_T_Function ( );
void 	ESC_Function (  );
void	F1_Function (  );
void	F2_Function (  );
void	F3_Function (  );
void	F4_Function (  );
void	F5_Function (  );
void	F6_Function (  );
void	F7_Function (  );
void	F8_Function (  );
void	Alt_F1_Function (  );
void	Alt_F2_Function (  );
void	Alt_F3_Function (  );
void	Alt_F4_Function (  );
void	Alt_F5_Function (  );
void	Alt_F6_Function (  );
void	Alt_F7_Function (  );
void	Alt_F8_Function (  );
void	Ctl_F1_Function (  );
void	Ctl_F2_Function (  );
void	Ctl_F3_Function (  );
void	Ctl_F4_Function (  );
void	Ctl_F5_Function (  );
void	Ctl_F6_Function (  );
void	Ctl_F7_Function (  );
void	Ctl_F8_Function (  );
void	F9_Function (  );
void	S_F9_Function (  );
void	Alt_F9_Function (  );
void	F10_Function (  );
void	F11_Function (  );
void	F12_Function (  );
void	Up_Function (  );
void	Down_Function (  );
void	Right_Function (  );
void	Left_Function (  );
void	AUp_Function (  );
void	ADown_Function (  );
void	ARight_Function (  );
void	ALeft_Function (  );
void	Pg_Up_Function (  );
void	Pg_Dn_Function (  );
void	Home_Function (  );
void	End_Function (  );
void	Tab_Function (  );
void	S_Tab_Function (  );
void	kp_0_Function ( void ); 
void	kp_1_Function ( void ); 
void	kp_2_Function ( void ); 
void	kp_3_Function ( void ); 
void	kp_4_Function ( void ); 
void	kp_5_Function ( void ); 
void	kp_6_Function ( void ); 
void	kp_7_Function ( void ); 
void	kp_8_Function ( void ); 
void	kp_9_Function ( void ); 
void	slash_Function ( void ); 
void	star_Function ( void ); 
void	minus_Function ( void );
void	plus_Function ( void ); 
void	Enter_Function ( void );
void	dot_Function ( void ); 
void 	Toggle_RnT ( void );
void	Save_Macro ( void );
void	Firmware_Upgrade ( void );
void	Incr_PGA_Gain ( void );
void	Decr_PGA_Gain ( void );
void	Clear_Buffers ( void );
void	Show_EEPROM ( void );
void	Record_Macro ( void );
void	Play_Macro ( void );
void	Erase_Macro ( void );
int		Get_Addr (int code );
void	Display_Menu ( int menu_num );
void	Setup ( void );
void	Setup_Menu ( void );
void	Toggle_Tune ( void );
void	Toggle_BackLight ( void );
void	Toggle_Logging ( void );
void	Configure ( void );
void	Save_Config ( void );
void	Restore_Config ( void );
void	PC_Bootload ( void );
void	Flash_Bootload ( void );
void	Download_Config ( void );
void	Upload_Config ( void );
void	Beacon_ON ( void );
void	Beacon_OFF ( void );
void	Toggle_Figures ( void );
void	RTTY_Reverse ( void );
void	Display_Settings ( void );
void	Display_Battery_Voltage ( uchar Xpos, uchar Ypos );
void	Display_TX_Audio_Level ( void );
void	Get_New_Beacon_Delay ( void );
void	Process_Menu_Select (unsigned char menu, unsigned char item);
unsigned char menu_index_mod_n (char menuindex, unsigned char lim);

void 	Write_String_Positioned_LCD1_Display ( unsigned char xposition, unsigned char yposition, char *string );

void 	Write_String_LCD1_Display ( char *string );

void 	Home_LCD1_Display ( );

void	Display_FFT ( void );

void 	Write_Integer_LCD1_Display ( int number );

void	Write_Integer_Formatted_LCD1_Display ( unsigned char xposition, unsigned char yposition, float *number );

void 	Reset_Keyboard_Pointers ( void );

void	Delay_100us(void);

void	Delay_10us(void);

void	Delay_5ms(void);

void	Delay_25ms(void);

void	init_Key_isr ( void );

void	init_clock ( void );

void 	init_SPI ( void );

void	init_ADC ( void );

void	init_AD1 ( void );

void	Set_PGA1_Gain	(	int gain_index );

void	Output_DAC_1A ( int	data );

void	Output_DAC_1B ( int	data );

int		Input_AD1	(void);

void 	Init_Filter_Keyboard_Pointers ( void );

void	Display_Freq ( );

void	Display_Hex_Data ( int data );

void	Strobe_LCD ( void );

void	Set_Display_Xaddr ( unsigned char Xpos );

void	Set_Display_Yaddr ( unsigned char Ypos );

void	Set_Display_XYAddr ( unsigned char Xpos, unsigned char Ypos );

void 	Display_Text_Cursor ( void );

void	Set_Display_Write ( void );

void	Clear_Cursor ( void );

void	Write_Char (unsigned char byte, uchar rev );

void	Write_Display_TX ( void );

void	Display_Tune ( void );

void	Display_Log ( void );

void	Display_Bcn ( void ) ;

void	Display_Beacon_Count ( void ) ;

void	Display_RTTY_Info ( void );

void	Clear_Log ( void );

void 	init_I2C1	( void );

void	I2C1_Byte_Write	( int Write_Address, char Write_Data );

char	I2C1_Byte_Read	( int Read_Address );

void 	Delay_5ms(void);

extern	void	InitPSK ( int Fs);

int 	GetTXCharsRemaining();

char 	PullTXQue();

void 	Set_Tuning_Steps ( void );

void	Set_PGA ( void );

void	Set_CWID_State ( void );

void	Set_Squelch_Th ( void );

void	Set_Mode ( void );

void	Insert_ON ( void );

void	Insert_OFF ( void );

void	WriteCharToBuffer ( char data, char done );

void	EmptyBuffer ( char done );

extern 	void PutTxQue(char txchar, int cntrl );

extern 	void CPSKInitDet( void );

extern  void ResetModem ( int mode );

extern 	void Init_FIR_PSK ( void );

extern 	void Init_FIR_RTTY ( void );

extern 	void PSKTXSetup ( void );

extern	void InitPSKMod ( void );

extern 	void ProcPSKDet ( void );

extern 	void ProcRTTYDet ( void );

extern	void Proc_FFT ( void );

extern	void Display_Cursor ( int cur_pos );

extern 	void Display_RTTYCursor ( void );

extern	void Process_Key_FIFO ( void );

extern	void SetRXFrequency ( double freq );

extern 	void Set_Display_TX ( void );

extern	void ClrQue ( void );

extern	void ResetDetector ( void );

extern	void SetSquelchThresh ( int thresh, int speed );

extern	void CalcPSK ( void );

extern 	int Add_CRLF_Capture_Buffer ( void );

extern void PutBfrStr ( char *string );

extern void ParseMacroBuff ( int EE_loc );

extern void WaitForError ( void );

extern void Read_Macro_From_Flash (int EE_Addr);

/*======================================================================*/
