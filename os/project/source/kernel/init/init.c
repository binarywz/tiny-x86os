/**
 * 内核初始化以及测试代码
 */
#include "init.h"
#include "common/boot_info.h"
#include "cpu/cpu.h"
#include "cpu/irq.h"
#include "dev/timer.h"
#include "tools/log.h"
#include "os_cfg.h"
#include "tools/klib.h"

/**
 * 内核入口
 * 汇编调用C函数如何传参: 通过栈传递参数
 */
void kernel_init(boot_info_t* boot_info) {
    // 初始化完成后重新加载GDT
    cpu_init();
    // 初始化日志模块
    log_init();
    // 初始化IDT
    irq_init();
    // 初始化定时器
    timer_init();
}


void init_main(void) {
    log_printf("Kernel is running...");
    log_printf("Version: %s", OS_VERSION);
    log_printf("%d %d %x %c", -123, 123456, 0x12345, 'a');

    // 断言功能调试
    int a = 3;
    ASSERT(a > 2);
    ASSERT(a < 2);

    // 调试使用
    // int a = 3 / 0;
    // irq_enable_global(); // 开启定时器中断
    for (;;) {}
}