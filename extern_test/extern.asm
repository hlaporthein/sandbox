.386
.model flat, stdcall
option casemap:none

includelib \masm32\lib\kernel32.lib

hello PROTO :BYTE, :DWORD, :WORD
ExitProcess PROTO C :DWORD

.data
Message      db "Yannis",0

.code
start:
	invoke hello, 0, addr Message, 0
	invoke ExitProcess,0

end start
