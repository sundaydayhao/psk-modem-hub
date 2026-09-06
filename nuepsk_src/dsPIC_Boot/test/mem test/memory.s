;********************************************************************
; FileName:		memory.s
; Dependencies:    
; Processor:		dsPIC33F Family
; Hardware:		Explorer 16
; Assembler:		ASM30 2.14
; Company:		Microchip Technology, Inc.
;
; Software License Agreement
;
; The software supplied herewith by Microchip Technology Incorporated
; (the “Company”) for its PICmicro® Microcontroller is intended and
; supplied to you, the Company’s customer, for use solely and
; exclusively on Microchip PICmicro Microcontroller products. The
; software is owned by the Company and/or its supplier, and is
; protected under applicable copyright laws. All rights are reserved.
; Any use in violation of the foregoing restrictions may subject the
; user to criminal sanctions under applicable laws, as well as to
; civil liability for the breach of the terms and conditions of this
; license.
;
; THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
; WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
; TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
; PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
; IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;
;********************************************************************/

.include "p33fxxxx.inc"

.global _LoadAddr,_WriteMem,_WriteLatch,_ReadLatch,_ResetDevice	;C called

;***************************************************************
_ReadLatch: ;W0=TBLPAG,W1=Wn - data in W1:W0
	mov	W0,TBLPAG	
	tblrdl [W1],W0
	tblrdh [W1],W1
	
	return
	
.end


;***************************************************************
