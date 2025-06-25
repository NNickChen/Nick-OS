/* bootpack */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>
#define MAX_SHTINV 5


void NickStartup(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct FIFO32 fifo, keycmd;
	struct SHTCTL *shtctl;
	char s[40];
	int fifobuf[128], keycmd_buf[32];
	int mx, my, i, x, y, j, mmx = -1, mmy = -1, mmx2 = 0, show = 0, *fat, new_mx = -1, new_my = 0, new_wx = 0x7fffffff, new_wy = 0;
	extern char english[4096];
	unsigned char *chinese;
	struct FILEINFO *finfo;
	unsigned int memtotal;
	struct MOUSE_DEC mdec;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	static char keytable[0x54] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08,   0x09,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0x0a,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0x27, '`',   0,   0x5c, 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   0, 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,  0 ,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};
	static char keytable1[0x54] = {
		0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 0x22, '~', 0, 0x7c, 'Z', 'X', 'C', 'V', 
		'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,   0,   0,   0,   0,   0,
		0,  0 ,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};
	unsigned char *buf_back, buf_mouse[256];
	struct SHEET *sht_back, *sht_mouse, *sht = 0, *key_win, *sht2;
	struct TASK *task_a, *task_time, *task;
	int key_shift = 0, key_leds = (binfo->leds >> 4) & 7, keycmd_wait = -1, key_ctl = 0;
	fifo32_init(&keycmd, 32, keycmd_buf, 0);

	init_gdtidt();
	init_pic();
	io_sti(); 
	fifo32_init(&fifo, 128, fifobuf, 0);
	*((int *) 0xfec) = (int) &fifo;
	init_pit();
	init_keyboard(&fifo, 256);
	enable_mouse(&fifo, 512, &mdec);
	io_out8(PIC0_IMR, 0xf8); 
	io_out8(PIC1_IMR, 0xef); 

	memtotal = memtest(0x00400000, 0xbfffffff);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
	memman_free(memman, 0x00400000, memtotal - 0x00400000);

	init_palette();
	/* if (binfo->vb != 0x0000){
		start_timer = timer_alloc();
		timer_init(start_timer, &fifo, 10);
		timer_settime(start_timer, 500);
		block_timer = timer_alloc();
		timer_init(block_timer, &fifo, 5);
		timer_settime(block_timer, 50);
		boxfill8(binfo->vram, binfo->scrnx, COL8_00FFFF, 0, 0, binfo->scrnx, binfo->scrny);
		putfonts8_asc(binfo->vram, binfo->scrnx, binfo->scrnx/2-9*8, binfo->scrny/2, COL8_000000, "Welcome to Nick OS!");
		putfonts8_asc(binfo->vram, binfo->scrnx, binfo->scrnx/2-7*8, binfo->scrny/2+16, COL8_000000, "Starting Up... ");
		x=binfo->scrnx/2-9*8;
		for(;;){
			io_cli();
			if(fifo32_status(&fifo) == 0){
				io_stihlt();
			} else {
				i = fifo32_get(&fifo);
				if(i == 5){
					boxfill8(binfo->vram, binfo->scrnx, COL8_000000, x, binfo->scrny/2+32, x+10, binfo->scrny/2+32+10);
					putfonts8_asc(binfo->vram, binfo->scrnx, binfo->scrnx/2-9*8, binfo->scrny/2, COL8_000000, "Welcome to Nick OS! ");
					putfonts8_asc(binfo->vram, binfo->scrnx, binfo->scrnx/2-7*8, binfo->scrny/2+16, COL8_000000, "Starting Up... ");
					x += 15;
					timer_settime(block_timer, 50);
				} else {
					if(i == 10){
						break;
					}
				}
			}
		}
	} */
	shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
	*((int *) 0xfe4) = (int) shtctl;
	task_a = task_init(memman);
	fifo.task = task_a;
	task_run(task_a, 1, 2);
	
	fat = (int *) memman_alloc_4k(memman, 4 * 2880);
	file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
	finfo = file_search("chinese.fnt", (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if(finfo != 0){
		i = finfo->size;
		chinese = file_load2(finfo->clustno, &i, fat);
	} else {
		chinese = (unsigned char *) memman_alloc_4k(memman, 169552);
		for(i = 0; i < 4096; i++){
			chinese[i] = english[i];
		}
		for(i = 4096; i < 169552; i++){
			chinese[i] = 0xff;
		}
	}
	*((int *) 0xfe8) = (int) chinese;
	memman_free_4k(memman, (int) fat, 4 * 2880);
	if(chinese[4096] != 0xff){
		task_a->langmode = 1;
	} else {
		task_a->langmode = 0;
	}
	task_a->langbyte1 = 0;

	/* sht_back */
	sht_back  = sheet_alloc(shtctl);
	buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1);
	init_screen8(buf_back, binfo->scrnx, binfo->scrny, 16 + 0 + 3 * 6 + 4 * 36);
	key_win = sht_back;
	task_time = task_alloc();
	task_time->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024 - 8;
	task_time->tss.eip = (int) &time_task;
	task_time->tss.es = 1 * 8;
	task_time->tss.cs = 2 * 8;
	task_time->tss.ss = 1 * 8;
	task_time->tss.ds = 1 * 8;
	task_time->tss.fs = 1 * 8;
	task_time->tss.gs = 1 * 8;
	*((int *) (task_time->tss.esp + 4)) = (int) sht_back;
	task_run(task_time, 1, 2);
	shtctl->sht_back = sht_back;
	
	/* sht_cons */
	key_win = open_console(shtctl, memtotal);


	/* sht_mouse */
	sht_mouse = sheet_alloc(shtctl);
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
	init_mouse_cursor8(buf_mouse, 99);
	mx = (binfo->scrnx - 16) / 2; 
	my = (binfo->scrny - 28 - 16) / 2;

	sheet_slide(sht_back,  0,  0);
	sheet_slide(key_win, 32, 4);
	sheet_slide(sht_mouse, mx, my);
	sheet_updown(sht_back,  0);
	sheet_updown(key_win, 1);
	sheet_updown(sht_mouse, 2);
	keywin_on(key_win);
	
	fifo32_put(&keycmd, 0xed);
	fifo32_put(&keycmd, key_leds);
	

	for (;;) {
		if(fifo32_status(&keycmd) > 0 && keycmd_wait < 0){
			keycmd_wait = fifo32_get(&keycmd);
			wait_KBC_sendready();
			io_out8(PORT_KEYDAT,keycmd_wait);
		}
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			if(new_mx >= 0){
				io_sti();
				sheet_slide(sht_mouse, new_mx, new_my);
				new_mx = -1;
			} else if(new_wx != 0x7fffffff){
				io_sti();
				sheet_slide(sht, new_wx, new_wy);
				new_wx = 0x7fffffff;
			} else {
				task_sleep(task_a);
				io_sti();
			}
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if(key_win != 0 && key_win->flags == 0){
				if(shtctl->top == 1){
					key_win = 0;
				} else {
					key_win = shtctl->sheets[shtctl->top - 1];
					keywin_on(key_win);
				}
			}
			if (256 <= i && i <= 511) { 
				if (show != 0){
					sprintf(s, "%02X", i - 256);
					putfonts8_asc_sht(sht_back, 0, 16, COL8_FFFFFF, get_bc(buf_back), s, 2);
				}
				if (i < 0x54 + 256 && key_ctl == 0) { 
					if(key_shift == 0){
						s[0] = keytable[i - 256];
					} else {
						s[0] = keytable1[i - 256];
					}
				} else { 
					s[0] = 0;
				}
				if('A' <= s[0] && s[0] <= 'Z'){
					if((key_leds & 4) == 0 && key_shift == 0){
						s[0] += 0x20;
					} else {
						if((key_leds & 4) != 0 && key_shift != 0){
							s[0] += 0x20;
						}
					}
				}
				if(s[0] != 0 && key_win != 0){
					fifo32_put(&key_win->task->fifo, s[0] + 256);
				}					   
				if (i == 256 + 0x48){	/* 向上键 */
					fifo32_put(&key_win->task->fifo, 256 + 255);
				}
				if (i == 256 + 0x50){	/* 向下键*/
					fifo32_put(&key_win->task->fifo, 256 + 254);
				}
				if (i == 256 + 0x4b){	/* 向左键*/
					fifo32_put(&key_win->task->fifo, 256 + 253);
				}
				if (i == 256 + 0x4d){	/* 向右键*/
					fifo32_put(&key_win->task->fifo, 256 + 252);
				}
				if (i == 256 + 0x01){	/* Esc */
					fifo32_put(&key_win->task->fifo, 256 + 251);
				}
				if (i == 256 + 0x0f && key_win != 0) { /* Tab */
					keywin_off(key_win);
					j = key_win->height - 1;
					if(j == 0){
						j = shtctl->top - 1;
					}
					key_win = shtctl->sheets[j];
					keywin_on(key_win);
				}
				if (i == 256 + 0x2a){
					key_shift |= 1;
				}
				if (i == 256+ 0x36){
					key_shift |= 2;
				}
				if (i == 256 + 0xaa){
					key_shift &= ~1;
				}
				if (i == 256 + 0xb6){
					key_shift &= ~2;
				}
				if (i == 256 + 0x1d){
					key_ctl = 1;
				}
				if (i == 256 + 0x9d){
					key_ctl = 0;
				}
				if (i == 256 + 0x3a){
					key_leds ^= 4;
					fifo32_put(&keycmd, 0xed);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x45){
					key_leds ^= 2;
					fifo32_put(&keycmd, 0xed);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x46){
					key_leds ^= 1;
					fifo32_put(&keycmd, 0xed);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0xfa){
					keycmd_wait = -1;
				}
				if (i == 256 + 0xfe){
					wait_KBC_sendready();
					io_out8(PORT_KEYDAT, keycmd_wait);
				}
				if (i == 256 + 0x01 && key_shift != 0){
					task = key_win->task;
					if(task != 0 && task->tss.ss0 != 0){
						io_cli();
						task->tss.eax = (int) &(task->tss.esp0);
						task->tss.eip = (int) end_app;
						io_sti();
					}
				}
				if (i == 256 + 0x57){
					sheet_updown(shtctl->sheets[1], shtctl->top - 1);
				}
				if (i == 256 + 0x3b && key_shift != 0 && key_win != 0){
					if(show == 0){
						show = 1;
					} else if(show != 0){
						boxfill8(buf_back, sht_back->bxsize, get_bc(buf_back), 0, 0, 150, 60);
						sheet_refresh(sht_back, 0, 0, 150, 60);
						show = 0;
					}
				}
				if (i == 256 + 0x3c && key_shift != 0){
					if(key_win != 0){
						keywin_off(key_win);
					}
					key_win = open_console(shtctl, memtotal);
					sheet_slide(key_win, 32, 4);
					sheet_updown(key_win, shtctl->top);
					keywin_on(key_win);
				}
				if (key_shift != 0 && i == 256 + 0x3d){
					if(key_win->task->langmode == 0){
						key_win->task->langmode = 1;
						fifo32_put(&key_win->task->fifo, 5);
					} else {
						key_win->task->langmode = 0;
						fifo32_put(&key_win->task->fifo, 6);
					}
				}
			} else if (512 <= i && i <= 767) {
				if (mouse_decode(&mdec, i - 512) != 0) {
					mx += mdec.x;
					my += mdec.y;
					if(show != 0){
						sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
						if ((mdec.btn & 0x01) != 0) {
							s[1] = 'L';
						}
						if ((mdec.btn & 0x02) != 0) {
							s[3] = 'R';
						}
						if ((mdec.btn & 0x04) != 0) {
							s[2] = 'C';
						}
						putfonts8_asc_sht(sht_back, 32, 16, COL8_FFFFFF, get_bc(buf_back), s, 15);
					}
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (mx > binfo->scrnx - 1) {
						mx = binfo->scrnx - 1;
					}
					if (my > binfo->scrny - 1) {
						my = binfo->scrny - 1;
					}
					if(show != 0){
						sprintf(s, "(%3d, %3d)", mx, my);
						putfonts8_asc_sht(sht_back, 0, 0, COL8_FFFFFF, get_bc(buf_back), s, 10);
					}
					new_mx = mx;
					new_my = my;
					if ((mdec.btn & 0x01) != 0) {
						if(mmx < 0){
							for(j = shtctl->top - 1; j > 0; j--){
								sht = shtctl->sheets[j];
								x = mx - sht->vx0;
								y = my - sht->vy0;
								if(0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize && sht->buf[x + y * sht->bxsize] != sht->col_inv){
									if(sht != key_win){
										sheet_updown(sht, shtctl->top - 1);
										keywin_off(key_win);
										key_win = sht;
										keywin_on(key_win);
									}
									if(y < 20){
										mmx = mx;
										mmy = my;
										mmx2 = sht->vx0;
										new_wy = sht->vy0;
										if(sht->bxsize - 21 <= x && x <= sht->bxsize - 5 && 5 <= y && y<= 19 && sht->col_inv <= 0){
											if((sht->flags & 0x10) != 0){
												task = sht->task;
												io_cli();
												task->tss.eax = (int) &(task->tss.esp0);
												task->tss.eip = (int) end_app;
												io_sti();
											} else {
												task = sht->task;
												sheet_updown(sht, -1);
												keywin_off(key_win);
												if(shtctl->top > 1){
													key_win = shtctl->sheets[shtctl->top - 1];
													keywin_on(key_win);
												} else {
													key_win = 0;
												}
												io_cli();
												fifo32_put(&sht->task->fifo, 4);
												io_sti();
											}
										}
									}
									break;
								} 
							}
						} else {
							x = mx - mmx;
							y = my - mmy;
							new_wx = (mmx2 + x + 2) & ~3;
							new_wy = new_wy + y;
							mmy = my;
						}
						if((key_win->flags & 0x10) != 0){
							fifo32_put(&key_win->task->fifo, 13);
							fifo32_put(&key_win->task->fifo, mx + 512);
							fifo32_put(&key_win->task->fifo, 14);
							fifo32_put(&key_win->task->fifo, my + 512);
						}
					} else {
						mmx = -1;
						if(new_wx != 0x7fffffff){
							sheet_slide(sht, new_wx, new_wy);
							new_wx = 0x7fffffff;
						}
					}
				}
			} else if (768 <= i && i <= 1023) {
				close_console(shtctl->sheets0 + (i - 768));
			} else if (1024 <= i && i <= 2023){
				close_constask(taskctl->tasks0 + (i - 1024));
			} else if (2024 <= i && i <= 2279){
				sht2 = shtctl->sheets0 + (i - 2024);
				memman_free_4k(memman, (int) sht2->buf, 500 * 500);
				sheet_free(sht2);
			}
		}
	}
}


void keywin_on(struct SHEET *key_win)
{
	if(key_win->flags2 == 0){
		change_wtitle8(key_win, 1);
	}
	sheet_refresh(key_win, 0, 0, key_win->bxsize, 25);
	if((key_win->flags & 0x20) != 0){
		fifo32_put(&key_win->task->fifo, 2);
	}
	return;
}

void keywin_off(struct SHEET *key_win)
{
	if(key_win->flags2 == 0){
		change_wtitle8(key_win, 0);
	}
	sheet_refresh(key_win, 0, 0, key_win->bxsize, 25);
	if((key_win->flags & 0x20) != 0){
		fifo32_put(&key_win->task->fifo, 3);
	}
	return;
}

struct SHEET *open_console(struct SHTCTL *shtctl, unsigned int memtotal)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct SHEET *sht;
	char *buf;
	sht = sheet_alloc(shtctl);
	buf = (unsigned char *) memman_alloc_4k(memman, 500 * 500);
	sheet_setbuf(sht, buf, 500, 500, -1);
	if(task_now()->langmode == 0){
		make_window8(buf, 500, 500, "Console", 0);
		sht->title = "Console";
	} else {
		make_window8(buf, 500, 500, "命令行窗口", 0);
		sht->title = "命令行窗口";
	}
	make_textbox8(sht, 8, 28, 484, 463, COL8_000000);
	sht->task = open_constask(sht, memtotal);
	sht->flags |= 0x20;
	return sht;
}

struct TASK *open_constask(struct SHEET *sht, unsigned int memtotal)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	int *fifobuf;
	struct TASK *task;
	fifobuf = (int *) memman_alloc_4k(memman, 128 * 4);
	task = task_alloc();
	task->stack = memman_alloc_4k(memman, 64 * 1024);
	task->tss.esp = task->stack + 64 * 1024 - 12;
	task->tss.eip = (int) &console_task;
	task->tss.es = 1 * 8;
	task->tss.cs = 2 * 8;
	task->tss.ss = 1 * 8;
	task->tss.ds = 1 * 8;
	task->tss.fs = 1 * 8;
	task->tss.gs = 1 * 8;
	*((int *) (task->tss.esp + 4)) = (int) sht;
	*((int *) (task->tss.esp + 8)) = memtotal;
	fifo32_init(&task->fifo, 128, fifobuf, task);
	task_run(task, 2, 2);
	return task;
}
	
void close_constask(struct TASK *task)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	task_sleep(task);
	memman_free_4k(memman, task->stack, 64 * 1024);
	memman_free_4k(memman, (int) task->fifo.buf, 128 * 4);
	task->flags = 0;
	return;
}

void close_console(struct SHEET *sht)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct TASK *task = sht->task;
	memman_free_4k(memman, (int)sht->buf, 500 * 500);
	sheet_free(sht);
	close_constask(task);
	return;
}
