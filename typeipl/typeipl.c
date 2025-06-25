#include "apilib.h"

void HariMain(void)
{
	int fh; /* 文件句柄 */
	char c; /* 存放读取的一字节内容 */
	fh = api_fopen("ipl20.nas"); /* 寻找文件并打开 */
	if (fh != 0) { /* 如果找到文件 */
		for (;;) { /*循环读取 */
			if (api_fread(&c, 1, fh) == 0) {/* 读取1字节数据（返回值为零代表已经读到文件的末尾了） */
				break; /* 读完了整个文件后，跳出循环 */
			}
			api_putchar(c); /* 显示读取的1字节内容 */
		}
	}
	api_end(); /* 结束 */
}
