#include "apilib.h"
#include <stdio.h>
#include <stdlib.h>

/* 
	补充：1.质数的定义：质数又称素数。一个大于1的自然数，除了1和它自身外，不能被其他自然数整除的数叫做质数；否则称为合数。
		 2.质数（prime number）又称素数，有无限个。
		质数定义为在大于1的自然数中，除了1和它本身以外不再有其他因数。
---------------------------------------------------------------------------- 
*/

void NickStartup(void)
{
	char cmdline[60], *p;
	int max, langmode = api_getlang();; /* max:最大值 langmode:当前显示模式 */
	api_cmdline(cmdline, 60); /* 获取命令行的内容 */
	
	for(p = cmdline; *p > ' '; p++){ } /* 跳过应用程序名（sosu或sosu.nck） */
	for(; *p == ' '; p++){ } /* 跳过空格 */
	max = strtol(p, 0, 10); /*将内容转换为十进制整数并存入变量max */
	if(max >= 1000000){ /* 超过此范围则报错，因为stack的大小是规定好的，我们在这里将stack的大小设为了11000， 刚好可以容下这个大小的变量 */
		if(langmode == 0){ /* 当前显示模式为英文模式 */
			api_putstr0("The limit is too big\n"); /* 错误信息（英文） */
		} else { /* 当前显示模式为中文模式 */
			api_putstr0("你输入的这个上限太大了！\n"); /* 错误信息（中文） */
		}
		api_end(); /* 结束 */
	}
	char flag[max]; /* 记录这个数是质数还是合数的数组，为0就是质数，为1就是合数 */
	int i, j;
	for(i = 0; i < max; i++){
		flag[i] = 0; /* 初始化数组flag[ ] */
	}
	for(i = 2; i < max; i++){ /*从2一直循环到上限 */
		if(flag[i] == 0){ /* 如果是质数 */
			printf("%d ", i); /* 标准函数 */
			for(j = 2 * i; j < max; j += i){
				flag[j] = 1; /* 将其的倍数标为合数 */
			}
		}
	}
	api_end(); /* 结束 */
}
