/*
 *  linux/kernel/traps.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * 'Traps.c' handles hardware traps and faults after we have saved some
 * state in 'asm.s'. Currently mostly a debugging-aid, will be extended
 * to mainly kill the offending process (probably by giving it a signal,
 * but possibly by killing it outright if necessary).
 */
/*
* 在程序 asm.s 中保存了一些状态后，本程序用来处理硬件陷阱和故障。
* 目前主要用于调试目的，以后将扩展用来杀死遭损坏的进程（主要是通过发送一个信号，但如果必要也会直接杀死） 。
*/
#include <string.h>			// 字符串头文件。主要定义了一些有关内存或字符串操作的嵌入函数。

#include <linux/head.h>		// head 头文件，定义了段描述符的简单结构，和几个选择符常量

// 调度程序头文件，定义了任务结构 task_struct、初始任务 0 的数据，
// 还有一些有关描述符参数设置和获取的嵌入式汇编函数宏语句。
#include <linux/sched.h>
#include <linux/kernel.h>	// 内核头文件。含有一些内核常用函数的原形定义
#include <asm/system.h>		// 系统头文件。定义了设置或修改描述符/中断门等的嵌入式汇编宏
#include <asm/segment.h>	// 段操作头文件。定义了有关段寄存器操作的嵌入式汇编函数
#include <asm/io.h>			// 输入/输出头文件。定义硬件端口输入/输出宏汇编语句

// 以下语句定义了三个嵌入式汇编宏语句函数 :

// 用圆括号括住的复合语句（花括号中的语句）“ ({}) ” 可以作为表达式使用，其中最后的 __res 是其输出值。
// 嵌入汇编语句寄存器名称前就必须写上两个百分号 " %% "

/**
 * @function: 取段 seg 中地址 addr 处的一个字节
 * @parameter: 
 * 		seg - 段选择符 , 指定的内存段值
 * 		addr - 段内指定地址 , 内存偏移地址量
 * @return {type} 
 *     success: 
 *     error: 
 * @note: 
 * 		register char __res : 定义一个寄存器变量 __res , 该变量将被保存在一个寄存器中，以便于快速访问和操作
 * 		想指定寄存器（例如 eax），那么我们可以把该句写成“ register char __res asm("ax");”
 * 
 * 		"__asm__" : 嵌入汇编语句的开始
 * 		push %%fs : 保存 fs 寄存器原值（段选择符）, 入栈
 * 		mov %%ax,%%fs : 用 seg 设置 fs
 * 		movb %%fs:%2,%%al : 取 seg:addr 处 1 字节内容到 al 寄存器中
 * 		pop %%fs : 恢复 fs 寄存器原内容
 * 
 * 		输出： %0 - eax (__res)；
 * 		输出寄存器列表, " a " : 加载代码, 使用寄存器 eax, "=" : 输出寄存器 
 * 		输出寄存器 eax 的值将被放入__res
 * 
 * 		输入： %1 - eax (seg)； %2 - 内存地址 (*(addr))
 * 		输入寄存器列表, 将 seg 放到 eax 寄存器中， "0" : 使用与上面同个位置的输出相同的寄存器, (*(addr))表示一个内存偏移地址值
 */
#define get_seg_byte(seg,addr) ({ \
register char __res; \
__asm__("push %%fs; \
		mov %%ax,%%fs;	\
		movb %%fs:%2,%%al; \
		pop %%fs" \
	:"=a" (__res) \
	:"0" (seg),"m" (*(addr))); \
__res;})

/**
 * @function: 取段 seg 中地址 addr 处的一个长字（ 4 字节）
 * @parameter: 
 * 		seg - 段选择符
 * 		addr - 段内指定地址
 * @return {type} 
 *     success: 
 *     error: 
 * @note: 
 * 		输出： %0 - eax (__res)
 * 		输入： %1 - eax (seg)； %2 - 内存地址 (*(addr))
 */
#define get_seg_long(seg,addr) ({ \
register unsigned long __res; \
__asm__("push %%fs;mov %%ax,%%fs;movl %%fs:%2,%%eax;pop %%fs" \
	:"=a" (__res):"0" (seg),"m" (*(addr))); \
__res;})

/**
 * @function: 取 fs 段寄存器的值（选择符）
 * @parameter: 
 * @return {type} 
 *     success: 
 *     error: 
 * @note: 
 * 		输出： %0 - eax (__res)
 */
#define _fs() ({ \
register unsigned short __res; \
__asm__("mov %%fs,%%ax":"=a" (__res):); \
__res;})

// 以下定义了一些函数原型
void page_exception(void);		// 页异常。实际是 page_fault , mm/page.s

void divide_error(void);				// int0（ kernel/asm.s
void debug(void);						// int1
void nmi(void);							// int2
void int3(void);						// int3
void overflow(void);					// int4
void bounds(void);						// int5
void invalid_op(void);					// int6（ kernel/asm.s
void device_not_available(void);		// int7（ kernel/sys_call.s
void double_fault(void);				// int8（ kernel/asm.s
void coprocessor_segment_overrun(void);	// int9
void invalid_TSS(void);					// int10
void segment_not_present(void);			// int11
void stack_segment(void);				// int12
void general_protection(void);			// int13（ kernel/asm.s
void page_fault(void);					// int14（ mm/page.s
void coprocessor_error(void);			// int16（ kernel/sys_call.s
void reserved(void);					// int15（ kernel/asm.s
void parallel_interrupt(void);			// int39（ kernel/sys_call.s
void irq13(void);						// int45 , kernel/asm.s, 协处理器中断处理
void alignment_check(void);				// int46（ kernel/asm.s

/**
 * @function: 该子程序用于在中断处理中打印错误名称、 出错码、调用程序的 EIP、 EFLAGS、 ESP、
 * 				fs 段寄存器值、段的基址、段的长度、进程号 pid、任务号、 10 字节指令码。
 * @parameter: 
 * 		str - 错误名字符串；
 * 		esp_ptr - 出错而被中断的程序在栈中信息的指针 esp0 处
 * 		nr - 出错码
 * @return {type} 
 *     success: 
 *     error: 
 * @note: 如果堆栈在用户数据段，则还打印 16 字节的堆栈内容。这些信息可用于程序调试。
 */
static void die(char * str, long esp_ptr, long nr)
{
	long * esp = (long *) esp_ptr;
	int i;

	printk("%s: %04x\n\r",str,nr&0xffff);

	// 下行打印语句显示当前调用进程的 CS:EIP、 EFLAGS 和 SS:ESP 的值。
	// 因此我们把这句拆分开来看为：
	// EIP:\t%04x:%p\n -- esp[1]是段选择符（ cs ）， esp[0]是 eip
	// EFLAGS:\t%p -- esp[2]是 eflags
	// ESP:\t%04x:%p\n -- esp[4]是原 ss， esp[3]是原 esp
	printk("EIP:\t%04x:%p\nEFLAGS:\t%p\nESP:\t%04x:%p\n",
		esp[1],esp[0],esp[2],esp[4],esp[3]);
	printk("fs: %04x\n",_fs());
	printk("base: %p, limit: %p\n", get_base(current->ldt[1]), get_limit(0x17));

	if (esp[4] == 0x17) 
	{
		// 若原 ss 值为 0x17（用户栈），则还打印出

		// 用户栈中的 4 个长字值（ 16 字节）
		printk("Stack: ");
		for (i = 0; i < 4; i++)
		{
			printk("%p ", get_seg_long(0x17, i + (long *)esp[3]));
		}

		printk("\n");
	}

	// 取当前运行任务的任务号 include/linux/sched.h
	str(i);

	// 进程号，任务号
	printk("Pid: %d, process nr: %d\n\r", current->pid, 0xffff & i);

	for(i = 0; i < 10; i++)
	{
		printk("%02x ", 0xff & get_seg_byte(esp[1], (i + (char *)esp[0])));
	}

	printk("\n\r");
	do_exit(11);		/* play segment exception */
}

// 以下这些以 do_开头的函数是 asm.s 中对应中断处理程序调用的 C 函数
void do_double_fault(long esp, long error_code)
{
	die("double fault", esp, error_code);
}

void do_general_protection(long esp, long error_code)
{
	die("general protection",esp,error_code);
}

void do_alignment_check(long esp, long error_code)
{
    die("alignment check",esp,error_code);
}

void do_divide_error(long esp, long error_code)
{
	die("divide error",esp,error_code);
}

// 参数是进入中断后被顺序压入堆栈的寄存器值。参见 asm.s 
void do_int3(long * esp, long error_code,
		long fs,long es,long ds,
		long ebp,long esi,long edi,
		long edx,long ecx,long ebx,long eax)
{
	int tr;

	// 取任务寄存器值 -> tr
	__asm__("str %%ax":"=a" (tr):"0" (0));
	printk("eax\t\tebx\t\tecx\t\tedx\n\r%8x\t%8x\t%8x\t%8x\n\r",
		eax,ebx,ecx,edx);
	printk("esi\t\tedi\t\tebp\t\tesp\n\r%8x\t%8x\t%8x\t%8x\n\r",
		esi,edi,ebp,(long) esp);
	printk("\n\rds\tes\tfs\ttr\n\r%4x\t%4x\t%4x\t%4x\n\r",
		ds,es,fs,tr);
	printk("EIP: %8x   CS: %4x  EFLAGS: %8x\n\r",esp[0],esp[1],esp[2]);
}

void do_nmi(long esp, long error_code)
{
	die("nmi",esp,error_code);
}

void do_debug(long esp, long error_code)
{
	die("debug",esp,error_code);
}

void do_overflow(long esp, long error_code)
{
	die("overflow",esp,error_code);
}

void do_bounds(long esp, long error_code)
{
	die("bounds",esp,error_code);
}

void do_invalid_op(long esp, long error_code)
{
	die("invalid operand",esp,error_code);
}

void do_device_not_available(long esp, long error_code)
{
	die("device not available",esp,error_code);
}

void do_coprocessor_segment_overrun(long esp, long error_code)
{
	die("coprocessor segment overrun",esp,error_code);
}

void do_invalid_TSS(long esp,long error_code)
{
	die("invalid TSS",esp,error_code);
}

void do_segment_not_present(long esp,long error_code)
{
	die("segment not present",esp,error_code);
}

void do_stack_segment(long esp,long error_code)
{
	die("stack segment",esp,error_code);
}

void do_coprocessor_error(long esp, long error_code)
{
	if (last_task_used_math != current)
		return;
	die("coprocessor error",esp,error_code);
}

void do_reserved(long esp, long error_code)
{
	die("reserved (15,17-47) error",esp,error_code);
}

// 下面是异常（陷阱）中断程序初始化子程序。设置它们的中断调用门（中断向量）。
// set_trap_gate()与 set_system_gate()都使用了中断描述符表 IDT 中的陷阱门（ Trap Gate） ，
// 它们之间的主要区别在于 前者设置的特权级为 0，后者是 3。
// 因此断点陷阱中断 int3 、溢出中断 overflow 和 边界出错中断 bounds 可以由任何程序调用。 
// 这两个函数均是嵌入式汇编宏程序，参见 include/asm/system.h
void trap_init(void)
{
	int i;

	set_trap_gate(0, &divide_error);	// 设置除操作出错的中断向量值
	set_trap_gate(1, &debug);
	set_trap_gate(2, &nmi);

	/* int3-5 可以被所有程序执行 */
	set_system_gate(3,&int3);	/* int3-5 can be called from all */
	set_system_gate(4,&overflow);
	set_system_gate(5,&bounds);
	
	set_trap_gate(6,&invalid_op);
	set_trap_gate(7,&device_not_available);
	set_trap_gate(8,&double_fault);
	set_trap_gate(9,&coprocessor_segment_overrun);
	set_trap_gate(10,&invalid_TSS);
	set_trap_gate(11,&segment_not_present);
	set_trap_gate(12,&stack_segment);
	set_trap_gate(13,&general_protection);
	set_trap_gate(14,&page_fault);
	set_trap_gate(15,&reserved);
	set_trap_gate(16,&coprocessor_error);
	set_trap_gate(17,&alignment_check);

	// 下面把 int17-47 的陷阱门均先设置为 reserved，以后各硬件初始化时会重新设置自己的陷阱门
	for (i = 18; i < 48; i++)
	{
		set_trap_gate(i,&reserved);
	}

	// 下面设置协处理器 int45（ 0x20+13）陷阱门描述符，并允许其产生中断请求。
	// 协处理器的中断请求号 IRQ13 连接在 8259 从芯片的 IR5 引脚上。
	// outb_p , outb 两语句用于允许协处理器发送中断请求信号。
	// 另外这里也设置并行口 1 的中断号 int39（ 0x20+7）的门描述符。
	// 其中断请求号 IRQ7 连接在 8259 主芯片的 IR7 引脚上。 
	// outb_p 语句对 8259 发送操作命令字 OCW1。该命令字用于对 8259 中断屏蔽寄存器 IMR 设置。 
	// 0x21 是主芯片端口地址，读入屏蔽码，与上 0xfb（ 0b11111011）后再立刻写入，
	// 表示清除主芯片上对应中断请求 IR2 的中断请求屏蔽位 M2。
	// 从芯片的请求引脚 INT 连接在主芯片 IR2 引脚上，
	// 因此该语句表示开启并允许接受从芯片发来的中断请求信号。
	// 类似地，outb 语句针对从芯片进行类似操作。 
	// 0xA1 是从芯片端口地址，读入屏蔽码，与上 0xdf（ 0b11011111）后再立刻写入， 
	// 表示清除从芯片上对应中断请求 IR5 的中断请求屏蔽位 M2。
	// 因为协处理器连接在该 IR5 引脚上，因此该语句开启并允许协处理器发送中断请求信号 IRQ13
	set_trap_gate(45, &irq13);

	outb_p(inb_p(0x21)&0xfb, 0x21);		// 允许 8259A 主芯片的 IRQ2 中断请求
	outb(inb_p(0xA1)&0xdf, 0xA1);		// 允许 8259A 从芯片的 IRQ13 中断请求

	set_trap_gate(39, &parallel_interrupt);		// 设置并行口 1 的中断 0x27 陷阱门描述符
}
