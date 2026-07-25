/*======================================================================

		NUE_PSK_Keyboard_Functions

		John H. Fisher	K5JHF
		Began On:		09/24/2006
		Current Date:	07/01/2008
		Mods by Milt Cram, W8NUE

======================================================================*/

#include	"NUE_PSK_Header.h"

/*======================================================================*/

	void	(*Alt_A_Pointer)()	= DisableAFC;
	void	(*Alt_F_Pointer)()	= Retrieve_Freq;
	void	(*Alt_K_Pointer)()	= Put_Beacon_Count;
	void	(*Alt_M_Pointer)()	= Put_My_Call;
	void	(*Alt_T_Pointer)()	= Put_Their_Call;
	void	(*Ctrl_A_Pointer)()	= EnableAFC;
	void	(*Ctrl_B_Pointer)() = Clear_Buffers;
	void	(*Ctrl_C_Pointer)() = Toggle_Figures;
	void	(*Ctrl_F_Pointer)() = Save_Freq;
	void	(*Ctrl_I_Pointer)() = Toggle_File_Input;
	void	(*Ctrl_K_Pointer)() = Clear_Temp_FIFO;
	void	(*Ctrl_L_Pointer)()	= Clear_Text_Display;
	void	(*Ctrl_M_Pointer)()	= Save_MyCall;
	void	(*Ctrl_N_Pointer)()	= Save_FileName;
	void	(*Ctrl_O_Pointer)() = Toggle_BackLight;
	void	(*Ctrl_P_Pointer)() = Paste_Kybd_to_XmitBfr;
	void	(*Ctrl_R_Pointer)() = RTTY_Reverse;
	void	(*Ctrl_T_Pointer)() = Save_TheirCall;
	void	(*Ctrl_U_Pointer)() = Toggle_Logging;
	void	(*Ctl_Tb_Pointer)() = Display_Freq;
	void	(*F1_Pointer)()		= Play_Macro;
	void	(*F2_Pointer)()		= Play_Macro;
	void	(*F3_Pointer)()		= Play_Macro;
	void	(*F4_Pointer)()		= Play_Macro;
	void	(*F5_Pointer)()		= Play_Macro;
	void	(*F6_Pointer)()		= Play_Macro;
	void	(*F7_Pointer)()		= Play_Macro;
	void	(*F8_Pointer)()		= Set_Tune_Flag;
	void	(*Alt_F1_Pointer)()	= Erase_Macro;
	void	(*Alt_F2_Pointer)()	= Erase_Macro;
	void	(*Alt_F3_Pointer)()	= Erase_Macro;
	void	(*Alt_F4_Pointer)()	= Erase_Macro;
	void	(*Alt_F5_Pointer)()	= Erase_Macro;
	void	(*Alt_F6_Pointer)()	= Erase_Macro;
	void	(*Alt_F7_Pointer)()	= Erase_Macro;
	void	(*Alt_F8_Pointer)()	= Dummy_Function;
	void	(*Ctl_F1_Pointer)()	= Record_Macro;
	void	(*Ctl_F2_Pointer)()	= Record_Macro;
	void	(*Ctl_F3_Pointer)()	= Record_Macro;
	void	(*Ctl_F4_Pointer)()	= Record_Macro;
	void	(*Ctl_F5_Pointer)()	= Record_Macro;
	void	(*Ctl_F6_Pointer)()	= Record_Macro;
	void	(*Ctl_F7_Pointer)()	= Record_Macro;
	void	(*Ctl_F8_Pointer)()	= Dummy_Function;
	void	(*F9_Pointer)()		= Save_Macro;
	void	(*S_F9_Pointer)()   = Beacon_ON;
	void	(*Alt_F9_Pointer)()	= Beacon_OFF;
	void	(*F10_Pointer)()	= Set_RnT_Flag;
	void	(*F11_Pointer)()	= Show_EEPROM;
	void	(*F12_Pointer)()	= Display_Settings;
	void	(*Up_Pointer)()		= Incr_NCO_Big; 
	void	(*Down_Pointer)()	= Decr_NCO_Big;
	void	(*Right_Pointer)()	= Incr_NCO_Small; 
	void	(*Left_Pointer)()	= Decr_NCO_Small;
	void	(*AUp_Pointer)()	= Incr_NCO_BigP; 
	void	(*ADown_Pointer)()	= Decr_NCO_BigP;
	void	(*ARight_Pointer)()	= Incr_NCO_SmallP; 
	void	(*ALeft_Pointer)()	= Decr_NCO_SmallP;
	void	(*Pg_Up_Pointer)()	= Incr_PGA_Gain;
	void	(*Pg_Dn_Pointer)()	= Decr_PGA_Gain;
	void	(*Home_Pointer)()	= Dummy_Function;
	void	(*End_Pointer)()	= Acquire;
	void	(*Tab_Pointer)()	= Dummy_Function;
	void	(*S_Tab_Pointer)()	= Dummy_Function;
	void	(*kp_0_Pointer)()	= Dummy_Function;
	void	(*kp_1_Pointer)()	= Dummy_Function;
	void	(*kp_2_Pointer)()	= Dummy_Function;
	void	(*kp_3_Pointer)()	= Dummy_Function;
	void	(*kp_4_Pointer)()	= Dummy_Function;
	void	(*kp_5_Pointer)()	= Dummy_Function;
	void	(*kp_6_Pointer)()	= Dummy_Function;
	void	(*kp_7_Pointer)()	= Dummy_Function;
	void	(*kp_8_Pointer)()	= Dummy_Function;
	void	(*kp_9_Pointer)()	= Dummy_Function;
	void	(*slash_Pointer)()	= Dummy_Function;
	void	(*star_Pointer)()	= Dummy_Function;
	void	(*minus_Pointer)()	= Dummy_Function;
	void	(*plus_Pointer)()	= Dummy_Function;
	void	(*Enter_Pointer)()	= Dummy_Function;
	void	(*dot_Pointer)()	= Dummy_Function;

/*======================================================================*/
void	Ctrl_A_Function ( void )	{ (*Ctrl_A_Pointer) (); }
/*======================================================================*/
void	Alt_A_Function ( void )		{ (*Alt_A_Pointer) (); }
/*======================================================================*/
void	Alt_K_Function ( void )		{ (*Alt_K_Pointer) (); }
/*======================================================================*/
void	Ctrl_B_Function ( void ) 	{ (*Ctrl_B_Pointer) (); }
/*======================================================================*/
void	Ctrl_C_Function ( void ) 	{ (*Ctrl_C_Pointer) (); }
/*======================================================================*/
void	Ctrl_F_Function ( void ) 	{ (*Ctrl_F_Pointer) (); }
/*======================================================================*/
void	Ctrl_I_Function ( void ) 	{ (*Ctrl_I_Pointer) (); }
/*======================================================================*/
void	Ctrl_K_Function ( void ) 	{ (*Ctrl_K_Pointer) (); }
/*======================================================================*/
void	Ctrl_M_Function ( void ) 	{ (*Ctrl_M_Pointer) (); }
/*======================================================================*/
void	Ctrl_N_Function ( void ) 	{ (*Ctrl_N_Pointer) (); }
/*======================================================================*/
void	Ctrl_L_Function ( void ) 	{ (*Ctrl_L_Pointer) (); }
/*======================================================================*/
void	Ctrl_O_Function ( void ) 	{ (*Ctrl_O_Pointer) (); }
/*======================================================================*/
void	Ctrl_P_Function ( void ) 	{ (*Ctrl_P_Pointer) (); }
/*======================================================================*/
void	Ctrl_R_Function ( void ) 	{ (*Ctrl_R_Pointer) (); }
/*======================================================================*/
void	Ctrl_T_Function ( void ) 	{ (*Ctrl_T_Pointer) (); }
/*======================================================================*/
void	Ctrl_U_Function ( void ) 	{ (*Ctrl_U_Pointer) (); }
/*======================================================================*/
void	Ctl_Tb_Function ( void ) 	{ (*Ctl_Tb_Pointer) (); }
/*======================================================================*/
void	Alt_F_Function ( void )		{ (*Alt_F_Pointer) (); }
/*======================================================================*/
void	Alt_M_Function ( void )		{ (*Alt_M_Pointer) (); }
/*======================================================================*/
void	Alt_T_Function ( void )		{ (*Alt_T_Pointer) (); }
/*======================================================================*/
void	F1_Function ( void ) {		(*F1_Pointer) (); }
/*======================================================================*/
void	F2_Function ( void ) {		(*F2_Pointer) (); }
/*======================================================================*/
void	F3_Function ( void ) {		(*F3_Pointer) (); }
/*======================================================================*/
void	F4_Function ( void ) {		(*F4_Pointer) (); }
/*======================================================================*/
void	F5_Function ( void ) {		(*F5_Pointer) (); }
/*======================================================================*/
void	F6_Function ( void ) {		(*F6_Pointer) (); }
/*======================================================================*/
void	F7_Function ( void ) {		(*F7_Pointer) (); }
/*======================================================================*/
void	F8_Function ( void ) {		(*F8_Pointer) (); }
/*======================================================================*/
void	Alt_F1_Function ( void ) {		(*Alt_F1_Pointer) (); }
/*======================================================================*/
void	Alt_F2_Function ( void ) {		(*Alt_F2_Pointer) (); }
/*======================================================================*/
void	Alt_F3_Function ( void ) {		(*Alt_F3_Pointer) (); }
/*======================================================================*/
void	Alt_F4_Function ( void ) {		(*Alt_F4_Pointer) (); }
/*======================================================================*/
void	Alt_F5_Function ( void ) {		(*Alt_F5_Pointer) (); }
/*======================================================================*/
void	Alt_F6_Function ( void ) {		(*Alt_F6_Pointer) (); }
/*======================================================================*/
void	Alt_F7_Function ( void ) {		(*Alt_F7_Pointer) (); }
/*======================================================================*/
void	Alt_F8_Function ( void ) {		(*Alt_F8_Pointer) (); }
/*======================================================================*/
void	Alt_F9_Function ( void ) {		(*Alt_F9_Pointer) (); }
/*======================================================================*/
void	Ctl_F1_Function ( void ) {		(*Ctl_F1_Pointer) (); }
/*======================================================================*/
void	Ctl_F2_Function ( void ) {		(*Ctl_F2_Pointer) (); }
/*======================================================================*/
void	Ctl_F3_Function ( void ) {		(*Ctl_F3_Pointer) (); }
/*======================================================================*/
void	Ctl_F4_Function ( void ) {		(*Ctl_F4_Pointer) (); }
/*======================================================================*/
void	Ctl_F5_Function ( void ) {		(*Ctl_F5_Pointer) (); }
/*======================================================================*/
void	Ctl_F6_Function ( void ) {		(*Ctl_F6_Pointer) (); }
/*======================================================================*/
void	Ctl_F7_Function ( void ) {		(*Ctl_F7_Pointer) (); }
/*======================================================================*/
void	Ctl_F8_Function ( void ) {		(*Ctl_F8_Pointer) (); }
/*======================================================================*/
void	F9_Function ( void ) {		(*F9_Pointer) (); }
/*======================================================================*/
 void	F10_Function ( void ) {		(*F10_Pointer) (); }				
/*======================================================================*/
void	S_F9_Function ( void ) {	(*S_F9_Pointer) (); }	
/*======================================================================*/
void	F11_Function ( void ) {		(*F11_Pointer) (); }
/*======================================================================*/
void	F12_Function ( void ) {		(*F12_Pointer) (); }
/*======================================================================*/
void	Up_Function ( void ) {		(*Up_Pointer) (); } 
/*======================================================================*/
void	Down_Function ( void ) {	(*Down_Pointer) (); }
/*======================================================================*/
void	Right_Function ( void ) {	(*Right_Pointer) (); }
/*======================================================================*/
void	Left_Function ( void ) {	(*Left_Pointer) (); }
/*======================================================================*/
void	AUp_Function ( void ) {		(*AUp_Pointer) (); } 
/*======================================================================*/
void	ADown_Function ( void ) {	(*ADown_Pointer) (); }
/*======================================================================*/
void	ARight_Function ( void ) {	(*ARight_Pointer) (); }
/*======================================================================*/
void	ALeft_Function ( void ) {	(*ALeft_Pointer) (); }
/*======================================================================*/
void	Pg_Up_Function ( void ) {	(*Pg_Up_Pointer) (); }
/*======================================================================*/
void	Pg_Dn_Function ( void ) {	(*Pg_Dn_Pointer) (); }
/*======================================================================*/
void	Home_Function ( void ) {	(*Home_Pointer) (); }
/*======================================================================*/
void	End_Function ( void ) {		(*End_Pointer) (); }
/*======================================================================*/
void	Tab_Function ( void ) {		(*Tab_Pointer) (); }
/*======================================================================*/
void	S_Tab_Function ( void ) {	(*S_Tab_Pointer) (); }
/*======================================================================*/
void	kp_0_Function ( void ) {	(*kp_0_Pointer) (); }
/*======================================================================*/
void	kp_1_Function ( void ) {	(*kp_1_Pointer) (); }
/*======================================================================*/
void	kp_2_Function ( void ) {	(*kp_2_Pointer) (); }
/*======================================================================*/
void	kp_3_Function ( void ) {	(*kp_3_Pointer) (); }
/*======================================================================*/
void	kp_4_Function ( void ) {	(*kp_4_Pointer) (); }
/*======================================================================*/
void	kp_5_Function ( void ) {	(*kp_5_Pointer) (); }
/*======================================================================*/
void	kp_6_Function ( void ) {	(*kp_6_Pointer) (); }
/*======================================================================*/
void	kp_7_Function ( void ) {	(*kp_7_Pointer) (); }
/*======================================================================*/
void	kp_8_Function ( void ) {	(*kp_8_Pointer) (); }
/*======================================================================*/
void	kp_9_Function ( void ) {	(*kp_9_Pointer) (); }
/*======================================================================*/
void	slash_Function ( void ) {	(*slash_Pointer) (); }
/*======================================================================*/
void	star_Function ( void ) {	(*star_Pointer) (); }
/*======================================================================*/
void	minus_Function ( void ) {	(*minus_Pointer) (); }
/*======================================================================*/
void	plus_Function ( void ) {	(*plus_Pointer) (); }
/*======================================================================*/
void	Enter_Function ( void ) {	(*Enter_Pointer) (); }
/*======================================================================*/
void	dot_Function ( void ) {		(*dot_Pointer) (); }
/*======================================================================*/
