#include "bootpack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cmd(struct CONSOLE *cons, unsigned int memtotal, int *fat, char *cmdline)
{
	struct TASK *task = task_now();
	if(strcmp(cmdline, "mem") == 0 && cons->sht != 0){
		cmd_mem(cons, memtotal);
	} else if (strcmp(cmdline, "cls") == 0 && cons->sht != 0){
		cmd_cls(cons);
	} else if (strcmp(cmdline, "dir") == 0 && cons->sht != 0){
		cmd_dir(cons);
	} else if(strncmp(cmdline, "type ", 5) == 0 && cons->sht != 0){
		cmd_type(cons, fat, cmdline);
	} else if (strcmp(cmdline, "time") == 0 && cons->sht != 0){
		cmd_time(cons);
	} else if (strcmp(cmdline, "date") == 0 && cons->sht != 0){
		cmd_date(cons);
	} else if (strncmp(cmdline, "bc ", 3) == 0){
		cmd_bc(cons, cmdline);
	} else if (strcmp(cmdline, "exit") == 0){
		cmd_exit(cons, fat);
	} else if (strncmp(cmdline, "start ", 6) == 0){
		cmd_start(cons, cmdline, memtotal);
	} else if (strncmp(cmdline, "ncst ", 5) == 0){
		cmd_ncst(cons, cmdline, memtotal);
	} else if (strncmp(cmdline, "langmode ", 9) == 0){
		cmd_langmode(cons, cmdline);
	} else if (strcmp(cmdline, "shutdown") == 0){
		shutdown2();
	} else if (strcmp(cmdline, "getlang") == 0){
		cmd_getlang(cons);
	} else if (cmdline[0] != 0){		
		if(cmd_run(cons, cmdline, fat) == 0){
			if(task->langmode == 0){
				cons_putstr0(cons, "Bad command.\n\n");
			} else {
				cons_putstr0(cons, "不是正确的命令，也不是可运行的程序。\n\n");
			}
		}
	}
	return;
}	
									
void cmd_mem(struct CONSOLE *cons, unsigned int memtotal)
{
	char s[30];
	struct TASK *task = task_now();
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	if(task->langmode == 0){
		sprintf(s, "total   %dMB\nfree %dKB\n\n", memtotal / (1024 * 1024), memman_total(memman) / 1024);
	} else {
		sprintf(s, "合计   %dMB\n剩余 %dKB\n\n", memtotal / (1024 * 1024), memman_total(memman) / 1024);
	}
	cons_putstr0(cons, s);
	return;
}

void cmd_cls(struct CONSOLE *cons)
{
	boxfill8(cons->sht->buf, cons->sht->bxsize, COL8_000000, 8, 28, 486, 480);
	sheet_refresh(cons->sht, 8, 28, 486, 480); 
	cons->cursor_y = 28;
	return;
}

void cmd_dir(struct CONSOLE *cons)
{
	struct FILEINFO *finfo = (struct FILEINFO *) (ADR_DISKIMG + 0x002600);
	int x, y;
	char s[50];
	int size = 0, count = 0;
	struct TASK *task = task_now();
	for(x = 0; x < 224; x ++){
		if(finfo[x].name[0] == 0x00){
			break;
		}
		if(finfo[x].name[0] != 0xe5){
			if((finfo[x].type & 0x18) == 0){
				sprintf(s, "filename.ext  %7dB\n", finfo[x].size);
				for(y = 0; y < 8; y ++){
					s[y] = finfo[x].name[y];
				}
				if(finfo[x].ext[0] != ' '){
					s[9] = finfo[x].ext[0];
				} else {
					s[8] = ' ';
					s[9] = ' ';
				}
				s[10] = finfo[x].ext[1];
 				s[11] = finfo[x].ext[2];
				cons_putstr0(cons, s);
				size += finfo[x].size;
				count++;
			}
		}
	}		
	if(task->langmode == 0){
		sprintf(s, "  %d files	%d Bytes\n  %dBytes free\n", count, size, 1474560 - size);
	} else {
		sprintf(s, "  %d 个文件	%d 字节\n  %d 可用字节\n", count, size, 1474560 - size);
	}
	cons_putstr0(cons, s);
	cons_newline(cons);
	return;
}

void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	char *p;
	struct FILEINFO *finfo = file_search(cmdline + 5, (struct FILEINFO *)(ADR_DISKIMG + 0x002600), 224);
	struct TASK *task = task_now();
	int i, size = finfo->size;

	if(finfo != 0){
		p = file_load2(finfo->clustno, &size, fat);
		for (i = 0; i < size; i++){
			cons_putchar(cons, p[i], 1);					
		}	
		memman_free_4k(memman, (int) p, finfo->size);
	} else {
		if(task->langmode == 0){
			cons_putstr0(cons, "File not found.\n");
		} else {
			cons_putstr0(cons, "找不到该文件。\n");
		}
	}
	cons_newline(cons);
	return;
}
	
void cmd_time(struct CONSOLE *cons)
{
	char s[30];
	struct TASK *task = task_now();
	if(task->langmode == 0){
		sprintf(s, "time:       %d:%02d\n\n", get_hour_hex(), get_min_hex());
	} else {
		sprintf(s, "当前时间:       %d:%02d\n\n", get_hour_hex(), get_min_hex());
	}
	cons_putstr0(cons, s);
	return;
}

void cmd_date(struct CONSOLE *cons)
{
	char s[30];
	struct TASK *task = task_now();
	if(task->langmode == 0){
		sprintf(s, "date:  %d/%d/%d\n\n", get_year(), get_mon_hex(), get_day_of_month());
	} else {
		sprintf(s, "当前日期:  %d/%d/%d\n\n", get_year(), get_mon_hex(), get_day_of_month());
	}
	cons_putstr0(cons, s);
	return;
}

void cmd_bc(struct CONSOLE *cons, char *cmdline)
{
	int bc = strtol(cmdline + 3, 0, 10);
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0xfe4);
	init_screen8(shtctl->sht_back->buf, shtctl->xsize, shtctl->ysize, bc);
	sheet_refresh(shtctl->sht_back, 0, 0, shtctl->xsize, shtctl->ysize);
	cons_newline(cons);
	return;
}

void cmd_exit(struct CONSOLE *cons, int *fat)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct TASK *task = task_now();
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0xfe4);
	struct FIFO32 *fifo = (struct FIFO32 *) *((int *) 0xfec);
	timer_cancel(cons->timer);
	memman_free_4k(memman, (int) fat, 4 * 2880);
	io_cli();
	if(cons->sht != 0){
		fifo32_put(fifo, cons->sht - shtctl->sheets0 + 768);
	} else {
		fifo32_put(fifo, task - taskctl->tasks0 + 1024);
	}
	io_sti();
	for(;;){
		task_sleep(task);
	}
}
void cmd_start(struct CONSOLE *cons, char *cmdline, int memtotal)
{
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0xfe4);
	struct SHEET *sht = open_console(shtctl, memtotal);
	struct FIFO32 *fifo = &sht->task->fifo;
	struct SHEET **key_win = (struct SHEET **) *((int *) 0xfe0);
	int i;
	sheet_slide(sht, 32, 4);
	sheet_updown(sht, shtctl->top);
	keywin_off(*key_win);
	*key_win = sht;
	keywin_on(*key_win);
	for(i = 6; cmdline[i] != 0; i++){
		fifo32_put(fifo, cmdline[i] + 256);
	}
	fifo32_put(fifo, 10 + 256);
	cons_newline(cons);
	return;
}

void cmd_ncst(struct CONSOLE *cons, char *cmdline, int memtotal)
{
	struct TASK *task = open_constask(0, memtotal);
	struct FIFO32 *fifo = &task->fifo;
	int i;
	for(i = 5; cmdline[i] != 0; i++){
		fifo32_put(fifo, cmdline[i] + 256);
	}
	fifo32_put(fifo, 10 + 256);
	cons_newline(cons);
	return;
}

void cmd_langmode(struct CONSOLE *cons, char *cmdline)
{
	struct TASK *task = task_now();
	int langmode = strtol(cmdline + 9, 0, 10);
	if(langmode <= 1){
		task->langmode = langmode;
		if(langmode == 0){
			cons_putstr0(cons, "English ASCII mode.\n\n");
		} else {
			cons_putstr0(cons, "中文EUC模式。\n\n");
		}
	} else {
		if(task->langmode == 0){
			cons_putstr0(cons, "Mode number error.\n\n");
		} else {
			cons_putstr0(cons, "此模式不存在。\n\n");
		}
	}
	return;
}

void cmd_getlang(struct CONSOLE *cons)
{
	struct TASK *task = task_now();
	int langmode = task->langmode;
	if(langmode != 0){
		cons_putstr0(cons, "当前模式：中文EUC模式。\n\n");
	} else {
		cons_putstr0(cons, "mode:English ASCII mode.\n\n");
	}
	return;
}
		
int cmd_run(struct CONSOLE *cons, char *cmdline, int *fat)
{
	int x, i, segsiz, esp, datsiz, nckdat, appsiz;
	struct TASK *task = task_now();
	struct SHTCTL *ctl = (struct SHTCTL *) *((int *) 0xfe4);
	struct SHEET *sht;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo;
	char *p, *q, name[18];
	for(x = 0; x < 13; x ++){
		if(cmdline[x] <= ' '){
			break;
		}
		name[x] = cmdline[x];
	}
	name[x] = 0;
	
	finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if(finfo == 0 && name[x - 1] != '.'){
		name[x] = '.';
		name[x + 1] = 'N';
		name[x + 2] = 'C';
		name[x + 3] = 'K';
		name[x + 4] = 0;
		finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	}
	if(finfo != 0){
		appsiz = finfo->size;
		p = file_load2(finfo->clustno, &appsiz, fat);
		if(finfo->size >= 36 && strncmp(p + 4, "Nick", 4) == 0 && *p == 0x00){
			segsiz = *((int *) (p + 0x0000));
			esp    = *((int *) (p + 0x000c));
			datsiz = *((int *) (p + 0x0010));
			nckdat = *((int *) (p + 0x0014));
			q = (char *) memman_alloc_4k(memman, segsiz);
			task->ds_base = (int) q;
			set_segmdesc(task->ldt + 0, appsiz - 1, (int) p, AR_CODE32_ER + 0x60);
			set_segmdesc(task->ldt + 1, segsiz - 1,      (int) q, AR_DATA32_RW + 0x60);
			for (i = 0; i < datsiz; i++) {
				q[esp + i] = p[nckdat + i];
			}
			start_app(0x1b, 0 * 8 + 4, esp, 1 * 8 + 4, &(task->tss.esp0));
			task->langbyte1 = 0;
			for(i = 0; i < MAX_SHEETS; i ++){
				sht = &ctl->sheets0[i];
				if((sht->flags & 0x10) != 0 && sht->task == task){
					sheet_free(sht);
				}
			}
			for(i = 0; i < 8; i++){
				if(task->fhandle[i].buf != 0){
					memman_free_4k(memman, (int) task->fhandle[i].buf, task->fhandle[i].size);
					task->fhandle[i].buf = 0;
				}
			}
			timer_cancelall(&task->fifo);
			i = io_in8(0x61);
			io_out8(0x61, i & 0xd);
			memman_free_4k(memman, (int) q, segsiz);
		} else {
			if(task->langmode == 0){
				cons_putstr0(cons, ".nck file format error.\n");
			} else {
				cons_putstr0(cons, ".nck 文件格式错误。\n");
			}
		}
		memman_free_4k(memman, (int) p, finfo->size);
		cons_newline(cons);
		return 1;
	} 
	return 0;
}
