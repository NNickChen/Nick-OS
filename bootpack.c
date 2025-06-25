/* bootpack */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>
#define MAX_SHTINV 5
void keywin_on(struct SHEET *key_win);
void keywin_off(struct SHEET *key_win);

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct FIFO32 fifo, keycmd;
	struct SHTCTL *shtctl;
	char s[40];
	int fifobuf[128], keycmd_buf[32];
	int mx, my, i, x, y, j, mmx = -1, mmy = -1, show = 0;
	unsigned int memtotal;
	struct MOUSE_DEC mdec;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	static char keytable[0x54] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0x0a,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 0, '`',   0,   0, 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   0, 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,  0 ,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};
	static char keytable1[0x54] = {
		0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', 0, '~', 0, 0, 'Z', 'X', 'C', 'V', 
		'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0,   0,   0,   0,   0,   0,
		0,  0 ,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};
	unsigned char *buf_back, buf_mouse[256], *buf_cons[2];
	struct SHEET *sht_back, *sht_mouse, *sht_cons[2], *sht = 0, *key_win, *shttable[MAX_SHTINV];
	struct TASK *task_a, *task_cons[2], *task_time, *task;
	int key_shift = 0, key_leds = (binfo->leds >> 4) & 7, keycmd_wait = -1, invs = 0, key_ctl = 0;
	fifo32_init(&keycmd, 32, keycmd_buf, 0);

	init_gdtidt();
	init_pic();
	io_sti(); 
	fifo32_init(&fifo, 128, fifobuf, 0);
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
	for(i = 0; i < 2; i++){
		sht_cons[i] = sheet_alloc(shtctl);
		buf_cons[i] = (unsigned char *) memman_alloc_4k(memman, 500 * 500);
		sheet_setbuf(sht_cons[i], buf_cons[i], 500, 500, -1);
		make_window8(buf_cons[i], 500, 500, "console", 0);
		make_textbox8(sht_cons[i], 8, 28, 484, 463, COL8_000000);
		task_cons[i] = task_alloc();
		task_cons[i]->tss.esp = memman_alloc_4k(memman, 70 * 1024) + 70 * 1024 - 12;
		task_cons[i]->tss.eip = (int) &console_task;
		task_cons[i]->tss.es = 1 * 8;
		task_cons[i]->tss.cs = 2 * 8;
		task_cons[i]->tss.ss = 1 * 8;
		task_cons[i]->tss.ds = 1 * 8;
		task_cons[i]->tss.fs = 1 * 8;
		task_cons[i]->tss.gs = 1 * 8;
		*((int *) (task_cons[i]->tss.esp + 4)) = (int) sht_cons[i];
		*((int *) (task_cons[i]->tss.esp + 8)) = memtotal;
		task_run(task_cons[i], 1, 2); /* level=2, priority=2 */
		sht_cons[i]->task = task_cons[i];
		sht_cons[i]->flags |= 0x20;
		sht_cons[i]->title = "console";
	}


	/* sht_mouse */
	sht_mouse = sheet_alloc(shtctl);
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
	init_mouse_cursor8(buf_mouse, 99);
	mx = (binfo->scrnx - 16) / 2; 
	my = (binfo->scrny - 28 - 16) / 2;

	sheet_slide(sht_back,  0,  0);
	sheet_slide(sht_cons[0], 8,  2);
	sheet_slide(sht_cons[1], 56, 6);
	sheet_slide(sht_mouse, mx, my);
	sheet_updown(sht_back,  0);
	sheet_updown(sht_cons[1], 1);
	sheet_updown(sht_cons[0], 2);
	sheet_updown(sht_mouse, 3);
	key_win = sht_cons[0];
	keywin_on(key_win);
	
	fifo32_put(&keycmd, 0xed);
	fifo32_put(&keycmd, key_leds);
	
	for(i = 0; i < 2; i++){
		task_run(task_cons[i], 2, 2); /* level=2, priority=2 */
	}

	for (;;) {
		if(fifo32_status(&keycmd) > 0 && keycmd_wait < 0){
			keycmd_wait = fifo32_get(&keycmd);
			wait_KBC_sendready();
			io_out8(PORT_KEYDAT,keycmd_wait);
		}
		io_cli();
		if (fifo32_status(&fifo) == 0) {
			task_sleep(task_a);
			io_sti();
		} else {
			i = fifo32_get(&fifo);
			io_sti();
			if (key_win->flags == 0){
				key_win = shtctl->sheets[shtctl->top - 1];
				keywin_on(key_win);
			}
			if (256 <= i && i <= 511) { 
				if (show != 0){
					sprintf(s, "%02X", i - 256);
					putfonts8_asc_sht(sht_back, 0, 16, COL8_FFFFFF, get_bc(buf_back), s, 2);
				}
				if (i < 0x54 + 256) { 
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
				if(s[0] != 0){
					fifo32_put(&key_win->task->fifo, s[0] + 256);
				}					   
				if (i == 256 + 0x48){	/* Œüãû */
					fifo32_put(&key_win->task->fifo, 256 + 255);
				}
				if (i == 256 + 0x50){	/* Œü‰ºû */
					fifo32_put(&key_win->task->fifo, 256 + 254);
				}
				if (i == 256 + 0x4b){	/* Œü¶û */
					fifo32_put(&key_win->task->fifo, 256 + 253);
				}
				if (i == 256 + 0x4d){	/* Œü‰Eû */
					fifo32_put(&key_win->task->fifo, 256 + 252);
				}
				if (i == 256 + 0x01){
					fifo32_put(&key_win->task->fifo, 256 + 251);
				}
				if (i == 256 + 0x0f) { /* Tab */
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
					keywin_off(key_win);
					key_win = shtctl->sheets[shtctl->top - 1];
					keywin_on(key_win);
				}
				if (i == 256 + 0x3b && key_shift != 0){
					if(show == 0){
						show = 1;
					} else if(show != 0){
						boxfill8(buf_back, sht_back->bxsize, get_bc(buf_back), 0, 0, 150, 60);
						sheet_refresh(sht_back, 0, 0, 150, 60);
						show = 0;
					}
				}
				for(j = 0; j <= MAX_SHTINV; j ++){
					if (i == 256 + j + 0x01 && key_ctl != 0){
						if(shttable[j] != 0){
							sheet_updown(shttable[j], shtctl->top - 1);
							invs--;
							shttable[j] = 0;
							putfonts8_asc_sht(sht_back, 130 * j, sht_back->bysize - 20, COL8_000000, COL8_C6C6C6, "               ", 15);
						}
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
					sheet_slide(sht_mouse, mx, my);
					if ((mdec.btn & 0x01) != 0) {
						if(mmx < 0){
							for(j = shtctl->top - 1; j >= 0; j--){
								sht = shtctl->sheets[j];
								x = mx - sht->vx0;
								y = my - sht->vy0;
								if(0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize && j > 0){
									if(sht != key_win){
										sheet_updown(sht, shtctl->top - 1);
										keywin_off(key_win);
										key_win = sht;
										keywin_on(key_win);
									}
									if(y < 20){
										mmx = mx;
										mmy = my;
										if(sht->bxsize - 21 <= x && x <= sht->bxsize - 5 && 5 <= y && y<= 19){
											if((sht->flags & 0x10) != 0){
												task = sht->task;
												io_cli();
												task->tss.eax = (int) &(task->tss.esp0);
												task->tss.eip = (int) end_app;
												io_sti();
											}
										}
										if(sht->bxsize - 38 <= x && x <= sht->bxsize - 24 && 5 <= y && y <= 19){
											if(invs < MAX_SHTINV){
												for(j = 1; j <= MAX_SHTINV; j++){
													if(shttable[j] == 0){
														putfonts8_asc_sht(sht_back, 130 * j, sht_back->bysize - 20, COL8_000000, COL8_848484, sht->title, 15);
														shttable[j] = sht;
														invs ++;
														sheet_updown(sht, -1);
														break;
													}
												}
											}
										}
									}
									break;
								} 
							}
						} else {
							x = mx - mmx;
							y = my - mmy;
							sheet_slide(sht, sht->vx0 + x, sht->vy0 + y);
							mmx = mx;
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
					}
				}
			} 
		}
	}
}


void keywin_on(struct SHEET *key_win)
{
	change_wtitle8(key_win, 1);
	if((key_win->flags & 0x20) != 0){
		fifo32_put(&key_win->task->fifo, 2);
	}
	return;
}

void keywin_off(struct SHEET *key_win)
{
	change_wtitle8(key_win, 0);
	if((key_win->flags & 0x20) != 0){
		fifo32_put(&key_win->task->fifo, 3);
	}
	return;
}
