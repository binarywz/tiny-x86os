/**
 * 任务管理
 */
#include "common/cpu_instr.h"
#include "core/task.h"
#include "tools/klib.h"
#include "os_cfg.h"
#include "tools/log.h"

static task_manager_t task_manager; // 任务管理器

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
int task_init(task_t* task, const char* name, uint32_t entry, uint32_t esp) {
    ASSERT(task != (task_t*)0);

    // 使用栈的方式切换进程时将tss初始化进行注释
    tss_init(task, entry, esp);

    // 任务字段初始化
    kernel_strncpy(task->name, name, TASK_NAME_SIZE);
    task->state = TASK_CREATED;
    list_node_init(&task->all_node);
    list_node_init(&task->run_node);

    // 插入就绪队列中和所有的任务队列中
    task_set_ready(task);
    list_insert_last(&task_manager.task_list, &task->all_node);

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

/**
 * @brief 将任务插入就绪队列
 */
void task_set_ready(task_t* task) {
    list_insert_last(&task_manager.ready_list, &task->run_node);
    task->state = TASK_READY;
}

/**
 * @brief 将任务从就绪队列移除
 */
void task_set_block(task_t* task) {
    list_remove(&task_manager.ready_list, &task->run_node);
}

void task_main_init(void) {
    task_init(&task_manager.main_task, "main task", 0, 0);

    // 写TR寄存器，指示当前运行的第一个任务
    write_tr(task_manager.main_task.tss_sel);
    task_manager.curr_task = &task_manager.main_task;
}

/**
 * @brief 返回初始任务
 */
task_t* task_main_task(void) {
    return &task_manager.main_task;
}

/**
 * @brief 任务管理器初始化
 */
void task_manager_init (void) {
    // 各队列初始化
    list_init(&task_manager.ready_list);
    list_init(&task_manager.task_list);

    task_manager.curr_task = (task_t*)0;
}