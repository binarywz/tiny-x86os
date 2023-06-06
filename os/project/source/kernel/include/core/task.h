/**
 * 任务实现
 */
#ifndef TASK_H
#define TASK_H

#include "common/types.h"
#include "cpu/cpu.h"

/**
 * @brief 任务控制块结构
 */
typedef struct _task_t {
	tss_t tss;				// 任务的TSS段
}task_t;

/**
 * task: 任务控制块结构
 * entry: 程序入口地址
 * esp: 栈顶指针
 */ 
int task_init (task_t* task, uint32_t entry, uint32_t esp);

#endif

