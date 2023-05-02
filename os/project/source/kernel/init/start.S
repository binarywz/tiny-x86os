/**
 * 内核
 */
// 不必加.code32因默认就是32位
.text
.global _start
.extern kernel_init

_start:
	call kernel_init
	jmp .