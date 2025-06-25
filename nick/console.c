#include "bootpack.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cons_newline(struct CONSOLE *cons)
{
	struct TASK *task = task_now();
	int x, y;
	struct SHEET *sheet = cons->sht;
	if(cons->cursor_y < 28 + 447){
		cons->cursor_y += 16;
	} else {
		if(sheet != 0){
			for(y = 28; y < 28 + 447; y ++){
				for(x = 8; x < 8 + 484; x ++){
					sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 17) * sheet->bxsize];
				}
			}
			boxfill8(sheet->buf, sheet->bxsize, COL8_000000, 8, 28 + 447, 8 + 484, 28 + 463);
			sheet_refresh(sheet, 8, 28, 8 + 484, 28 + 464);
		}
	}
	cons->cursor_x = 8;
	if(task->langmode == 1 && task->langbyte1 != 0){
		cons->cursor_x += 8;
	}
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
			if(((cons->cursor_x - 8) & 0x1f) ==0){
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
			
void console_task(struct SHEET *sheet, unsigned int memtotal)
{
	struct TASK *task = task_now();
	int i;
	struct CONSOLE cons;
	cons.sht = sheet;
	cons.cursor_x = 8;
	cons.cursor_y = 76;
	cons.cursor_c = -1;
	task->cons = &cons;
	char cmdline[60], s[2];
	task->cmdline = cmdline;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	int *fat = (int *) memman_alloc_4k(memman, 4 * 2880);
	struct FILEHANDLE fhandle[8];
	unsigned char *chinese = (unsigned char *) *((int *) 0xfe8);
	if(chinese[4096] != 0xff){
		task->langmode = 1;
	} else {
		task->langmode = 0;
	}
	task->langbyte1 = 0;
	if(cons.sht != 0){
		cons.timer = timer_alloc();
		timer_init(cons.timer, &task->fifo, 1);
		timer_settime(cons.timer, 50);
	}
	file_readfat(fat, (unsigned char *)(ADR_DISKIMG + 0x000200));
	for(i = 0; i < 8; i++){
		fhandle[i].buf = 0;
	}
	task->fhandle = fhandle;
	task->fat = fat;
	if(task->langmode == 0){
		putfonts8_asc_sht(cons.sht, 8, 28, COL8_FFFFFF, COL8_000000, "Nick OS [Version  0.0.9]", 23);
		putfonts8_asc_sht(cons.sht, 8, 44, COL8_FFFFFF, COL8_000000, "(c) 2018 Nick ", 26);
		putfonts8_asc_sht(cons.sht, 8, 60, COL8_FFFFFF, COL8_000000, "Mode:English ASCII mode.", 19);
	} else if(task->langmode == 1){
		putfonts8_asc_sht(cons.sht, 8, 28, COL8_FFFFFF, COL8_000000, "Nick OS [版本  0.0.9]", 23);
		putfonts8_asc_sht(cons.sht, 8, 44, COL8_FFFFFF, COL8_000000, "(c) 2018 Nick 保留所有权利。", 26);
		putfonts8_asc_sht(cons.sht, 8, 60, COL8_FFFFFF, COL8_000000, "当前显示模式：中文EUC模式。", 30);
	}
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
			if(i == 5){
				if(cons.sht != 0){
					cons_putstr0(&cons, "switch(key):\n中文EUC模式。\n\n");
					cons_putchar(&cons, '>', 1);
				}
			}
			if(i == 6){
				if(cons.sht != 0){
					cons_putstr0(&cons, "switch(key):\nEnglish ASCII mode.\n\n");
					cons_putchar(&cons, '>', 1);
				}
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
						if(i <= 256 + 250){
							s[0] = i - 256;
							s[1] = 0;
							cmdline[cons.cursor_x / 8 - 2] = i - 256;
							cons_putchar(&cons, s[0], 0);
							cons.cursor_x += 8;
						}
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
