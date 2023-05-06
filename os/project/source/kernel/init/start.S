/**
 * 内核
 */
// 不必加.code32因默认就是32位
.text
.global _start
.extern kernel_init

# void start(boot_info_t* boot_info)
_start:
  	push %ebp
	mov %esp, %ebp
	mov 0x8(%ebp), %eax # 0x8(%ebp) -> 0x8+%ebp

	# kernel_init(boot_info)
  	push %eax # 压入kernel_init()函数所需要的参数
	call kernel_init
	jmp .