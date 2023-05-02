/**
 * 16位引导代码
 * 二级引导，负责进行硬件检测，进入保护模式，然后加载内核，并跳转至内核运行
 */

// 16位代码，必须加上放在开头，以便有些io指令生成为16位
__asm__(".code16gcc");

static void show_msg(const char* msg) {
    char c;

	// 使用bios写显存，持续往下写
	while ((c = *msg++) != '\0') {
		__asm__ __volatile__(
            "mov $0xe, %%ah;"
            "mov %[ch], %%al;"
            "int $0x10"::[ch]"r"(c) // [ch]为操作数指定一个符号名
        );
	}
}

void loader_entry(void) {
    show_msg("....loading.....\r\n");
    for(;;) {}
}