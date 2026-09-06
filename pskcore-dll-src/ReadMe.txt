Revision History( from latest back to first release):
ver. 1.16 Maintenance release 5-23-03
  Functional Changes:
      Added double speed mode for BPSK and QPSK (PSK63)
              (see fnSetRXPSKMode and fnStartTX function changes in interface spec)
      Changed clock adjust range to +/- 20000.
  Bug Fixes:
    Fixed problem with QPSK squelch on some repeating character sequences.
    Fixed problem with missing FFT point in middle.
    Fixed problem with FFT bin frequencies when clock adjust is used.

ver. 1.15 Maintenance release 6-22-02
  Bug Fixes:
    Fixed problem with receive bit sync where characters could get scrogged periodially.
    Fixed problem with soundcard closing.

ver. 1.14 Maintenance release 4-27-02
  Bug Fixes:
    Fixed problem with DLL function name exports for the last few new functions that were added.
    Fixed problem with soundcard clock adjustment range on transmit signal.

ver. 1.13 Maintenance release 9-7-01
  Functional Changes:
    Added fnEnableRXChannel(long chan, long enable) and	fnIsRXChannelActive(long chan) to aid in manipulating the multiple receiver channels.

ver. 1.12 Maintenance release 8-4-01
  Functional Changes:
    Added wave file reading/writing support.
    Reduced CPU overhead for decoder

Ver. 1.11 Maintenance release 6-1-01
  Bug Fixes:
Keeps serial port open during receive to help some laptop problems.
  Functional Changes:
Bumped Tx buffer to 32K.

Ver. 1.10 Maintenance release 1-28-01
  Bug Fixes:
Clears RTS and DTR when fnSetComPort(..) is called.
Use different API call for serial port commands to get around some laptop issues.

Ver. 1.09 Maintenance release 12-24-00
  Functional Changes:
Added Full duplex operation. (see fnStartTX )
Added Fast tracking AFC mode for doppler tracking.(See fnSetAFCLimit)

Ver. 1.08 Maintenance release 12-08-00
  Bug Fixes:
Fixed Soundcard error message problem.
  Functional Changes:
Increased max allowed Receiver channels to 50
Include files now available for implicit or explicit DLL linking

Ver. 1.07 Maintenance release 8-18-00
  Bug Fixes:
Fixed FFT bounds problem.
Reworked dll initialization scheme so settings can be set prior to starting the soundcard thread.
Fixed Vector data glitch on transmit start.
Fixed TX Finishing status problem.

Ver. 1.06 Maintenance release 8-03-00
  Functional Changes:
Added some time delay between audio and PTT.
Added fnRewindInput() function to back-up in time up to 25 seconds and re-process the audio data.

Ver. 1.05 Maintenance release 7-02-00
  Functional Changes:
Added some more error checking to com port routines.
  Bug fixes:
Fixed Problem sending error message during initialization.
Fixed Com Port8 selection problem.

Ver. 1.04 Maintenance release 6-24-00
  Functional Changes:
Limited PSK31 frequency range to 100-3500Hz to prevent distortion problems.
Added Low audio input disable to prevent garbage characters from soundcard background noise.
Updated Software Specification/Tech Guide.
  Bug fixes:
Fixed FFT baseline clipping bug.

Ver. 1.00 Initial release 5-29-00
