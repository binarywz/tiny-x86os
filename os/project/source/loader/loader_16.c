/**
 * 16位引导代码
 * 二级引导，负责进行硬件检测，进入保护模式，然后加载内核，并跳转至内核运行
 */

// 16位代码，必须加上放在开头，以便有些io指令生成为16位
__asm__(".code16gcc");

#include "loader.h"

static boot_info_t boot_info;			// 启动参数信息

/**
 * BIOS下显示字符 
 */
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

/**
 * 内存检测
 * 参考：https://wiki.osdev.org/Memory_Map_(x86)
 * 1MB以下比较标准, 在1M以上会有差别
 * 检测：https://wiki.osdev.org/Detecting_Memory_(x86)#BIOS_Function:_INT_0x15.2C_AH_.3D_0xC7
 */
static void detect_mem(void) {
    show_msg("detect memory start...\r\n");
    boot_info.ram_region_count = 0;

    /**
     * 初次: EDX=0x534D4150,EAX=0xE820,ECX=24,INT 0x15, EBX=0（初次）
     * 返回结果: EAX=0x534D4150，CF标志清0，EBX被设置为某个数值用于下次调用，CL=实际读取的字节数
     * 后续: EAX=0xE820,ECX=24
     * 返回结果: EAX=0x534D4150，CF标志清0，若EBX=0则表明读取完毕
     */
    uint32_t contId = 0;
	SMAP_entry_t smap_entry;
	int signature, bytes;
    for (int i = 0; i < BOOT_RAM_REGION_MAX; i++) {
        SMAP_entry_t * entry = &smap_entry;

		__asm__ __volatile__("int  $0x15"
			: "=a"(signature), "=c"(bytes), "=b"(contId)
			: "a"(0xE820), "b"(contId), "c"(24), "d"(0x534D4150), "D"(entry));
		if (signature != 0x534D4150) {
            show_msg("detect memory failed...\r\n");
			return;
		}

		// TODO: 20字节
		if (bytes > 20 && (entry->ACPI & 0x0001) == 0){
			continue;
		}

        // 保存RAM信息，只取32位，空间有限无需考虑更大容量的情况
        if (entry->Type == 1) {
            boot_info.ram_region_cfg[boot_info.ram_region_count].start = entry->BaseL;
            boot_info.ram_region_cfg[boot_info.ram_region_count].size = entry->LengthL;
            boot_info.ram_region_count++;
        }

		if (contId == 0) {
			break;
		}
    }
    show_msg("detect memory end...\r\n");
}

void loader_entry(void) {
    show_msg("....loading.....\r\n");
    detect_mem();
    for(;;) {}
}