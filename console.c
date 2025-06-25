#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void cons_newline(struct CONSOLE *cons);
void cmd(struct CONSOLE *cons, unsigned int memtotal, int *fat, char *cmdline);
void cmd_mem(struct CONSOLE *cons, unsigned int memtotal);
void cmd_cls(struct CONSOLE *cons);
void cmd_dir(struct CONSOLE *cons);
void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline);
void cmd_time(struct CONSOLE *cons);
void cmd_date(struct CONSOLE *cons);
void cmd_bc(struct CONSOLE *cons, char *cmdline);
void cmd_exit(struct CONSOLE *cons, int *fat);
void cmd_start(struct CONSOLE *cons, char *cmdline, int memtotal);
void cmd_ncst(struct CONSOLE *cons, char *cmdline, int memtotal);
int cmd_run(struct CONSOLE *cons, char *cmdline, int *fat);
void linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col);
int strtol(char *s, char **endp, int base);

struct MOUSE {
	int x, y;
	int phase;
};

void cons_newline(struct CONSOLE *cons)
{
	int x, y;
	struct SHEET *sheet = cons->sht;
	if(cons->cursor_y < 28 + 447){
		cons->cursor_y += 16;
	} else {
		if(sheet != 0){
			for(y = 28; y < 28 + 447; y ++){
				for(x = 8; x < 8 + 484; x ++){
					sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 16) * sheet->bxsize];
				}
			}
			boxfill8(sheet->buf, sheet->bxsize, COL8_000000, 8, 28 + 447, 8 + 484, 28 + 463);
			sheet_refresh(sheet, 8, 28, 8 + 484, 28 + 463);
		}
	}
	cons->cursor_x = 8;
	return;
}

void cons_putchar(struct CONSOLE *cons, int chr, char move)
{
	char s[2];
	s[0] = chr;
	s[1] = 0;
	if(s[0] == 0x09){
		for(;;){
			if(cons->sht != 0){
				putfonts8_asc_sht(cons->sht, cons->cursor_x, cons->cursor_y, COL8_FFFFFF, COL8_000000, " ", 1);
				cons->cursor_x += 8;
			}
			if(cons->cursor_x == 8 + 480){
				cons_newline(cons);
			}
			if(((cons->cursor_x - 8) % 32) ==0){
				break;
			}
		}
	} else if(s[0] == 0x0a){
		cons_newline(cons);
	} else if(s[0] == 0x0d){
	
	} else {
		if(cons->sht != 0){
			putfonts8_asc_sht(cons->sht, cons->cursor_x, cons->cursor_y, COL8_FFFFFF, COL8_000000, s, 1);
		}
		if(move != 0){
			cons->cursor_x += 8;
			if(cons->cursor_x == 480){
				cons_newline(cons);
			}
		}
	}
	return;
}

void cons_putstr0(struct CONSOLE *cons, char *s)
{
	for(; *s != 0; s++){
		cons_putchar(cons, *s, 1);
	}
	return;
}

void cons_putstr1(struct CONSOLE *cons, char *s, int l)
{
	int i;
	for(i = 0; i <= l; i ++){
		cons_putchar(cons, *s, 1);
		s ++;
	}
}

int *api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
	struct TASK *task = task_now();
	struct FIFO32 *sys_fifo = (struct FIFO32 *) *((int *) 0xfec);
	int ds_base = task->ds_base, i;
	struct CONSOLE *cons = task->cons;
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0xfe4);
	struct SHEET *sht;
	struct MOUSE *mouse = (struct MOUSE *) *((int *) 0xfe0);
	mouse->phase = 0;
	int *reg = &eax + 1;
	/* reg[0] : EDI,   reg[1] : ESI,   reg[2] : EBP,   reg[3] : ESP */
	/* reg[4] : EBX,   reg[5] : EDX,   reg[6] : ECX,   reg[7] : EAX */
	if(edx == 1){
		cons_putchar(cons, eax & 0xff, 1);
	} else if(edx == 2){
		cons_putstr0(cons,(char *) ebx + ds_base);
	} else if(edx == 3){
		cons_putstr1(cons, (char *) ebx + ds_base, ecx);
	} else if(edx == 4){
		return &(task->tss.esp0);
	} else if(edx == 5){
		sht = sheet_alloc(shtctl);
		sheet_setbuf(sht, (char *) ebx + ds_base, esi, edi, eax);
		make_window8((char *) ebx + ds_base, esi, edi, (char *) ecx + ds_base, 0);
		sheet_slide(sht, (shtctl->xsize - esi)/ 2 & ~3, (shtctl->ysize - edi) / 2);
		sheet_updown(sht, shtctl->top);	
		sht->task = task;
		sht->flags |= 0x10;
		sht->title = (char *) (ecx + ds_base);
		reg[7] = (int) sht;
	} else if(edx == 6){
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		putfonts8_asc(sht->buf, sht->bxsize, esi, edi, eax, (char *) ebp + ds_base);
		if((ebx & 1) == 0){
			sheet_refresh(sht, esi, edi, esi + ecx * 8, edi + 16);
		}
	} else if(edx == 7){
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		boxfill8(sht->buf, sht->bxsize, ebp, eax, ecx, esi, edi);
		if((ebx & 1) == 0){ 
			sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
		}
	} else if (edx == 8) {
		memman_init((struct MEMMAN *) (ebx + ds_base));
		ecx &= 0xfffffff0;	
		memman_free((struct MEMMAN *) (ebx + ds_base), eax, ecx);
	} else if (edx == 9) {
		ecx = (ecx + 0x0f) & 0xfffffff0;
		reg[7] = memman_alloc((struct MEMMAN *) (ebx + ds_base), ecx);
	} else if (edx == 10) {
		ecx = (ecx + 0x0f) & 0xfffffff0; 
		memman_free((struct MEMMAN *) (ebx + ds_base), eax, ecx);
	} else if(edx == 11){
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		sht->buf[sht->bxsize * edi + esi] = eax;
		if((ebx & 1) == 0){
			sheet_refresh(sht, esi, edi, esi + 1, edi + 1);
		}
	} else if(edx == 12){
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		sheet_refresh(sht, eax, ecx, esi, edi);
	} else if(edx == 13){
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		make_textbox8(sht, eax, ecx, esi, edi, ebp);
		if((ebx & 1) == 0){
			sheet_refresh(sht, 0, 0, sht->bxsize, sht->bysize);
		}
	} else if(edx == 14){
		sht = (struct SHEET *) (ebx & 0xfffffffe);
		linewin(sht, eax, ecx, esi, edi, ebp);
		if((ebx & 1) == 0){
			sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
		}
	} else if(edx == 15){
		sht = (struct SHEET *)ebx;
		sheet_free(sht);
	} else if(edx == 16){
		for(;;){
			io_cli();
			if(fifo32_status(&task->fifo) == 0){
				if(eax != 0){
					task_sleep(task);
				} else {
					io_sti();
					reg[7] = -1;
					return 0;
				}
			}
			i = fifo32_get(&task->fifo);
			io_sti();
			if(i <= 1){
				timer_init(cons->timer, &task->fifo, 1);
				timer_settime(cons->timer, 50);
			}
			if(i == 2){
				cons->cursor_c = COL8_FFFFFF;
			} 
			if(i == 3){
				cons->cursor_c = -1;
			}
			if(i == 4){
				timer_cancel(cons->timer);
				io_cli();
				fifo32_put(sys_fifo, cons->sht - shtctl->sheets0 + 2024);
				cons->sht = 0;
				io_sti();
			}
			if(i >= 256 && i <= 511){
				reg[7] = i - 256;
				return 0;
			}
		}
	} else if(edx == 17){
		sht = (struct SHEET *) ebx;
		for(;;){
			io_cli();
			if(fifo32_status(&task->fifo) == 0){
				if(eax != 0){
					task_sleep(task);
				} else {
					io_sti();
					reg[7] = -1;
					return 0;
				}
			}
			i = fifo32_get(&task->fifo);
			io_sti();
			if(i <= 1){
				timer_init(cons->timer, &task->fifo, 1);
				timer_settime(cons->timer, 50);
			}
			if(i == 2){
				cons->cursor_c = COL8_FFFFFF;
			} 
			if(i == 3){
				cons->cursor_c = -1;
			}
			if(i == 13){
				mouse->phase = 1;
			}
			if(i == 14){
				mouse->phase = 2;
			}
			if(i == 4){
				timer_cancel(cons->timer);
				io_cli();
				fifo32_put(sys_fifo, cons->sht - shtctl->sheets0 + 2024);
				cons->sht = 0;
				io_sti();
			}
			if (i >= 512){
				i -= 512;
				if(mouse->phase == 1){
					mouse->phase = 0;
					mouse->x = i - sht->vx0;
					if(sht->vx0 <= i && i <= sht->bxsize + sht->vx0){
						
					} else {
						mouse->x = 0;
					}
				} else {
					if(mouse->phase == 2){
						mouse->y = i - sht->vy0;
						mouse->phase = 0;
						if(sht->vy0 <= i && i <= sht->bysize + sht->vy0){
							if(mouse->x != 0){
								*((int *) (ecx + ds_base)) = mouse->x;
								*((int *) (esi + ds_base)) = mouse->y;
								return 0;
							}
						}
					}
				}
			}
		}
	} else if(edx == 18){
		reg[7] = (int) timer_alloc();
		((struct TIMER *) reg[7])->flags2 = 1;
	} else if(edx == 19){
		timer_init((struct TIMER *) ebx, &task->fifo, eax + 256);
	} else if(edx == 20){
		timer_settime((struct TIMER *) ebx, eax);
	} else if(edx == 21){
		timer_free((struct TIMER *) ebx);
	} else if(edx == 22){
		if(eax == 0){
			i = io_in8(0x61);
			io_out8(0x61, i & 0xd);
		} else {
			i = 1193180000 / eax;
			io_out8(0x43, 0xb6);
			io_out8(0x42, i & 0xff);
			io_out8(0x42, i >> 8);
			i = io_in8(0x61);
			io_out8(0x61, (i | 0x03) & 0x0f);
		}
	}
	return 0;
}
	
void linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col)
{
	int dx, dy, len, i, x, y;
	
	dx = x1 - x0;
	dy = y1 - y0;
	x = x0 << 10;
	y = y0 << 10;
	
	if(dx < 0){
		dx = -dx;
	}
	if(dy < 0){
		dy = -dy;
	}
	if(dx >= dy){
		len = dx + 1;
		if(x0 > x1){
			dx = -1024;
		} else {
			dx = 1024;
		}
		if(y0 <= y1){
			dy = ((y1 - y0 + 1) << 10) / len;
		} else {
			dy = ((y1 - y0 - 1) << 10) / len;
		}
	} else {
		len = dy + 1;
		if(y0 > y1){
			dy = -1024;
		} else {
			dy = 1024;
		}
		if(x0 <= x1){
			dx = ((x1 - x0 + 1) << 10) / len;
		} else {
			dx = ((x1 - x0 - 1) << 10) / len;
		}
	}
	for(i = 0; i < len; i ++){
		sht->buf[(y >> 10) * sht->bxsize + (x >> 10)] = col;
		x += dx;
		y += dy;
	}
	return;
}
		
	
void cmd(struct CONSOLE *cons, unsigned int memtotal, int *fat, char *cmdline)
{
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
	} else if (cmdline[0] != 0){		
		if(cmd_run(cons, cmdline, fat) == 0){
		cons_putstr0(cons, "Bad command.\n\n");
		}
	}
	return;
}	
									
void cmd_mem(struct CONSOLE *cons, unsigned int memtotal)
{
	char s[30];
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	
	sprintf(s, "total   %dMB\nfree %dKB\n\n", memtotal / (1024 * 1024), memman_total(memman) / 1024);
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
	char s[30];
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
			}
		}
	}		
	cons_newline(cons);
	return;
}

void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	char *p;
	struct FILEINFO *finfo = file_search(cmdline + 5, (struct FILEINFO *)(ADR_DISKIMG + 0x002600), 224);
	
	if(finfo != 0){
		p = (char *) memman_alloc_4k(memman, finfo->size);
		file_load(finfo->clustno, finfo->size, p, fat, (char *)(ADR_DISKIMG + 0x003e00));
		cons->cursor_x = 8;
		cons_putstr1(cons, p, finfo->size);
		memman_free_4k(memman, (int) p, finfo->size);
	} else {
		cons_putstr0(cons, "File not found.\n");
	}
	cons_newline(cons);
	return;
}
	
void cmd_time(struct CONSOLE *cons)
{
	char s[30];
	
	sprintf(s, "time:       %d:%02d\n\n", get_hour_hex(), get_min_hex());
	cons_putstr0(cons, s);
	return;
}

void cmd_date(struct CONSOLE *cons)
{
	char s[30];
	
	sprintf(s, "date:  %d.%d.%d\n\n", get_year(), get_mon_hex(), get_day_of_month());
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
	int i;
	sheet_slide(sht, 32, 4);
	sheet_updown(sht, shtctl->top);
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

int cmd_run(struct CONSOLE *cons, char *cmdline, int *fat)
{
	int x, i, segsiz, esp, datsiz, nckdat;
	struct TASK *task = task_now();
	struct SHTCTL *ctl = (struct SHTCTL *) *((int *) 0xfe4);
	struct SHEET *sht;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo;
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	char *p, *q, name[18], s[10];
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
		name[x + 1] = 'n';
		name[x + 2] = 'c';
		name[x + 3] = 'k';
		name[x + 4] = 0;
		finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	}
	if(finfo != 0){
		p = (char *) memman_alloc_4k(memman, finfo->size);
		file_load(finfo->clustno, finfo->size, p, fat, (char *)(ADR_DISKIMG + 0x003e00));
		if(finfo->size >= 36 && strncmp(p + 4, "Nick", 4) == 0 && *p == 0x00){
			segsiz = *((int *) (p + 0x0000));
			esp    = *((int *) (p + 0x000c));
			datsiz = *((int *) (p + 0x0010));
			nckdat = *((int *) (p + 0x0014));
			q = (char *) memman_alloc_4k(memman, segsiz);
			task->ds_base = (int) q;
			set_segmdesc(task->ldt + 0, finfo->size - 1, (int) p, AR_CODE32_ER + 0x60);
			set_segmdesc(task->ldt + 1, segsiz - 1,      (int) q, AR_DATA32_RW + 0x60);
			for (i = 0; i < datsiz; i++) {
				q[esp + i] = p[nckdat + i];
			}
			start_app(0x1b, 0 * 8 + 4, esp, 1 * 8 + 4, &(task->tss.esp0));
			for(i = 0; i < MAX_SHEETS; i ++){
				sht = &ctl->sheets0[i];
				if((sht->flags & 0x10) != 0 && sht->task == task){
					sheet_free(sht);
				}
			}
			timer_cancelall(&task->fifo);
			i = io_in8(0x61);
			io_out8(0x61, i & 0xd);
			memman_free_4k(memman, (int) q, segsiz);
		} else {
			cons_putstr0(cons, ".nck file format error.\n");
		}
		memman_free_4k(memman, (int) p, finfo->size);
		cons_newline(cons);
		return 1;
	}
	return 0;
}

void console_task(struct SHEET *sheet, unsigned int memtotal)
{
	struct TASK *task = task_now();
	int i;
	struct CONSOLE cons;
	cons.sht = sheet;
	cons.cursor_x = 8;
	cons.cursor_y = 60;
	cons.cursor_c = -1;
	task->cons = &cons;
	char cmdline[60], s[2];
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	int *fat = (int *) memman_alloc_4k(memman, 4 * 2880);
	if(cons.sht != 0){
		cons.timer = timer_alloc();
		timer_init(cons.timer, &task->fifo, 1);
		timer_settime(cons.timer, 50);
	}

	file_readfat(fat, (unsigned char *)(ADR_DISKIMG + 0x000200));
	putfonts8_asc_sht(cons.sht, 8, 28, COL8_FFFFFF, COL8_000000, "Nick OS [version 0.0.5]", 23);
	putfonts8_asc_sht(cons.sht, 8, 44, COL8_FFFFFF, COL8_000000, "(c) 2017 Nick", 13);
	cons_putchar(&cons, '>', 1);

	for (;;) {
		io_cli();
		if (fifo32_status(&task->fifo) == 0) {
			task_sleep(task);
			io_sti();
		} else {
			i = fifo32_get(&task->fifo);
			io_sti();
			if (i <= 1 && cons.sht != 0) { 
				if (i != 0) {
					timer_init(cons.timer, &task->fifo, 0); 
					if(cons.cursor_c >= 0){
						cons.cursor_c = COL8_FFFFFF;
					}
				} else {
					timer_init(cons.timer, &task->fifo, 1); 
					if(cons.cursor_c >= 0){
						cons.cursor_c = COL8_000000;
					}
				}
				timer_settime(cons.timer, 50);
			}
			if(i == 2){
				cons.cursor_c = COL8_000000;
			}
			if(i == 3){
				if(cons.sht != 0){
				boxfill8(cons.sht->buf, cons.sht->bxsize, COL8_000000, cons.cursor_x, cons.cursor_y + 16 - 4, cons.cursor_x + 7, cons.cursor_y + 16);
				}
				cons.cursor_c = -1;
			}
			if(i == 4){
				cmd_exit(&cons, fat);
			}
			if (256 <= i && i <= 511) {
				if (i == 8 + 256) {
					if (cons.cursor_x > 16) {
						cons_putchar(&cons, ' ', 0);
						putfonts8_asc_sht(cons.sht, cons.cursor_x - 8, cons.cursor_y, COL8_FFFFFF, COL8_000000, " ", 1);
						cons.cursor_x -= 8;
					}
				} else if(i == 10 + 256){
					cons_putchar(&cons, ' ', 0);
					cmdline[cons.cursor_x / 8 - 2] = 0;
					cons_newline(&cons);
					cmd(&cons, memtotal, fat, cmdline);
					cons_putchar(&cons, '>', 1);
					if(cons.sht == 0){
						cmd_exit(&cons, fat);
					}
				} else {
					if (cons.cursor_x < 480){
						s[0] = i - 256;
						s[1] = 0;
						cmdline[cons.cursor_x / 8 - 2] = i - 256;
						cons_putchar(&cons, s[0], 1);
					}
				}
			}
			if(cons.sht != 0){
				if(cons.cursor_c >= 0){
					boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cursor_c, cons.cursor_x , cons.cursor_y + 16 - 4, cons.cursor_x + 7, cons.cursor_y + 15);
				}
				sheet_refresh(cons.sht, cons.cursor_x, cons.cursor_y + 16 - 4, cons.cursor_x + 8, cons.cursor_y + 16);
			}
		}
	}
}

int *inthandler0d(int *esp)
{
	char s[30];
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	cons_putstr0(cons, "\nINT 0D :\n General Protected Exception.\n");
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr0(cons, s);
	return &(task->tss.esp0);
}

int *inthandler0c(int *esp)
{
	char s[30];
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	cons_putstr0(cons, "\nINT 0C :\n Stack Exception.\n");
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr0(cons, s);
	return &(task->tss.esp0);
}

int *inthandler00(int *esp)
{
	char s[30];
	struct TASK *task = task_now();
	struct CONSOLE *cons = task->cons;
	cons_putstr0(cons, "\nINT 00 :\n Devide '0' Exception.\n");
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr0(cons, s);
	return &(task->tss.esp0);
}
