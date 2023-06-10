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
#include "core/task.h"
#include "common/cpu_instr.h"
#include "tools/list.h"

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

static task_t entry_task;
static uint32_t entry_task_stack[1024];	// 空闲任务堆栈

void init_task_entry(void) {
    int count = 0;
    for (;;) {
        log_printf("init task: %d", count++);
        task_switch_from_to(&entry_task, task_main_task());
    }
}

/**
 * 链表功能验证代码
 */ 
void list_validate(void) {
    struct type_t {
        int i;
        list_node_t node;
    } v = {0x123456};

    list_node_t* v_node = &v.node;
    struct type_t* p = list_node_parent(v_node, struct type_t, node);
    if (p->i != 0x123456) {
        log_printf("error");
    }
}

void init_main(void) {
    // 链表功能验证
    // list_validate();

    log_printf("Kernel is running...");
    log_printf("Version: %s", OS_VERSION);
    log_printf("%d %d %x %c", -123, 123456, 0x12345, 'a');

    // 断言功能调试
    // int a = 3;
    // ASSERT(a > 2);
    // ASSERT(a < 2);

    // 异常处理调试
    // int a = 3 / 0;
    // irq_enable_global(); // 开启定时器中断

    /**
     * 初始化任务
     * x86的栈是从高地址往低地址，故栈顶指针设置为(uint32_t)&entry_task_stack[1024]
     */ 
    task_init(&entry_task, (uint32_t)init_task_entry, (uint32_t)&entry_task_stack[1024]);
    task_main_init();

    int count = 0;
    for (;;) {
        log_printf("init main: %d", count++);
        task_switch_from_to(task_main_task(), &entry_task);
    }
}