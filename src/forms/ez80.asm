;
; Native eZ80 assembly routines
	section	.text
	assume	adl = 1

	public _Lcd_Dim
	public _Lcd_Bright
	public _GetRtcSeconds
	public _GetCSC_Breakable
	public _CheckIfOnKeyPressed
	public _ClearOnKeyPressed
	public _RtcTimer_Expired
	extern _ExitClean
	public _CallIfNotNull
	public _CallIfNotNull8
	public _CallIfNotNull24

_GetCSC                    = 002014Ch
_RestoreLCDBrightness      = 0021AB8h
_DimLCDSlow                = 0021AC0h
flags			= 0D00080h		; location of OS Flags (+-80h)
rtc_Seconds = 0F30000h
rtc_Minutes = 0F30004h


;-------------------------------------------------------------------------------
_CallIfNotNull:
_CallIfNotNull8:
_CallIfNotNull24:
; Allows C code to attempt to call a potentially NULL callback.
; CONTAINS SMC
	; Shuffle the stack.
	pop	de
	ld	hl, retPoint + 1
	ld	(hl), de
	dec	hl
	ex	(sp), hl
	; Return 0 in both A and HL if HL is NULL
	xor	a
	ex	de, hl
	sbc	hl, hl
	adc	hl, de
	ret	z
	jp	(hl)
retPoint:
	jp	0


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


_RtcTimer_Expired:
	call	_GetRtcSeconds
	ex	de, hl
	pop	bc
	pop	hl
	push	hl
	push	bc
	ld	bc, 3600	; 0xE10
	xor	a
	sbc	hl, bc
	add	hl, bc
	ex	de, hl
	jr	c, rtcCheckNoWrap
	ld	b, 7		; BC = 0x710
	; NC reset from above
	sbc	hl, bc
	add	hl, bc
	ret	nc
	ld	b, 0Eh		; BC = 0xE10 again
	add	hl, bc
rtcCheckNoWrap:
	xor	a
	sbc	hl, de
	ccf
	adc	a, a
	ret
	

;-------------------------------------------------------------------------------
_GetCSC_Breakable:
; Wraps GetCSC, but also implements a sort of ERR: BREAK handler.
	ld	iy, flags
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
