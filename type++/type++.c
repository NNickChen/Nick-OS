#include "apilib.h"
#include <stdio.h>
#include <stdlib.h>

void NickStartup(void)
{
	int fh; /* 文件句柄 */
	char c, cmdline[60], *p; /* 存放读取的一字节内容 */
	
	api_cmdline(cmdline, 60);
	for(p = cmdline; *p > ' '; p++){ } /* 跳过应用程序名（sosu或sosu.nck） */
	for(; *p == ' '; p++){ } /* 跳过空格 */
	fh = api_fopen(p); /* 寻找文件并打开 */
	if (fh != 0) { /* 如果找到文件 */
		for (;;) { /*循环读取 */
			if (api_fread(&c, 1, fh) == 0) {/* 读取1字节数据（返回值为零代表已经读到文件的末尾了） */
				break; /* 读完了整个文件后，跳出循环 */
			}
			putchar(c); /* 显示读取的1字节内容 */
		}
	} else {
		if(api_getlang == 0){
			printf("File not found.\n");
		} else {
			printf("找不到文件。\n");
		}
	}
	api_end();
}
