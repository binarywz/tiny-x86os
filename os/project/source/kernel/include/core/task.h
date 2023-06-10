/**
 * 任务实现
 */
#ifndef TASK_H
#define TASK_H

#include "common/types.h"
#include "cpu/cpu.h"
#include "tools/list.h"

/**
 * @brief 任务控制块结构
 */
typedef struct _task_t {
    // uint32_t* stack;        // 栈顶指针
	tss_t tss;				// 任务的TSS段
    uint16_t tss_sel;		// tss选择子
}task_t;

/**
 * task: 任务控制块结构
 * entry: 程序入口地址
 * esp: 栈顶指针
 */ 
int task_init(task_t* task, uint32_t entry, uint32_t esp);
void task_switch_from_to(task_t* from, task_t* to);

/**
 * from -> from栈的地址
 * to   -> to栈的栈顶指针
 */ 
void simple_switch(uint32_t** from, uint32_t* to);

typedef struct _task_manager_t {
    task_t* curr_task;  // 当前运行的任务

	list_t ready_list;  // 就绪队列
	list_t task_list;	// 所有已创建任务的队列

	task_t main_task;	// 内核任务
} task_manager_t;

void task_manager_init(void);
void task_main_init(void);
task_t* task_main_task(void);

#endif

