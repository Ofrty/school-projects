TITLE Program 05 - QuickSort     (Project05.asm)

; Author: Kirkham, Joe
; Last Modified: 2018/05/29
; OSU email address: kirkhamj@oregontate.edu
; Course number/section: CS271/400
; Project Number: 05                Due Date: 2018/05/28 12:00:00AM **(+2 GRACE DAYS = 2018/05/30 12:00:00AM PST)**
; Description:	This program displays its and its programmer's names, then prompts the user to enter a (validated) integer N between [MIN,MAX] (defined as constants below),
;				and explains that the program will generate N pseudo-random integers in the range [LO,HI] (defined as constants below), display the raw list, sort the list
;				display the median, and then display the sorted list.
;
;				The program then follows on its promises to the user.
; 

INCLUDE Irvine32.inc

		MIN		=	10		; min of N integers that may be entered by user
		MAX		=	200		; max of N integers that may be entered by user
		LO		=	100		; lowest possible pseudorandom int
		HI		=	999		; highest possible pseudorandom int
		INSCNT	=	5		; # of instruction ilnes
		LINE_LIM=	10		; max vals displayed per line

.data

		prog		BYTE	"*****Project 05 - QuickSort*****",0
		prgrmr		BYTE	"*****Written by Joe Kirkham*****",0
		ec1			BYTE	"**EC1: (Option 2): Recursive QuickSort algorithm used for sorting",0
		instra		BYTE	"**Enter an integer N between [10,200]. I will then do the following:",0
		instrb		BYTE	"-- Generate N pseudorandom integers in the range [100,999]",0
		instrc		BYTE	"-- Display these N integers in the order in which they were generated",0
		instrd		BYTE	"-- Calculate and display the (rounded) median value of the integers",0
		instre		BYTE	"-- Sort descending and display the list",0
		request		DWORD	?			; user-entered integer N; determines how many pseudorandom integers to display
		prompt		BYTE	"**Enter integer: ",0
		invld		BYTE	"**The entered integer N must be in range [10,200] inclusive. Please try again.",0
		array		DWORD	MAX	DUP(0)	; array into which pseudorandom ints will be placed. initialized to 0 to detect end of user ints
		ttlUnsort	BYTE	"**The unsorted random numbers:",0
		ttlSort		BYTE	"**The sorted random numbers:",0
		buff		BYTE	"   ",0		; buffer between displayed vals
		medText		BYTE	"**The (rounded) median value is: "

.code
main PROC

		; set random seed
		call	Randomize

		; introduction and instructions
		push	OFFSET ec1			; arg (n - EC#)...arg n = extra credit lines
		push	OFFSET instre
		push	OFFSET instrd
		push	OFFSET instrc
		push	OFFSET instrb
		push	OFFSET instra		; arg 4...arg (argc - EC#) = instruction lines
		push	OFFSET prgrmr		; arg 3 = progammer name
		push	OFFSET prog			; arg 2 = program name
		push	inscnt				; arg 1 = # of instruction lines
		call	introduction

		; get user request
		push	OFFSET invld		; arg 3 = invalid entry message
		push	OFFSET prompt		; arg 2 = prompt text
		push	OFFSET request		; arg 1 = address of integer request variable
		call	getData

		; fill array
		push	OFFSET array		; arg 2 = array head address
		push	request				; arg 1 = N requested integers to display
		call	fillArray

		; display the unsorted list
		push	OFFSET buff			; arg 4 = buffer between vals on same line
		push	OFFSET ttlUnsort	; arg 3 = title of the list (unsorted)
		push	request				; arg 2 = # of vals to display
		push	OFFSET array		; arg 1 = array head address
		call	displayList

		; sort the list using QuickSort
		push	request				; arg 3 = N vals in array AKA end index+1
		push	0					; arg 2 = beginning index
		push	OFFSET array		; arg 1 = array head address
		call	sortListQuickMain	; top recursive sorting call

		; calc and display the median value of the now-sorted list
		push	OFFSET medText		; arg 3 = median message text
		push	request				; arg 2 = length of array
		push	OFFSET array		; arg 1 = array head address
		call	displayMedian

		; display the sorted list
		push	OFFSET buff			; arg 4 = buffer between vals on same line
		push	OFFSET ttlSort		; arg 3 = title of the list (unsorted)
		push	request				; arg 2 = # of vals to display
		push	OFFSET array		; arg 1 = array head address
		call	displayList

		exit	; exit to operating system
main ENDP

;-----------------------------------------------------
introduction PROC USES ecx edx ; +8 to stack parameters
;
; Simple procedure that displays the progam and user name,
; and provides instructions to the user.
; 
; Receives:
;	ebp+16 = instruction line count
;	ebp+20 = program name
;	ebp+24 = programmer name
;	ebp+28 = instruction line A
;	ebp+32 = instruction line B
;	ebp+36 = instruction line C
;	ebp+40 = instruction line D
;	ebp+44 = instruction line E
;	ebp+48 = extra credit 1 text
;
; Returns: 
;	Nothing.
;-----------------------------------------------------
		; set the stack frame
		push	ebp
		mov		ebp,			esp

		; display program name
		mov		edx,			[ebp+20]
		call	WriteString
		call	CrLF

		; display user name
		mov		edx,			[ebp+24]
		call	WriteString
		call	CrLF
		call	CrLF

		; display EC
		mov		edx,			[ebp+48]
		call	WriteString
		call	CrLF
		call	CrLF
		
		; display instructions via loop
		mov		ecx,			[ebp+16]	; # of instruction lines in ecx
		mov		ebx,			28			; initialize offset to 28; would be better if this val could be programmaticaly ascertained

	instDispStrtPnt:
		mov		edx,			[ebp+ebx]	; instructions in ebp + offset to next inst line
		call	WriteString
		call	CrLF
		add		ebx,			4			; increment offset to next inst line in ebp

		loop	instDispStrtPnt

		call	CrLF

		pop		ebp
		ret		44
introduction ENDP

;-----------------------------------------------------
getData PROC USES eax ebx edx ; +12 to stack parameters
;
; Prompts user to enter an integer N in range [MIN,MAX].
; Validates that the integer is in range, and returns reprompts
; user if necessary
; 
; Receives:
;	ebp+20 = address of request variable
;	ebp+24 = prompt text
;	ebp+28 = invalid entry text
;
; Returns: 
;	request = validated user integer N
;-----------------------------------------------------
		; set the stack frame
		push	ebp
		mov		ebp,			esp

		; prompt user
	promptPnt:
		mov		edx,			[ebp+24]
		call	WriteString

		; read value
		call	ReadInt
		
		; validate request
		cmp		eax,			MIN			; too small?
		jl		invalidPnt					; if so, tell user and reprompt
		cmp		eax,			MAX			; too big?
		jg		invalidPnt					; if so, tell user and reprompt
		jmp		validPnt					; else, the int was valid. jump to store and return

		; if here, integer was invalid. display error message and jump up to reprompt user
	invalidPnt:
		call	CrLF
		mov		edx,			[ebp+28]
		call	WriteString
		call	CrLF
		jmp		promptPnt

		; if here, integer is valid. store in the address of request variable
	validPnt:
		mov		ebx,			[ebp+20]	; addr of request now in ebx
		mov		[ebx],			eax			; validated request now in request

		pop		ebp
		ret		12
getData ENDP

;-----------------------------------------------------
fillArray PROC USES	eax ebx ecx edi ; +16 to stack parameters
;
; Generates pseudorandom integers in the range [LO,HI],
; and places these integers into an array.
; 
; Precondition: Irvine Randomize PROC called prior to fillArray (generating random seed)
;
; Receives:
;	ebp+24 = N requested integers to display 
;	ebp+28= array head addres
;
; Returns:
;	The passed array will be filled with ints.
;-----------------------------------------------------
		; set the stack frame
		push	ebp
		mov		ebp,			esp

		; point edi to head of array
		mov		edi,			[ebp+28]
		cld								; ensure direction flag is clear so that we inc edi forward

		; via loop, generate ints and place into array N times
		mov		ecx,			[ebp+24]; N now in counter
		xor		ebx,			ebx		; initialize edi OFFSET

	fillStartPnt:
		; generate int into eax
		; satisfy precondition that MAX val in eax. compensate for range [LO,HI]
		mov		eax,			HI		; hi...
		sub		eax,			LO		;      (hi-lo)...
		inc		eax						;                (hi-lo)+1

		call	RandomRange				; pseudorandom int now in eax
		add		eax,			LO		; compensate for range

		; place pseudorandom int into array, inc edi
		stosd							; wrapper for register-indirect addressing; stosd = mov [esi], eax && add eax, 4 (dir flag = 0)

		; go to next int (or end)
		loop	fillStartPnt

		pop		ebp
		ret		8
fillArray ENDP

;-----------------------------------------------------
displayList PROC USES eax ecx edx esi ; +0 to stack params because LOCAL executes first, doing ebp stuff before pushing the regs
;
; Displays N (arg1) elements of a DWORD array (arg2), 10 numbers per line.
; The list will be identified by the passed title (arg3).
; 
; Receives:
;	ebp+8= address of array to display
;	ebp+12= N integers to display (generally array.length)
;	ebp+16= title of list
;	ebp+20= buffer between vals on the same line
;
; Returns:
;	Nothing.
;-----------------------------------------------------
		; declare local variables; ALREADY PUSHES EBP AND SETS EBP=ESP
		LOCAL	lineCount:	BYTE		; number of vals displayed on this line

		; display list title
		call	CrLF
		call	CrLF
		mov		edx,		[ebp+16]
		call	WriteString
		call	CrLF
		call	CrLF

		; read array head into esi
		mov		esi,		[ebp+8]
		cld								; ensure direction flag is clear

		; loop through array, displaying each element, and checking for # already on line
		mov		ecx,		[ebp+12]	; initialize counter with N integers to display
		mov		lineCount,	0			; initialize lineCount to 0

	dispLoopStartPnt:
		; move current element to eax and increment esi
		lodsd							; wrapper for register-indirect addressing; lodsd = mov eax, [esi] && add eax, 4 (dir flag = 0)

		; display element
		call	WriteDec
		
		; handle whitespacing
	whitespacePnt:
		; update count shown on this line, and check if we've hit max vals per line
		inc		lineCount
		cmp		lineCount,	LINE_LIM

		; if still room on this line, then move to add buffer
		jnz		buffPnt

		; if here, we've run out of room on this line. add new line, reset line counter, and jump to end
		call	CrLF
		mov		lineCount,	0
		jmp		dispLoopEndPnt

		; if here, there's still room on this line, so write buffer
	buffPnt:
		mov		edx,		[ebp+20]	; buffer now in edx
		call	WriteString

	dispLoopEndPnt:
		loop	dispLoopStartPnt
		
		call	CrLF
		call	CrLF

		;pop	ebp						; already handled by LOCAL directive
		ret		16
displayList ENDP

;-----------------------------------------------------
sortListQuickMain PROC
;
; Main, recursive function of QuickSort. Sorts the input
; array in DESCENDING order. Relies on recursive calls
; to itself and calls to helper function sortListQuickPart.
; 
; Receives:
;	ebp+8 = address of current array portion to be sorted
;	ebp+12= beginning index
;	ebp+16= length (NOT the ending index)
;
; Returns:
;	Nothing, but the input array will be altered/sorted.
;-----------------------------------------------------
		; set the stack frame
		push	ebp
		mov		ebp,			esp

		; check base case
		mov		eax,			[ebp+12]
		mov		ebx,			[ebp+16]
		dec		ebx
		cmp		eax,			ebx
		jge		endQuickMainPnt		; base case. we done. commence unzipping.

		; if here, we still have partitioning to do.
		; get initial pivot index
		push	[ebp+16]			; arg 3 = end index
		push	[ebp+12]			; arg 2 = beg index
		push	[ebp+8]				; arg 1 = array head

		call	sortListQuickPart	; returns pivot index to edx

		push	edx					; store pivot index to stack to restore later

		; sort before the pivot index
		push	edx					; arg 3 = end index of bottom portion of array
		push	[ebp+12]			; arg 2 = beg index of bottom portion of array
		push	[ebp+8]				; arg 1 = array head
		call	sortListQuickMain	; recursive call to quicksort for bottom portion of array

		; sort after the pivot index
		pop		edx					; restore original pivot index
		inc		edx					; pivot + 1
		push	[ebp+16]			; arg 3 = end index of top portion of array
		push	edx					; arg 2 = beg index of top portion of array
		push	[ebp+8]				; arg 1 = array head
		call	sortListQuickMain	; recursive call to quicksort for top portion of array

	; done with this portion of the array.
	endQuickMainPnt:
		pop		ebp
		ret		12
sortListQuickMain ENDP

;-----------------------------------------------------
sortListQuickPart PROC
;
; Helper function of sortListQuickMain. Incrementally loops
; from the beginning index to the end, moving larger
; elements to smaller index positions and vice-versa.
; Relies on calls to exchangeElements for swapping.
; 
; Receives:
;	ebp+8 = address of current array portion to be sorted
;	ebp+12= beginning index
;	ebp+16= ending index+1
;
; Returns:
;	EDX = the pivot index to be relied on for further partitioning & sorting
;-----------------------------------------------------
		; declare local variables; ALREADY PUSHES EBP AND SETS EBP=ESP
		LOCAL	pivotVal:	DWORD,		; the value of pivot index, being compared. will always be the value of the top-most index
				iOff:		DWORD,		; offset from arr head of moving index "i" value; will be initialized to top index + 1 (high-level); SHOULD ALWAYS BE IN TERMS OF BYTES (*4)
				jOff:		DWORD		; offset from arr head of "j" index traversing the entire list, denoting the current index being compared; SHOULD ALWAYS BE IN TERMS OF BYTES (*4)

		; set array head to esi
		mov		esi,		[ebp+8]

		; initialize vars (except jindex, which will be in loop)
		; moving index
		mov		eax,		[ebp+16]	; end index+1 into eax
		mov		ebx,		4			; mul by 4 to convert to dword
		xor		edx,		edx
		mul		ebx						; offset of end index + 1 now in eax
		sub		eax,		4			; compensate for the pivot index, which won't be compared against itself
		mov		iOff,		eax			; the moving index is now initialized to the last index

		; pivotVal
		add		eax,		esi			; @pivotVal now in eax
		mov		eax,		[eax]		; value of the pivot val is now in eax
		;mov		eax,		[esi+eax]
		mov		pivotVal,	eax			; put value of last index into pivotVal

		; loop counter
		mov		eax,		[ebp+16]	; end index+1 now in eax
		dec		eax						; compensate for pivot index, which won't be compared against itself
		sub		eax,		[ebp+12]	; compensate for beginning index sets counter to end minus beginning
		mov		ecx,		eax			; put into counter
		
		; loop through elements, comparing to pivot val. swap into moving index if a lower-indexed value is smaller than the higher indexed value
	partLoopStartPnt:
		; get current "j". we can use the counter to determine our j, but we have to compensate for our pos in the array by accounting for the beg index
		mov		eax,		4			; dword conversion factor
		xor		edx,		edx			; clear edx for multiplication
		mov		ebx,		ecx			; counter is now in ebx
		add		ebx,		[ebp+12]	; compensate for the beg index
		mul		ebx						; current element offset now in eax
		sub		eax,		4			; compensate for count->index (4)
		mov		jOff,		eax

		add		eax,		esi			; @cur element is now in eax
		mov		eax,		[eax]		; value of the current index is now in eax
		cmp		eax,		pivotVal	; compare value in cur index to pivotVal
		jge		partLoopEndPnt			; current element is already greater than or equal, so do nothing. loop.

		; if here, cur element <= pivotVal, so we need decrement our running index and to swap elems
		sub		iOff,		4			; dec running index in terms of Bytes

		; arg 2 prep: cur eleme address
		mov		ebx,		esi			; array head address now in ebx
		add		ebx,		jOff		; address of cur eleme now in ebx

		; arg 1 prep: mov index address
		mov		eax,		esi			; array head address now in eax
		add		eax,		iOff		; address of moving index now in eax
		
		push	ebx						; arg 2 = address of current element
		push	eax						; arg 1 = address of index

		call	exchangeElements		; swap index of current elem and index

		; end of work with the current element, go to next element if there is one.
	partLoopEndPnt:
		loop	partLoopStartPnt

		; last thing to do is get the pivot val into the right location. swap top elem with the spot in moving index
		;add		iOff,		4

		; arg 2 prep: address of pivot/end elem
		mov		eax,		[ebp+16]	; length now in eax
		dec		eax						; pivot/end index now in eax
		mov		ebx,		4			
		mul		ebx						; pivot/end index in terms of Byte offset in eax
		add		eax,		esi			; add base array address to the pivot offset in eax, so now pivot/end address in eax
		push	eax						; arg 2 = pivot/end address

		; arg 1 prep: address of moving index
		mov		eax,		iOff		; moving index in eax
		add		eax,		esi			; add base array address to the moving index offset in eax, so now pivot/end address is in eax
		push	eax						; arg 1 = address of index

		call	exchangeElements

		; get the pivot index into edx for return
		mov		eax,		iOff		; put iOff (in terms of Bytes) into eax
		mov		ebx,		4
		xor		edx,		edx
		div		ebx						; index now in terms of high-level elem-counting in eax
		mov		edx,		eax			; index in return position

		; pop	ebp						; ALREADY DONE BY LOCAL INSTR, WHICH GENERATES A LEAVE INSTRUCTION
		ret		12
sortListQuickPart ENDP

;-----------------------------------------------------
displayMedian PROC USES	eax ebx edx ; +12 to stack params
;
; Calculates and displays the the median of a sorted array
; 
; Precondition: The array parameter is sorted.
;
; Receives:
;	ebp+20= array head address
;	ebp+24= N integers to display (generally array.length)
;	ebp+28= median display text
;
; Returns:
;	Nothing.
;-----------------------------------------------------
		; set the stack frame
		push	ebp
		mov		ebp,		esp

		; point esi to array head
		mov		esi,		[ebp+20]

		; calculate the median index
		; N/2
		mov		eax,		[ebp+24]	; N now in eax
		mov		ebx,		2
		xor		edx,		edx			; clear edx
		div		ebx						; N/2: quotient in eax, rem in edx
		
		; if even, take average of quotient & quotient-1
		cmp		edx,		1			; if zero, then N is odd. if non-zero, then N is even.
		jz		oddPnt					; if odd, then find the median for odd. else will find median for even.

		; if here, N is even. calculate average of (quotient as HLL-index) & (quotient-1 as HLL-index)
		; convert HLL-index to DWORD offset
		dec		eax						; move down one index (accounting for 0-indexing of arrays!)
		mov		ebx,		4
		xor		edx,		edx			; clear edx
		mul		ebx						; eax now has correct DWORD elem OFFSET to median1 value

		; get median1 value
		add		esi,		eax			; array head + median1 OFFSET
		mov		eax,		[esi]		; median1 value now in eax

		; get median2 value
		add		esi,		4			; increment to median2 OFFSET
		add		eax,		[esi]		; add median2 value to median1 value in eax
		mov		ebx,		2
		div		ebx						; median average quotient now in eax, rem in edx

		; round the calc'd value, if median1+median2 was odd
		add		eax,		edx			; if median1+median2 was even, then edx is 0 and this doesn't change eax. if odd, then m1+m2 was odd, and after /2 needs to be rounded up.

		jmp		dispMedianPnt			; display the median, jumping over the odd branch

		; if here, N is odd. just use quotient as HLL-index
	oddPnt:
		; convert HLL-index to DWORD offset
		mov		ebx,		4
		xor		edx,		edx			; clear edx
		mul		ebx						; eax now has correct DWORD elem OFFSET to median value

		; get median value
		add		esi,		eax			; array head + median OFFSET
		mov		eax,		[esi]		; median value now in eax

		; display calculated median
	dispMedianPnt:
		mov		edx,		[ebp+28]	; median text msg now in edx
		call	WriteString				; display median text
		call	WriteDec				; display median value, which is still in eax from either even or odd branch above
		
		call	CrLF

		pop		ebp
		ret		12
displayMedian ENDP

;-----------------------------------------------------
exchangeElements PROC USES eax esi edi	; +12 to parameters
;
; Exchanges the values of the two passed addresses.
; 
; Receives:
;	ebp+20= address of val1
;	ebp+24= address of val2
;
; Returns:
;	Nothing, but the values in the addresses will be swapped.
;
; CITATION: Used general structure of example SWAP PROC in Irvine "Assembly Language for x86 Processors", v.6, Ch. 8, p. 308.
;
;-----------------------------------------------------
		; set the stack frame
		push	ebp
		mov		ebp,		esp

		; exchange elements
		mov		esi,		[ebp+20]	; addr1 in esi
		mov		edi,		[ebp+24]	; addr2 in edi
		mov		eax,		[esi]		; val1 in eax
		xchg	eax,		[edi]		; val 1 now in addr2, val 2 now in eax
		mov		[esi],		eax			; val 2 now in addr1

		pop		ebp
		ret		8
exchangeElements ENDP

;-----------------------------------------------------
showStack PROC
;
; Shows the values on the stack in DWORD intervals, ebp-16 to ebp+40.
; For debugging purposes.
; 
; Receives:
;	ebp+8 = EBP of calling stack.
;
; Returns:
;	Nothing.
;
; FUTURE:
;	Have this read in two more parameters:
;		--x DWORD boundaries to display below stack
;		--y DWORD boundaries to display above the stack
;-----------------------------------------------------
		; set stack frame
		push	ebp
		mov		ebp,		esp

		; manually save eax ebx and esi
		push	eax
		push	ebx
		push	esi

		call	CrLF
		call	CrLF
		mov		esi,		[ebp+8]	; put calling stack EBP into ESI

		; show values on the stack at dword interval up from ebp-16 to ebp+40 Bytes
		mov		eax,		-16
		call	WriteInt
		call	CrLF
		mov		eax,		[esi-16]
		call	WriteDec
		call	CrLF
		call	CrLF
		
		mov		eax,		-12
		call	WriteInt
		call	CrLF
		mov		eax,		[esi-12]
		call	WriteDec
		call	CrLF
		call	CrLF
		
		mov		eax,		-8
		call	WriteInt
		call	CrLF
		mov		eax,		[esi-8]
		call	WriteDec
		call	CrLF
		call	CrLF
		
		mov		eax,		-4
		call	WriteInt
		call	CrLF
		mov		eax,		[esi-4]
		call	WriteDec
		call	CrLF
		call	CrLF
		
		mov		eax,		0
		call	WriteInt
		call	CrLF
		mov		eax,		[esi]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		4
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+4]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		8
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+8]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		12
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+12]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		16
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+16]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		20
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+20]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		24
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+24]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		28
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+28]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		32
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+32]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		36
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+36]
		call	WriteDec
		call	CrLF
		call	CrLF

		mov		eax,		40
		call	WriteInt
		call	CrLF
		mov		eax,		[esi+40]
		call	WriteDec
		call	CrLF
		call	CrLF
		
		; restore old eax ebx and esi
		pop		esi
		pop		ebx
		pop		eax

		pop		ebp
		ret		4
showStack ENDP

END main