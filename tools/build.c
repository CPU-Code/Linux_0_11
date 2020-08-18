/*
 *  linux/tools/build.c
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * This file builds a disk-image from three different files:
 *
 * - bootsect: max 510 bytes of 8086 machine code, loads the rest
 * - setup: max 4 sectors of 8086 machine code, sets up system parm
 * - system: 80386 code for actual system
 *
 * It does some checking that all files are of the correct type, and
 * just writes the result to stdout, removing headers and padding to
 * the right amount. It also writes some system data to stderr.
 */
/*
* 该程序从三个不同的程序中创建磁盘映像文件：
*
* - bootsect：该文件的 8086 机器码最长为 510 字节，用于加载其他程序。
* - setup：该文件的 8086 机器码最长为 4 个磁盘扇区，用于设置系统参数。
* - system：实际系统的 80386 代码。
*
* 该程序首先检查所有程序模块的类型是否正确，并将检查结果在终端上显示出来，
* 然后删除模块头部并扩充大正确的长度。该程序也会将一些系统数据写到 stderr。
*/

/*
 * Changes by tytso to allow root device specification
 *
 * Added swap-device specification: Linux 20.12.91
 */
/*
* tytso 对该程序作了修改，以允许指定根文件设备。
*
* 添加了指定交换设备功能： Linus 20.12.91
*/

#include <stdio.h>			/* fprintf 使用其中的 fprintf()函数 */
#include <string.h>			// 字符串操作函数
#include <stdlib.h>			/* contains exit 含 exit 函数原型说明*/
#include <sys/types.h>		/* unistd.h needs this  该头文件供 unistd.h 文件使用 */
#include <sys/stat.h>		// 含文件状态信息结构定义
#include <linux/fs.h>		// 文件系统头文件
#include <unistd.h>			/* contains read/write 含 read/write 函数原型说明 */
#include <fcntl.h>			// 包含文件操作模式符号常数

#define MINIX_HEADER 32		// minix 二进制目标文件模块头部长度为 32 字节
#define GCC_HEADER 1024		// GCC 头部信息长度为 1024 字节

#define SYS_SIZE 0x3000		// system 文件最长节数(字节数为 SYS_SIZE * 16=128KB)

// 默认情况下， Linux 根文件系统设备是第 2 个硬盘的第 1 个分区（即设备号为 0x0306） 。
// 这是因为 Linus 当时开发 Linux 时，把第 1 个硬盘用作 MINIX 系统盘，
// 而第 2 个硬盘用作为 Linux 的根文件系统盘。
#define DEFAULT_MAJOR_ROOT 3		// 默认根设备主设备号 - 3（硬盘）
#define DEFAULT_MINOR_ROOT 6		// 默认根设备次设备号 - 6（第 2 个硬盘的第 1 分区）

#define DEFAULT_MAJOR_SWAP 0		// 默认交换设备主设备号
#define DEFAULT_MINOR_SWAP 0		// 默认交换设备次设备号

/* max nr of sectors of setup: don't change unless you also change
 * bootsect etc */
// 下面指定 setup 模块占的最大扇区数：不要改变该值，除非也改变 bootsect 等相应文件
#define SETUP_SECTS 4				// setup 最大长度为 4 个扇区（ 2KB）

#define STRINGIFY(x) #x				// 把 x 转换成字符串类型，用于出错显示语句中

// 显示出错信息，并终止程序
void die(char * str)
{
	fprintf(stderr, "%s\n", str);
	exit(1);
}

// 显示程序使用方法，并退出
void usage(void)
{
	die("Usage: build bootsect setup system [rootdev] [> image]");
}

// 主程序开始。
// 该程序首先检查命令行上的参数是否合规， 并设置 根设备号 和 交换设备号， 
// 然后分别读取和处理 bootsect、 setup 和 system 模块文件，
// 并写入到已经重定向到 Image 文件的标准输出中。
int main(int argc, char ** argv)
{
	int i,c,id;
	char buf[1024];
	char major_root, minor_root;
	char major_swap, minor_swap;
	struct stat sb;

	// 首先检查 build 程序执行时实际命令行参数个数，并根据参数个数作相应设置。
	// build 程序要求有 4 到 6 个参数， 
	// 如果命令行上参数个数不满足要求（程序名算作 1 个），则显示程序用法后退出执行。 
	if ((argc < 4) || (argc > 6))
	{
		usage();
	}

	if (argc > 4) 
	{
		// 如果程序命令行上有多于 4 个参数，

		if (strcmp(argv[4], "FLOPPY")) 
		{
			// 那么若根设备名不是软盘"FLOPPY"，则取该设备文件的状态信息，并从中取主、 次设备号作为根设备号

			if (stat(argv[4], &sb)) 
			{
				perror(argv[4]);
				die("Couldn't stat root device.");
			}

			// 取设备名状态结构中设备号
			major_root = MAJOR(sb.st_rdev);
			minor_root = MINOR(sb.st_rdev);
		} 
		else 
		{
			// 若根设备就是 FLOPPY 设备，则让主、 次设备号取 0， 表示根设备是当前启动引导设备

			major_root = 0;
			minor_root = 0;
		}
	} 
	else 
	{
		// 若参数只有 4 个，则让 主设备号 和 次设备号 = 系统默认的根设备号

		major_root = DEFAULT_MAJOR_ROOT;
		minor_root = DEFAULT_MINOR_ROOT;
	}

	// 若程序命令行上有 6 个参数, 最后一个表示交换设备的参数不是无（ "NONE"），
	// 则取该设备文件的状态信息， 则从中取 主次设备号 作为 交换设备号。
	if (argc == 6) 
	{
		if (strcmp(argv[5], "NONE")) 
		{

			if (stat(argv[5], &sb)) 
			{
				perror(argv[5]);
				die("Couldn't stat root device.");
			}
			// 取设备名状态结构中设备号。
			major_swap = MAJOR(sb.st_rdev);
			minor_swap = MINOR(sb.st_rdev);
		} 
		else 
		{
			// 如果最后一个参数就是"NONE"，则让交换设备的主设备号和次设备号取为 0。
			// 表示交换设备就是当前启动引导设备。
			major_swap = 0;
			minor_swap = 0;
		}
	} 
	else 
	{
		// 若参数没有 6 个而是 5 个，表示命令行上没有带交换设备名。
		// 于是就让 交换设备主设备号 和  次设备号 = 系统默认的交换设备号。
		major_swap = DEFAULT_MAJOR_SWAP;
		minor_swap = DEFAULT_MINOR_SWAP;
	}

	// 接下来在标准错误终端上显示上面所选择的 根设备主 、次设备号 和 交换设备主、次设备号。
	fprintf(stderr, "Root device is (%d, %d)\n", major_root, minor_root);
	fprintf(stderr, "Swap device is (%d, %d)\n", major_swap, minor_swap);

	// 如果主设备号 !=  2（软盘）或 3（硬盘），也 != 0（取系统默认设备），则显示出错信息并退出。
	// 终端的标准输出被定向到文件 Image，因此被用于输出保存内核代码数据，生成内核映像文件。
	if ((major_root != 2) && 
		(major_root != 3) &&
	    (major_root != 0)) 
	{
		fprintf(stderr, "Illegal root device (major = %d)\n",
			major_root);
		die("Bad root device --- major #");
	}

	if (major_swap && major_swap != 3) 
	{
		fprintf(stderr, "Illegal swap device (major = %d)\n",
			major_swap);
		die("Bad root device --- major #");
	}

	//下面开始执行读取各文件内容并进行相应复制处理。
	//初始化 1KB 的缓冲区
	for (i = 0; i < sizeof buf; i++) 
	{
		buf[i] = 0;
	}

	// 只读方式打开参数 1 指定的文件（ bootsect）
	if ((id = open(argv[1], O_RDONLY, 0)) < 0)
	{
		die("Unable to open 'boot'");
	}

	// 从中读取 32 字节的 MINIX 执行文件头结构内容到缓冲区 buf 中。
	if (read(id, buf, MINIX_HEADER) != MINIX_HEADER)
	{
		die("Unable to read header of 'boot'");
	}

	// 根据 MINIX 头部结构判断 bootsect 是否为一个有效的 MINIX 执行文件。若是，则从文件中
	// 读取 512 字节的引导扇区代码和数据。 
	// 其中数值“ 0x04100301 ” 含义为：
	//  0x0301 - MINIX 头部的魔数字段 a_magic；
	//  0x10 - 可执行标志 a_flag； 
	// 0x04 - 机器类型 a_cpu, Intel 8086 机器码。
	if (((long *) buf)[0] != 0x04100301)
		die("Non-Minix header of 'boot'");

	// 对首部信息进行一系列检查。
	// 检查头部长度字段 a_hdrlen （字节）是否正确（ 32 字节）。
	if (((long *) buf)[1] != MINIX_HEADER)
		die("Non-Minix header of 'boot'");
	
	// 确认数据段长 a_data 字段(long)内容是否为 0
	if (((long *) buf)[3] != 0)
		die("Illegal data segment in 'boot'");

	// 确认堆 a_bss 字段(long)内容是否为 0
	if (((long *) buf)[4] != 0)
		die("Illegal bss in 'boot'");
	
	// 确认执行点 a_entry 字段(long)内容是否为 0
	if (((long *) buf)[5] != 0)
		die("Non-Minix header of 'boot'");
	
	//确认符号表长字段 a_sym 的内容是否为 0
	if (((long *) buf)[7] != 0)
		die("Illegal symbol table in 'boot'");

	// 读取文件中随后的实际代码数据, 应该返回读取字节数为 512 字节。
	// 因为 bootsect 文件中包含的是 1 个扇区的引导扇区代码 和 数据，
	// 并且最后 2 字节应该是可引导标志 0xAA55
	i = read(id, buf, sizeof buf);
	fprintf(stderr, "Boot sector %d bytes.\n", i);

	if (i != 512)
		die("Boot block must be exactly 512 bytes");

	if ((*(unsigned short *)(buf + 510)) != 0xAA55)
		die("Boot block hasn't got boot flag (0xAA55)");

	// 修改缓冲中内容， 
	// 506 、 507 偏移处需存放交换设备号， 
	//508 、 509 偏移处需存放根设备号。
	buf[506] = (char) minor_swap;
	buf[507] = (char) major_swap;
	buf[508] = (char) minor_root;
	buf[509] = (char) major_root;	

	// 将该 512 字节的数据写到标准输出 stdout
	// 在 linux/Makefile 中， build 程序利用” > ” 指示符把 标准输出重定向 到 内核映像文件 Image 上，
	// 因此 引导扇区代码 和 数据 会被写到 Image 开始的 512 字节处。
	i = write(1, buf, 512);

	if (i != 512)
		die("Write call failed");

	// 关闭 bootsect 文件
	close (id);
	
	// 以只读方式打开参数 2 指定的文件（ setup）
	if ((id = open(argv[2], O_RDONLY, 0)) < 0)
	{
		die("Unable to open 'setup'");
	}

	// 读取 32 字节的 MINIX 执行文件头结构内容到缓冲区 buf 中
	if (read(id, buf, MINIX_HEADER) != MINIX_HEADER)
	{
		die("Unable to read header of 'setup'");
	}

	// 根据 MINIX 头部结构判断 setup 是否为一个有效的 MINIX 执行文件。
	if (((long *) buf)[0] != 0x04100301)
	{
		die("Non-Minix header of 'setup'");
	}
	if (((long *) buf)[1] != MINIX_HEADER)
	{
		die("Non-Minix header of 'setup'");
	}

	// 数据段长 a_data 字段
	if (((long *) buf)[3] != 0)
	{
		die("Illegal data segment in 'setup'");
	}

	// 堆 a_bss 字段
	if (((long *) buf)[4] != 0)
	{
		die("Illegal bss in 'setup'");
	}

	// 执行起始点 a_entry 字段
	if (((long *) buf)[5] != 0)
	{
		die("Non-Minix header of 'setup'");
	}
	if (((long *) buf)[7] != 0)
	{
		die("Illegal symbol table in 'setup'");
	}

	// 读取文件中随后的实际代码数据
	for (i = 0; (c = read(id, buf, sizeof buf)) > 0; i += c )
	{
		// 写到终端标准输出. 同时统计写的长度（ i ）
		if (write(1, buf, c) != c)
		{
			die("Write call failed");
		}
	}

	//关闭 setup 模块文件
	close (id);

	// 判断执行写操作的代码和数据长度值，该值不应大于(SETUP_SECTS * 512)字节，
	if (i > SETUP_SECTS * 512)
	{
		// 重新修改 build、 bootsect 和 setup 程序中设定的 setup 所占扇区数并重新编译内核
		die("Setup exceeds " STRINGIFY(SETUP_SECTS)
			" sectors - rewrite build/boot/setup");
	}

	//显示 setup 实际长度值。
	fprintf(stderr, "Setup is %d bytes.\n", i);

	// 将缓冲区 buf 清零
	for (c = 0; c < sizeof(buf) ; c++)
	{
		buf[c] = '\0';
	}

	// 判断实际写的 setup 长度与 (SETUP_SECTS * 512)的数值差, 若 setup 长度小于该长度（ 4 * 512 字节）
	while (i < SETUP_SECTS * 512) 
	{
		//用 NULL 字符将 setup 填足为 4 * 512 字节。

		c = SETUP_SECTS * 512 - i;

		if (c > sizeof(buf))
		{
			c = sizeof(buf);
		}

		if (write(1, buf, c) != c)
		{
			die("Write call failed");
		}

		i += c;
	}
	
	// 下面开始处理 system 模块文件。
	// 该文件使用 gas/gcc 编译，因此具有 a.out 目标文件格式。
	// 以只读方式打开文件
	if ((id = open(argv[3], O_RDONLY, 0)) < 0)
		die("Unable to open 'system'");
	
	// 读取其中 a.out 格式头部结构信息（ 1KB 长度）。
	if (read(id, buf, GCC_HEADER) != GCC_HEADER)
		die("Unable to read header of 'system'");
	
	// 判断 system 是一个有效的 a.out 格式文件
	if (((long *) buf)[5] != 0)
		die("Non-GCC header of 'system'");
	
	//把该文件随后的所有数据都写到标准输出（ Image 文件）中
	for (i = 0; (c = read(id, buf, sizeof buf)) > 0 ; i += c )
	{
		if (write(1, buf, c) != c)
			die("Write call failed");
	}

	//关闭该文件
	close(id);

	//显示 system 模块的长度
	fprintf(stderr, "System is %d bytes.\n", i);

	//若 system 代码 和 数据长度 超过 SYS_SIZE 节 , 即 128KB 字节
	if (i > SYS_SIZE * 16)
	{
		//显示出错信息并退出
		die("System is too big");
	}

	// 无错，则返回 0，表示正常退出
	return(0);
}
