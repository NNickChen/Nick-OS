#include "bootpack.h"

int *api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
	struct TASK *task = task_now();
	struct FIFO32 *sys_fifo = (struct FIFO32 *) *((int *) 0xfec);
	int ds_base = task->ds_base, i;
	struct CONSOLE *cons = task->cons;
	struct SHTCTL *shtctl = (struct SHTCTL *) *((int *) 0xfe4);
	struct SHEET *sht;
	struct MOUSE *mouse = task->mouse;
	struct FILEINFO *finfo;
	struct FILEHANDLE *fh;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	mouse->phase = 0;
	int x, y, btn;
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
		if(ebp != 0){
			sht->flags2 = 1;
			boxfill8(sht->buf, sht->bxsize, COL8_C6C6C6, 3, 3, sht->bxsize - 3, 20);
		}
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
		sht = (struct SHEET *) ebx;
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
			if(i >= 256){
				reg[7] = i - 256;
				return 0;
			}
		}
	} else if(edx == 17){
		sht = (struct SHEET *) ebx;
		x = *((int *) 0xfdc);
		x -= sht->vx0;
		if(x >= 0 && x <= sht->bxsize){
			reg[7] = x;
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
			beep_off();
		} else {
			beep_settone(eax);
			beep_on();
		}
	} else if(edx == 23){
		for(i = 0; i < 8; i++){
			if(task->fhandle[i].buf == 0){
				break;
			}
		}
		fh = &task->fhandle[i];
		reg[7] = 0;
		if(i < 8){
			finfo = file_search((char *) (ebx + ds_base), (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
			if(finfo != 0){
				reg[7] = (int) fh;
				fh->size = finfo->size;
				fh->pos = 0;
				fh->buf = file_load2(finfo->clustno, &fh->size, task->fat);
			}
		}
	} else if(edx == 24){
		fh = (struct FILEHANDLE *) eax;
		memman_free_4k(memman, (int) fh->buf, fh->size);
		fh->buf = 0;
	} else if(edx == 25){
		fh = (struct FILEHANDLE *) eax;
		if(ecx == 0){
			fh->pos = ebx;
		} else if(ecx == 1){
			fh->pos += ebx;
		} else if(ecx == 2){
			fh->pos = fh->size + ebx;
		}
		if(fh->pos < 0){
			fh->pos = 0;
		}
		if(fh->pos > fh->size){
			fh->pos = fh->size;
		}
	} else if(edx == 26){
		fh = (struct FILEHANDLE *) eax;
		if(ecx == 0){
			reg[7] = fh->size;
		} else if(ecx == 1){
			reg[7] = fh->pos;
		} else if(ecx == 2){
			reg[7] = fh->size - fh->pos;
		}
	} else if(edx == 27){
		fh = (struct FILEHANDLE *) eax;
		for(i = 0; i < ecx; i++){
			if(fh->pos == fh->size){
				break;
			}
			*((char *)ebx + ds_base + i) = fh->buf[fh->pos];
			fh->pos++;
		}
		reg[7] = i;
	} else if(edx == 28){
		for(i = 0; i < ecx; i++){
			*((char *) ebx + ds_base + i) = task->cmdline[i];
			if(task->cmdline[i] == 0){
				break;
			}
		}
		reg[7] = i;
	} else if(edx == 29){
		reg[7] = task->langmode;
	} else if(edx == 30){
		task->langmode = eax;
		if(eax == 1){
			if(cons->sht != 0){
				cons_putstr0(cons, "switch(key):\n中文EUC模式。\n\n");
				cons_putchar(cons, '>', 1);
			}
		} else {
			if(cons->sht != 0){
				cons_putstr0(cons, "switch(key):\nEnglish ASCII mode.\n\n");
				cons_putchar(cons, '>', 1);
			}
		}
	} else if(edx == 31){
		sht = (struct SHEET *) ebx;
		y = *((int *) 0xfd8);
		y -= sht->vy0;
		if(y >= 0 && y <= sht->bysize){
			reg[7] = y;
		}
	} else if(edx == 32){
		btn = *((int *) 0xfd4);
		if((btn & 0x01) != 0){
			reg[7] = 1;
		} else if((btn & 0x02) != 0){
			reg[7] = 2;
		} else if((btn & 0x04) != 0){
			reg[7] = 3;
		} else {
			reg[7] = -1;
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
