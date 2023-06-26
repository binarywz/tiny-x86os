/**
 * 任务实现
 */
#ifndef TASK_H
#define TASK_H

#include "common/types.h"
#include "cpu/cpu.h"
#include "tools/list.h"

#define TASK_NAME_SIZE				32			// 任务名字长度
#define TASK_TIME_SLICE_DEFAULT		10			// 时间片计数

/**
 * @brief 任务控制块结构
 */
typedef struct _task_t {
    enum {
		TASK_CREATED,
		TASK_RUNNING,
		TASK_SLEEP,
		TASK_READY,
		TASK_WAITING,
	} state;

    char name[TASK_NAME_SIZE];  // 任务名字

	int time_slice;				// 时间片
	int slice_ticks;			// 递减时间片计数

	tss_t tss;			    	// 任务的TSS段
    uint16_t tss_sel;		    // tss选择子

    list_node_t run_node;       // 运行相关结点,插入就绪队列中
	list_node_t all_node;       // 所有队列结点,插入任务队列中
} task_t;

/**
 * task: 任务控制块结构
 * entry: 程序入口地址
 * esp: 栈顶指针
 */ 
int task_init(task_t* task, const char* name, uint32_t entry, uint32_t esp);
void task_switch_from_to(task_t* from, task_t* to);
void task_set_ready(task_t* task);
void task_set_block(task_t* task);
int sys_yield(void);
void task_dispatch(void);
// task_t* task_next_run(void);
task_t* task_current(void);
void task_time_tick(void);			// 定时器中断处理函数

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

void task_manager_init(void);	// 任务管理器初始化
void task_main_init(void);		// 内核任务初始化
task_t* task_main_task(void);	// 获取内核任务

#endif

