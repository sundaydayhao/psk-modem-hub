/*==================================================================*/
/* PSKMod.h: interface for the CPSKMod class.			    		*/
/*==================================================================*/
/*  Modified by Milt Cram, W8NUE				    				*/
/*==================================================================*/
/* PSK31Core Library for transmission and reception of PSK31	    */ 
/* signals using a PC soundcard  or .wav files.			    		*/
/*	               Copyright 2000, Moe Wheatley, AE4JY	    		*/
/*								    								*/
/*This library is free software; you can redistribute it and/or	    */
/*modify it under the terms of the GNU Lesser General Public	    */
/*License as published by the Free Software Foundation; either	    */
/*version 2.1 of the License, or (at your option) any later version.*/
/*								    								*/
/*This library is distributed in the hope that it will be useful,   */
/*but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/*Lesser General Public License for more details.		    		*/
/*								    								*/
/*You should have received a copy of the GNU Lesser General Public  */
/*License along with this library; if not, write to the Free 	    */
/* Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,   */
/* MA  02111-1307  USA 						    					*/
/* http://www.gnu.org/copyleft/lesser.html			    			*/
/*==================================================================*/


/* used to specify which mode to use */
#define BPSK_MODE 0
#define QPSKU_MODE 1
#define QPSKL_MODE 2
#define RTTY_MODE 6
#define TUNE_MODE 3
#define TUNE_MODE_WID 4
#define CW_MODE 5
#define TRUE 1
#define FALSE 0
#define TX_CNTRL_AUTOSTOP 4
#define TX_CNTRL_ADDCWID 5
#define TX_CNTRL_NOSQTAIL 6
#define TX_BUF_SIZE 256
#define BACK_SPACE_CODE 0x08
#define MARK 1
#define SPACE 0
#define LTRS 0
#define FIGS 1


	extern int m_PSK63Mode;
	int m_NoSquelchTail; 
	int m_NeedCWid; 
	int m_NeedShutoff; 
	int m_TempNoSquelchTail;
	int m_TempNeedCWid; 
	int m_TempNeedShutoff;
	extern long m_IQPhaseArray[20];
	int m_Fs;
	int m_Ramp;
	int m_CWIDSpeed;
	int m_SavedMode;
	int m_Channel;
	int m_VectLookup[6][2];
	int m_pTail;
	int m_pHead;
	char GetTxChar();
	int m_AmblePtr;
	char m_Preamble[33]; /* 32 characters + NewLine char */
	char m_Postamble[33]; /* 32 characters + NewLine char */
	char RTTY_Preamble[5];
	int m_TXState;
	int m_PSKmode;
	int m_CWState;
	int m_CWtimer;
	int m_CWSetSpeed;
	char XmitBuffer[TX_BUF_SIZE];
	char m_CWIdString[20];
	int Cw_On; 
/* PSK31 and CW generator variables */
	int Itbl_num;
	int Qtbl_num;
	int m_AddEndingZero;
	int m_PresentPhase;
	char m_Lastsymb;
	int Time_test;
	unsigned int m_TxShiftReg;
	int m_TxCodeWord;
	double m_SymbolRate;
	extern double m_TxFreq;
	extern double tempPHZ;
	extern double m_SampleFreq; 
	double m_TClkError; 
	float  m_RMSConstant;
	double m_2PI;
	double m_PSKSecPerSamp;
	double m_PSKPeriodUpdate;
	double m_PSKPhaseInc;
//	extern const float PSKShapeTbl[2049]; 
	extern const int PSKPhaseLookupTable[6][5];

/* RTTY variables */


/* PSK31 and CW modulator private functions */
	char GetChar( );
	char GetNextBPSKSymbol(void);
	char GetNextQPSKSymbol(void);
	char GetNextCWSymbol(void);
	char GetNextRTTYSymbol(void);
	char GetNextTuneSymbol(void);

/* PSK31 and CW modulator */

	void InitPSKMod(void);
	void CalcPSK( void );
	void PutTxQue(char txchar, int cntrl ); 
	void ClrQue();

	void SetCWIDSpeed(long speed);

	void SetTXFreq(int freq){m_TxFreq = (double)freq;}
	void SetTXMode(int mode);
	void SetAutoShutoff(int val){m_NeedShutoff = val;}
	void SetAutoCWID(int val){ m_NeedCWid = val;}
	int GetTXState(){return m_TXState;}
/*	void SetSampleClkAdj(int ppm){m_TClkError = 1.0;}; */
/*	void SetSampleClkAdj(int ppm){m_TClkError = 1.0 - (double)ppm/1000000.0;}; */
/*	void GetVectorData(long* VectData); */
	void PSKTXSetup (void);



