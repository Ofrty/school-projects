TITLE Program 06a - Assembly Skill Validation     (Project06.asm)

; Author: Kirkham, Joe
; Last Modified: 2018/06/10
; OSU email address: kirkhamj@oregontate.edu
; Course number/section: CS271/400
; Project Number: Project 06a                 Due Date: 2018/06/11 12:00:00AM
; Description:	This program displays its title, and instructs the user to enter INT_CNT (default 10) integers,
;				which may be positive or negative, must contain all digits (excepting the first char,
;				which can be +/- as long as at least one digit follows). The numbers must all be small enough
;				to fit into a 32 bit reg (for signed numbers: 2147,483,647 is max). Invalid numbers are discarded,
;				and the user is repropmted. As users are entering integers, a running sum is also displayed.
;				After entering INT_CNT integers, the validated integers are re-displayed on the screen,
;				as well as the sum and average of the entered integers.
;
;				NOTE:	I misread the project requirements, and thought that all proc parameters had to be addresses.
;						I now see only at the end that that requirement only applied to macros. That is why all of my
;						parameters are passed by reference. Ideally, many of the simple running sums and counts would
;						have been passed by value. But I don't want to refactor and risk breaking it now.
;				

INCLUDE Irvine32.inc

	MAX_LEN	=	12		; the max read length of a user-entered string . 32-bit signed int max is 2,147,483,647 (10 digits long). allowing for +/- sign at the beginning + 1 more space for OF detection, we want 12 as the value.
	INT_CNT	=	10		; the number of integers to be entered by the user

;-----------------------------------------------------
mGetString MACRO inStrAddr, inStrLenAddr, inStrMaxLenAddr
;
; Macro that reads in a string to @inStrAddr (arg1) up to a length specified in @inStrLenMaxAddr (arg2).
; @inStrLenAddr EXCLUDES NULL TERMINATOR.
; The length of the entered string will be stored in @inStrCurLenAddr (arg3).
; LOCAL var inStrLenVal is used to store the actual value held @inStrLenVal.
; 
; Receives:
;	inStrAddr = @ in string store head
;	inStrCurLenAddr = @ max length of the string
;	inStrMaxLenAddr = @ entered length of the string
;
; Returns: 
;	The read-in string will be stored in the location passed to inStrAddr.
;-----------------------------------------------------
	LOCAL	inStrLenVal				; holds the val stored @inStrLenAddr
	
	; save used regs
	push	eax
	push	ecx
	push	edx

	; get the string length val from address stored in arg1
	mov		eax,				inStrMaxLenAddr	; mov @ of length into eax
	mov		ecx,				[eax]			; mov val into ecx
	inc		ecx									; room for null terminator

	; read string into the string head stored in arg2
	mov		edx,				inStrAddr		; string head addr now in edx
	call	ReadString							; user enters string, stored in arg2 string

	; store the actual entered length in inStrCurLenAddr
	mov		ebx,				[inStrLenAddr]
	mov		[ebx],				eax

	; new line
	call			CrLF

	; pop used regs
	pop		edx
	pop		ecx
	pop		eax
ENDM

;-----------------------------------------------------
mDisplaystring MACRO outStrAddr
;
; Macro that writes, to standard output, a string stored in @outStrAddr (arg1)
; 
; Receives:
;	outStrAddr = the head address of the string
;
; Returns: 
;	Nothing.
;-----------------------------------------------------
	; save used reg
	push	edx

	; display the string and skip to new line.
	mov		edx,	 outStrAddr
	call	WriteString

	; restore used reg
	pop		edx
ENDM

.data

	progName		BYTE	"*****Project 06a: 'Assembly Skill Validation'  -  by Joe Kirkham*****",0
	EC1				BYTE	"***EC: Option 1 - Integer entry lines are numbered & running sum is displayed.***",0
	EC2				BYTE	"***EC: Option 2 - Signed integers are handled.***",0
	EC3				BYTE	"***EC: Option 3 - ReadVal/WriteVal have recursive subprocedures.***",0
	instrucA		BYTE	"**Please enter 10 integers. They may be positive or negative. Your entry must contain only digits.",0
	instrucB		BYTE	"**The first character may be a + or - sign, if there is at least 1 digit after it.",0
	instrucC		BYTE	"**Each of your integers must also have an absolute value <= 2,147,483,647.",0
	instrucD		BYTE	"**Invalid entries will be discarded.",0
	promptA			BYTE	"Int #",0
	promptB			BYTE	". Running Sum = ",0
	promptC			BYTE	"Please enter an integer: ",0
	uIntCnt			DWORD	INT_CNT				; N integers to be read in. would reference as constant but assignment specifies passing by address.
	tempStrMaxLen	DWORD	MAX_LEN				; the max length that any newly-entered string can be; should not change. would reference as constant but assignment specifies passing by address.
	tempStrCurLen	DWORD	MAX_LEN				; the current length of the user-entered string; will change as user enters info
	tempStrCurPos	DWORD	?					; cur pos of the user string (count # not idx); decremented in successive calls of recursive convert/validate function
	valArr			SDWORD	INT_CNT DUP(0)		; storage array for the entered ints. initialized to 0, as vals will be added to this pos.
	tempVal			DWORD	?					; storage address for a value to-be-displayed. cannot just use valArr location because of the need to convert from two's complment if negative.
	joeSFlag		BYTE	0					; a simple flag that indicates if the entered string has a minus sign (-) in pos 0. critical for decision to flip to two's complement or not. 0 = positive, 1 = negative
	invalFlag		BYTE	0					; a simple flag that indicates if the entered string has any "bad chars" (i.e., non-digits, or +/- signs not in pos 1) or is too big for 32-bit reg. 0=inval, 1=val.
	invalMsg		BYTE	"Invalid entry. It was either a) too big (in terms of absolute value), b) contained invalid characters, or c) nothing at all. Re-enter.",0
	sumVal			SDWORD	0					; the sum of the user-entered ints.
	avgVal			SDWORD	0					; the average of the user-entered ints
	bye				BYTE	"Cheers, and have a lovely day!",0
	one				DWORD	1					; just to be able to pass # of things to display by reference....
	intDispTitle	BYTE	"**You entered these integers: ",0
	sumDispTitle	BYTE	"**The final sum of these integers is: ",0
	avgDispTitle	BYTE	"**The average of these integers is: ",0
	tempStr			BYTE	MAX_LEN DUP(?)		; the user-entered string

.code
main PROC
		; intro
		push			OFFSET instrucD									; arg7 = @instruc line 4			
		push			OFFSET instrucC									; arg6 = @instruc line 3
		push			OFFSET instrucB									; arg5 = @instruc line 2
		push			OFFSET instrucA									; arg4 = @instruc line 1
		push			OFFSET EC3										; arg3 = @EC line 2
		push			OFFSET EC2										; arg3 = @EC line 2
		push			OFFSET EC1										; arg2 = @EC line 1
		push			OFFSET progName									; arg1 = @progam/progammer name
		call			intro
		
		; get data
		push			OFFSET tempVal									; arg15= @tempval (for temp write storage of run sum)
		push			OFFSET one										; arg14= @the number 1
		push			OFFSET promptC									; arg13= @prompt text 3
		push			OFFSET promptB									; arg12= @prompt text 2
		push			OFFSET promptA									; arg11= @prompt text 1
		push			OFFSET sumVal									; arg10= @sum of values
		push			OFFSET uIntCnt									; arg9 = @count of ints to be read in
		push			OFFSET invalMsg									; arg8 = @invalMsg
		push			OFFSET tempStrCurPos							; arg7 = @cur pos of the user string (count # not idx)
		push			OFFSET tempStrMaxLen							; arg6 = @string max size
		push			OFFSET tempStrCurLen							; arg5 = @string cur size
		push			OFFSET tempStr									; arg4 = @string head
		push			OFFSET invalFlag								; arg3 = @inval flag
		push			OFFSET joeSFlag									; arg2 = @pos or neg flag
		push			OFFSET valArr[0]								; arg1 = @val storage
		call			getData											; get INT_CNT # of ints from user

		; write ints to screen
		call			CrLF
		mDisplayString	OFFSET intDispTitle									; screen: "You entered these integers: "
		call			CrLF

		push			OFFSET uIntCnt									; arg5 = @int count
		push			OFFSET joeSFlag									; arg4 = @joe sign flag
		push			OFFSET tempVal									; arg3 = @tempVal
		push			OFFSET tempStr									; arg2 = @tempSTr
		push			OFFSET valArr[0]								; arg1 = @valArr head
		call			writeData										; display all of the user-entered ints to user

		; write final sum to screen
		call			CrLF
		mDisplayString	OFFSET sumDispTitle								; screen: "The final sum of these integers is: "

		push			OFFSET one										; arg5 = @number one (times to loop through vals)
		push			OFFSET joeSFlag									; arg4 = @joe sign flag
		push			OFFSET tempVal									; arg3 = @tempVal
		push			OFFSET tempStr									; arg2 = @tempStr
		push			OFFSET sumVal									; arg1 = @runSum
		call			writeData

		; avg
		call			CrLF
		mDisplayString	OFFSET avgDispTitle								; screen: "The average of these integers is: "

		; calc avg
		push			OFFSET joeSFlag									; arg4 = @joe sign flag
		push			OFFSET avgVal									; arg3 = @ avg val storage
		push			OFFSET uIntCnt									; arg2 = @ number of ints entered
		push			OFFSET sumVal									; arg1 = @ sum of vals entered
		call			calcAvg

		; write avg to screen
		push			OFFSET one										; arg5 = @number one (times to loop through vals)
		push			OFFSET joeSFlag									; arg4 = @joe sign flag
		push			OFFSET tempVal									; arg3 = @tempVal
		push			OFFSET tempStr									; arg2 = @tempStr
		push			OFFSET avgVal									; arg1 = @avg
		call			writeData										; avg displayed on screen
		call			CrLF
		
		; bye
		mDisplayString	OFFSET bye										; felicia

		call			CrLF

		exit															; exit to operating system
main ENDP

;-----------------------------------------------------
intro PROC
;
; Simple procedure that displays the progam and user name,
; extra credit info, and provides instructions to the user.
; 
; Receives:
;	ebp+8 = @progam/progammer name
;	ebp+12= @EC line 1
;	ebp+16= @EC line 2
;	ebp+20= @EC line 3
;	ebp+24= @instrucA
;	ebp+28= @instrucB
;	ebp+32= @instrucC
;	ebp+36= @instrucD
;
; Returns: 
;	Nothing.
;-----------------------------------------------------
		; set up stack frame
		push			ebp
		mov				ebp,	esp

		; title
		mDisplayString	[ebp+8]
		call			CrLF
		call			CrLF

		; extra credit lines
		mDisplayString	[ebp+12]
		call			CrLF
		mDisplayString	[ebp+16]
		call			CrLF
		mDisplayString	[ebp+20]
		call			CrLF
		call			CrLF

		; instructions to user
		mDisplayString	[ebp+24]
		call			CrLF
		mDisplayString	[ebp+28]
		call			CrLF
		mDisplayString	[ebp+32]
		call			CrLF
		mDisplayString	[ebp+36]
		call			CrLF
		call			CrLF
		
		pop				ebp
		ret				32
intro ENDP

;-----------------------------------------------------
getData PROC
;
; Gets N signed integers from the user, where N is specified by constant INT_CNT.
; Integers are read in via calls readValMain (itself dependent up on recursive func readValConvert).
; Validated ints are stored in contiguous locations in the array passed to ebp+8.
;
;
; Receives:
;	ebp+8 = @ val storage array head
;	ebp+12= @ joeSFlag
;	ebp+16= @ invalFlag
;	ebp+20= @ user input string's head
;	ebp+24= @ user input string's cur length
;	ebp+28= @ user input string's max length DO NOT MODIFY
;	ebp+32= @ user cur pos
;	ebp+36= @ invalid error message
;	ebp+40= @ uIntCnt
;	ebp+44= @ runSum
;	ebp+48= @ prompt text 1
;	ebp+52= @ prompt text 2
;	ebp+56= @ prompt text 3
;	ebp+60= @ the number 1
;	ebp+64= @ tempVal storage (for writing run sum)
;
; Returns: 
;	Nothing.
;-----------------------------------------------------
		; set up stack frame
		push			ebp
		mov				ebp,			esp

		; save used regs
		push			eax
		push			ebx
		push			ecx
		push			edx

		; LOOP - prompt for val INT_CNT times and store validated ints into array
		; prep ecx
		mov				ecx,			[ebp+40]; ecx: @cnt of ints to be read in
		mov				ecx,			[ecx]	; ecx: cnt of ints to be read in

	getIntStartPnt:
		; display prompt
		; line number
		mDisplayString	[ebp+48]				; screen: "Int# "

		mov				eax,			[ebp+40]; eax: @uIntCnt
		mov				eax,			[eax]	; eax: int count val
		sub				eax,			ecx		; int#-1
		inc				eax						; int#
		call			WriteDec				; display line number

		; run sum
		mDisplayString	[ebp+52]				; screen: "(Running Sum = "
		push			[ebp+60]				; arg5 = @the number one. (how many vals to write to screen).
		push			[ebp+12]				; arg4 = @joe sign flag
		push			[ebp+64]				; arg3 = @temp val storage
		push			[ebp+20]				; arg2 = @temp string
		push			[ebp+44]				; arg1 = @run sum
		call			writeData				; write runsum to screen

		; finish prompt
		mDisplayString	[ebp+56]				; screen: "). Please enter an integer: "

		; set up the address into which the read-in int will be stored
		; get byte offset
		mov				eax,			[ebp+40]; eax: @cnt of ints to be read in
		mov				eax,			[eax]	; eax: cnt of ints to be read in
		sub				eax,			ecx		; eax: how many ints have been read in so far
		mov				ebx,			4		; ebx: # bytes in a DWORD
		xor				edx,			edx		; clear out edx
		mul				ebx						; eax: byte offset for next int storage

		; get effective addr
		mov				ebx,			[ebp+8]	; ebx: addr of the val storage head
		add				ebx,			eax		; ebx: effective addr of mem location where val should be stored
		
		; push n call
		push			[ebp+36]				; arg8 = @invalMsg
		push			[ebp+32]				; arg7 = @cur pos of the user string (count # not idx)
		push			[ebp+28]				; arg6 = @string max size
		push			[ebp+24]				; arg5 = @string cur size
		push			[ebp+20]				; arg4 = @string head
		push			[ebp+16]				; arg3 = @inval flag
		push			[ebp+12]				; arg2 = @pos or neg flag
		push			ebx						; arg1 = @val storage
		call			readValMain

		; update the running sum
		mov				eax,			[ebx]	; eax: just-added integer value
		mov				ebx,			[ebp+44]; ebx: @runSum
		add				eax,			[ebx]	; eax: just-added int + running sum
		mov				[ebx],			eax		; @runSum updated
		
		; continue; go get next int or break
		loop			getIntStartPnt

		; restore used regs
		pop				edx
		pop				ecx
		pop				ebx
		pop				eax

		pop				ebp
		ret				60
getData ENDP

;-----------------------------------------------------
readValMain PROC
;
; readValMain reads in a string of characters from the user,
; validates that the characters represent a signed integer (that can be
; represented in 32 bits), and stores the validated integer into an 
; address specified by the calling function. This function relies on
; recursive calls to readValConvert for converting characters to digits.
; 
; Receives:
;	ebp+8 = @ val storage address
;	ebp+12= @ joeSFlag
;	ebp+16= @ invalFlag
;	ebp+20= @ user input string's head
;	ebp+24= @ user input string's cur length
;	ebp+28= @ user input string's max length DO NOT MODIFY
;	ebp+32= @ user cur pos
;	ebp+36= @ invalid error message
;
; Returns: 
;	Nothing.
;-----------------------------------------------------
		; set up the stack frame
		push			ebp
		mov				ebp,				esp

		; save used regs
		push			eax
		push			ebx
		push			ecx
		push			edx
		push			esi

		; get the string from the user
	getStrPnt:
		; (re)initialize all critical values
		; clear the memory at the val storage location; otherwise, previous invalid attempts might be lingering in mem.
		mov				eax,				[ebp+8]; eax: @val storage address
		mov				ebx,				0
		mov				[eax],				ebx		; nothing now in the val storage address.

		; clear inval flag
		mov				al,					0
		mov				ebx,				[ebp+16]; ebx: @invalFlag
		mov				[ebx],				al		; invalFlag now clear

		; clear joe sign flag
		mov				al,					0
		mov				ebx,				[ebp+12]; ebx: @invalFlag
		mov				[ebx],				al		; invalFlag now clear

		; reset string length to max allowable
		mov				eax,				[ebp+24]; eax: @user input string cur length
		mov				ebx,				[ebp+28]; ebx: @user input string max length
		mov				ebx,				[ebx]	; ebx: user input string max length actual val
		mov				[eax],				ebx		; string reset to max allowable length

		;read in the string
		mGetString		[ebp+20],			[ebp+24],		[ebp+28]

		; set cur pos = string length
		mov				eax,				[ebp+24]; eax: @str length
		mov				ebx,				[ebp+32]; ebx: @cur pos
		mov				eax,				[eax]	; ebx: cur pos val
		mov				[ebx],				eax		; cur pos = str len

		; convert and validate chars
		; read the user-entered string into esi, so that we can use lodsb per the requirements.
		mov				esi,				[ebp+20]; @string head now in esi
		mov				eax,				[ebp+24]; @length offset now in esi
		add				esi,				[eax]	; string head + length offset now in esi (so we're 1 beyond the final index)
		dec				esi							; esi now pointing at final byte of user-entered array. NOT GETTING ANYTHING YET SO NOT USING LODSB.
		std											; set direction flag. we're moving "backwards" from the "end" pos to "beg" pos
		
		; push args and call first instance of recursive function
		push			[ebp+16]					; arg5 = @ invalFlag
		push			[ebp+12]					; arg4 = @ joeSFlag
		push			[ebp+8]						; arg3 = @ place in main val array to store val; before final termination, running sum of the current string
		push			[ebp+32]					; arg2 = @ current pos (same as length for this initial call)
		push			[ebp+24]					; arg1 = @ string length
		call			readValConvert

		; check if invalid. 
		mov				eax,				[ebp+16]; eax:@inval flag
		mov				al,					[eax]	; eax: invalFlag val itself
		cmp				al,					1		; is inval flag set?
		jne				readValMainEndPnt			; if flag not set, then retain val and continue.

		; if here, then there was something wrong with the entered val. throw error message and reprompt.
		mDisplayString	[ebp+36]
		jmp				getStrPnt
		
		; if here, user string was validated and stored. terminate.
	readValMainEndPnt:
		; restore used regs
		pop				esi
		pop				edx
		pop				ecx
		pop				ebx
		pop				eax

		pop				ebp
		ret				32
readValMain ENDP

;-----------------------------------------------------
readValConvert PROC USES eax ebx ecx edx ; +0 to parameters, because LOCAL pushes ebp/esp *prior to* regs being pushed
;
; readValConvert is a recursive function that converts and validates
; each char of the input string. Chars are evaluated from the "end" of the string
; to the "beginning" of the string (i.e., from the least significant
; digits to the most significant digits, or right-to-left).
; 
; Receives:
;	esi	  = @ of current pos in user string
;			(I know this is being passed to the subproc in a reg, but it's the only way to use lodsb in a recursive function.
;			Pushing @curpos onto the stack would be pointless. esi is push/popped in main calling func.)
;	ebp+8 = @ string length
;	ebp+12= @ current character # assessed (NOT index)
;	ebp+16= @ place in main val array to store val; before final termination, running sum of the current string
;	ebp+20= @ joeSFlag
;	ebp+24= @ invalFlag
;
; Returns: 
;	Any portion of the string that has been validated & converted to an int
;	will be stored at the point in the array passed to [ebp+20].
;-----------------------------------------------------
		; declare local vars; stack frame set up (ebp/esp stuff)
		LOCAL	curVal:				SDWORD,		; current value of converted digit. modified throughout this proc to take magnitude of curPos into account
				curMag:				SDWORD		; magnitude of the current pos. will be initialized to 1 and then set based on 10^(length - curchar#)

		; base case(s).
		; base case 1: if str len = 0, then user entered nothing. error.
		mov		eax,					[ebp+8] ; eax: @str len
		mov		eax,					[eax]	; eax: strLen actual value
		cmp		eax,					0		; len zero?
		jz		invalDetectedPnt				; if len zero, error. nothing to do with this string. flag invalid and terminate.

		; if here, length is > 0
		; base case 2: char pos <= 0. check to see if we still have any chars to evaluate in the string.
		mov		eax,					[ebp+12]; eax: @cur pos count
		mov		eax,					[eax]	; eax: @cur pos count itself
		cmp		eax,					0		; cur char # zero?
		jz		readValConvertEndPnt			; if current char # is zero, we've reached the 'end' of the string. terminate.

		; if here, we haven't hit the base case, and still have at least this cur char to validate/convert
		; go ahead and load byte into eax, because testing may happen along a few branches here.
		xor		eax,					eax		; clear out eax
		lodsb									; val at cur pos now in eax

		; if we're at pos 1 and the length > 1, then we can allow +/- at pos 1.
		mov		ebx,					[ebp+12]; ebx: @cur pos
		mov		ebx,					[ebx]	; ebx: cur pos val
		cmp		ebx,					1		; cur pos vs. 1
		jne		digitValPnt						; if we're not at pos 1, then skip straight to checking if it's a digit

		; if here, we're at pos 1. now if the str len > 1, +/- is allowed in this pos.
		mov		ebx,					[ebp+8] ; ebx: @str len
		mov		ebx,					[ebx]	; ebx: strLen actual value
		cmp		ebx,					1		; len vs. 1
		je		digitValPnt						; if there's only one char in the entire string, then no +/- allowed. jump to digit validation
		
		; if here, string length is > 1, and we are at pos 1. +/- are allowed in this position, so check for a sign.
	signValPnt:
		; if +, then valid, and there's nothing else to do in this string, because the sum has already been calc'd and placed arr memory store. jump to end.
		cmp		eax,					43		; is this char a +?
		je		readValConvertEndPnt			; if +, we done. terminate.		

		; if here, pos 1 was not a +. check to see if it's a minus (-)
		cmp		eax,					45		; is this char a -?
		jne		digitValPnt						; if it's not a -, then skip to digit validation

		; if here, pos1 is a minus (-). need to flip the sign flag of the run sum, and then terminate.
		mov		ebx,					[ebp+16]; ebx: @runSum i.e. its ultimate storage pos in mem
		mov		eax,					[ebx]	; eax: runSum val itself
		not		eax								; two's complement step 1: flip the bits
		inc		eax								; two's complement step 2: add 1
		xor		edx,					edx		; prep for step 3: clear out edx
		imul	eax,					1		; step 3: set the sign
		
		mov		[ebx],					eax		; two's complemented val now in @runSum.

		jmp		readValConvertEndPnt			; terminate.	

		; if here, either: a) pos=1, len>1 and pos1 <> +/-, or b) pos>1.
		; check if digit. if digit detected, jmp to convert and validation process (convertStartPnt)
	digitValPnt:
		; if cur pos is not a digit, jump to flag invalid and terminate.
		cmp		eax,					48		; be sure our val is >= lowest digit ASCII val
		jl		invalDetectedPnt				; if the cur pos/byte doesn't is above lower bound of ASCII digits, the char is invalid so jump to handle invalid entry
		cmp		eax,					57		; be sure our val is <= highest digit ASCII val
		jg		invalDetectedPnt				; if the cur pos/byte doesn't is below upper bound of ASCII digits, the char is invalid so jump to handle invalid entry

		; if here, we vaildated the current char as a digit. add the curValto the running sum, and be sure we didn't overflow.
	convertStartPnt:
		; convert ASCII-val to actual integer value and store
		sub		eax,					48		; now in eax: converted ASCII-val to its actual integer value
		mov		curVal,					eax		; this val now in curVal (does not yet account for curPos's mag)

		; calculate magnitude of the curPos
		mov		curMag,					1		; initialize to 1

		; get length - curPos into ecx for loop counter. if we're at the "beginning" (i.e., the highest idx), skip this.
		mov		ecx,					[ebp+8] ; ecx: @str length
		mov		ecx,					[ecx]	; ecx: str len va;
		mov		ebx,					[ebp+12]; ebx: @cur pos count
		mov		ebx,					[ebx]	; ebx: cur pos count val itself
		sub		ecx,					ebx		; now in ecx: str length - curPos (not idx)
		cmp		ecx,					0		; if we're in the top position, then we don't want to go through the loop because the counter would be initialized to 0.
		jz		magMulPnt						; if at top idx then go straight to mag calculation.

		; multiply by 10 for every magnitude under the current pos. {in retrospect, I should have just multiplied this at each recursive call and passed as a param.}
	magCalcStartPnt:
		mov		eax,					curMag  ; now in eax: curMag (running product)
		mov		ebx,					10
		xor		edx,					edx		; empty edx overflow
		imul	eax,					ebx		; now in eax:edx: the product of the current magnitude of the val.

		; check to ensure that the curMag does not exceed the limits of a 32-bit reg. this should only happen if the user entered 11 digits, where the first digit is 10bil (10^10 magnitude.)
		; note - (2^32)-1 and (2^31)-1 can each store a max val in magnitude 10^9 (1bil), so no real need to worry about DWORD v SDWORD here.
		jo		invalDetectedPnt				; if the magnitude exceeded the size of a 32-bit reg, then the overflow (and carry) flags would be set; if so, terminate.

		; if here, this pos mag was OK (i.e., 10^9 or less). continue.
		mov		curMag,					eax		; store running mag
		loop	magCalcStartPnt

		; if here, the magnitude is small enough to be handled within a 32-bit reg.
	magMulPnt:
		; apply the magnitude to the integer digit in the curPos
		mov		eax,					curVal	; now in eax: curVal (not adjusted for magnitude)
		mov		ebx,					curMag	; now in ebx: magnitude of the curPos
		xor		edx,					edx		; clear out edx
		imul	ebx								; now in eax:edx: the actual value represented by the digit in the string

		; not all vals of mag 10^9 fit. verify that this value fits in an SDWORD (i.e., <= 2,147,483,647). previous multiplication would have overflowed if not.
		jo		invalDetectedPnt				; overflow detected? too big, terminate.

		; if here, then the significant digit in at this magnitude will fit in an SDWORD. now, we need to add it to the running sum, and do a final check on its size.
		mov		ebx,					[ebp+16]; now in ebx: the address of the running sum
		add		eax,					[ebx]	; now in eax: the current pos's mag-adjusted value + the running sum; i.e., the exact value of the entered int up to the current mag (assuming no overflow)
		jo		invalDetectedPnt				; overflow detected? too big, terminate.

		; if here, then the value represented by the current pos and all 'less significant' digits is a-ok. "up to" this char is completely validated. store this val as new running sum and move to next char.
	validPnt:
		mov		[ebx],					eax		; the running sum has been updated.

		; decrement the curPos
		mov		eax,					[ebp+12]; eax: @cur pos count
		mov		ebx,					[eax]	; ebx: @cur pos val
		dec		ebx								; dec pos count
		mov		[eax],					ebx		; store decremented pos count in the @curposcount, sitting in eax
		
		push	[ebp+24]
		push	[ebp+20]
		push	[ebp+16]
		push	[ebp+12]
		push	[ebp+8]
		call	ReadValConvert
		jmp		readValConvertEndPnt			; once we break, jump to end, skipping over inval flag

		; if here, something went wrong and we need to flag it and bail.
	invalDetectedPnt:
		mov		al,						1
		mov		ebx,					[ebp+24]; @invalFlag now in eax
		mov		[ebx],					al		; invalFlag now set

		; reached the end. if here, either 1) ran through all chars and all was valid, 2) encountered invalid char, or 3) val was too big to fit.
	readValConvertEndPnt:

		; no need to pop ebp; handled by LOCAL
		ret		20
readValConvert ENDP

;-----------------------------------------------------
writeData PROC
;
; A procedure that will write arg5 # of ints to the screen.
; This procedure mostly handles the iteration through an array
; of vals. Actual conversion and display of data is handled within
; subprocs writeValMain and writeValConvert.
; 
; Receives:
;	ebp+8 = @valArr head (or just @ val of single value)
;	ebp+12= @tempStr
;	ebp+16= @temp val storage
;	ebp+20= @joe sign flag
;	ebp+24= @number of ints to process
;
; Returns: 
;	Nothing.	
;-----------------------------------------------------
		; set up the stack frame
		push			ebp
		mov				ebp,				esp

		; save used regs
		push			eax
		push			ebx
		push			ecx
		push			edx

		; prep loop
		mov				ecx,				[ebp+24]; ecx: @uIntCnt
		mov				ecx,				[ecx]	; ecx: uIntCnt value

		; loop through the array, displaying each integer to standard output
	writeValsStartPnt:
		; set up the address of the int to be written out
		; get byte offset
		mov				eax,				[ebp+24]; eax: @cnt of ints to be written out
		mov				eax,				[eax]	; eax: cnt of ints to be written out
		sub				eax,				ecx		; eax: how many ints have been read in so far
		mov				ebx,				4		; ebx: # bytes in a DWORD
		xor				edx,				edx		; clear out edx
		mul				ebx							; eax: byte offset for next int storage

		; get effective addr
		mov				ebx,				[ebp+8]	; ebx: addr of the val storage head
		add				ebx,				eax		; ebx: effective addr of mem location where val should be stored
		
		; push n call - write cur val arr pos's int to screen
		push			[ebp+20]				; arg4 = @joe sign flag
		push			[ebp+16]				; arg3 = @temp val storage
		push			[ebp+12]				; arg2 = @temp string address to which converted val will be written
		push			ebx						; arg1 = @val arr pos to be read
		call			writeValMain
		call			CrLF

		loop			writeValsStartPnt

		; restore used regs
		pop				edx
		pop				ecx
		pop				ebx
		pop				eax

		pop				ebp
		ret				20
writeData ENDP

;-----------------------------------------------------
writeValMain PROC 
;
; Takes a signed or unsigned integer value and writes it to standard output.
; Relies on recursive calls to writeValConvert to convert the int to ASCII vals,
; stored into a temp string (arg 2). 1 call to writeValConvert per byte.
; Note: negative ints are first converted to their absolute value (unsigned representation)
; for easier conversion.
; 
; Receives:
;	ebp+8 = @val to be displayed
;	ebp+12= @tempStr
;	ebp+16= @temp val storage (if converted from two's complement)
;	ebp+20= @joe sign flag
;
; Returns: 
;	Nothing.	
;-----------------------------------------------------
		; set up the stack frame
		push			ebp
		mov				ebp,				esp
	
		; save used regs
		push			eax
		push			ebx
		push			edx
		push			edi

		; reset all critical values	
		; clear the memory in tempStr. it can be 12 characters long, which is 3 DWORDs. make enough replacements to clear out all 12 bytes.
		xor				eax,				eax		; eax: 32 bits of punishing void
		mov				ebx,				[ebp+12]; ebx: head of tempStr
		mov				[ebx],				eax		; tempstr[0-3] clear
		add				ebx,				4		; ebx: tempStr[4]
		mov				[ebx],				eax		; tempstr[4-7] clear
		add				ebx,				4		; ebx: tempStr[8]
		mov				[ebx],				eax		; tempstr[8-11] clear

		; clear joe sign flag
		mov				al,					0
		mov				ebx,				[ebp+20]; ebx: @ joe sign flag
		mov				[ebx],				al		; flag now clear

		; get string into edi and set direction flag
		mov				edi,				[ebp+12]; edi: temp string head
		add				edi,				11		; edi: temp string final idx. values to be filled in from here toward the front
		std

		; is the value positive or negative? if positive, leave sign flag blank. if negative, convert back from two's complement before pushing to make int -> char conversion simpler.
		; set joe sign flag
		mov				eax,				[ebp+8] ; eax: @val to be displayed
		mov				eax,				[eax]	; eax: actual val of val to be displayed
		add				eax,				0		; let's see what the sign flag is
		jns				valConvertPnt				; if positive, just jump to convert

		; if here, val is negative.
		; set joe sign flag
		mov				bl,					1
		mov				edx,				[ebp+20]; ebx: @ joe sign flag
		mov				[edx],				bl		; flag now set

		; deconvert from two's complement
		xor				edx,				edx
		imul			eax,				-1		; simple method		

		; push n call recursive function to set up string
	valConvertPnt:
		; get val into mem location so we can push it by address. it's still sitting in eax, whether it was pos or neg
		mov				ebx,				[ebp+16]; ebx: @temp val storage
		mov				[ebx],				eax		; @temp val storage = absolute value of current val

		push			[ebp+16]					; arg1: @abs value of current val
		call			WriteValConvert				; after call: integer converted to ascii vals and stored in tempStr, and edi points to byte before most significant digit char.
		
		; display a + or -, dependent upon the joe sign flag.
		mov				ebx,				[ebp+20]; ebx: @joe sign flag
		mov				ebx,				[ebx]	; ebx: val of joe sign flag
		cmp				bl,					0		; joe sign flag vs. 0
		jnz				negCharPnt					; if sign flag is set, move to insert a minus (-) sign

		; if here, val is positive. insert a + into the pos at edi.
	posCharPnt:
		mov				eax,				43		; eax: ASCII code for plus sign (+)
		stosb										; inserted + before string.
		jmp				dispStringPnt				; skip neg char display

		; if here, val is positive. insert a + into the pos at edi.
	negCharPnt:
		mov				eax,				45		; eax: ASCII code for minus sign (-)
		stosb										; inserted - before string.

	dispStringPnt:
		; display string
		inc				edi							; return edi to the +/- position. otherwise, we may likely be sitting on 00
		mDisplayString	edi							; passing edi rather than the stack-passed ebp, because ASCII representations of user-entered ints may vary from 2-11 bytes. edi is already @head.

		; restore used regs
		pop				edi
		pop				edx
		pop				ebx
		pop				eax

		pop				ebp
		ret				16
writeValMain ENDP

;-----------------------------------------------------
writeValConvert PROC
;
; Recursive function that converts an unsigned integer value into its ASCII digit chars. The digits are stored in order of least-significnt digit
; to most-significant digit (i.e., high string index to low string index, right-to-left). The digit chars are ascertained by continually dividing the
; original val by an order of magnitude, collecting the remainders and adding 48 to get their ASCII representations. The quotient is then passed to the next recursive call.
; Each successive call will thus be working with a val one magnitude smaller than the last. Base case is hit when the quotient is 0.
; (This is sort-of a 'post-test' recursive algorithm; any value passed to this function is necessarily divisible by 10 x times, where x is 0...n.
; Thus, even a value inside magnitude 10^0 will be successfully converted & stored.)
;
; CITATION: Inspiration for the algorithm credited to AsmGuru62 in her/his top comment at this link: https://programmersheaven.com/discussion/67893/converting-number-to-string
; 
; Receives:
;	edi	  = @ of current pos in temp string
;			(I know this is being passed to the subproc in a reg, but it's the only way to use stosb in a recursive function.
;			Pushing @curpos onto the stack would be pointless. edi is push/popped in main calling func.)
;	ebp+8 = @abs val of cur val. (No telling what would happen if a signed negative int were passed here).
;
; Returns: 
;	Nothing, but the temp string will be filled with everything but the +/- (handled in main caller).
;-----------------------------------------------------
		; set up the stack frame
		push		ebp
		mov			ebp,		esp
	
		; save used regs
		push		eax
		push		ebx
		push		edx

		; divide val by 10 to break into quotient & remainder
		mov			eax,		[ebp+8]		; eax: @ cur val
		mov			eax,		[eax]		; eax: cur val itself
		mov			ebx,		10
		xor			edx,		edx			; edx gets so dirty all the time. more like esx, amirite?
		div			ebx						; eax: quotient; edx: remainder (i.e., the next digit, not yet ASCII-fied)
		push		eax						; store the quotient while processing the remainder

		; convert remainder to ASCII code and store as char
		add			dl,			48			; edx: ASCII representation of quotient
		mov			al,			dl			; al: ASCII representation of quotient
		stosb								; ASCII-represented value now stored in string, and string has been decremented to next char location
	
		; base case checking
		; have we done all of the digits? if quotient > 0, then we can still divide further into it.
		pop			eax						; restore the quotient
		cmp			eax,		0			; quotient vs. 0
		jz			writeValConvertEndPnt	; if quotient is 0, then we've processed the entire integer. terminate.

		; if here, quotient > 0. get the next char.
		mov			ebx,		[ebp+8]		; ebx: @curval
		mov			[ebx],		eax			; @curval = quotient, i.e., previous curVal with least-significant place removed.
		push		[ebp+8]					; arg1 = @curVal, least-significant place removed.
		call		WriteValConvert

		; if here, either we've processed the entire digit, or we're unzipping.
	writeValConvertEndPnt:
		; restore used regs
		pop			eax
		pop			ebx
		pop			edx

		pop			ebp
		ret			4
writeValConvert ENDP

;-----------------------------------------------------
calcAvg PROC
;
; Divides the sum (arg1) by the integer count (arg2) and stores the result in arg3.
; 
; Receives:
;	ebp+8 = @sum
;	ebp+12= @int count
;	ebp+16= @location to store the quotient
;	ebp+20= @joe sign flag
;
; Returns: 
;	The quotient will be stored in arg3. The remainder is discarded.
;-----------------------------------------------------
		; set up the stack frame
		push	ebp
		mov		ebp,	esp
	
		; save used regs
		push	eax
		push	ebx
		push	edx

		; reset sign flag
		mov				al,					0
		mov				ebx,				[ebp+20]; ebx: @ joe sign flag
		mov				[ebx],				al		; flag now clear
		
		; is the value positive or negative? if positive, leave sign flag blank. if negative, convert back from two's complement to make avg calc simpler.
		; set joe sign flag
		mov				eax,				[ebp+8] ; eax: @sum
		mov				eax,				[eax]	; eax: sum val
		add				eax,				0		; let's see what the sign flag is
		jns				avgCalcPnt					; if positive, just jump to calc

		; if here, val is negative.
		; set joe sign flag
		mov				bl,					1
		mov				edx,				[ebp+20]; ebx: @ joe sign flag
		mov				[edx],				bl		; flag now set

		; deconvert from two's complement
		xor				edx,				edx
		imul			eax,				-1		; simple method		

		; calculate
	avgCalcPnt:
		mov		ebx,			[ebp+12]; ebx: @int count
		mov		ebx,			[ebx]	; ebx: int count val
		xor		edx,			edx
		idiv	ebx						; eax: avg

		; if sum was negative, convert back
		mov		ebx,			[ebp+8] ; eax: @sum
		mov		ebx,			[ebx]	; eax: sum val
		add		ebx,			0		; let's see what the sign flag is
		jns		avgStorePnt				; if positive, just jump to store

		; if here, val is negative.
		; convert back to two's complement
		xor				edx,				edx
		imul			eax,				-1		; simple method		

		; store avg
	avgStorePnt:
		mov		ebx,	[ebp+16]; ebx: @quo storage
		mov		[ebx],	eax		; storeQuo now stored with quo, donchyaknow

		; restore used reg
		pop		edx
		pop		ebx
		pop		eax

		pop		ebp
		ret		16
calcAvg ENDP

END main