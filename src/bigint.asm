	.assume adl=1

	.public	_BigIntZero
	.public	_BigIntTen
;	.public	_BigIntNegativeOne
	.public	_BigIntCopyFromTo
	.public	_BigIntSetToZero
	.public	_BigIntSetToOne
	.public	_BigIntSetToNegativeOne
	.public	_BigIntIncrement
	.public	_BigIntDecrement
	.public	_BigIntAdd
	.public	_BigIntSubtract
;	.public	_BigIntAddInPlace
;	.public	_BigIntSubtractInPlace
	.public	_BigIntNegate
;	.public	_BigIntNegateInPlace
	.public	_BigIntIsNonZero
	.public	_BigIntIsZero
	.public	_BigIntGetSign
	.public	_BigIntCompare
	.public	_BigIntToNativeInt
	.public	_BigIntBinify
	.public	_BigIntOctify
	.public	_BigIntHexify
	.public	_BigIntShiftLeft
	.public	_BigIntShiftBitInOnLeft
;	.public	_BigIntShiftLeftInPlace
	.public	_BigIntShiftRight
;	.public	_BigIntShiftUnsignedRightInPlace
	.public	_BigIntSignedShiftRight
	.public	_BigIntShiftBitInOnRight
;	.public	_BigIntShiftRightInPlace
	.public	_BigIntAnd
	.public	_BigIntOr
	.public	_BigIntXor
	.public	_BigIntNot
	.public	_BigIntNand
	.public	_BigIntGetBit
	.public	_BigIntSetBit
	.public	_BigIntMultiply
	.public	_BigIntDivide
	.public	_BigIntToStringBin
	.public	_BigIntToStringOct
	.public	_BigIntToStringHex
	.public	_BigIntToString

.text
; Some routines jump to the end of a BigInt using LEA, and some routines make a
; stack local BigInt, so this should be less than 120.
;
; GetBit and SetBit assume 8-bit bit index numbers.  If you make this bigger
; than 32, you'll need to switch to the currently-commented-out code that
; enables 16-bit indexes.
;
; The divide routine assumes 8-bit indexes as well, so it'll need to be
; reworked.  Maybe make it suck less while you're at it.
;
; ToStringOctal's initial digits routine needs to be changed depending on
; whether the remainder of BIG_INT_SIZE / 3 is zero, one, or two.
BIG_INT_SIZE := 16

_BigIntTen:
	.db	10
_BigIntZero:
	.db	0, 0, 0, 0, 0, 0, 0, 0
	.db	0, 0, 0, 0, 0, 0, 0, 0
;_BigIntNegativeOne:
;	.db	255, 255, 255, 255, 255, 255, 255, 255
;	.db	255, 255, 255, 255, 255, 255, 255, 255

;-------------------------------------------------------------------------------
_BigIntCopyFromTo:
	pop	bc
	pop	hl
	pop	de
	push	de
	push	hl
	push	bc
BigIntCopyFromTo:
	ld	bc, BIG_INT_SIZE
	ldir
	ret


;-------------------------------------------------------------------------------
_BigIntSetToZero:
	pop	hl
	pop	de
	push	de
	push	hl
BigIntSetToZero:
; Input:
;  - DE: Target address
	sbc	hl,hl
	adc	hl,de
	ld	(hl),0
	inc	de
	ld	bc,BIG_INT_SIZE - 1
	ldir
	ret


;-------------------------------------------------------------------------------
_BigIntSetToOne:
	pop	hl
	pop	de
	push	de
	push	hl
BigIntSetToOne:
; Input:
;  - DE: Target address
	ld	a,1
	ld	(de),a
	inc	de
	sbc	hl,hl
	adc	hl,de
	inc	de
	ld	bc,BIG_INT_SIZE - 1
	ldir
	ret


;-------------------------------------------------------------------------------
_BigIntSetToNegativeOne:
	pop	hl
	pop	de
	push	de
	push	hl
BigIntSetToNegativeOne:
; Input:
;  - DE: Target address
	sbc	hl,hl
	adc	hl,de
	ld	(hl),255
	inc	de
	ld	bc,BIG_INT_SIZE - 1
	ldir
	ret


;-------------------------------------------------------------------------------
_BigIntIncrement:
	pop	bc
	pop	hl
	push	hl
	push	bc
BigIntIncrement:
; Input:
;  - HL: Number
	ld	bc,BIG_INT_SIZE * 256
	scf
biincloop:
	ld	a,(hl)
	adc	a,c
	ld	(hl),a
	inc	hl
	djnz	biincloop
	sbc	a,a
	ret


;-------------------------------------------------------------------------------
_BigIntDecrement:
	pop	bc
	pop	hl
	push	hl
	push	bc
BigIntDecrement:
; Input:
;  - HL: Number
	ld	bc,BIG_INT_SIZE * 256
	scf
bidecloop:
	ld	a,(hl)
	sbc	a,c
	ld	(hl),a
	inc	hl
	djnz	bidecloop
	ld	a,0
	ret	po
	inc	a
	ret


;-------------------------------------------------------------------------------
;_BigIntAdd:
;	pop	iy
;	pop	bc
;	pop	hl
;	pop	de
;	push	de
;	push	hl
;	push	bc
;	push	iy
;BigIntAdd:
; Inputs:
;  - BC: n1
;  - HL: n2
; Output:
;  - DE: sum
; Destroys:
;  - IYL,A
;	ld	iyl,BIG_INT_SIZE
;	or	a
;bialoop:
;	ld	a,(bc)
;	inc	bc
;	adc	a,(hl)
;	inc	hl
;	ld	(de),a
;	inc	de
;	dec	iyl
;	jr	nz,bialoop
;	sbc	a,a
;	neg
;	ret


;-------------------------------------------------------------------------------
;_BigIntAddInPlace:
_BigIntAdd:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
;BigIntAddInPlace:
BigIntAdd:
; Inputs:
;  - DE: n1, also gets sum
;  - HL: n2
	ld	b,BIG_INT_SIZE
	or	a
biaiploop:
	ld	a,(de)
	adc	a,(hl)
	inc	hl
	ld	(de),a
	inc	de
	djnz	biaiploop
	sbc	a,a
	neg
	ret


;-------------------------------------------------------------------------------
;_BigIntSubtract:
;	pop	iy
;	pop	bc
;	pop	hl
;	pop	de
;	push	de
;	push	hl
;	push	bc
;	push	iy
;BigIntSubtract:
; Inputs:
;  - BC: n1
;  - HL: n2
; Output:
;  - DE: n1 - n2
; Destroys:
;  - IYL,A
;	ld	iyl,BIG_INT_SIZE
;	or	a
;bisloop:
;	ld	a,(bc)
;	inc	bc
;	sbc	a,(hl)
;	inc	hl
;	ld	(de),a
;	inc	de
;	dec	iyl
;	jr	nz,bisloop
;	sbc	a,a
;	ret


;-------------------------------------------------------------------------------
;_BigIntSubtractInPlace:
_BigIntSubtract:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
;BigIntSubtractInPlace:
BigIntSubtract:
; Inputs:
;  - DE: n1, also gets difference
;  - HL: n2
	ld	b,BIG_INT_SIZE
	or	a
bisiploop:
	ld	a,(de)
	sbc	a,(hl)
	inc	hl
	ld	(de),a
	inc	de
	djnz	bisiploop
	sbc	a,a
	ret


;-------------------------------------------------------------------------------
;_BigIntNegate:
;	pop	bc
;	pop	hl
;	pop	de
;	push	de
;	push	hl
;	push	bc
;BigIntNegate:
; Inputs:
;  - DE: n
;  - HL: target
;	ld	bc,BIG_INT_SIZE * 256
;	scf
;binegloop:
;	ld	a,(hl)
;	inc	hl
;	cpl
;	adc	a,c
;	ld	(de),a
;	inc	de
;	djnz	binegloop
;	ret


;-------------------------------------------------------------------------------
;_BigIntNegateInPlace:
_BigIntNegate:
	pop	bc
	pop	hl
	push	hl
	push	bc
;BigIntNegateInPlace:
BigIntNegate:
; Input:
;  - HL
	ld	bc,BIG_INT_SIZE * 256
	scf
biniploop:
	ld	a,(hl)
	cpl
	adc	a,c
	ld	(hl),a
	inc	hl
	djnz	biniploop
	ret


;-------------------------------------------------------------------------------
_BigIntIsNonZero:
	pop	bc
	pop	hl
	push	hl
	push	bc
BigIntIsNonZero:
; Input:
;  - HL
	ld	b,BIG_INT_SIZE
	xor	a
biinzloop:
	sub	a,(hl)
	ret	nz
	inc	hl
	djnz	biinzloop
	ret


;-------------------------------------------------------------------------------
_BigIntIsZero:
	pop	bc
	pop	hl
	push	hl
	push	bc
BigIntIsZero:
; Input:
;  - HL
	ld	b,BIG_INT_SIZE
	xor	a
biizloop:
	cp	a,(hl)
	ret	nz
	inc	hl
	djnz	biizloop
	inc	a
	ret


;-------------------------------------------------------------------------------
_BigIntGetSign:
	pop	bc
	pop	hl
	push	hl
	push	bc
BigIntGetSign:
; Input:
;  - HL
	ld	b,BIG_INT_SIZE
	xor	a
bigsloop:
	cp	a,(hl)
	jr	nz,bigsnz
	inc	hl
	djnz	bigsloop
	ret
bigsnz:
	; Copy B to C and zero BCU and B
	ld	c,1
	mlt	bc
	add	hl,bc
	dec	hl
	ld	a,(hl)
	add	a,a
	sbc	a,a
	or	1
	ret


;-------------------------------------------------------------------------------
_BigIntCompare:
	pop	bc
	pop	hl
	pop	de
	push	de
	push	hl
	push	bc
BigIntCompare:
; Input:
;  - HL: n1
;  - DE: n2
	ld	bc,BIG_INT_SIZE
	add	hl,bc
	ex	de,hl
	add	hl,bc
	ld	b,c
bicloop:
	dec	de
	ld	a,(de)
	dec	hl
	sub	a,(hl)
	jr	nz,bicne
	djnz	bicloop
	ret
bicne:
	sbc	a,a
	or	a,1
	ret


;-------------------------------------------------------------------------------
_BigIntBinify:
	pop	de
	pop	bc
	pop	hl
	push	hl
	push	bc
	push	de
BigIntBinify:
; Inputs:
;  - C: byte
;  - HL: target
	ld	b,8
bibloop:
	ld	a,'0' / 2
	rl	c
	rla
	ld	(hl),a
	inc	hl
	djnz	bibloop
	ld	(hl),0
	ret


;-------------------------------------------------------------------------------
_BigIntOctify:
	pop	de
	pop	bc
	pop	hl
	push	hl
	push	bc
	push	de
BigIntOctify:
; Inputs:
;  - C: byte
;  - HL: target
	; This rotates C nine times instead of special-casing the first digit.
	ld	b,3
	; This ensures a zero is the first bit rolled in.
	or	a,a
bioloop:
	ld	a,'0' / 8
	rla
	rl	c
	rla
	rl	c
	rla
	rl	c
	ld	(hl),a
	inc	hl
	djnz	bioloop
	ld	(hl),0
	ret


;-------------------------------------------------------------------------------
_BigIntHexify:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
BigIntHexify:
; Inputs:
;  - E: byte
;  - HL: target
	ld	a,e
	rra
	rra
	rra
	rra
	call	bihnib
	ld	a,e
bihnib:	or	0F0h
	daa
	add	a,0A0h
	adc	a,40h
	ld	(hl),a
	inc	hl
	ld	(hl),0
	ret


;-------------------------------------------------------------------------------
;_BigIntShiftLeft:
;	pop	bc
;	pop	hl
;	pop	de
;	push	de
;	push	hl
;	push	bc
;BigIntShiftLeft:
; Input:
;  - HL
; Output:
;  - DE
;	ld	b,BIG_INT_SIZE
;	or	a
;bislloop:
;	ld	a,(hl)
;	inc	hl
;	rla
;	ld	(de), a
;	inc	de
;	djnz	bislloop
;	ld	a,0
;	adc	a,a
;	ret


;-------------------------------------------------------------------------------
;_BigIntShiftLeftInPlace:
_BigIntShiftLeft:
	pop	bc
	pop	hl
	push	hl
	push	bc
;BigIntShiftLeftInPlace:
BigIntShiftLeft:
; Input:
;  - HL
	xor	a
bislentry:
	ld	b,BIG_INT_SIZE
bisliploop:
	rl	(hl)
	inc	hl
	djnz	bisliploop
	adc	a,a
	ret
;-------------------------------------------------------------------------------
_BigIntShiftBitInOnLeft:
	pop	bc
	pop	hl
	pop	de
	push	de
	push	hl
	push	bc
BigIntShiftBitInOnLeft:
; Input:
;  - HL: n
;  - E: zero or one
	xor	a
	rr	e
	jr	bislentry


;-------------------------------------------------------------------------------
;_BigIntUnsignedShiftRight:
;	pop	bc
;	pop	de
;	pop	hl
;	push	hl
;	push	de
;	push	bc
;BigIntUnsignedShiftRight:
; Input:
;  - DE
; Output:
;  - HL
;	ld	bc,BIG_INT_SIZE
;	add	hl,bc
;	ex	de,hl
;	add	hl,bc
;	ld	b,c
;	or	a
;bisruloop:
;	dec	hl
;bisrentry:
;	ld	a,(hl)
;	rra
;	dec	de
;	ld	(de), a
;	djnz	bisruloop
;	ld	a,0
;	adc	a,a
;	ret
;-------------------------------------------------------------------------------
;_BigIntShiftRight:
;	pop	bc
;	pop	de
;	pop	hl
;	push	hl
;	push	de
;	push	bc
;BigIntShiftRight:
; Input:
;  - DE
; Output:
;  - HL
;	ld	bc,BIG_INT_SIZE
;	add	hl,bc
;	ex	de,hl
;	add	hl,bc
;	ld	b,c
;	dec	hl
;	ld	a,(hl)
;	add	a,a
;	jr	bisrentry


;-------------------------------------------------------------------------------
;_BigIntUnsignedShiftRightInPlace:
_BigIntShiftRight:
	pop	bc
	pop	hl
	push	hl
	push	bc
;BigIntUnsignedShiftRightInPlace:
BigIntShiftRight:
; Input:
;  - HL
	ld	bc,BIG_INT_SIZE
	add	hl,bc
	ld	b,c
	xor	a,a
bisruiploop:
	dec	hl
bisripentry:
	rr	(hl)
	djnz	bisruiploop
	adc	a,a
	ret
;-------------------------------------------------------------------------------
;_BigIntShiftRightInPlace:
_BigIntSignedShiftRight:
	pop	bc
	pop	hl
	push	hl
	push	bc
;BigIntShiftRightInPlace:
BigIntSignedShiftRight:
; Input:
;  - HL
	ld	bc,BIG_INT_SIZE - 1
	add	hl,bc
	ld	b,c
	sra	(hl)
	xor	a,a
	jr	bisripentry
;-------------------------------------------------------------------------------
_BigIntShiftBitInOnRight:
	pop	bc
	pop	hl
	pop	de
	push	de
	push	hl
	push	bc
BigIntShiftBitInOnRight:
; Input:
;  - HL: n
;  - E: zero or one
	ld	bc,BIG_INT_SIZE
	add	hl,bc
	ld	b,c
	xor	a,a
	rr	e
	jr	bisruiploop


;-------------------------------------------------------------------------------
_BigIntAnd:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
BigIntAnd:
; Inputs:
;  - DE: n1, destination
;  - HL: n2
	ld	b,BIG_INT_SIZE
biandloop:
	ld	a,(de)
	and	a,(hl)
	inc	hl
	ld	(de),a
	inc	de
	djnz	biandloop
	ret


;-------------------------------------------------------------------------------
_BigIntOr:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
BigIntOr:
; Inputs:
;  - DE: n1, destination
;  - HL: n2
	ld	b,BIG_INT_SIZE
biorloop:
	ld	a,(de)
	or	a,(hl)
	inc	hl
	ld	(de),a
	inc	de
	djnz	biorloop
	ret


;-------------------------------------------------------------------------------
_BigIntXor:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
BigIntXor:
; Inputs:
;  - DE: n1, destination
;  - HL: n2
	ld	b,BIG_INT_SIZE
bixorloop:
	ld	a,(de)
	xor	a,(hl)
	inc	hl
	ld	(de),a
	inc	de
	djnz	bixorloop
	ret


;-------------------------------------------------------------------------------
_BigIntNot:
	pop	bc
	pop	hl
	push	hl
	push	bc
BigIntNot:
; Inputs:
;  - HL
	ld	b,BIG_INT_SIZE
binotloop:
	ld	a,(hl)
	cpl
	ld	(hl),a
	inc	hl
	djnz	binotloop
	ret


;-------------------------------------------------------------------------------
_BigIntNand:
	pop	bc
	pop	de
	pop	hl
	push	hl
	push	de
	push	bc
BigIntNand:
; Inputs:
;  - DE: n1, destination
;  - HL: n2
	ld	b,BIG_INT_SIZE
binandloop:
	ld	a,(de)
	and	a,(hl)
	inc	hl
	cpl
	ld	(de),a
	inc	de
	djnz	binandloop
	ret


;-------------------------------------------------------------------------------
_BigIntGetBit:
	pop	de
	pop	hl
	pop	bc
	push	bc
	push	hl
	push	de
BigIntGetBit:
; Inputs:
;  - HL: n
;  - BC: bit number
	xor	a
; In case you want XL Ints
;	srl	b
;	rr	c
;	rra
;	srl	b
;	rr	c
;	rra
;	srl	b
;	rr	c
	srl	c
	rra
	srl	c
	rra
	srl	c
	ld.sis	b,0
	rla
	rla
	rla
	add	hl,bc
	ld	b,a
	inc	b
	ld	a,(hl)
bigbloop:
	rra
	djnz	bigbloop
	sbc	a,a
	and	1
	ret


;-------------------------------------------------------------------------------
_BigIntSetBit:
	pop	iy
	pop	hl
	pop	bc
	pop	de
	push	de
	push	bc
	push	hl
	push	iy
BigIntSetBit:
; Inputs:
;  - HL: n
;  - BC: bit number
;  - E: zero or one
	xor	a
; In case you want XL Ints
;	srl	b
;	rr	c
;	rra
;	srl	b
;	rr	c
;	rra
;	srl	b
;	rr	c
	srl	c
	rra
	srl	c
	rra
	srl	c
	ld.sis	b,0
	rla
	rla
	rla
	add	hl,bc
	ld	b,a
	inc	b
	xor	a
	scf
bisbloop:
	rla
	djnz	bisbloop
	bit	0,e
	jr	nz,bisb
	cpl
	and	a,(hl)
	ld	(hl),a
	ret
bisb:
	or	a,(hl)
	ld	(hl),a
	ret


;-------------------------------------------------------------------------------
_BigIntMultiply:
; Simple long multiplication routine.
; This routine does not return a double-precision result; the result is
; truncated on overflow.
bim:
; .i1 and .i2 count down instead of up for assembly language reasons.
; As a result, instead of indexing n1[.i1] and n2[.i2] directly, we cache
; separate pointers.
bim.i1 := 0
bim.i2 := bim.i1 + 1
bim.of := bim.i2 + 1
bim.tempptr := bim.of + 1
bim.temp := bim.tempptr + 3
bim.localsSize := bim.temp + BIG_INT_SIZE
bim.oldStackFrame := bim.localsSize
bim.retAddr := bim.oldStackFrame + 3
bim.arg1 := bim.retAddr + 3
bim.n1 := bim.arg1
bim.arg2 := bim.arg1 + 3
bim.n2 := bim.arg2
bim.prod := bim.arg2 + 3
; Open stack frame
	push	ix
	ld	ix,-bim.localsSize
	add	ix,sp
	ld	sp,ix
; Function body
	; of = 0;
	ld	(ix + bim.of),0
	; BigIntSetToZero(.prod);
	ld	de,(ix + bim.prod)
	call	BigIntSetToZero
	; for (.i2 = BIG_INT_SIZE, .tempptr = &.temp; . . . )
	ld	(ix + bim.i2),BIG_INT_SIZE
	lea	hl,ix + bim.temp - 1
	ld	(ix + bim.tempptr),hl
bim.outerLoop:
	; BigIntSetToZero(&.tempptr);
	lea	de,ix + bim.temp
	call	BigIntSetToZero
; IX: frame pointer
; IYL: n1
; IYH: n2[i2++]
; HL: n1 ptr
; DE: .temp ptr
; A, carry flag: carry
	; .i2++;
	ld	de,(ix + bim.tempptr)
	inc	de
	ld	(ix + bim.tempptr),de
	; for (.i1 = .i2, carry = 0; . . . )
	ld	iy,(ix + bim.i2)
	; cache .n2[.i2++]
	ld	hl,(ix + bim.n2)
	ld	a,(hl)
	inc	hl
	ld	iyh,a
	ld	(ix + bim.n2),hl
	ld	hl,(ix + bim.arg1)
	; carry = 0.  Also carry_flag = 0.
	xor	a,a
bim.byteLoop:
	; p = .n1[.i1] * .n2[.i2];
	ld	c,(hl)
	inc	hl
	ld	b,iyh
	mlt	bc
	; p += carry;
	adc	a,c
	; .temp[.i1 + .i2] = p & 0xFF;
	ld	(de),a
	inc	de
	; carry = p >> 8; (with carry flag)
	ld	a,b
	; for ( . . . ; .i1 > 0; i1--);
	dec	iyl
	jr	nz,bim.byteLoop
	; .of |= BigIntAdd(.prod, &.temp);
	ld	de,(ix + bim.prod)
	push	hl
	push	af
	lea	hl,ix + bim.temp
	call	BigIntAdd
	pop	de
	pop	hl
; Check if continuing the multiply to a full double-precision size would generate an overflow
; We check this each time for each non-zero byte in n2
	; if (of) goto .knownOverflow;
	jr	nz,bim.setOverflow
	bit	0,(ix + bim.of)
	jr	nz,bim.knownOverflow
	; for (b = BIG_INT_SIZE - .i2; b > 0; . . . )
	ld	a,BIG_INT_SIZE
	sub	a,(ix + bim.i2)
	or	a
	jr	z,bim.knownOverflow
	ld	b,a
	; if (.n2[.i2] == 0) goto .knownOverflow;
	xor	a
	cp	d
	jr	z,bim.knownOverflow
bim.ofLoop:
	; A |= .n2[B];
	or	a,(hl)
	inc	hl
	; for ( . . . ; b > 0; b--)
	djnz	bim.ofLoop
	; if (!A) goto .knownOverflow
	jr	z,bim.knownOverflow
bim.setOverflow:
	; .of = 0xFF;
	ld	(ix + bim.of),255
bim.knownOverflow:
	; for ( . . . ; .i2 > 0; .i2--)
	dec	(ix + bim.i2)
	jr	nz,bim.outerLoop
	; return .of;
	ld	a,(ix + bim.of)
; Close stack frame
	ld	hl,ix + bim.localsSize
	ld	sp,hl
	pop	ix
	ret


;-------------------------------------------------------------------------------
_BigIntDivide:
; Dumb restoring division routine.
; Algorithm probably sucks but it works and that's all I'm looking for right
; now.
bid:
bid.i := 0
bid.localsSize := bid.i + 1
bid.oldStackFrame := bid.localsSize
bid.retAddr := bid.oldStackFrame + 3
bid.n := bid.retAddr + 3
bid.d := bid.n + 3
bid.q := bid.d + 3
bid.r := bid.q + 3
; Open stack frame
	push	ix
	ld	ix,-bid.localsSize
	add	ix,sp
	ld	sp,ix
; Function body
	ld	de,(ix + bid.q)
	call	BigIntSetToZero
	ld	de,(ix + bid.r)
	call	BigIntSetToZero
	ld	(ix + bid.i),BIG_INT_SIZE * 8
bid.loop:
	ld	hl,(ix + bid.r)
	call	BigIntShiftLeft
	ld	hl,(ix + bid.n)
	ld	c,(ix + bid.i)
	dec	c
	call	BigIntGetBit
	ld	e,a
	ld	hl,(ix + bid.r)
	ld	c,0
	push	hl
	call	BigIntSetBit
	pop	hl
	ld	de,(ix + bid.d)
	push	hl
	push	de
	call	BigIntCompare
	pop	hl
	pop	de
	bit	7,a	; could also do jp,m which is same size
	jr	nz,bid.skip
	call	BigIntSubtract
	ld	hl,(ix + bid.q)
	ld	c,(ix + bid.i)
	dec	c
	ld	e,1
	call	BigIntSetBit
bid.skip:	
	dec	(ix + bid.i)
	jr	nz,bid.loop
; Close stack frame
	lea	hl,ix + bid.localsSize
	ld	sp,hl
	pop	ix
	ret


;-------------------------------------------------------------------------------
_BigIntToStringBin:
	pop	bc
	pop	iy
	pop	hl
	push	hl
	push	hl
	push	bc
BigIntToStringBin:
; Inputs:
;  - IY: number
;  - HL: ASCII target
	lea	iy,iy + BIG_INT_SIZE - 1
	ld	e,BIG_INT_SIZE
bitsb.loop:
	ld	c,(iy)
	dec	iy
	call	BigIntBinify
	dec	e
	jr	nz,bitsb.loop
	ret


;-------------------------------------------------------------------------------
_BigIntToStringOct:
	pop	bc
	pop	iy
; If residual octal bytes is 1
	pop	hl
	push	hl
; Else
;	pop	de
;	push	de
; End if
	push	hl
	push	bc
BigIntToStringOct:
; Inputs:
;  - IY: number
;  - DE or HL: ASCII target
; If residual octal bytes is 1
	lea	iy,iy + BIG_INT_SIZE - 4
	ld	c,(iy + 3)
	call	BigIntOctify
	ex	de,hl
	ld	c,BIG_INT_SIZE / 3
; If residual octal bytes is 2
;	lea	iy,iy + BIG_INT_SIZE - 2
;	ld	hl,(iy)
;	inc	hl
;	dec.sis	hl
;	ld	bc,(6 * 256) + (BIG_INT_SIZE / 3)
;	jr	bitsoinnerloop
; If residual octal bytes is 0
;	lea	iy,iy + BIG_INT_SIZE - 3
;	ld	c,BIG_INT_SIZE / 3
bitsoouterloop:
	ld	b,24 / 3
	ld	hl,(iy)
bitsoinnerloop:
	ld	a,'0' / 8
	add	hl,hl
	adc	a,a
	add	hl,hl
	adc	a,a
	add	hl,hl
	adc	a,a
	ld	(de),a
	inc	de
	djnz	bitsoinnerloop
	lea	iy,iy - 3
	dec	c
	jr	nz,bitsoouterloop
	ex	de,hl
	ld	(hl),0
	ret


;-------------------------------------------------------------------------------
_BigIntToStringHex:
	pop	bc
	pop	iy
	pop	hl
	push	hl
	push	hl
	push	bc
BigIntToStringHex:
; Inputs:
;  - IY: number
;  - HL: ASCII target
	lea	iy,iy + BIG_INT_SIZE - 1
	ld	b,BIG_INT_SIZE
bitsh.loop:
	ld	e,(iy)
	dec	iy
	call	BigIntHexify
	djnz	bitsh.loop
	ret


;-------------------------------------------------------------------------------
_BigIntToString:
; This optimized routine is courtesy of @jacobly0 .
bits:
; <jacobly> and if you want hints, iyh is digit pair count, iyl is a zero indicator, and c is carry (well remainder I guess)
; <jacobly> it could be slightly faster with a dedicated skip leading zeros loop at the beginning, but it doesn't affect the input I'm testing so whatever >.>
; <jacobly> also might be smaller with double pushes and a sla b for the last loop
; <jacobly> also it occurs to me that my slow division routines might actually be really fast for arbitrary division...
; <jacobly> since I can amortize the overhead outside the inner loop
; <jacobly> but then again maybe fast long division would still win at these sizes...
bits.n := 0
bits.start := bits.n + BIG_INT_SIZE
bits.localsSize := bits.start + 3
bits.arg0 = bits.localsSize + 6
bits.buffer = bits.arg0 + 3
; Mateo, I just want you to know that the spaces in here were put there by jacobly.
	push	ix
	ld	ix, -bits.localsSize
	add	ix, sp
	ld	sp, ix
	ld	hl, (ix + bits.arg0)
	lea	de, ix + bits.n
	call	BigIntCopyFromTo
	ld	(ix + bits.start), de
	ld	iyh, 0
bits.digitsLoop:
	ld	de, (ix + bits.start)
	ld	c, 0
	ld	iyl, c
bits.divLoop:
	dec	de
	ld	a, (de)
	ld	l, a
	ld	b, ((1 shl 14) + 99) / 100
	ld	h, b
	mlt	bc
	mlt	hl
	ld	l, h
	ld	h, 0
	add	hl, bc
	add	hl, hl
	add	hl, hl
	ld	c, 100
	ld	b, h
	ld	l, c
	mlt	hl
	sub	a, l
	add	a, c
	jr	c, bits.fixed
	sub	a, c
	inc	b
bits.fixed:
	dec	b
	ld	c, a
	ld	a, b
	ld	(de), a
	or	a, iyl
	ld	iyl, a
	jr	nz, bits.notZero
	ld	(ix + bits.start), de
bits.notZero:
	ld	a, e
	cp	a, ixl
	jr	nz, bits.divLoop
	ld	a, c
	ld	b, ((1 shl 10) + 9) / 10
	mlt	bc
	srl	b
	srl	b
	ld	e, b
	ld	c, 10
	mlt	bc
	sub	a, c
	ld	d, a
	push	de
	inc	iyh
	ld	a, iyl
	or	a, a
	jr	nz, bits.digitsLoop
	ld	c, '0'
	ld	b, iyh
	ld	hl, (ix + bits.buffer)
	pop	de
	ld	a, e
	or	a, a
	jr	z, bits.asciiEnter
	db	$3E
bits.asciiLoop:
	pop	de
	ld	a, e
	add	a, c
	ld	(hl), a
	inc	hl
bits.asciiEnter:
	ld	a, d
	add	a, c
	ld	(hl), a
	inc	hl
	djnz	bits.asciiLoop
	ld	(hl), 0
	lea	ix, ix + bits.localsSize
	ld	sp, ix
	pop	ix
	ret


;-------------------------------------------------------------------------------
_BigIntToNativeInt:
	pop	bc
	pop	hl
	push	hl
	push	bc
	ld	de,(hl)
	inc	hl
	inc	hl
	inc	hl
	xor	a
	ld	b,BIG_INT_SIZE - 3
bitnil:
	or	a,(hl)
	inc	hl
	djnz	bitnil
	ex	de,hl
	ret	z
	ld	hl,-1
	ret
