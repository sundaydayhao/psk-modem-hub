/*======================================================================

		NUE_PSK_Keyboard Routines

		John H. Fisher	K5JHF
		Milton E. Cram	W8NUE
		Began On:		09/24/2006
		Current Date:	04/22/2008
		Modified by W8NUE	Apr 2008
======================================================================*/
 
#include	"NUE_PSK_Header.h"

/*======================================================================*/

#define	Key_Strobe		PORTDbits.RD8		/* Keybd Strobe Pin           */
#define	Key_Strobe_I	TRISDbits.TRISD8	/* Keybd Strobe Pin Direction */
#define Key_IP			IPC5bits.INT1IP		/* Keybd Interrupt Priority   */
#define Key_IF			IFS1bits.INT1IF		/* Keybd Interrupt Flag       */
#define	Key_IE			IEC1bits.INT1IE		/* Keybd Interrupt Enable     */
#define Key_EP			INTCON2bits.INT1EP	/* Keybd Strobe Edge Direction */
#define Key_isr			_INT1Interrupt		/* Keybd Interrupt Vector	 */

/*======================================================================*/

void init_Key_isr ( void )	{

	Key_Strobe_I=	1;	/*	Keyboard Strobe In	*/
	Key_EP		=	0;	/*	Interrupt on Rising Edge of Strobe	*/
	Key_IP		=	4;	/*	Set Interrupt Priority  */
	Key_IF		=	0;	/*	Clear Interrupt Flag prior to enabling	*/
	Key_IE		=	1;	/*	Enable Keyboard Interrupt	*/
	Key_0_tris  =   1;	/*	Keyboard Data0  In	*/
	Key_1_tris  =   1;	/*	Keyboard Data1  In	*/
	Key_2_tris  =   1;	/*	Keyboard Data2  In	*/
	Key_3_tris  =   1;	/*	Keyboard Data3  In	*/
	Key_4_tris  =   1;	/*	Keyboard Data4  In	*/
	Key_5_tris  =   1;	/*	Keyboard Data5  In	*/
	Key_6_tris  =   1;	/*	Keyboard Data6  In	*/
	Key_7_tris  =   1;	/*	Keyboard Data7  In	*/
	
	}

/*======================================================================*/

void __attribute__((__interrupt__)) Key_isr ( void )	{
	int Scancode, Keycode;
	extern int LastScanCode;
	extern Flag_type Flag;
	static char E1_count;

	Scancode =  ((PORTB>>8) & 0x0FF);
	Key_IF	=	0;
	MAX_Cnt = 5;
	switch	( Scancode )	{ /* check for Shift, Alt, Ctl, E0, F0 */
		case	0xAA:	/* POST OK! */
			break;
		case	0xF0:
			if (Flag.E1_char) break;
			Flag.F0_char = 1;
			break;
		case	0xE0:
			Flag.E0_char = 1;
			break;
		case	0xE1:
			if (Flag.E1_char) break;
			Flag.E1_char = 1;
			E1_count = 2;
			break;
		case	0x12:
			if ( Flag.F0_char )	Flag.Shift_key = 0;
			else				Flag.Shift_key = 1;
			Flag.F0_char = 0;
			break;
		case	0x59:
			if ( Flag.F0_char )	Flag.Shift_key = 0;
			else				Flag.Shift_key = 1;
			Flag.F0_char = 0;
			break;
		case	0x14:
			if (Flag.E1_char) break;
			if ( Flag.F0_char )	Flag.Ctrl_key = 0;
			else				Flag.Ctrl_key = 1;
			Flag.F0_char = 0;
			Flag.E0_char = 0;
			break;
		case	0x11:
			if ( Flag.F0_char )	Flag.Alt_key = 0;
			else				Flag.Alt_key = 1;
			Flag.F0_char = 0;
			Flag.E0_char = 0;
			break;
		default:	/* Not one of the above */
			if ( Flag.F0_char ) {
				Flag.F0_char = 0;
				Flag.E0_char = 0;
				break; /* ignore if previous code was F0 */
			}
			else if (Flag.E1_char) {
				if (--E1_count) break;
				Flag.E1_char = 0;
				Flag.Alt_key = 0;
				Flag.Shift_key = 0;
				Flag.Ctrl_key = 0;
				break;
			}
			else {
				if ((Flag.E0_char)&&(!Flag.Alt_key))  Keycode = Scan_table [ Scancode ] [ 4 ];
				else if ( Flag.Alt_key	)	Keycode = Scan_table [ Scancode ] [ 3 ];
				else if ( Flag.Ctrl_key	)	Keycode = Scan_table [ Scancode ] [ 2 ];
				else if ( Flag.Shift_key )	Keycode = Scan_table [ Scancode ] [ 1 ];
				else						Keycode = Scan_table [ Scancode ] [ 0 ];
				LastScanCode = Scancode;
				Push_Key_FIFO (Keycode);
			}	
			break;

	}
}

/*======================================================================*/

void	Process_Key_FIFO ( void ) {
	extern Flag_type Flag;
	extern int m_TXState;
	extern int Last_Address;
	static char skip_count;
	union {
		int  i;
		int	(*p) ();
	} temp;

	if ((temp.i = Pull_Key_FIFO()) ) {
		if (temp.i > 0x8000) { /* we have a pointer to a function */
			temp.i &= 0x7FFF;  /* strip the MSB */
			( *temp.p )(); /* call the function */
		}
		else { /* have a printable character */

		   if (m_PSKmode == RTTY_MODE) {  /* print a space in place of non-Baudot FIGS */
    			switch (temp.i) {
     				case '%': temp.i = ' '; break;
     				case '@': temp.i = ' '; break;
     				case '^': temp.i = ' '; break;
     				case '*': temp.i = ' '; break;
     				case '{': temp.i = ' '; break;
     				case '}': temp.i = ' '; break;
     				case '[': temp.i = ' '; break;
     				case ']': temp.i = ' '; break;
     				case '|': temp.i = ' '; break;
     				case '<': temp.i = ' '; break;
     				case '>': temp.i = ' '; break;
     				case '~': temp.i = ' '; break;
     				case '`': temp.i = ' '; break;
     				case '+': temp.i = ' '; break;
     				case '=': temp.i = ' '; break;
     				case '_': temp.i = ' '; break;
     				case '-': temp.i = ' '; break;
     				case '\\': temp.i = ' '; break;
    			}
   			}

			switch (m_TXState) {
				case TX_OFF_STATE:
					Push_Temp_FIFO ( (char) temp.i );
					break;
				case TX_MACRO_REC:
					if (Flag.MacRec == TRUE) {
						Buffered_LCD_Write ((char) temp.i );
						if (temp.i == 0x08) {
							Record_Address--;
							break;
						}
						I2C1_Byte_Write (Record_Address++, (char) temp.i);

					}
					break;
				case TX_FILE_INPUT:
					Buffered_LCD_Write ((char) temp.i );
					break;
				case TX_SENDING_STATE:
					PutTxQue ( (char) temp.i, FALSE ); 
					break;
				case TX_PAUSED_STATE:
					break;
			}
		}
	}
}

/*======================================================================*/
#define Immediate_Execution 0x8000

#define	Dummy	(int) Dummy_Function 	+ Immediate_Execution 
#define Alt_A	(int) Alt_A_Function	+ Immediate_Execution
#define	Alt-K	(int) Alt_K_Function	+ Immediate_Execution
#define Alt_M	(int) Alt_M_Function	+ Immediate_Execution
#define Alt_T	(int) Alt_T_Function	+ Immediate_Execution
#define Ctrl_A	(int) Ctrl_A_Function	+ Immediate_Execution
#define Ctrl_B	(int) Ctrl_B_Function	+ Immediate_Execution
#define Ctrl_C	(int) Ctrl_C_Function	+ Immediate_Execution
#define Ctrl_F	(int) Ctrl_F_Function	+ Immediate_Execution
#define Ctrl_I	(int) Ctrl_I_Function	+ Immediate_Execution
#define Ctrl_K	(int) Ctrl_K_Function	+ Immediate_Execution
#define Ctrl_L	(int) Ctrl_L_Function	+ Immediate_Execution
#define Ctrl_M	(int) Ctrl_M_Function	+ Immediate_Execution
#define Ctrl_N	(int) Ctrl_N_Function	+ Immediate_Execution
#define Ctrl_O	(int) Ctrl_O_Function	+ Immediate_Execution
#define Ctrl_P	(int) Ctrl_P_Function	+ Immediate_Execution
#define Ctrl_R	(int) Ctrl_R_Function	+ Immediate_Execution
#define Ctrl_T	(int) Ctrl_T_Function	+ Immediate_Execution
#define Ctrl_U	(int) Ctrl_U_Function	+ Immediate_Execution
#define Ctl_Tb	(int) Ctl_Tb_Function	+ Immediate_Execution
#define Alt_F	(int) Alt_F_Function	+ Immediate_Execution 
#define	F1		(int) F1_Function		+ Immediate_Execution 
#define	F2		(int) F2_Function		+ Immediate_Execution 
#define	F3		(int) F3_Function		+ Immediate_Execution 
#define	F4		(int) F4_Function		+ Immediate_Execution 
#define	F5		(int) F5_Function		+ Immediate_Execution 
#define	F6		(int) F6_Function		+ Immediate_Execution 
#define	F7		(int) F7_Function		+ Immediate_Execution 
#define	F8		(int) F8_Function		+ Immediate_Execution 
#define	Alt_F1	(int) Alt_F1_Function	+ Immediate_Execution 
#define	Alt_F2	(int) Alt_F2_Function 	+ Immediate_Execution 
#define	Alt_F3	(int) Alt_F3_Function	+ Immediate_Execution 
#define	Alt_F4	(int) Alt_F4_Function	+ Immediate_Execution 
#define	Alt_F5	(int) Alt_F5_Function	+ Immediate_Execution 
#define	Alt_F6	(int) Alt_F6_Function	+ Immediate_Execution 
#define	Alt_F7	(int) Alt_F7_Function	+ Immediate_Execution 
#define	Alt_F8	(int) Alt_F8_Function	+ Immediate_Execution
#define	Alt_F9	(int) Alt_F9_Function	+ Immediate_Execution 
#define	Ctl_F1	(int) Ctl_F1_Function	+ Immediate_Execution 
#define	Ctl_F2	(int) Ctl_F2_Function	+ Immediate_Execution 
#define	Ctl_F3	(int) Ctl_F3_Function	+ Immediate_Execution 
#define	Ctl_F4	(int) Ctl_F4_Function	+ Immediate_Execution 
#define	Ctl_F5	(int) Ctl_F5_Function	+ Immediate_Execution 
#define	Ctl_F6	(int) Ctl_F6_Function	+ Immediate_Execution 
#define	Ctl_F7	(int) Ctl_F7_Function	+ Immediate_Execution 
#define	Ctl_F8	(int) Ctl_F8_Function	+ Immediate_Execution 
#define	F9		(int) F9_Function		+ Immediate_Execution
#define S_F9	(int) S_F9_Function		+ Immediate_Execution 
#define	F10		(int) F10_Function		+ Immediate_Execution 
#define	F11		(int) F11_Function		+ Immediate_Execution 
#define	F12		(int) F12_Function		+ Immediate_Execution
#define Up		(int) Up_Function		+ Immediate_Execution 
#define Down	(int) Down_Function		+ Immediate_Execution 
#define Right	(int) Right_Function	+ Immediate_Execution 
#define Left	(int) Left_Function		+ Immediate_Execution 
#define AUp		(int) AUp_Function		+ Immediate_Execution 
#define ADown	(int) ADown_Function	+ Immediate_Execution 
#define ARight	(int) ARight_Function	+ Immediate_Execution 
#define ALeft	(int) ALeft_Function	+ Immediate_Execution 
#define Pg_Up	(int) Pg_Up_Function	+ Immediate_Execution 
#define Pg_Dn	(int) Pg_Dn_Function	+ Immediate_Execution 
#define Home	(int) Home_Function		+ Immediate_Execution 
#define End		(int) End_Function		+ Immediate_Execution 
#define Tab		(int) Tab_Function		+ Immediate_Execution 
#define S_Tab	(int) S_Tab_Function	+ Immediate_Execution 
#define kp_0	(int) kp_0_Function		+ Immediate_Execution 
#define kp_1	(int) kp_1_Function		+ Immediate_Execution 
#define kp_2	(int) kp_2_Function		+ Immediate_Execution 
#define kp_3	(int) kp_3_Function		+ Immediate_Execution 
#define kp_4	(int) kp_4_Function		+ Immediate_Execution 
#define kp_5	(int) kp_5_Function		+ Immediate_Execution 
#define kp_6	(int) kp_6_Function		+ Immediate_Execution 
#define kp_7	(int) kp_7_Function		+ Immediate_Execution 
#define kp_8	(int) kp_8_Function		+ Immediate_Execution 
#define kp_9	(int) kp_9_Function		+ Immediate_Execution 
#define slash	(int) slash_Function	+ Immediate_Execution 
#define star	(int) star_Function		+ Immediate_Execution 
#define minus	(int) minus_Function	+ Immediate_Execution 
#define plus	(int) plus_Function		+ Immediate_Execution 
#define Enter	(int) Enter_Function	+ Immediate_Execution 
#define dot		(int) dot_Function		+ Immediate_Execution 

/*======================================================================*/

const unsigned int	Scan_table [ 0x84 ][ 5 ] 	=	{

/*	Hex	Key			Norm	Shift	Ctrl	Alt		Ext		*/
/*	00		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	01	F9	*/	{	0x1A,	S_F9,	Dummy,	Alt_F9,	Dummy	},
/*	02		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	03	F5	*/	{	F5,		Dummy,	Ctl_F5,	Alt_F5,	Dummy	},
/*	04	F3	*/	{	F3,		Dummy,	Ctl_F3,	Alt_F3,	Dummy	},
/*	05	F1	*/	{	F1,		Dummy,	Ctl_F1,	Alt_F1,	Dummy	},
/*	06	F2	*/	{	F2,		Dummy,	Ctl_F2,	Alt_F2,	Dummy	},
/*	07	F12	*/	{	F12,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	08		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	09	F10	*/	{	F10,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	0A	F8	*/	{	F8,		Dummy,	Ctl_F8,	Alt_F8,	Dummy	},
/*	0B	F6	*/	{	F6,		Dummy,	Ctl_F6,	Alt_F6,	Dummy	},
/*	0C	F4	*/	{	F4,		Dummy,	Ctl_F4,	Alt_F4,	Dummy	},
/*	0D	Tab	*/	{	Tab,	S_Tab,	Ctl_Tb,	Dummy,	Dummy	},
/*	0E	`~	*/	{	'`',	'~',	Dummy,	Dummy,	Dummy	},
/*	0F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	10		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	11		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	12		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	13		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	14		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	15	qQ	*/	{	'q',	'Q',	0x02,	0x06,	Dummy	},
/*	16	1!	*/	{	'1',	'!',	Dummy,	Dummy,	Dummy	},
/*	17		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	18		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	19		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	1A	zZ	*/	{	'z',	'Z',	0x1A,	Dummy,	Dummy	},
/*	1B	sS	*/	{	's',	'S',	0x03,	0x04,	Dummy	},
/*	1C	aA	*/	{	'a',	'A',	Ctrl_A,	Alt_A,	Dummy	},
/*	1D	wW	*/	{	'w',	'W',	Dummy,	Dummy,	Dummy	},
/*	1E	2@	*/	{	'2',	'@',	Dummy,	Dummy,	Dummy	},
/*	1F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	20		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	21	cC	*/	{	'c',	'C',	Ctrl_C,	0x05,	Dummy	},
/*	22	xX	*/	{	'x',	'X',	Dummy,	Dummy,	Dummy	},
/*	23	dD	*/	{	'd',	'D',	Dummy,	Dummy,	Dummy	},
/*	24	eE	*/	{	'e',	'E',	Dummy,	Dummy,	Dummy	},
/*	25	4$	*/	{	'4',	'$',	Dummy,	Dummy,	Dummy	},
/*	26	3#	*/	{	'3',	'#',	Dummy,	Dummy,	Dummy	},
/*	27		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	28		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	29		*/	{	' ',	' ',	Dummy,	Dummy,	Dummy	},
/*	2A	vV	*/	{	'v',	'V',	Dummy,	Dummy,	Dummy	},
/*	2B	fF	*/	{	'f',	'F',	Ctrl_F,	Alt_F,	Dummy	},
/*	2C	tT	*/	{	't',	'T',	Ctrl_T,	0x0F,	Dummy	},
/*	2D	rR	*/	{	'r',	'R',	Ctrl_R,	Dummy,	Dummy	},
/*	2E	5%	*/	{	'5',	'%',	Dummy,	Dummy,	Dummy	},
/*	2F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	30		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	31	nN	*/	{	'n',	'N',	Ctrl_N,	Dummy,	Dummy	},
/*	32	bB	*/	{	'b',	'B',	Ctrl_B,	Dummy,	Dummy	},
/*	33	hH	*/	{	'h',	'H',	Dummy,	Dummy,	Dummy	},
/*	34	gG	*/	{	'g',	'G',	Dummy,	Dummy,	Dummy	},
/*	35	yY	*/	{	'y',	'Y',	Dummy,	Dummy,	Dummy	},
/*	36	6^	*/	{	'6',	'^',	Dummy,	Dummy,	Dummy	},
/*	37		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	38		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	39		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	3A	mM	*/	{	'm',	'M',	Ctrl_M,	0x0E,	Dummy	},
/*	3B	jJ	*/	{	'j',	'J',	Dummy,	Dummy,	Dummy	},
/*	3C	uU	*/	{	'u',	'U',	Ctrl_U,	Dummy,	Dummy	},
/*	3D	7&	*/	{	'7',	'&',	Dummy,	Dummy,	Dummy	},
/*	3E	8*	*/	{	'8',	'*',	Dummy,	Dummy,	Dummy	},
/*	3F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	40		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	41	,<	*/	{	',',	'<',	Dummy,	Dummy,	Dummy	},
/*	42	kK	*/	{	'k',	'K',	Ctrl_K,	0x10,	Dummy	},
/*	43	iI	*/	{	'i',	'I',	Ctrl_I,	Dummy,	Dummy	},
/*	44	oO	*/	{	'o',	'O',	Ctrl_O,	Dummy,	Dummy	},
/*	45	0)	*/	{	'0',	')',	Dummy,	Dummy,	Dummy	},
/*	46	9(	*/	{	'9',	'(',	Dummy,	Dummy,	Dummy	},
/*	47		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	48		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	49	.>	*/	{	'.',	'>',	Dummy,	Dummy,	Dummy	},
/*	4A	/?	*/	{	'/',	'?',	Dummy,	Dummy,	slash	},
/*	4B	lL	*/	{	'l',	'L',	Ctrl_L,	Dummy,	Dummy	},
/*	4C	;:	*/	{	';',	':',	Dummy,	Dummy,	Dummy	},
/*	4D	pP	*/	{	'p',	'P',	Ctrl_P,	Dummy,	Dummy	},
/*	4E	-_	*/	{	'-',	'_',	Dummy,	Dummy,	Dummy	},
/*	4F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	50		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	51		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	52	'"	*/	{	0x27,	'"',	Dummy,	Dummy,	Dummy	},
/*	53		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	54	[{	*/	{	'[',	'{',	Dummy,	Dummy,	Dummy	},
/*	55	=+	*/	{	'=',	'+',	Dummy,	Dummy,	Dummy	},
/*	56		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	57		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	58		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	59		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	5A	Entr*/	{	0x0D,	Dummy,	Dummy,	Dummy,	Enter	},
/*	5B	]}	*/	{	']',	'}',	Dummy,	Dummy,	Dummy	},
/*	5C		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	5D	\|	*/	{	0x5C,	'|',	Dummy,	Dummy,	Dummy	},
/*	5E		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	5F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	60		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	61		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	62		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	63		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	64		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	65		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	66	BkSp*/	{	0x08,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	67		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	68		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	69	11	*/	{	kp_1,	'1',	Dummy,	Dummy,	End		},
/*	6A		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	6B	44	*/	{	kp_4,	'4',	Dummy,	ALeft,	Left	},
/*	6C	77	*/	{	kp_7,	'7',	Dummy,	Dummy,	Home	},
/*	6D		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	6E		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	6F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	70	00	*/	{	kp_0,	'0',	Dummy,	Dummy,	Dummy	},
/*	71	..	*/	{	dot,	'.',	Dummy,	Dummy,	Dummy	},
/*	72	22	*/	{	kp_2,	'2',	Dummy,	ADown,	Down	},
/*	73	55	*/	{	kp_5,	'5',	Dummy,	Dummy,	Dummy	},
/*	74	66	*/	{	kp_6,	'6',	Dummy,	ARight,	Right	},
/*	75	88	*/	{	kp_8,	'8',	Dummy,	AUp,	Up		},
/*	76	Esc	*/	{	0x1B,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	77	NumL*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	78	F11	*/	{	F11,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	79	++	*/	{	plus,	'+',	Dummy,	Dummy,	Dummy	},
/*	7A	33	*/	{	kp_3,	'3',	Dummy,	Dummy,	Pg_Dn	},
/*	7B	--	*/	{	minus,	'-',	Dummy,	Dummy,	Dummy	},
/*	7C	**	*/	{	star,	'*',	Dummy,	Dummy,	Dummy	},
/*	7D	99	*/	{	kp_9,	'9',	Dummy,	Dummy,	Pg_Up	},
/*	7E		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	7F		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},

/*	Hex				Norm	Shift	Ctrl	Alt		Ext		*/
/*	80		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	81		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	82		*/	{	Dummy,	Dummy,	Dummy,	Dummy,	Dummy	},
/*	83	F7	*/	{	F7,		Dummy,	Ctl_F7,	Alt_F7,	Dummy	},

};

/*======================================================================*/
const unsigned char	RTTY_table [ 0x20 ][ 2 ] 	=	{
/* this is the US alphabet */
/*	Hex	Key			Norm	Shift	*/
/*	00		*/	{	'\0',	'\0'	},
/*	01	F9	*/	{	'E',	'3'		},
/*	02		*/	{	'\n',	'\n'	},
/*	03	F5	*/	{	'A',	'-'		},
/*	04	F3	*/	{	' ',	' '		},
/*	05	F1	*/	{	'S',	'\0'	},
/*	06	F2	*/	{	'I',	'8'		},
/*	07	F12	*/	{	'U',	'7'		},
/*	08		*/	{	'\r',	'\r'	},
/*	09	F10	*/	{	'D',	'$'		},
/*	0A	F8	*/	{	'R',	'4'		},
/*	0B	F6	*/	{	'J',	'\''	},
/*	0C	F4	*/	{	'N',	','		},
/*	0D	Tab	*/	{	'F',	'!'		},
/*	0E	`~	*/	{	'C',	':'		},
/*	0F		*/	{	'K',	'('		},

/*	Hex				Norm	Shift	*/
/*	10		*/	{	'T',	'5'		},
/*	11		*/	{	'Z',	'"'		},
/*	12		*/	{	'L',	')'		},
/*	13		*/	{	'W',	'2'		},
/*	14		*/	{	'H',	'#'		},
/*  15		*/	{	'Y',	'6'		},
/*	16	1!	*/	{	'P',	'0'		},
/*	17		*/	{	'Q',	'1'		},
/*	18		*/	{	'O',	'9'		},
/*	19		*/	{	'B',	'?'		},
/*	1A	zZ	*/	{	'G',	'&'		},
/*	1B	sS	*/	{	'\0',	'\0'	},
/*	1C	aA	*/	{	'M',	'.'		},
/*	1D	wW	*/	{	'X',	'/'		},
/*	1E	2@	*/	{	'V',	';'		},
/*	1F		*/	{	'\0',	'\0'	},

};


	