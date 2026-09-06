/*======================================================================

		NUE_PSK Subroutines

		John H. Fisher	K5JHF
		Milton E. Cram  W8NUE
		George L. Heron	N2APB
		Began On:		09/24/2006
		Current Date:	01/19/2009

======================================================================*/

#include	"NUE_PSK_Header.h"
#include 	<math.h>

/*======================================================================*/
#define AFC_OFF 		0
#define AFC_ON 			1
#define SYMBOL_RATE31 	31.25	/* 31.25 Symbols per Second */
#define SYMBOL_RATE63 	62.5	/* PSK63 62.50 Symbols per Second */
#define SYMBOL_RATERTTY 45.45	/* RTTY 45.45 Symbols per Second */
#define TXOFF_CODE 		2		/* control codes that can be placed in the transmit */
#define TXON_CODE 		3		/* queue for various control functions */
#define TXTOG_CODE 		1

#define MYCALL 			0
#define THEIRCALL 		1
#define FILENAME 		2
#define FILEINPUT 		3
#define MACRO 			4
#define	Key_IE			IEC1bits.INT1IE		/* Keybd Interrupt Enable     */

/*======================================================================*/

void	Bailout ( void )		{ 
	extern Flag_type Flag;
	}

/*======================================================================*/

void	Dummy_Function ( void )	{	}

/*======================================================================*/

void Init_Filter_Keyboard_Pointers ( void )	{

	Alt_A_Pointer   = DisableAFC;
	Ctrl_A_Pointer	= EnableAFC;
	Ctrl_B_Pointer	= Clear_Buffers;
	Ctrl_C_Pointer	= Toggle_Figures;
	Ctrl_F_Pointer	= Save_Freq;
	Ctrl_I_Pointer	= Toggle_File_Input;
	Ctrl_K_Pointer	= Clear_Temp_FIFO;
	Ctrl_L_Pointer	= Clear_Text_Display;
	Ctrl_M_Pointer  = Save_MyCall;
	Ctrl_N_Pointer  = Save_FileName;
	Ctrl_O_Pointer	= Toggle_BackLight;
	Ctrl_P_Pointer	= Paste_Kybd_to_XmitBfr;
	Ctrl_R_Pointer	= RTTY_Reverse;
	Ctrl_T_Pointer	= Save_TheirCall;
	Ctrl_U_Pointer	= Toggle_Logging;
	Ctl_Tb_Pointer	= Display_Freq;
	F1_Pointer		= Play_Macro;
	F2_Pointer		= Play_Macro;
	F3_Pointer		= Play_Macro;
	F4_Pointer		= Play_Macro;
	F5_Pointer		= Play_Macro;
	F6_Pointer		= Play_Macro;
	F7_Pointer		= Play_Macro;
	F8_Pointer		= Set_Tune_Flag;
	F9_Pointer		= Save_Macro;
	Alt_F9_Pointer  = Beacon_OFF;
	S_F9_Pointer    = Beacon_ON;
	F10_Pointer		= Set_RnT_Flag;
	F11_Pointer		= Show_EEPROM;
	F12_Pointer		= Display_Settings;
	Up_Pointer		= Incr_NCO_Big;
	Down_Pointer	= Decr_NCO_Big;
	Right_Pointer	= Incr_NCO_Small;
	Left_Pointer	= Decr_NCO_Small;
	AUp_Pointer		= Incr_NCO_BigP;
	ADown_Pointer	= Decr_NCO_BigP;
	ARight_Pointer	= Incr_NCO_SmallP;
	ALeft_Pointer	= Decr_NCO_SmallP;
	Pg_Up_Pointer	= Incr_PGA_Gain;
	Pg_Dn_Pointer	= Decr_PGA_Gain;
	Home_Pointer	= Dummy_Function;
	End_Pointer		= Acquire;
	Tab_Pointer		= Dummy_Function;
	S_Tab_Pointer	= Dummy_Function;
	kp_0_Pointer	= Dummy_Function; 
	kp_1_Pointer	= Dummy_Function;
	kp_2_Pointer	= Dummy_Function;
	kp_3_Pointer	= Dummy_Function;
	kp_4_Pointer	= Dummy_Function;
	kp_5_Pointer	= Dummy_Function;
	kp_6_Pointer	= Dummy_Function;
	kp_7_Pointer	= Dummy_Function;
	kp_8_Pointer	= Dummy_Function;
	kp_9_Pointer	= Dummy_Function;
	slash_Pointer	= Dummy_Function;
	star_Pointer	= Dummy_Function;
	minus_Pointer	= Dummy_Function;
	plus_Pointer	= Dummy_Function;
	Enter_Pointer	= Dummy_Function;
	dot_Pointer		= Dummy_Function;

	}

/*======================================================================*/

void	Push_Key_FIFO ( int Data )	{

	Key_FIFO [ Key_FIFO_In++ ] = Data;
	Key_FIFO_In	&= 0x3F;

	}

/*======================================================================*/

int		Pull_Key_FIFO ( void )		{

	int	temp;	

	if ( Key_FIFO_In == Key_FIFO_Out ) return 0;
	temp = Key_FIFO [ Key_FIFO_Out++ ];
	Key_FIFO_Out &= 0x3F;
	return temp;

	}

/*======================================================================*/

void	Clear_Key_FIFO ( void )		{

	Key_FIFO_In = Key_FIFO_Out = 0;

	}

/*======================================================================*/

void	Push_Temp_FIFO ( int Data )	{

	Temp_FIFO [ Temp_FIFO_In++ ] = Data;
	Temp_FIFO_In	&= 0x3F;

	}

/*======================================================================*/

int		Pull_Temp_FIFO ( void )		{

	int	temp;	

	if ( Temp_FIFO_In == Temp_FIFO_Out ) return 0;
	temp = Temp_FIFO [ Temp_FIFO_Out++ ];
	Temp_FIFO_Out &= 0x3F;
	return temp;

	}

/*======================================================================*/

void	Clear_Temp_FIFO ( void )		{
	/* callable with Ctrl-K */
	Temp_FIFO_In = Temp_FIFO_Out = 0;

	}

/*======================================================================*/
	
void	Output_DAC_1A ( int	data )	{

		int	temp;

		MCP4922_CS1	=	0;

		SPI1BUF	=	( ( data & 0x0FFF ) | 0x3000 );

		while ( !SPI1STATbits.SPIRBF );
	
		temp	=	SPI1BUF;

		MCP4922_CS1	=	1;
		
	}
	
/*=====================================================================*/		
	
void	Output_DAC_1B ( int	data )	{

		int	temp;

		MCP4922_CS1	=	0;

		SPI1BUF	=	( ( data & 0x0FFF ) | 0xB000 );

		while ( !SPI1STATbits.SPIRBF );
	
		temp	=	SPI1BUF;

		MCP4922_CS1	=	1;
		
	}
	

/*=====================================================================*/		

int		Input_AD1	(void)	{

		while ( !AD1CON1bits.DONE );

		AD1CON1bits.DONE	=	0;

		return	ADC1BUF0;	
	
	}

/*=====================================================================*/		

void	Display_Freq ( ) {

	extern double NCO_Frequency;
	char buffer[16];
	sprintf (buffer, "Freq: %5.1f ", NCO_Frequency );
	Write_String_Positioned_LCD1_Display ( 4,0, buffer );
	Set_Display_XYAddr (7,0);
	}

/*======================================================================*/		

void	Display_Hex_Data ( int data ) {

	char buffer[16];
	sprintf (buffer, "%4x ", data );
	Write_String_Positioned_LCD1_Display ( 4,0, buffer );
	Set_Display_XYAddr (7,0);
	}

/*=====================================================================*/

void 	EnableAFC ( void ) {
	extern int m_AFCmode;
	m_AFCmode = AFC_ON;
	AFC = AFC_ON;
	}

/*=====================================================================*/

void	DisableAFC ( void ) {
	extern int m_AFCmode;
	m_AFCmode = AFC_OFF;
	AFC = AFC_OFF;
	}
/*=====================================================================*/

void	Incr_NCO_Small ( void ) { Tune_NCO (1); }
	/* RIGHT ARROW */

/*=====================================================================*/

void	Incr_NCO_SmallP ( void ) { Tune_NCO (5); }
	/* RIGHT ARROW */

/*=====================================================================*/

void	Incr_NCO_Big ( void ) {	Tune_NCO (2); }
	/* UP ARROW */

/*=====================================================================*/

void	Incr_NCO_BigP ( void ) {	Tune_NCO (10); }
	/* UP ARROW */

/*=====================================================================*/

void	Decr_NCO_Small ( void ) { Tune_NCO (-1); }
	/* RIGHT ARROW */

/*=====================================================================*/

void	Decr_NCO_SmallP ( void ) { Tune_NCO (-5); }
	/* RIGHT ARROW */

/*=====================================================================*/

void	Decr_NCO_Big ( void ) { Tune_NCO (-2); }
	/* DOWN ARROW */

/*=====================================================================*/

void	Decr_NCO_BigP ( void ) { Tune_NCO (-10); }
	/* DOWN ARROW */

/*=====================================================================*/

void	Tune_NCO (int change) {
	extern double NCO_Frequency;
	extern int RXTuneCnt, Delta2, cursor_position;
	extern Flag_type Flag;
	cursor_position += change;
	if (cursor_position < 0) cursor_position = 0;
	if (cursor_position > 0x7F) cursor_position = 0x7F;
	Flag.RXTune = 1;
	RXTuneCnt = 0;
	NCO_Frequency = 500. + ((double) cursor_position) * DELTAFREQ;
	Display_Freq ( );
	}	

/*=====================================================================*/

void	Acquire ( void ){
	extern double NCO_Frequency;
	extern int count;
	extern int char_count, cursor_position;
	extern unsigned int FFT_Output_Buffer[];
	long i, S1, S2, W;
	double delta;

		/* "END" is HotKey */ 
				Clear_Text_Display ();
				Reset_LCD_buffer ();
				Set_Display_XYAddr (7,0);
				/* this is where I  add a correction to the NCO frequency
					based on the nearby spectral peaks */
				S1 = 0;
				S2 = 0;
				delta = 0.;
				for (i=-2; i<3; i++){
					W = (long)FFT_Output_Buffer[cursor_position + 32 + i];
					S1 += W*i;
					S2 += W;
				} 
				if (S2 != 0) delta = (double) S1/((double)S2);
				if (m_PSKmode != RTTY_MODE) {
					NCO_Frequency +=  delta * DELTAFREQ;
				}
				SetRXFrequency (NCO_Frequency );
				count = 0;
				char_count = 0;
				}

/*=====================================================================*/

void	Toggle_RnT ( void ) { 
	/* F10 HotKey */
	extern Flag_type Flag;
	extern double NCO_Frequency;
	extern double m_NCOphzinc;
	extern double m_PSKPhaseInc;
	extern double tempPHZ;
	extern int Cw_On, m_TXState, m_Ramp;
	extern int m_NeedCWid;
	extern int Itbl_num, Qtbl_num;
	extern int *pADC_Buffer;
	extern int count;
	extern int char_count;
	extern int byte_count;
	extern uchar RegCount;

		IEC0bits.T1IE = 0;  /* turn off timer 1 interrupts */
		switch (m_TXState) {
			case TX_OFF_STATE:
				if (Cw_On == TRUE) m_NeedCWid = TRUE;
				Set_Display_TX ();
				Reset_LCD_buffer ();
				Clear_Text_Display ();
				Set_Display_XYAddr (7,0); 
				Display_Text_Cursor ();
				free(pADC_Buffer);
				m_PSKPhaseInc = m_NCOphzinc; /* set TX frequency same as RX frequency */
				tempPHZ = m_NCOphzinc;
				m_TXState = TX_PREAMBLE_STATE; 
 				PTT_On = 1; 
				Itbl_num = 0;
				Qtbl_num = 0;
				m_Ramp = 0;
				RegCount = 0;
				Clear_Key_FIFO ();
				Clear_Temp_FIFO ();
				ClrQue ();
				char_count = 0;
				if (Status.LOGGING) {
					PutUartChar ('\r', CHK);
					PutUartChar ('\n', CHK);
				}
				break;
			case TX_SENDING_STATE:
			case TX_PREAMBLE_STATE:
				m_TXState = TX_POSTAMBLE_STATE;
				ResetDetector ();
				m_NCOphzinc = tempPHZ;
				Clear_Text_Display ();
				/* turn off blinking cursor here */
				Reset_LCD_buffer ();
				pADC_Buffer = (int *)malloc(ADC_BUFFER_LENGTH2*sizeof(int)); /* allocate buffer */
				char_count = 0;
				Set_Display_XYAddr (7,0); 
				Output_DAC_1A (0x800);
				if (Status.LOGGING){
					PutUartChar ('\r', CHK);
					PutUartChar ('\n', CHK);
				}
				break;
			case TX_TUNE_STATE:
			case TX_CWID_STATE:
			case TX_MACRO_REC:
				break;

		}
		IEC0bits.T1IE = 1;  /* turn on timer 1 interrupts */
	}

/*=====================================================================*/

void	Toggle_File_Input ( void ) { 
	/*  Ctrl-I HotKey */
	extern Status_type Status;

	if (Status.LOGGING) {
		if (m_TXState == TX_OFF_STATE) {
			m_TXState = TX_PAUSED_STATE; 
			Buffered_Text_Capture (FILEINPUT);
		}
	}
}

/*=====================================================================*/
/*  End Macro Recording and Save Macro */
void	Save_Macro ( ) {
	/* called by F9 */
	extern int Record_Address;
	extern int char_count;
	extern Flag_type Flag;	
	}

/*=====================================================================*/
/*  Initiate Macro recording */
void	Record_Macro ( void ) {
	/* called by Ctrl-F(n) */
	extern int LastScanCode;
	extern int Record_Address;
	extern Flag_type Flag;
	m_TXState = TX_MACRO_REC;
	Flag.MacRec = TRUE;
	Record_Address = Get_Addr(LastScanCode); /* based on Function key */
	Last_Address = Record_Address;
	Buffered_Text_Capture (MACRO);
}
/*=====================================================================*/

void	Play_Macro ( void ) {
	/* called by Fn key */
	extern int LastScanCode;
	extern int m_TXState;
	int i,addr;
	char ch;
	addr = Get_Addr(LastScanCode);
	if (m_TXState == TX_OFF_STATE) {
		ch = I2C1_Byte_Read (addr);
		if (ch == TXON_CODE) {
			Toggle_RnT ();
			addr++;
		}
		else return;
	}
	if ((m_TXState == TX_SENDING_STATE) | (m_TXState == TX_PREAMBLE_STATE)){
		for (i=0; i<255; i++){	
			ch = I2C1_Byte_Read (addr+i);
			switch (ch) {
				case GET_MYCALL:
					Insert_MyCall();
					break;
				case GET_THEIRCALL:
					Insert_TheirCall();
					break;
				case GET_BEACON_COUNT:
					Insert_Beacon_Count();
					break;
				case '\0':
					return;
				default:
					PutTxQue(ch,FALSE);
					break;
			}
		}
	}
}
/*=====================================================================*/

void	Erase_Macro ( void ) {
	/* called by Alt-Fn key */
	extern int LastScanCode;
	int addr;
	addr = Get_Addr(LastScanCode);
	I2C1_Byte_Write (addr,'\0');
}
/*=====================================================================*/
int	Get_Addr ( int code ) {
	/* return EEPROM address based on last Function key scancode */
	switch (code){
		case 0x05:
			return (0x100);
		case 0x06:
			return (0x200);
		case 0x04:
			return (0x300);
		case 0x0C:
			return (0x400);
		case 0x03:
			return (0x500);
		case 0x0B:
			return (0x600);
		case 0x83:
			return (0x700);
		case 0x0A:
			return (0x800);
		default:
			return (0);
	}
}

/*=====================================================================*/

void	Incr_PGA_Gain ( ) {
	++PGA;
	if (PGA>3) PGA = 3;
		Set_PGA1_Gain (PGA);
	}	

/*=====================================================================*/

void	Decr_PGA_Gain ( ) {
	--PGA; 
	if (PGA < 0) PGA = 0;
		Set_PGA1_Gain (PGA);
	}

/*=====================================================================*/

/*	000 = Gain of +1 
	001 = Gain of +2
	010 = Gain of +4
	011 = Gain of +5
	100 = Gain of +8
	101 = Gain of +10
	110 = Gain of +16 (Default)
	111 = Gain of +32	*/

/*=====================================================================*/		

void	Set_PGA1_Gain	(	int gain_index )	{

	int	temp;
	int gain;

	switch (gain_index) {
		case 0:
			gain = x1;
			break;
		case 1:
			gain = x5;
			break;
		case 2:
			gain = x16;
			break;
		case 3:
			gain = x32;
			break;
		default:
			break;
	}

	MCP6S21_CS1	=	0;	
	
	SPI2BUF		=	(  ( gain & 0x0007 ) | 0x4000  );
	
	while ( !SPI2STATbits.SPIRBF );
	
	temp		=	SPI2BUF;
	
	MCP6S21_CS1	=	1;

	}

/*=====================================================================*/

void	Clear_Buffers ( ) {
	ClrQue();
	}

/*=====================================================================*/

void	Show_EEPROM ( ) {
	/* F11 HotKey */
	int i, addr;
	char ch;
	addr = 0x20;
	for (i=0; i<0x10; i++) {
	ch = I2C1_Byte_Read(addr++);
	if (ch == '\0') return;
	Write_Char (ch, 0);
	}
}

/*=====================================================================*/

	void Insert_ON ( void ) {
	extern int Record_Address;
	if (m_TXState == TX_MACRO_REC) {
		I2C1_Byte_Write(Record_Address++ , TXON_CODE );
	}
}

/*=====================================================================*/

	void Insert_OFF ( void ) {
	extern int Record_Address;
	if (m_TXState == TX_MACRO_REC) {
		I2C1_Byte_Write(Record_Address++ , TXOFF_CODE );
	}
	if (m_TXState == TX_SENDING_STATE) {
		PutTxQue (TX_CNTRL_AUTOSTOP, TRUE);
	}
}

/*=====================================================================*/

	void Toggle_Tune ( void ) {
	extern Flag_type Flag;
	extern int m_TXState;
	extern double m_PSKPhaseInc;
	extern double m_NCOphzinc;
		IEC0bits.T1IE = 0; /*  turn off timer 1 interrupts */
	switch (m_TXState) {
		case TX_OFF_STATE:
			Display_Tune ();
			PTT_On = 1; 
			m_PSKPhaseInc = m_NCOphzinc;
			m_TXState = TX_TUNE_STATE;
			break;
		case TX_TUNE_STATE:
			PTT_On = 0; 
			/* turn off blinking cursor */
			m_TXState = TX_OFF_STATE;
			Output_DAC_1A (0x800);
			break;
	}
		IEC0bits.T1IE = 1; /*  turn on timer 1 interrupts */
}

/*======================================================================*/

void End_Transmission ( void ) {
	m_TXState = TX_OFF_STATE;
	PTT_On = 0; 
}

/*======================================================================*/

void 	Save_TheirCall ( void ) { /* called with Ctrl-T */

	Buffered_Text_Capture ( THEIRCALL);
}

/*======================================================================*/

void 	Save_FileName ( void ) { /* called with Ctrl-N */

	Buffered_Text_Capture (FILENAME);
}

/*======================================================================*/

void	Save_MyCall ( void ) { /* called with Ctrl-M */

	Buffered_Text_Capture (MYCALL);
}

/*======================================================================*/

void	Write_CWId ( void ) {
	int EE_Addr;
	char ch;
	extern char m_CWIdString[];

	/* copy callsign to CWIdString */ 
	EE_Addr = 0x20;
	while ((ch = I2C1_Byte_Read (EE_Addr)) != '\0') {
		m_CWIdString[EE_Addr-0x20] = ch;
		EE_Addr++;
	}
	m_CWIdString[EE_Addr-0x20] = '\0';

}
/*======================================================================*/

void 	Save_Freq ( void ) { /* called with Ctrl-F */
	extern double NCO_Frequency;
	extern u_val last_freq;
	int i;
	int EE_Addr = 0x30;
	last_freq.f = NCO_Frequency;
	/* we need to record 4 bytes */
	for (i=0; i<4; i++) I2C1_Byte_Write (EE_Addr++, last_freq.c[i]);
}
/*======================================================================*/

void 	Retrieve_Freq ( void ) {
	extern double NCO_Frequency, m_NCOphzinc, m_PSKPhaseInc;
	extern u_val last_freq;
	int i;
	int EE_Addr = 0x30;
	/* we need to record 4 bytes */
	for (i=0; i<4; i++) last_freq.c[i] = I2C1_Byte_Read (EE_Addr++);
	NCO_Frequency = last_freq.f;
	Acquire();
}

/*======================================================================*/

void 	Paste_Kybd_to_XmitBfr ( void ) { /* called with Ctrl-P */
	char ch;
	while ((ch = Pull_Temp_FIFO()) ) PutTxQue (ch,FALSE);
}

/*======================================================================*/

void	Insert_TheirCall ( void ) { 
	int i, temp;
	for (i=0; i<0x10; i++) {
		temp = CallSign[i];
		if (temp == '\0') break;
		PutTxQue ( CallSign[i], FALSE );
	}
}

/*======================================================================*/

void	Insert_MyCall ( void ) { 
	int EE_Addr = 0x23;
	char ch;
	while ((ch = I2C1_Byte_Read (EE_Addr++)) != '\0') PutTxQue(ch,FALSE);
}

/*======================================================================*/

void	Insert_Beacon_Count ( void ) { 
	unsigned char BC_Bfr[6];
	extern int Beacon_Count;
	char ch;
	int i;
	sprintf (BC_Bfr, "%i", Beacon_Count);
	for (i=0; i<6; i++) {
		ch = BC_Bfr[i];
		if (!ch) break;
		PutTxQue(ch,FALSE);
	}
}

/*======================================================================*/

void	Put_My_Call ( void ) { /* called with Alt-M */
	extern int Record_Address;
	if (m_TXState == TX_MACRO_REC) {
		I2C1_Byte_Write(Record_Address++ , GET_MYCALL );
	}
}

/*======================================================================*/

void	Put_Their_Call ( void ) { /* called with Alt-T */
	extern int Record_Address;
	if (m_TXState == TX_MACRO_REC) {
		I2C1_Byte_Write(Record_Address++ , GET_THEIRCALL );
	}
}

/*======================================================================*/

void	Put_Beacon_Count ( void ) { /* called with Alt-K */
	extern int Record_Address;
	if (m_TXState == TX_MACRO_REC) {
		I2C1_Byte_Write(Record_Address++ , GET_BEACON_COUNT );
	}
}

/*======================================================================*/

void	Set_RnT_Flag ( void ) { /* set flag to call Toggle_Rnt */
	extern Flag_type Flag;
	Flag.TglRnT = TRUE;
}

/*======================================================================*/

void	Set_Tune_Flag ( void) { /* set flag to call Toggle_Tune */
	extern Flag_type Flag;
	Flag.TglTune = TRUE;
}

/*======================================================================*/

void	Proc_QEI ( void ) {	/* process approx every 50ms */
	extern double NCO_Frequency;
	extern double m_NCOphzinc;
	extern Flag_type Flag;
	extern int RXTuneCnt, cursor_position, Delta1;
	extern Status_type Status;
	extern unsigned char Press_cnt;

	if (!Status.CONFIG) {
		if (POSCNT != 100){
			if (POSCNT <99) cursor_position -= Delta1;
			if (POSCNT >101) cursor_position += Delta1;
			if (cursor_position < 0) cursor_position = 0;
			if (cursor_position > 0x7F) cursor_position = 0x7F;
			NCO_Frequency = 500. + ((double) cursor_position) * DELTAFREQ;
			Flag.RXTune = 1;
			RXTuneCnt = 0;
			Flag.DispFreq = 1;
			POSCNT = 100;
		}
	}

		if (!Button1) ++Press_cnt; 	/* button pressed? */
		else { /* button has been released */
			if ((Press_cnt >= 2) && (Press_cnt < 10)) Status.TAP = 1; 
			if (Press_cnt >= 10) Status.HOLD = 1;
			Press_cnt = 0;
	
		}
}

/*======================================================================*/

void	Toggle_BackLight ( void ) {

		BackLight = ~BackLight; /* toggle the backlight */
		BL = BackLight;
	}

/*======================================================================*/

void	Configure ( void ) {
	extern char Menu_Index;
	extern unsigned char Menu_Level;
	extern unsigned char Menu_Num;
	extern unsigned char Item_Select;
	extern unsigned char Press_cnt;
	unsigned char tempMODE;
	unsigned char tempSQTH;
	unsigned char tempPGA;
	unsigned char tempBL;
	unsigned char tempCWID;
	extern unsigned int BeaconInterval;
	
	int i;	
	#define Blank "                    "
	#define StringSU "CONFIGURE"
	#define String00 "Exit"
	#define String01 "Mode"
	#define String02 "Squelch Threshold"
	#define String03 "CWID"
	#define String04 "PGA Gain"
	#define String05 "Tune Rate"
	#define String06 "Battery Voltage"
	#define String07 "TX Audio Level"
	#define String08 "Backlight"
	#define String09 "Flash Bootload"
	#define String0A "PC Bootload"
	#define String0B "Beacon Interval"
	#define String0C "Save Config"
	#define String0D "Download Config"
	#define String0E "Upload Config"
	#define String10 "BPSK"
	#define String11 "QPSK"
	#define String12 "QPSK/R"
	#define String13 "MFSK16"
	#define String14 "RTTY"
	#define String15 "PSK63"
	#define String20 "50"
	#define String21 "25"
	#define String30 "OFF"
	#define String31 "ON"
	#define String40 "X1"
	#define String41 "X5"
	#define String42 "X16"
	#define String43 "X32"
	#define String50 "Normal"
	#define String51 "Fast"
	#define String91 "Start Bootload"
	#define StringA1 "Start Bootload"
	#define StringZ  "Abort"
	#define String92 "Start Download"
	#define StringA2 "Start Upload"

	tempMODE = mMODE;
	tempSQTH = SQTH;
	tempPGA = PGA;
	tempBL = BL;
	tempCWID = CWID;

	menu_start:
	Clear_Text_Display ();
	while (1) {
		Write_String_Positioned_LCD1_Display ( 4,0, StringSU );
		switch (Menu_Index) {
			case 0:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String00);
				break;
			case 1:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String01);
				break;
			case 2:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String02);
				break;
			case 3:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String03);
				break;
			case 4:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String04);
				break;
			case 5:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String05);
				break;
			case 6:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String06);
				break;
			case 7:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String07);
				break;
			case 8:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String08);
				break;
			case 9:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String09);
				break;
			case 0xA:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String0A);
				break;
			case 0xB:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String0B);
				break;
			case 0xC:
				Write_String_Positioned_LCD1_Display ( 5,0, Blank);
				Write_String_Positioned_LCD1_Display ( 5,0, String0C);
			default:
				break;
			case 0xD:  /* Download Config */
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, StringZ);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String92);
						break;
				}
				break;	
			case 0xE:  /* Upload Config */
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, StringZ);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, StringA2);
						break;
				}
				break;
		}
		for (i=0; i<4; i++) Delay_25ms();
	/* test POSCNT and BUTTON1 here */
		while (Button1 & (POSCNT == 100)) ; /* wait until a choice has been made */
		if (!Button1) {
			Menu_Num = Menu_Index;
			Menu_Index = 0;
			Press_cnt = 0;
			break; /* break from While loop */
		}
		if (POSCNT<99) --Menu_Index;
		if (POSCNT>101) ++Menu_Index;
		POSCNT = 100;
		Menu_Index = menu_index_mod_n (Menu_Index, MAXMENU);
	} /* loop back if Select Button not pressed */

	/* begin second menu loop */
	POSCNT = 100;
	for (i=0;i<10;i++) Delay_25ms();
	while (1) {
		switch (Menu_Num) {
			case 0: /* exit */
				Menu_Num = 0;
				Item_Select = 0;
				Menu_Level = 0;
				Menu_Index = 0;
				Clear_Text_Display ();
				Status.CONFIG = 0;
				return;
			case 1: /* Set Mode */
				Menu_Index = (tempMODE == RTTY_MODE)?3:tempMODE;
				Menu_Index = menu_index_mod_n (Menu_Index, 4);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String10);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String11);
						break;
					case 2:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String12);
						break;
					case 3:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String14);
						break; 
				}
				break;
			case 2:
				Menu_Index = tempSQTH;
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String20);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String21);
						break;
				}
				break;
			case 3:
				Menu_Index = tempCWID;
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String30);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String31);
						break;
				}
				break;
			case 4:
				Menu_Index = tempPGA;
				Menu_Index = menu_index_mod_n (Menu_Index, 4);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String40);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String41);
						break;
					case 2:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String42);
						break;
					case 3:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String43);
						break;
				}
				break;
			case 5:
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String50);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String51);
						break;
				}
				break;
			case 6:
				/* Read and display Battery Voltage */
				while (Button1) {	
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Display_Battery_Voltage (6,0);
						Delay_25ms();
						Delay_25ms();
				}
						break;
			case 7:
				/* Read and display TX Audio Level */
				while (Button1) {
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Display_TX_Audio_Level ();
						Delay_25ms();
				}
						break;
			case 8:
				Menu_Index = tempBL;
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String30);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String31);
						break;
				}
				break;
			case 9:
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, StringZ);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, String91);
						break;
				}
				break;
			case 0xA:
				Menu_Index = menu_index_mod_n (Menu_Index, 2);
				switch (Menu_Index) {
					case 0:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, StringZ);
						break;
					case 1:
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Write_String_Positioned_LCD1_Display ( 6,0, StringA1);
						break;
				}
				break;
			case 0xB:  /* Select Beacon Interval */
				while (Button1) {
						Write_String_Positioned_LCD1_Display ( 6,0, Blank);
						Get_New_Beacon_Delay ();
						Delay_25ms();
				}
						PR7 = (unsigned int) ((float)BeaconInterval * 2.38419);
						PR6 = (unsigned int) ((unsigned long) (BeaconInterval*156250) - (unsigned long) (PR7 * 65536));
						break;		
			case 0xC:
				Save_Config ();
				Menu_Num = 0;
				Item_Select = 0;
				Menu_Level = 0;
				Menu_Index = 0;
				Clear_Text_Display ();
				Status.CONFIG = 0;
				return;		
		}
		for (i=0; i<4; i++) Delay_25ms();
	/* test POSCNT and BUTTON1 here */
		while (Button1 & (POSCNT == 100)); /* wait until a choice has been made */
		if (!Button1) {
			Item_Select = Menu_Index;
			Press_cnt = 0;
			break; /* break from While loop */
		}
		if (POSCNT<99) --Menu_Index;
		if (POSCNT>101) ++Menu_Index;
		POSCNT = 100;
		switch (Menu_Num) {
			case 0: 
				break;
			case 1:
				tempMODE = Menu_Index;
				break;
			case 2:
				tempSQTH = Menu_Index;
				break;
			case 3:
				tempCWID = Menu_Index;
				break;
			case 4:
				tempPGA = Menu_Index;
				break;
			case 8:
				tempBL = Menu_Index;
				break;
			default:
				break;
		}

	} /* loop back if Select Button has not been tapped */
 
	/* we've finished selecting, so process selection */
			Process_Menu_Select (Menu_Num, Item_Select);
			Menu_Num = 0;
			Item_Select = 0;
			Menu_Level = 0;
			Menu_Index = 0;
			Clear_Text_Display ();
			for (i=0;i<10;i++) Delay_25ms();
  /* check to see if other selections are desired */
	goto menu_start;
}
/*======================================================================*/

void Process_Menu_Select (unsigned char menu, unsigned char item) {
	
	extern int m_PSKmode, Cw_On, Delta1;
	extern int cursor_position;
	extern double m_SymbolRate;
	extern double m_PSKPeriodUpdate;
	extern char Menu_Index;
	extern unsigned char Menu_Level;
	extern unsigned char Menu_Num;
	extern unsigned char Item_Select;
	
	switch (menu) {
		case 0:
			break;
		case 1:
			switch (item) {
				case 0:
					/* m_PSKmode = BPSK_MODE; */
					ResetModem (BPSK_MODE);
					mMODE = BPSK_MODE;
				/*	m_SymbolRate = SYMBOL_RATE31; */
					break;
				case 1:
					/* m_PSKmode = QPSKU_MODE; */
					ResetModem (QPSKU_MODE);
					mMODE = QPSKU_MODE;
				/*	m_SymbolRate = SYMBOL_RATE31; */
					break;
				case 2:
					/* m_PSKmode = QPSKL_MODE; */
					ResetModem (QPSKL_MODE);
					mMODE = QPSKL_MODE;
				/*	m_SymbolRate = SYMBOL_RATE31; */
					break;
				case 3:
					/* m_PSKmode = RTTY_MODE; */
					ResetModem (RTTY_MODE);
					mMODE = RTTY_MODE;
				/*	m_SymbolRate = SYMBOL_RATERTTY; 
					m_PSKPeriodUpdate = 1/m_SymbolRate; */
					break;  
			}
			break;
		case 2:
			if (!item) SetSquelchThresh (50,20);
			else SetSquelchThresh (25,20);
			SQTH = item;
			break;
		case 3:
			if (!item) Cw_On = FALSE;
			else Cw_On = TRUE;
			CWID = item;
			break;
		case 4:
			Set_PGA1_Gain (item);
			PGA = item;
			break;
		case 5:
			if (!item) Delta1 = 1;
			else Delta1 = 2;
			break;
		case 6:
			/* Measure and display battery voltage */
			break;
		case 7:
			/* Measure and display TX Audio level */
			break;
		case 8:
			if (!item) LATCbits.LATC14 = 0;
			else LATCbits.LATC14 = 1;
			BL = item;
			break;
		case 9:
			if (item) {
			Flash_Bootload ();
			}
			break;
		case 0xA:
			if (item) {
			PC_Bootload ();
			}
			break;
		case 0xB:
			/* Set Beacon Interval */
			break;
		case 0xC:
			/* Save Config */
			break;
	}
	Clear_Cursor ();
	if (m_PSKmode != RTTY_MODE) Display_Cursor (cursor_position);
	else Display_RTTYCursor ();
}

/*======================================================================*/

void	Display_Battery_Voltage ( uchar Xpos, uchar Ypos ) {
	char buffer[20];
	double BatteryVoltage,NEWBV;
	static double OLDBV;
	/* Get ADC data */
	AD2CHS0bits.CH0SA 	=	1;	/* select AN1 */ 
	while (!AD2CON1bits.DONE);
	AD2CON1bits.DONE = 0;
	NEWBV = (double)ADC2BUF0*KB;
	BatteryVoltage = 0.9*OLDBV + 0.1*NEWBV;
	OLDBV = NEWBV;
	sprintf(buffer, " Volts:  %3.1f", BatteryVoltage + 0.3);
	Write_String_Positioned_LCD1_Display (Xpos,Ypos,buffer);
	Delay_25ms();
	Delay_25ms();
	/* Set_Display_XYAddr (7,0); */
}

/*======================================================================*/

void	Display_TX_Audio_Level ( ) {
	char buffer[20];
	double TXAudioLevel,NEWAL;
	static double OLDAL;
	/* Get ADC data */
	AD2CHS0bits.CH0SA 	=	2;	/* select AN2 */ 
	while (!AD2CON1bits.DONE);
	AD2CON1bits.DONE = 0;
	NEWAL = (double)ADC2BUF0*KV; 
	TXAudioLevel = 0.9*OLDAL + 0.1*NEWAL;
	OLDAL = NEWAL;
	sprintf(buffer, " %%=  %3.0f", TXAudioLevel);
	Write_String_Positioned_LCD1_Display (6,0,buffer);
	Delay_25ms();
	Delay_25ms();
	/*Set_Display_XYAddr (7,0); */
}

/*======================================================================*/

void	Get_New_Beacon_Delay ( ) {
	char buffer[20];
	extern unsigned int BeaconInterval;
	int i;

	/* start with default interval */
/*	while (1) { */
		sprintf (buffer, " %u seconds   ", BeaconInterval);
		Write_String_Positioned_LCD1_Display (6,0,buffer);
		for (i=0;i<4;i++) Delay_25ms();
/*		while (Button1 & (POSCNT == 100)) ;  wait until PB pressed or dial turned 
		if (!Button1) {
			break; 							 break from While loop 
		} */
		if (POSCNT<99)
			if (BeaconInterval > 4) BeaconInterval-=5;
		if (POSCNT>101) 
			if (BeaconInterval < 13700) BeaconInterval+=5;
		POSCNT = 100;
/*	} */
}

/*======================================================================*/

unsigned char menu_index_mod_n ( char menuindex, unsigned char lim ) {
	if (menuindex <0) return menuindex + lim;
	if (menuindex >= lim) return menuindex - lim;
	return menuindex;
	}
/*======================================================================*/

void	Save_Config  ( void ) {
	extern char CFG1, CFG2;
	I2C1_Byte_Write (0x00, 's');
	CFG1 = (PGA<<4) + mMODE;
	CFG2 = (BL<<5) + (SQTH<<2) + (AFC<<1) + CWID;
	I2C1_Byte_Write (0x01, CFG1);
	I2C1_Byte_Write (0x02, CFG2);
	}	
/*======================================================================*/

void	Restore_Config ( void ) {
	extern int m_AFCmode, Cw_On;
	extern char CFG1,CFG2;

	AUTO = (I2C1_Byte_Read (0x0));
	if (AUTO == 's') {
		CFG1 = I2C1_Byte_Read (0x01);
		CFG2 = I2C1_Byte_Read (0x02);

		PGA = (CFG1 & 0x70)>>4;
		mMODE = CFG1 & 0x0F;
		BL = (CFG2 & 0x20)>>5;
		SQTH = (CFG2 & 0x0C)>>2;
		AFC = (CFG2 & 0x02)>>1;
		CWID = (CFG2 & 0x01);

		/* now we need to set configuration variables from this retrieved data */
		Set_PGA1_Gain (PGA);
		ResetModem (mMODE);
		LATCbits.LATC14 = BL;
		m_AFCmode = AFC;
		Cw_On = CWID;
		if (SQTH ==1)	SetSquelchThresh ( 50 , 20 );
		else SetSquelchThresh ( 25 , 20 );
	}}	
/*======================================================================*/
void	Display_Settings ( void ) {
	/* show current saved parameters */
	int i;
	char ch;
	Clear_Text_Display ();
		switch (mMODE) {
			case 0:
				Write_String_Positioned_LCD1_Display ( 4,0, "Mode  BPSK");
				break;
			case 1:
				Write_String_Positioned_LCD1_Display ( 4,0, "Mode  QPSK");
				break;
			case 2:
				Write_String_Positioned_LCD1_Display ( 4,0, "Mode  QPSK/R");
				break;
			case 6:
				Write_String_Positioned_LCD1_Display ( 4,0, "Mode  RTTY");
				break;
		}
		if (SQTH) Write_String_Positioned_LCD1_Display ( 5,0, "SqThr 25");
		else Write_String_Positioned_LCD1_Display ( 5,0, "SqThr 50");

		if (CWID) Write_String_Positioned_LCD1_Display ( 6,0, "CwId  ON");
		else Write_String_Positioned_LCD1_Display ( 6,0, "CwId  OFF");

		switch (PGA) {
			case 0:
				Write_String_Positioned_LCD1_Display ( 7,0, "PGA   X1");
				break;
			case 1:
				Write_String_Positioned_LCD1_Display ( 7,0, "PGA   X5");
				break;
			case 2:
				Write_String_Positioned_LCD1_Display ( 7,0, "PGA   X16");
				break;
			case 3:
				Write_String_Positioned_LCD1_Display ( 7,0, "PGA   X32");
				break;
		}
	while ( !(ch=Pull_Key_FIFO() ) );
	Clear_Text_Display ();
}

/*======================================================================*/

void	Toggle_Logging ( void ) {
	extern Status_type Status;
	extern int byte_count;
	extern int Timer_Count;
	int i, temp;
	int EE_Addr;
	char UartDat;
	uchar ch;
		if (!RXD) { 		/* RXD will be low if nothing is attached */
			Beep();
			return;
		} 
		Status.LOGGING = ~Status.LOGGING;
		if (Status.LOGGING) {
			Display_Log ();
			Timer_Count = 0;
			UartDat = '\0';
			PutUartChar ('\r', NCHK); /* send a CR */			
			while (UartDat != '>') {
				temp = GetUartChar (&UartDat);
				if (temp != 0){
					if (temp == 1) Write_String_Positioned_LCD1_Display ( 5,0, "Overrun Error"); 					
					if (temp == 2) Write_String_Positioned_LCD1_Display ( 5,0, "Framing Error");
					if (temp == 3) Write_String_Positioned_LCD1_Display ( 5,0, "Timeout Error");	
					Beep();
					Status.LOGGING = ~Status.LOGGING;
					Clear_Log ();
					return;
				}
			}	
			EE_Addr = 0x51;
			PutUartString ("OPW ", NCHK);
			/* send filename */
			while (ch = I2C1_Byte_Read (EE_Addr++)) PutUartChar(ch, NCHK); 
			PutUartChar ('\r', NCHK);
			byte_count = 0;
			PutUartString ("WRF 20\r", NCHK); 
		}
		else {
			Clear_Log ();
			PutUartChar ('\r', CHK);
			PutUartChar ('\n', CHK);
			PutUartChar ('\r', CHK);
			PutUartChar ('\n', CHK);
			for (i=0; i<(20-byte_count); i++) PutUartChar (' ', NCHK);
			EE_Addr = 0x51;
			PutUartString ("CLF ", NCHK);
			/* send filename */
			while (ch = I2C1_Byte_Read (EE_Addr++)) PutUartChar(ch, NCHK); 
			PutUartChar ('\r', NCHK);
		}
}

/*======================================================================*/

void Buffered_Text_Capture ( uchar mode ) {
	char lcd_buffer[60];
	extern char Capture_buffer[256];
	int hpos; /* position along bottom line of LCD */
	int bpos; /* position within lcd buffer */
	extern int cpos; /* position within capture buffer */
    int error;
	int i,j;
	int EE_Addr;
	extern int maxchar;
	int dat;
	char ch;
	extern int LastScanCode;
	extern int Record_Address;
	extern int cursor_position;
	extern Flag_type Flag;
	IEC0bits.T1IE = 0; /*  turn off timer 1 interrupts */
    restart:	
	/* Display headers/instructions at top of LCD based on mode */
	Clear_Display (0);
	bpos = 0;
	cpos = 0;
	hpos = 0;
	error = 0;
	for (i=0; i<60; i++) lcd_buffer[i] ='\0';
	Clear_Key_FIFO ();
	switch (mode) {
	       case MYCALL:
			   if (m_TXState != TX_OFF_STATE) return;
               maxchar = 0x10;
               Write_String_Positioned_LCD1_Display (0,1, "MYCALL?"); 
               Write_String_Positioned_LCD1_Display (1,1, "(16 chars max)");               
               break;                 
           case THEIRCALL:
			   if (m_TXState != TX_OFF_STATE) return;
               Write_String_Positioned_LCD1_Display (0,1, "THEIRCALL?"); 
               Write_String_Positioned_LCD1_Display (1,1, "(16 chars max)");                
               maxchar = 0x10; 
               break; 
           case FILENAME:
			   if (m_TXState != TX_OFF_STATE) return;
               Write_String_Positioned_LCD1_Display (0,1, "FileName?"); 
               Write_String_Positioned_LCD1_Display (1,1, "(8.3 format)");                
               maxchar = 0x0C; 
               break; 
           case FILEINPUT:
			   if (m_TXState != TX_PAUSED_STATE) return;
               Write_String_Positioned_LCD1_Display (0,1, "Enter Text"); 
               Write_String_Positioned_LCD1_Display (1,1, "(250 chars max)");                
               maxchar = 0xFF;
			   Add_CRLF_Capture_Buffer ( );
               break;
           case MACRO:
			   if (m_TXState != TX_MACRO_REC) return;
               Write_String_Positioned_LCD1_Display (0,1, "Macro Text?"); 
               Write_String_Positioned_LCD1_Display (1,1, "(255 chars max)");                
               maxchar = 0xFF;              
               break; 
    }
			Set_Display_XYAddr ( 7,0 ) ;
			Display_Text_Cursor ();      
	
	while (1) {
     	/* while (!(dat = Pull_Key_FIFO())) ;  wait for a character */
		while (1) { /* check every 50ms for a character */
			if (dat = Pull_Key_FIFO () ) break;
			Delay_25ms();
			Delay_25ms();
		}
     	if (dat == 0x1A) { /* got a Ctrl-Z character */
			if (mode == FILEINPUT) {
				/* add CRLF to capture buffer */
				Capture_buffer[cpos++] = '\r';
				if (cpos>maxchar) {
					error = TRUE;
					break;
				}
				Capture_buffer[cpos++] = '\n';
				if (cpos>maxchar) {
					error = TRUE;
					break;
				}
			}
			Capture_buffer[cpos] = '\0';
			break; /* go to final processing */
		}
		if (dat == 0x1B) { /* got an ESC character so abort */
			cpos = 0;
			bpos = 0;
			hpos = 0;
			Clear_Display (0);
			m_TXState = TX_OFF_STATE;
			Flag.MacRec = FALSE;
			Clear_Key_FIFO ();
			Clear_Cursor ();
			if (m_PSKmode != RTTY_MODE) Display_Cursor (cursor_position);
			else Display_RTTYCursor ();
			IEC0bits.T1IE = 1; /*  turn on timer 1 interrupts */
			return;
		}
		if (dat == 8) { 	/* got a backspace */
			Write_Char (0x20, 0);
			hpos--;
			cpos--;
			cpos = (cpos<0)? 0:cpos;
			if (hpos<0) { 
				Set_Display_XYAddr (6,6*(hpos + 20));
				Display_Text_Cursor ();
				Set_Display_XYAddr (6,6*(hpos + 20));
			}
			else {
				Set_Display_XYAddr (7,6*hpos);
				Display_Text_Cursor ();
				Set_Display_XYAddr (7,6*hpos);
			}
		}
		else { /* Not Ctrl-Z, ESC, or backspace */
            Capture_buffer[cpos++] = (char)dat; 
            if (cpos > maxchar) {
				error = TRUE;            
				break;
			}
			Write_Char ( (char)dat, 0 );
			Display_Text_Cursor ();

			/* don't worry about buffer, if on bottom line */
			if (bpos >= 0) lcd_buffer[bpos] = (char) dat;
			bpos++;
			bpos %= 60;
			hpos++;
			if (hpos == 0) Set_Display_XYAddr(7,0 ) ;
			if (hpos == 20) {
				hpos = 0;
				Clear_Display (4);
				for (j=0; j<3; j++) {
					Set_Display_XYAddr (j+4,0);
					for (i = 0 ; i < 20 ; i++) {
						Write_Char (lcd_buffer[(bpos+i+j*20) % 60], 0);
					}
 				}
				/* reset to beginning of line */
				Set_Display_XYAddr ( 7,0 ) ;
				Display_Text_Cursor ();
			}
		}
	}
	if (error) {
       	Clear_Display(0);
       	Write_String_Positioned_LCD1_Display (4,1, "ERROR");
		Beep();
	   	for (i=0;i<40;i++) Delay_25ms();
       	goto restart;
    }   
	/* now process buffers based on mode */
	i=0;
	switch (mode) {
	       case MYCALL:
				EE_Addr = 0x23;
				I2C1_Byte_Write (0x20, 'd');
				I2C1_Byte_Write (0x21, 'e');
				I2C1_Byte_Write (0x22, ' ');
				while ((ch = Capture_buffer[i++]) != '\0') I2C1_Byte_Write (EE_Addr++, ch);
				I2C1_Byte_Write (EE_Addr, '\0');
				Write_CWId ();
               	break;                 
           case THEIRCALL:
				while ((ch = Capture_buffer[i]) != '\0') CallSign [i++]= ch;
				CallSign [i] = '\0';				
               	break; 
           case FILENAME:
				EE_Addr = 0x51;
				while ((ch = Capture_buffer[i++]) != '\0') I2C1_Byte_Write (EE_Addr++, ch);
				I2C1_Byte_Write (EE_Addr, '\0');
                break; 
           case FILEINPUT:
 				Clear_Text_Display();
				while (ch = Capture_buffer[i++]) {
					Buffered_LCD_Write (ch);
					Delay_5ms(); /* need to pace the transmission so we don't overflow the uart */
				}
				m_TXState = TX_OFF_STATE;
               	break;
           case MACRO:
				while ((ch = Capture_buffer[i++]) != '\0') I2C1_Byte_Write (Record_Address++,ch);
				I2C1_Byte_Write (Record_Address,'\0');  /* Write end of message character */
				m_TXState = TX_OFF_STATE;
				Flag.MacRec = FALSE;
               	break; 
    }
	if (error) {
       	Clear_Display(0);
       	Write_String_Positioned_LCD1_Display (4,1, "ERROR");
		Beep();
	   	for (i=0;i<40;i++) Delay_25ms();
       	goto restart;
    }
	Clear_Display(4);
	Clear_Cursor ();
	if (m_PSKmode == RTTY_MODE){
	Display_RTTYCursor();
	}
	else {
	Display_Cursor(cursor_position);
	}
	Clear_Key_FIFO ();
	IEC0bits.T1IE = 1; /*  turn on timer 1 interrupts */	
}   
/*======================================================================*/

void Beep ( void ) {
	extern int Timer_Count;
	Timer_Count = 0;
	while (Timer_Count < 1000) Piezo = (Timer_Count%2) ;
	Piezo = 0;  /* make sure that the output level returns low. */
}
/*======================================================================*/

int Add_CRLF_Capture_Buffer ( void ) {
	extern char Capture_buffer[];
	extern int cpos;
	extern int maxchar;
	/* add CRLF to capture buffer */
	Capture_buffer[cpos++] = '\r';
	if (cpos>maxchar) {
		return (0);
	}
	Capture_buffer[cpos++] = '\n';
	if (cpos>maxchar) {
		return (0);
	}
	return (1);
}
/*======================================================================*/

void Init_Filename ( void ){
    char    *file;
    char    ch;
    int     i;
    int     Addr;
    file = "NUE_PSK.txt";
	if (I2C1_Byte_Read (0x50) == ' ') return;
	I2C1_Byte_Write (0x50, ' ');
    Addr = 0x51;
    while (ch = *file++) I2C1_Byte_Write(Addr++, ch);
    I2C1_Byte_Write(Addr, '\0');
}
/*======================================================================*/

void Flash_Bootload ( void ) {

	extern int * pADC_Buffer;
	
 	IEC0bits.T1IE = 0;		/* need to stop interrupts before calling bootloader */
	TIMER4 = 0;
	
	free(pADC_Buffer); 		/* now free up space for bootloader buffer */
	__asm__	("goto 0x014800"); 
}
/*======================================================================*/

void PC_Bootload ( void ) {
	
	extern int Timer_Count;
	char	UartDat;

	PutUartString ("SC S\r", NCHK);	/* issue Set Current command, specifying Slave device */

	USB_DSR = 0;           			/* drop DSR to initiate pass thru sequence */
	Delay_25ms();

 	IEC0bits.T1IE = 0;	   			/* need to stop interrupts before calling bootloader */
	TIMER4 = 0;

	/*  goto *bootloader;  */
	__asm__	("goto 0x015000");			

}

/*======================================================================*/

void	Beacon_ON (void) {
	extern int LastScanCode;

	T67IE = 1;
	Nop();
	T67ON = 1;
	LastScanCode = 0x83; /* scan code for F7 */
	Display_Bcn();		 /* display "BCN" at right side of display */
	Play_Macro();

}

/*======================================================================*/

void	Beacon_OFF (void) {
	extern int RunBeacon;

	RunBeacon = 0;
	T67IE = 0;
	Nop();
	T67ON = 0;
	Clear_Log();		/* clear the BCN indicator at the right side of display */

}

/*======================================================================*/

void	Toggle_Figures (void) {
	extern RTTY_type Rtty;

	Rtty.FIGURES = ~Rtty.FIGURES;	
	
}

/*======================================================================*/

void	RTTY_Reverse (void) {
	extern RTTY_type Rtty;

	Rtty.REVERSE = ~Rtty.REVERSE;	
	
}

/*======================================================================*/

void Download_Config ( void ) {
	char	UartDat;
	int	FieldNum, i, EE_Addr, all_done, field_done, temp;
	extern int Timer_Count;
	
	if (!RXD) { 							/* RXD will be low if USB card is not attached */
		Beep();
		return;
	} 
	Timer_Count = 0;						/* Test for flash drive being there */
	UartDat = '\0';
	PutUartChar ('\r', NCHK); 				/* send a CR */			
	while (UartDat != '>') {
		temp = GetUartChar (&UartDat);
		if (temp != 0){
			if (temp == 1) Write_String_Positioned_LCD1_Display ( 5,0, "Overrun Error"); 					
			if (temp == 2) Write_String_Positioned_LCD1_Display ( 5,0, "Framing Error");
			if (temp == 3) Write_String_Positioned_LCD1_Display ( 5,0, "Timeout Error");	
			Beep();
			return;
		}
	}
	PutUartString ("DIR CONFIG.TXT\r", NCHK);							/* check for file presence */
	GetUartChar (&UartDat);												/* first char in always a '\r' */
	GetUartChar (&UartDat);												/* next char in always a 'C' */
	GetUartChar (&UartDat);
	GetUartChar (&UartDat);
	if (UartDat == 'o') {												/* if 3rd char is 'o', then Command Failed */
		Beep ();
		return;
	}
	Delay_5ms ();														/* wait for the standard DIR response of file size */

	PutUartString ("OPR CONFIG.TXT\r", NCHK); 							/* Open the Config File for reading */
	FieldNum = 1;
	all_done = 0;
	field_done = 0;
	while (!all_done) 		{		   									/*  process data until all fields processed */
		Read_Flash_Char (&UartDat);
		switch (UartDat) {						
			case ':':
				switch (FieldNum) {
					case (0x01):					/* Field 1: MyCall */
						EE_Addr = 0x23;
						Read_Flash_Char (&UartDat);  					/* get rid of space after colon */
						while (!field_done) {
							Read_Flash_Char (&UartDat);
							if (UartDat == '\n') {
								I2C1_Byte_Write (EE_Addr++, 0x00);
								field_done = 1;
								break;
							}
							I2C1_Byte_Write (EE_Addr++,UartDat);
						}
						field_done = 0;
						FieldNum++ ;
						break;
					case (0x02):					/* Field 2: Log Fname */
						EE_Addr = 0x51;
						Read_Flash_Char (&UartDat);  					/* get rid of space after colon */
						while (!field_done) {
							Read_Flash_Char (&UartDat);
							if (UartDat == '\n') {
								I2C1_Byte_Write (EE_Addr++, 0x00);
								field_done = 1;
								break;
							}
							I2C1_Byte_Write (EE_Addr++,UartDat);
						}
						field_done = 0;
						FieldNum++;
						break;
					case (0x03):					/* Field 3: PGA */
						Read_Flash_Char (&UartDat);  					/* get rid of space after colon */
						while (!field_done) {
							Read_Flash_Char (&UartDat);
							if (UartDat == 'x') {
								Read_Flash_Char (&UartDat);
								switch (UartDat) {
									case '1':
										Read_Flash_Char (&UartDat);
										if (UartDat == '6') PGA = 2;   	/* type 2 = x16 */
										else PGA = 0;    				/* type 0 = x1 */
										break;
									case '3':
										PGA = 3;  						/* type 3 = x32 */
										break;
									case '5':
										PGA = 1;						/* type 1 = x5 */
										break;
									}
								field_done = 1;
								}
							else field_done = 1;	/* if initial char not an x, then abort */	
						}
						field_done = 0;
						FieldNum++ ;
						break;
					case (0x04):					/* Field 4: Mode */
						Read_Flash_Char (&UartDat);  					/* get rid of space after colon */
						while (!field_done) {
							Read_Flash_Char (&UartDat);
							switch (UartDat) {
								case 'B':
									mMODE = 0;   						/* type 0 = BPSK */
									break;
								case 'R':
									mMODE = 6;							/* type 6 = RTTY */
									break;
								case 'Q':
									Read_Flash_Char (&UartDat);  		/* read 'P' */
									Read_Flash_Char (&UartDat);			/* read 'S' */
									Read_Flash_Char (&UartDat);			/* read 'K' */
									Read_Flash_Char (&UartDat);		
									if (UartDat == '/') mMODE = 2;		/* type 2 = QPSK/R */
									else mMODE = 1;						/* type 1 = QPSK */
									break;
								}
							field_done = 1;
						}			
						field_done = 0;
						FieldNum++;
						break;
					case (0x05):					/* Field 5: Backlight */
						Read_Flash_Char (&UartDat);  		/* get rid of space after colon */
						Read_Flash_Char (&UartDat);  		/* get rid of 'O' */
						Read_Flash_Char (&UartDat);
						if (UartDat == 'N') BL = 1;			/* Type 1 = ON */
						else BL = 0;						/* Type 1 = OFF */
						BackLight = BL;
						field_done = 0;
						FieldNum++;
						break;
					case (0x06):					/* Field 6: Squelch */
						Read_Flash_Char (&UartDat);  		/* get rid of space after colon */
						Read_Flash_Char (&UartDat);
						if (UartDat == '2') SQTH = 1;		/* Type 1 = 25 */
						else SQTH = 0;						/* Type 0 = 50 */
						field_done = 0;
						FieldNum++;
						break;
					case (0x07):					/* Field 7: AFC */
						Read_Flash_Char (&UartDat);  		/* get rid of space after colon */
						Read_Flash_Char (&UartDat);  		/* get rid of 'O' */
						Read_Flash_Char (&UartDat);
						if (UartDat == 'N') AFC = 1;		/* Type 1 = ON */
						else AFC = 0;						/* Type 1 = OFF */
						field_done = 0;
						FieldNum++;
						break;
					case (0x08):					/* Field 8: CWID */
						Read_Flash_Char (&UartDat);			  /* get rid of space after colon */
						Read_Flash_Char (&UartDat);			  /* get rid of 'O' */
						Read_Flash_Char (&UartDat);
						if (UartDat == 'N') CWID = 1;		/* Type 1 = ON */
						else CWID = 0;						/* Type 1 = OFF */
						field_done = 0;
						FieldNum++;
						break;
					case (0x09):					/* Field 9: Macro 1 */
						Read_Macro_From_Flash (0x100);
						FieldNum++;
						break;
					case (0x0A):					/* Field 10: Macro 2 */
						Read_Macro_From_Flash (0x200);
						FieldNum++;
						break;
					case (0x0B):					/* Field 11: Macro 3 */
						Read_Macro_From_Flash (0x300);
						FieldNum++;
						break;
					case (0x0C):					/* Field 12: Macro 4 */
						Read_Macro_From_Flash (0x400);
						FieldNum++;
						break;
					case (0x0D):					/* Field 13: Macro 5 */
						Read_Macro_From_Flash (0x500);
						FieldNum++;
						break;
					case (0x0E):					/* Field 14: Macro 6 */
						Read_Macro_From_Flash (0x600);
						FieldNum++;
						break;
					case (0x0F):					/* Field 15: Macro 7 */
						Read_Macro_From_Flash (0x700);
						FieldNum++;
						all_done = 1;
						break;
					}
			default:
				break;
		}
	}
	PutUartString ("RDF 100\r", NCHK);			/* Exhaust the file, expect error */
	WaitForError ();
	PutUartString ("CLF CONFIG.TXT\r", NCHK);		/* Close the Config File */
}
/*======================================================================*/

void Read_Macro_From_Flash (int EE_Addr) {

	char	UartDat;
	int	FieldNum, i, field_done;

	Read_Flash_Char (&UartDat);						  /* get rid of space after colon */
	while (!field_done) {
		Read_Flash_Char (&UartDat);
		if (UartDat == '<') {
			Read_Flash_Char (&UartDat);  				 /* get char after '<' */
			switch (UartDat) {
				case 'M':
					I2C1_Byte_Write (EE_Addr++,0x0E);  /* <MYCALL> = 0E code */
					break;
				case 'T':
					Read_Flash_Char (&UartDat);				/* get char after 'T' */
					if (UartDat == 'H')
						I2C1_Byte_Write (EE_Addr++,0x0F);  /* <THEIRCALL> = 0F code */
					if (UartDat == 'X') {
						Read_Flash_Char (&UartDat);				/* read 3rd char */
						Read_Flash_Char (&UartDat);				/* read 4th char */
						if (UartDat == 'F')
							I2C1_Byte_Write (EE_Addr++,0x02);  /* <TXOFF> = 02 code */
						else 
							I2C1_Byte_Write (EE_Addr++,0x03);  /* <TXON> = 03 code */
					}
					break;
			}
			for ( i=0; i<13; i++) {					/* exhaust the <..> command characters */
				Read_Flash_Char (&UartDat);
				if (UartDat == '>') break;   		/* exit loop when end bracket found */
				if (i==12)	{
					Beep ();
					break;							/* or exit loop and beep when end bracket not found */
				}
			}
		}
		if (UartDat == '\n') {
			I2C1_Byte_Write (EE_Addr++, 0x00);
			field_done = 1;
			break;
		}
		if (UartDat != '>')  
			I2C1_Byte_Write (EE_Addr++,UartDat);
	}
	field_done = 0;
}

/*======================================================================*/

int	Read_Flash_Char ( char * ptrChar ) {

	char UartDat;
	PutUartString ("RDF 1\r", NCHK);
	GetUartChar(&UartDat);
	* ptrChar = UartDat;
}

/*======================================================================*/

void WaitForError ( void ) {
	char UartDat;
	int	i;
	for (i=0; i<50; i++) 
		Delay_25ms ();    
}

/*======================================================================*/

void Upload_Config ( void ) {

	#define CfgStr00a "\n =============================================================================================="
	#define CfgStr01  "\n                                NUE-PSK CONFIG"
	#define CfgStr02  "\n                                =============="
	#define CfgStr00b "\n Configuration file for the NUE-PSK Digital Modem for device settings and macro strings."
	#define CfgStr00c "\n\n DESCRIPTION ... This file is generated by the 'Upload Config' selection in the"
	#define CfgStr00d "\n     modem's CONFIGURE menu, and is written to a USB flash drive inserted into the modem."
	#define CfgStr00e "\n\n CONFIG.TXT ... This file reflects all the current settings of the modem, and may be" 
	#define CfgStr00f "\n     modified as desired on a PC by using a simple text editor like Microsoft WordPad.  "
	#define CfgStr00g "\n\n EDITING ... Editing this text file must be done very carefully by changing characters after "
	#define CfgStr00h "\n     the colon and space.  The options available for each field are listed in parentheses"
	#define CfgStr00i "\n     for each field and must be entered exactly as shown. Special control characters are"
	#define CfgStr00j "\n     entered into macro strings as <TXON>, <TXOFF>, <MYCALL> and <THEIRCALL>. For example ..."
	#define CfgStr00k "\n           <TXON>Thanks <THEIRCALL>. QTH HERE IS BALTIMORE, de <MYCALL><TXOFF>"
	#define CfgStr00l "\n\n LOADING INTO MODEM ... The settings and strings contained in the CONFIG.TXT file may loaded"
	#define CfgStr00m "\n     into the modem by placing the file onto the flash drive, inserting it into the modem,"
	#define CfgStr00n "\n     and then selecting 'Download Config' in the modem's CONFIGURE menu."
	#define CfgStr00o "\n\n RESULTS ... If the modem is able to successfully read and transfer all the settings to the modem," 
	#define CfgStr00p "\n     'Exit' will be displayed and pressing the Select pushbutton will bring you back to the"
	#define CfgStr00q "\n     normal operating mode of the mode with the new settings in effect.  Otherwise, the modem"
	#define CfgStr00r "\n     will beep will beep and the Download Config operation will terminate."
	#define CfgStr00s "\n ==============================================================================================="
	#define CfgStr03  "\n\n 1)  My Call: "
	#define CfgStr05  "\n\n 2)  Log Fname: "
	#define CfgStr06  "\n\n 3)  PGA (x1,x5,x16,x32): "
	#define CfgStr07  "\n\n 4)  Mode (BPSK,QPSK,QPSK/R,RTTY): "
	#define CfgStr08  "\n\n 5)  BackLight (ON,OFF): "
	#define CfgStr09  "\n\n 6)  SQLCH (25, 50): "
	#define CfgStr10  "\n\n 7)  AFC (ON,OFF): "
	#define CfgStr11  "\n\n 8)  CWID (ON,OFF): "
	#define CfgStr12  "\n\n 9)  Macro 1: "
	#define CfgStr13  "\n\n 10) Macro 2: "
	#define CfgStr14  "\n\n 11) Macro 3: "
	#define CfgStr15  "\n\n 12) Macro 4: "
	#define CfgStr16  "\n\n 13) Macro 5: "
	#define CfgStr17  "\n\n 14) Macro 6: "
	#define CfgStr18  "\n\n 15) Macro 7: "

	extern Status_type Status;
	extern int byte_count;
	extern int Timer_Count;
	extern char CFG1,CFG2;
	int i, temp;
	int EE_Addr;
	char UartDat;
	uchar ch;

		if (!RXD) { 		/* RXD will be low if nothing is attached */
			Beep();
			return;
		} 
		Timer_Count = 0;
		UartDat = '\0';
		PutUartChar ('\r', NCHK); /* send a CR */			
		while (UartDat != '>') {
			temp = GetUartChar (&UartDat);
			if (temp != 0){
				if (temp == 1) Write_String_Positioned_LCD1_Display ( 5,0, "Overrun Error"); 					
				if (temp == 2) Write_String_Positioned_LCD1_Display ( 5,0, "Framing Error");
				if (temp == 3) Write_String_Positioned_LCD1_Display ( 5,0, "Timeout Error");	
				Beep();
				return;
			}
		}	

		/* Delete the Existing File */
			PutUartString ("DLF CONFIG.TXT", NCHK);
			PutUartChar ('\r', NCHK);
			Delay_5ms ();				/* don't bother checking for error response or confirmation - just wait */

		/* Open the Config File */
			PutUartString ("OPW CONFIG.TXT", NCHK);
			PutUartChar ('\r', NCHK);
			byte_count = 0;
			PutUartString ("WRF 20\r", NCHK); 

			Restore_Config ();			/* get modem variables to the saved values, if done before */

/*			CFG1 = I2C1_Byte_Read (0x01);
			CFG2 = I2C1_Byte_Read (0x02);
			mMODE = CFG1 & 0x0F;
			PGA = (CFG1 & 0x70)>>4;
			BL = (CFG2 & 0x20)>>5;
			SQTH = (CFG2 & 0x0C)>>2;
			AFC = (CFG2 & 0x02)>>1;
			CWID = (CFG2 & 0x01);             */

			PutUartStr (CfgStr00a, CHK);
			PutUartStr (CfgStr01, CHK);											/*     Title Bar  */
			PutUartStr (CfgStr02, CHK);
																				/*     Instructions  */
			PutUartStr (CfgStr00b, CHK);  PutUartStr (CfgStr00c, CHK);  PutUartStr (CfgStr00d, CHK);
			PutUartStr (CfgStr00e, CHK);  PutUartStr (CfgStr00f, CHK);  PutUartStr (CfgStr00g, CHK);
			PutUartStr (CfgStr00h, CHK);  PutUartStr (CfgStr00i, CHK);  PutUartStr (CfgStr00j, CHK);
			PutUartStr (CfgStr00k, CHK);  PutUartStr (CfgStr00l, CHK);  PutUartStr (CfgStr00m, CHK);
			PutUartStr (CfgStr00n, CHK);  PutUartStr (CfgStr00o, CHK);  PutUartStr (CfgStr00p, CHK);
			PutUartStr (CfgStr00q, CHK);  PutUartStr (CfgStr00r, CHK);  PutUartStr (CfgStr00s, CHK); 

			PutUartStr (CfgStr03, CHK);											/* 1)  My Call    */
			EE_Addr = 0x23;
			while ((ch = I2C1_Byte_Read (EE_Addr++)) != '\0') {
				PutUartChar(ch, CHK);
				Delay_5ms;
			}
					
			PutUartStr (CfgStr05, CHK);											/* 2)  Log Fname  */
			EE_Addr = 0x51;
			while ((ch = I2C1_Byte_Read (EE_Addr++)) != '\0') {
				PutUartChar(ch, CHK);
				Delay_5ms;
			}

			PutUartStr (CfgStr06, CHK);											/* 3)  PGA        */
			switch (PGA) {
				case 0: PutUartStr ("x1", CHK);  break;
				case 1: PutUartStr ("x5", CHK);  break;
				case 2: PutUartStr ("x16", CHK); break;
				case 3: PutUartStr ("x32", CHK); break;
				default: break;
			}

			PutUartStr (CfgStr07, CHK);											/* 4)  Mode       */
			switch (mMODE) {
				case 0: PutUartStr ("BPSK", CHK);  break;
				case 1: PutUartStr ("QPSK", CHK);  break;
				case 2: PutUartStr ("QPSK/R", CHK); break;
				case 6: PutUartStr ("RTTY", CHK); break;
				default: break;
			}
	
			PutUartStr (CfgStr08, CHK);											/* 5)  Backlight  */
			if (BL)   PutUartStr ("ON", CHK);
			else	PutUartStr ("OFF", CHK);
	
			PutUartStr (CfgStr09, CHK);											/* 6)  Squelch    */
			if (SQTH)	PutUartStr ("25", CHK);
			else 	PutUartStr ("50", CHK);

			PutUartStr (CfgStr10, CHK);											/* 7)  AFC        */
			if (AFC)   PutUartStr ("ON", CHK);
			else	PutUartStr ("OFF", CHK);

			PutUartStr (CfgStr11, CHK);											/* 8)  CWID       */
			if (CWID)   PutUartStr ("ON", CHK);
			else	PutUartStr ("OFF", CHK);
	
			PutUartStr (CfgStr12, CHK);											/* 9)  Macro 1    */
			ParseMacroBuff (0x100);

			PutUartStr (CfgStr13, CHK);											/* 10) Macro 2    */
			ParseMacroBuff (0x200);

			PutUartStr (CfgStr14, CHK);											/* 11) Macro 3    */
			ParseMacroBuff (0x300);
	
			PutUartStr (CfgStr15, CHK);											/* 12) Macro 4    */
			ParseMacroBuff (0x400);

			PutUartStr (CfgStr16, CHK);											/* 13) Macro 5    */
			ParseMacroBuff (0x500);

			PutUartStr (CfgStr17, CHK);											/* 14) Macro 6    */
			ParseMacroBuff (0x600);

			PutUartStr (CfgStr18, CHK);											/* 15) Macro 7    */
			ParseMacroBuff (0x700);

		/* Close the Config File */
			PutUartChar ('\r', CHK);
			PutUartChar ('\n', CHK);
			PutUartChar ('\r', CHK);
			PutUartChar ('\n', CHK);
			for (i=0; i<(20-byte_count); i++) PutUartChar (' ', NCHK);
			PutUartString ("CLF CONFIG.TXT", NCHK);
			PutUartChar ('\r', NCHK);

}
/*======================================================================*/
void ParseMacroBuff ( int EE_loc )	{

	char ch;

	while ((ch = I2C1_Byte_Read (EE_loc++)) != '\0') {
	switch (ch) {
		case 0x03:	PutUartStr ("<TXON>", CHK); 		break;
		case 0x02:	PutUartStr ("<TXOFF>", CHK); 		break;
		case 0x0E:	PutUartStr ("<MYCALL>", CHK); 		break;
		case 0x0F:	PutUartStr ("<THEIRCALL>", CHK); 	break;
		default:
			PutUartChar(ch, CHK);
			Delay_5ms;
			break;
		}
	}
}
/*======================================================================*/
void PutUartStr ( uchar *string, char Check_Count )	{

	char temp;
	while ( ( temp = *string++ ) ) {
		PutUartChar ( temp, Check_Count );
		Delay_5ms ();
	}
}
/******************************************************************************/
int Sync_Uart ( void ) {
	char UartDat;

	while (UartDat != 'e') {
		PutUartString ("E\r");
		while (UartDat != 'E') GetChar (&UartDat);
		GetUartChar (&UartDat); /* flush out the CR */
		PutUartString ("e\r");
		GetUartChar (&UartDat);
	}
	GetUartChar (&UartDat); /* flush out the CR */
	return 0;	
}

/*======================================================================*/
