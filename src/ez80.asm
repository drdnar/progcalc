;
; Native eZ80 assembly routines

	.assume adl=1
	.def _Lcd_Dim
	.def _Lcd_Bright
	.def _GetRtcSeconds
	.def _GetRtcSecondsPlus
	.def _GetCSC_Breakable
	.def _CheckIfOnKeyPressed
	.def _ClearOnKeyPressed
	.ref _ExitClean

_GetCSC                    = 002014Ch
_RestoreLCDBrightness      = 0021AB8h
_DimLCDSlow                = 0021AC0h
flags			= 0D00080h		; location of OS Flags (+-80h)
rtc_Seconds = 0F30000h
rtc_Minutes = 0F30004h

.text


;-------------------------------------------------------------------------------
_Lcd_Dim:
; Calls the OS's LCD idle dimming routines.
	ld	iy, flags
	jp	_DimLCDSlow


_Lcd_Bright:
; Restores the LCD brightness to the user's preference.
	ld	iy, flags
	jp	_RestoreLCDBrightness


;-------------------------------------------------------------------------------
_GetRtcSeconds:
; Returns the number of seconds elapsed since the start of the hour.
; Used by APD routines.
	ld	hl, rtc_Seconds
	ld	a, (hl)
	ld	de, (rtc_Minutes)
	ld	hl, (hl)
	cp	l
	jr	nz, _GetRtcSeconds
	ld	d, 60
	mlt	de
	add	hl, de
	ret


_GetRtcSecondsPlus:
; Returns the number of seconds elapsed since the start of the hour, plus an
; offset.  If the resulting time passes the end of the hour, wraps the time
; around.
; Used by APD routines.
	call	_GetRtcSeconds
	pop	bc
	pop	de
	push	de
	push	bc
	add	hl, de
	ld	de, 3600
	or	a
	sbc	hl, de
	ret	nc
	add	hl, de
	ret


;-------------------------------------------------------------------------------
_GetCSC_Breakable:
; Wraps GetCSC with two differences:
;  - Assumes you're using this in an input loop, so it does EI \ HALT to save a
;    tiny bit of power.
;  - If the ON key has been pressed and the user then presses CLEAR, aborts the
;    program immediately.
	ei
	halt
	call	_GetCSC
	cp	15		; skClear
	ret	nz
	ld	hl, flags + 9	; onFlags
	bit	4, (hl)		; onInterrupt
	ret	z
	jp	_ExitClean


_CheckIfOnKeyPressed:
; Returns 1 if the ON key has been pressed since the last time the
; ON-key-pressed flag was checked.
; Returns 0 if the ON key has not been pressed.
	ld	hl, flags + 9
	and	10h
	ret	z
	inc	a
	ret


_ClearOnKeyPressed:
; Resets the ON-key-pressed flag.
	ld	hl, flags + 9	; iy + onFlags
	res	4, (hl)		; onInterrupt
	ret
