/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-22 20:06:51
 * @FilePath: \Linux_0_11\include\linux\tty.h
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 * 'tty.h' defines some structures used by tty_io.c and some defines.
 *
 * NOTE! Don't touch this without checking that nothing in rs_io.s or
 * con_io.s breaks. Some constants are hardwired into the system (mainly
 * offsets into 'tty_queue'
 */

#ifndef _TTY_H
#define _TTY_H

#define MAX_CONSOLES	8
#define NR_SERIALS	2
#define NR_PTYS		4

extern int NR_CONSOLES;

#include <termios.h>

#define TTY_BUF_SIZE 1024

struct tty_queue {
	unsigned long data;				// 等待队列缓冲区中当前数据统计值 ,  对于串口终端，则存放串口端口地址
	unsigned long head;				// 缓冲区中数据头指针
	unsigned long tail;				// 缓冲区中数据尾指针
	struct task_struct * proc_list;	// 等待本缓冲队列的进程列表
	char buf[TTY_BUF_SIZE];			// 队列的缓冲区
};

#define IS_A_CONSOLE(min)	(((min) & 0xC0) == 0x00)
#define IS_A_SERIAL(min)	(((min) & 0xC0) == 0x40)
#define IS_A_PTY(min)		((min) & 0x80)
#define IS_A_PTY_MASTER(min)	(((min) & 0xC0) == 0x80)
#define IS_A_PTY_SLAVE(min)	(((min) & 0xC0) == 0xC0)
#define PTY_OTHER(min)		((min) ^ 0x40)

#define INC(a) ((a) = ((a)+1) & (TTY_BUF_SIZE-1))
#define DEC(a) ((a) = ((a)-1) & (TTY_BUF_SIZE-1))
#define EMPTY(a) ((a)->head == (a)->tail)
#define LEFT(a) (((a)->tail-(a)->head-1)&(TTY_BUF_SIZE-1))
#define LAST(a) ((a)->buf[(TTY_BUF_SIZE-1)&((a)->head-1)])
#define FULL(a) (!LEFT(a))
#define CHARS(a) (((a)->head-(a)->tail)&(TTY_BUF_SIZE-1))
#define GETCH(queue,c) \
(void)({c=(queue)->buf[(queue)->tail];INC((queue)->tail);})
#define PUTCH(c,queue) \
(void)({(queue)->buf[(queue)->head]=(c);INC((queue)->head);})

#define INTR_CHAR(tty) ((tty)->termios.c_cc[VINTR])
#define QUIT_CHAR(tty) ((tty)->termios.c_cc[VQUIT])
#define ERASE_CHAR(tty) ((tty)->termios.c_cc[VERASE])
#define KILL_CHAR(tty) ((tty)->termios.c_cc[VKILL])
#define EOF_CHAR(tty) ((tty)->termios.c_cc[VEOF])
#define START_CHAR(tty) ((tty)->termios.c_cc[VSTART])
#define STOP_CHAR(tty) ((tty)->termios.c_cc[VSTOP])
#define SUSPEND_CHAR(tty) ((tty)->termios.c_cc[VSUSP])

struct tty_struct {
	struct termios termios;					// 终端 io 属性和控制字符数据结构
	int pgrp;								// 所属进程组
	int session;					
	int stopped;							// 停止标志
	void (*write)(struct tty_struct * tty);	// tty 写函数指针
	struct tty_queue *read_q;				// tty 读队列
	struct tty_queue *write_q;				// tty 写队列
	struct tty_queue *secondary;			// tty 辅助队列(存放规范模式字符序列), 可称为规范(熟)模式队列
	};

// 保存系统中每个终端设备的信息
extern struct tty_struct tty_table[];		// tty 结构数组
extern int fg_console;

#define TTY_TABLE(nr) \
(tty_table + ((nr) ? (((nr) < 64)? (nr)-1:(nr))	: fg_console))

/*	intr=^C		quit=^|		erase=del	kill=^U
	eof=^D		vtime=\0	vmin=\1		sxtc=\0
	start=^Q	stop=^S		susp=^Z		eol=\0
	reprint=^R	discard=^U	werase=^W	lnext=^V
	eol2=\0
*/
#define INIT_C_CC "\003\034\177\025\004\0\1\0\021\023\032\0\022\017\027\026\0"

void rs_init(void);
void con_init(void);
void tty_init(void);

int tty_read(unsigned c, char * buf, int n);
int tty_write(unsigned c, char * buf, int n);

void con_write(struct tty_struct * tty);
void rs_write(struct tty_struct * tty);
void mpty_write(struct tty_struct * tty);
void spty_write(struct tty_struct * tty);

void copy_to_cooked(struct tty_struct * tty);

void update_screen(void);

#endif
