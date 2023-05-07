/**
 * 内核
 */
#include "os_cfg.h"

# 不必加.code32因默认就是32位
.text
.global _start
.extern kernel_init
.extern init_main

# void start(boot_info_t* boot_info)
_start:
  	push %ebp
	mov %esp, %ebp
	mov 0x8(%ebp), %eax # 0x8(%ebp) -> 0x8+%ebp

	# kernel_init(boot_info)
  	push %eax # 压入kernel_init()函数所需要的参数
	call kernel_init

	jmp $KERNEL_SELECTOR_CS, $gdt_reload
gdt_reload:
	// 重新加载所有的数据段描述符
	mov $KERNEL_SELECTOR_DS, %ax // 数据段选择子
	mov %ax, %ds
	mov %ax, %ss
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	// 栈设置
	mov $(stack + KERNEL_STACK_SIZE), %esp

	jmp init_main

.bss
.comm stack, KERNEL_STACK_SIZE // comm声明未初始化的通用内存区域，以字节计