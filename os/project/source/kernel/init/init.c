/**
 * 内核初始化以及测试代码
 */
#include "common/boot_info.h"
#include "cpu/cpu.h"

/**
 * 内核入口
 * 汇编调用C函数如何传参: 通过栈传递参数
 */
void kernel_init(boot_info_t* boot_info) {
    cpu_init();
    for(;;) {}
}