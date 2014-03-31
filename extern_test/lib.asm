include c:\masm32\include\masm32rt.inc

hello PROTO :BYTE, :DWORD, :WORD

.code
Yannis PROC instance:dword,reason:dword,unused:dword
	mov eax,1
	ret
Yannis ENDP

hello PROC truc1:BYTE, truc2:DWORD, truc3:WORD
	printf("Hello %s\n", truc2)
	ret
hello ENDP

end Yannis
