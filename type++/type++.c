#include "apilib.h"
#include <stdio.h>
#include <stdlib.h>

void NickStartup(void)
{
	int fh; /* �ļ���� */
	char c, cmdline[60], *p; /* ��Ŷ�ȡ��һ�ֽ����� */
	
	api_cmdline(cmdline, 60);
	for(p = cmdline; *p > ' '; p++){ } /* ����Ӧ�ó�������sosu��sosu.nck�� */
	for(; *p == ' '; p++){ } /* �����ո� */
	fh = api_fopen(p); /* Ѱ���ļ����� */
	if (fh != 0) { /* ����ҵ��ļ� */
		for (;;) { /*ѭ����ȡ */
			if (api_fread(&c, 1, fh) == 0) {/* ��ȡ1�ֽ����ݣ�����ֵΪ������Ѿ������ļ���ĩβ�ˣ� */
				break; /* �����������ļ�������ѭ�� */
			}
			putchar(c); /* ��ʾ��ȡ��1�ֽ����ݮ� */
		}
	} else {
		if(api_getlang == 0){
			printf("File not found.\n");
		} else {
			printf("�Ҳ����ļ���\n");
		}
	}
	api_end();
}
