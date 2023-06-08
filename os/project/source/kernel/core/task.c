/**
 * 任务管理
 */
#include "common/cpu_instr.h"
#include "core/task.h"
#include "tools/klib.h"
#include "os_cfg.h"
#include "tools/log.h"

static int tss_init(task_t* task, uint32_t entry, uint32_t esp) {
    // 为TSS分配GDT
    int tss_sel = gdt_alloc_desc();
    if (tss_sel < 0) {
        log_printf("alloc tss failed.\n");
        return -1;
    }

    segment_desc_set(tss_sel, (uint32_t)&task->tss, sizeof(tss_t), 
            SEG_P_PRESENT | SEG_DPL0 | SEG_TYPE_TSS);

    // tss段初始化
    kernel_memset(&task->tss, 0, sizeof(tss_t));
    task->tss.eip = entry;
    task->tss.esp = task->tss.esp0 = esp;
    task->tss.ss0 = KERNEL_SELECTOR_DS;
    task->tss.eip = entry;
    task->tss.eflags = EFLAGS_DEFAULT | EFLAGS_IF;
    task->tss.es = task->tss.ss = task->tss.ds 
            = task->tss.fs = task->tss.gs = KERNEL_SELECTOR_DS; // 暂时写默认值
    task->tss.cs = KERNEL_SELECTOR_CS; // 暂时写默认值
    task->tss.iomap = 0;
    task->tss_sel = tss_sel; // 保存段选择子
    return 0;
}

/**
 * @brief 初始化任务
 */
int task_init(task_t* task, uint32_t entry, uint32_t esp) {
    ASSERT(task != (task_t*)0);

    // uint32_t* pesp = (uint32_t*)esp;
    // if (pesp) {
    //     *(--pesp) = entry;
    //     *(--pesp) = 0;
    //     *(--pesp) = 0;
    //     *(--pesp) = 0;
    //     *(--pesp) = 0;
    //     task->stack = pesp;
    // }

    // 使用栈的方式切换进程时将tss初始化进行注释
    tss_init(task, entry, esp);
    return 0;
}

/**
 * @brief 切换至指定任务
 */
void task_switch_from_to(task_t* from, task_t* to) {
    switch_to_tss(to->tss_sel);
    // 后续代码不使用栈进行切换任务
    // simple_switch(&from->stack, to->stack);
}