#include "apilib.h"

void HariMain(void)
{
	int fh; /* �ļ���� */
	char c; /* ��Ŷ�ȡ��һ�ֽ����� */
	fh = api_fopen("ipl20.nas"); /* Ѱ���ļ����� */
	if (fh != 0) { /* ����ҵ��ļ� */
		for (;;) { /*ѭ����ȡ */
			if (api_fread(&c, 1, fh) == 0) {/* ��ȡ1�ֽ����ݣ�����ֵΪ������Ѿ������ļ���ĩβ�ˣ� */
				break; /* �����������ļ�������ѭ�� */
			}
			api_putchar(c); /* ��ʾ��ȡ��1�ֽ����ݮ� */
		}
	}
	api_end(); /* ���� */
}
