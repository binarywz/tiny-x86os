/**
 * 中断处理
 */
#include "cpu/irq.h"
#include "cpu/cpu.h"
#include "common/cpu_instr.h"
#include "os_cfg.h"

#define IDT_TABLE_NR	128	// IDT表项数量

static gate_desc_t idt_table[IDT_TABLE_NR];	// 中断描述表

static void do_default_handler(const char* message) {
    for (;;) {}
}

void do_handler_unknown() {
	do_default_handler("Unknown exception.");
}

void irq_init(void) {	
	for (uint32_t i = 0; i < IDT_TABLE_NR; i++) {
        gate_desc_set(idt_table + i, KERNEL_SELECTOR_CS, (uint32_t) exception_handler_unknown,
                  GATE_P_PRESENT | GATE_DPL0 | GATE_TYPE_IDT);
	}
	lidt((uint32_t)idt_table, sizeof(idt_table));
}