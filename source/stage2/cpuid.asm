; __cdecl extern bool cpuid_exist(void);
[extern cpuid_exist]
cpuid_exist:
	push ebp
	mov ebp, esp

	pushfd
	pushfd
	xor dword [esp], 0x00200000
	popfd
	pushfd
	pop eax
	xor eax, [esp]
	popfd
	test eax, 0x00200000
	jz .done
	xor eax, eax
	inc eax

.done:
	mov esp, ebp
	pop ebp
	ret
