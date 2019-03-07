#include "apilib.h" /* ������ɫ����Ϣ */
#include <stdio.h> /* sprintf�������ڵ�ͷ�ļ� */

/* �ṹ�嶨�� */
struct key { /* ����״̬ӳ�� */
	int right, left, space, esc;
};

/* �������� */
void wait(int i, int timer, struct key *key); 
void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen);
void putstr(int win, char *winbuf, int x, int y, int col, int strlen, char *s);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);

/* invader:"abcd", fighter:"efg", laser:"h" */

void NickStartup(void) /* ��ں���+��Ϸ��������� */
{
	int win, timer, i, mx, my, btn, langmode = api_getlang();
	int score = 0, high = 0, point, fx = 336 / 2 - 1, ix, iy;
	int movewait0, movewait, idir, j, p, invline, laserwait;
	int lx, ly, x, y, hit;
	char winbuf[336 * 261], s[17];
	struct key key; /* ��¼����״̬ */
	static char invstr0[32] = "abcd abcd abcd abcd abcd ";
	int map[5 * 6];
	
	/*
		fx���Ի���x���꣨fighter_x��
		lx,ly���������ڵ������꣨laser_x,laser_y��
		ix,iy��������Ⱥ�����꣨invaders_x,in-vaders_y��
		idir��������Ⱥ���ƶ�����invaders_direc-tion��
		laserwait���������ڵ���ʣ����ʱ��
		movewait�������������Ϊ0ʱ������Ⱥǰ��һ��
		movewait0��movewait�ĳ�ʼֵ������30ֻ���˺���٣�
		invline��������Ⱥ��������invaders_line��
		score����ǰ�÷�
		high����ߵ÷�
		point���÷ֵ�������������ĵ��ۣ�
		map:ÿ�������˵�״̬��Ϊ1������������˻����ţ�Ϊ0��������������Ѿ�������
	*/
	
	if(langmode != 1){ /* ��ʼ������ģʽ */
		api_setlang(1);
	}
	/* ��ʱ��׼�� */
	api_initmalloc();
	timer = api_alloctimer(); 
	api_inittimer(timer, 128);
	/* ����׼�� */
	win = api_openwin(winbuf, 336, 261, -1, "��������Ϸ", 0);
	api_txtbox(win, 8, 28, 320, 225, 0);
	
	 /* ������ʾ */
	api_putstrwin(win, 30, 30, 7, 36, "20XX�꣬��ңԶ��������һ�˷���һȺ");
	api_putstrwin(win, 30, 60, 7, 36, "���������ߣ����ǲ��ô�̫�շ��Ϳ���");
	api_putstrwin(win, 30, 90, 7, 38, "�����������档����Ҳû�з����������");
	api_putstrwin(win, 80, 120, 7, 20, "��һֱ��������Ư����");
	api_putstrwin(win, 30, 150, 7, 38, "��Ȼ��û�ж��⣬���������ϸ�Ⱦ��һ��");
	api_putstrwin(win, 30, 180, 7, 40, "�׶�Ĳ���������ĿƼ�ˮƽ���޷��Ը���");
	api_putstrwin(win, 30, 210, 7, 38, "������������������ཫ�ܿ��ܻ����");
	api_putstrwin(win, 90, 230, 3, 38, "���������������");
	i = api_getkey(1);
	api_txtbox(win, 8, 28, 320, 225, 0);
	api_putstrwin(win, 30, 30, 7, 36, "������������˵�����ǣ������Ǹ�����");
	api_putstrwin(win, 30, 60, 7, 38, "����ͨ��ʱ����ȣ� ֻ����Ϧ������");
	api_putstrwin(win, 30, 90, 7, 40, "����ֻ�ܳ������еĵ������ڣ�����");
	api_putstrwin(win, 80, 120, 7, 36, "����ͬ����һ��������ˣ�");
	make_button8(win, 120, 170, 200, 200, "��ʼ��Ϸ��", 0, 10);
	for(;;){
		mx = api_getmouse(win, 0); /* ȡ������x */
		my = api_getmouse(win, 1);/* ȡ������y */
		btn = api_getmouse(win, 2);/* ȡ�����İ�����״̬ */
		if(119 <= mx && mx <= 201 && 169 <= my && my <= 201 && btn == 1){
			make_button8(win, 120, 170, 200, 200, "��ʼ��Ϸ��", 1, 10);
			wait(20, timer, &key);
			make_button8(win, 120, 170, 200, 200, "��ʼ��Ϸ��", 0, 10);
			break;
		}
	}
	api_txtbox(win, 8, 28, 320, 225, 0);
	api_putstrwin(win, 180, 30, COL8_FFFFFF, 17, "��߷֣�00000000");
	high = 0; /* ��߷ֹ��� */
restart: /* ���¿�ʼ */
	lx = -1;
	ly = -1;
	point = 1;
	laserwait = 15;
	api_boxfilwin(win, 8, 48, 320, 250, 0);
	fx = 336 / 2 - 1;
	score = 0; /* ����ǰ�÷ֹ��� */
	putstr(win, winbuf, fx, 235, COL8_00FFFF, 3, "efg"); /* ��ʾ�������� */
	putstr(win, winbuf, 40, 30, COL8_000000, 15, "               ");
	api_putstrwin(win, 40, 30, COL8_FFFFFF, 15, "�÷֣�00000000");
	movewait0 = 20;
	wait(100, timer, &key); /* ��һ��� */
next_group: /* ��һ�������� */
	ix = 40;
	iy = 65;
	api_boxfilwin(win, 60, 80, 320, 220, 0);
	for(i = iy; i < iy + 16 * 6; i += 16){
		putstr(win, winbuf, 40, i, COL8_00FF00, 30, invstr0);
	}
	/* ��ʼ������ */
	key.left = 0;
	key.right = 0;
	key.space = 0;
	key.esc = 0;
	movewait = movewait0;
	idir = 1; /* ������ */
	invline = 6; /* ��ʼΪ���� */
	hit = 0;
	for(i = 0; i < 5 * 6; i++){
		map[i] = 1;
	}
	wait(100, timer, &key); /* ��һ��� */
	
	/* ��Ϸ��������� */
	for(;;){
		wait(4, timer, &key); /* �ȴ� */
		
		/* �����Ĵ��� */
		if(key.left != 0){ /* <- */
			if(fx >= 10){
				api_boxfilwin(win, fx, 235, fx + 3 * 8, 235 + 16, COL8_000000);
				fx -= 5;
				putstr(win, winbuf, fx, 235, COL8_00FFFF, 3, "efg");
			}
			key.left = 0;
		}
		if(key.right != 0){ /* -> */
			if(fx <= 300){
				api_boxfilwin(win, fx, 235, fx + 3 * 8, 235 + 16, COL8_000000);
				fx += 5;
				putstr(win, winbuf, fx, 235, COL8_00FFFF, 3, "efg");
			}
			key.right = 0;
		}
		if(key.space != 0){ /* �ո� */
			if(lx < 0 && ly < 0 && laserwait == 0){
				lx = fx + 8;
				ly = 235 - 17;
				laserwait = 5;
			}
			key.space = 0;
		}	
		if(key.esc != 0){ /* Esc */
			api_boxfilwin(win, 7, 48, 325, 250, 0);
			api_putstrwin(win, 100, 60, COL8_FFFFFF, 20, "ȷ��Ҫ�رմ˳�����");
			make_button8(win, 100, 120, 140, 150, "ȷ��", 0, 4);
			make_button8(win, 200, 120, 240, 150, "ȡ��", 0, 4);
			for(;;){
				mx = api_getmouse(win, 0);
				my = api_getmouse(win, 1);
				btn = api_getmouse(win, 2);
				if(99 <= mx && mx <= 141 && 119 <= my && my <= 151 && btn == 1){
					make_button8(win, 100, 120, 140, 150, "ȷ��", 1, 4);
					wait(20, timer, &key);
					make_button8(win, 100, 120, 140, 150, "ȷ��", 0, 4);
					api_end(); /* �˳� */
				} else if(199 <= mx && mx <= 241 && 120 <= my && my <= 150 && btn == 1){
					make_button8(win, 200, 120, 240, 150, "ȡ��", 1, 4);
					wait(20, timer, &key);
					make_button8(win, 200, 120, 240, 150, "ȡ��", 0, 4);
					api_boxfilwin(win, 7, 48, 325, 250, 0);
					putstr(win, winbuf, fx, 235, COL8_00FFFF, 3, "efg");
					key.esc = 0;
					break;
				} 
			}
		}
		if(laserwait != 0){ /* �ȴ��������ڳ�� */
			laserwait--;
			key.space = 0; /* ��ʱ���ո�û���� */
		}
		/* �ڵ��Ĵ��� */
		if(lx >= 0 && ly >= 0){ 
			putstr(win, winbuf, lx, ly, 3, 1, " ");
			ly -= 15;
			putstr(win, winbuf, lx, ly, 3, 1, "h");
			if(ly <= 56){ /* ������ */
				api_boxfilwin(win, lx, ly, lx + 8, ly + 16, COL8_000000);
				hit = 0;
				lx = -1;
				ly = -1;
			}
			if(ly <= iy + invline * 16 && lx - ix >= 0 && lx - ix <= 200){
				x = lx - ix;
				y = ly - iy;

				x /= 5 * 8;
				y /= 16;
				if(map[x + y * 5] != 0){
					/* hit! */
					hit = 1;
					score += point;
					point++;
					api_boxfilwin(win, 40, 30, 40 + 17 * 8, 30 + 16, COL8_000000);
					sprintf(s, "�÷֣�%08d", score); /* ��ǰ�÷���ʾ */
					api_putstrwin(win, 40, 30, COL8_FFFFFF, 17, s);
					if(high < score){
						high = score; /* ������߷� */
						api_boxfilwin(win, 180, 30, 180 + 17 * 8, 30 + 16, COL8_000000);
						sprintf(s, "��߷֣�%08d", high); /* ��ߵ÷���ʾ */
						api_putstrwin(win, 180, 30, COL8_FFFFFF, 17, s);
					}
					map[x + y * 5] = 0;
					api_boxfilwin(win, lx, ly, lx + 8, ly + 16, COL8_000000);
					lx = -1;
					ly = -1;
					if((y + 1) == invline){ /* ���б��� */
						if(map[y * 5] == 0 && map[y * 5 + 1] == 0 && map[y * 5 + 2] == 0 && map[y * 5 + 3] == 0 
						&& map[y * 5 + 4] == 0){
							invline--; /* �����˵�����-- */
						}
					}
				}
			}
			for(i = 0; i < 5 * invline;){
				for(j = 0; j < invline; j++){
					for(p = 0; p < 5; p++){
						if(map[i] != 0){
							putstr(win, winbuf, ix + p * 40, iy + j * 16, COL8_00FF00, 5, "abcd ");
						} else {
							putstr(win, winbuf, ix + p * 40, iy + j * 16, COL8_00FF00, 5, "     ");
						}
						i++;
					}
				}
			}
			for(i = 0; i < 5 * 6; i++){ /* �����һ��������ȫ���������� */
				if(map[i] != 0){
					break;
				}
			}
			if(i == 30){ /* ��һ��������ȫ���������� */
				movewait0 -= movewait0 / 3; /* �����Ѷ� */
				goto next_group; /* ��һ�� */
			}
			if(lx >= 0 && ly >= 0){ /* ��ʾ */
				putstr(win, winbuf, lx, ly, 3, 1, "h");
			}
		}
		/* �������ƶ� */
		if(movewait != 0){
			movewait--;
		} else {
			movewait = movewait0;
			if(idir > 0){ /* �����ƶ� */
				ix += 10; 
				if(ix >= 110){ 
					idir = -idir; /* ȡ�� */
				}
			} else { /* �����ƶ� */
				ix -= 10;
				if(ix <= 20){ 
					idir = -idir; /* ȡ�� */
					iy += 16; /* ������һ�� */
					api_boxfilwin(win, ix - 2, iy - 16, ix + 200 + 10, (iy - 16) + 16, COL8_000000);
				}
			}
			for(i = 0; i < 5 * invline;){
				for(j = 0; j < invline; j++){
					for(p = 0; p < 5; p++){
						if(map[i] != 0){
							putstr(win, winbuf, ix + p * 40, iy + j * 16, COL8_00FF00, 5, "abcd ");
						} else {
							putstr(win, winbuf, ix + p * 40, iy + j * 16, COL8_00FF00, 5, "     ");
						}
						i++;
					}
				}
			}
			api_boxfilwin(win, ix - 10, iy, ix - 1, iy + 16 * invline, COL8_000000);
			if(iy + invline * 16 >= 235){ /* ����������һ�� */
					/* ��Ϸ������*/
				api_boxfilwin(win, 7, 48, 325, 250, 0);
				api_putstrwin(win, 140, 140, COL8_FF0000, 10, "��Ϸ������");
				make_button8(win, 50, 190, 90, 220, "�˳�", 0, 10);
				make_button8(win, 216, 190, 286, 220, "���¿�ʼ", 0, 10);
				for(;;){
					mx = api_getmouse(win, 0);
					my = api_getmouse(win, 1);
					btn = api_getmouse(win, 2);
					if(49 <= mx && mx <= 91 && 189 <= my && my <= 221 && btn == 1){
						make_button8(win, 50, 190, 90, 220, "�˳�", 1, 10);
						wait(20, timer, &key);
						make_button8(win, 50, 190, 90, 220, "�˳�", 0, 10);
						api_end(); /* �˳� */
					} else if(215 <= mx && mx <= 287 && 189 <= my && my <= 221 && btn == 1){
						make_button8(win, 216, 190, 286, 220, "���¿�ʼ", 1, 10);
						wait(20, timer, &key);
						make_button8(win, 216, 190, 286, 220, "���¿�ʼ", 0, 10);
						break;
					}
				}
				goto restart; /* ���¿�ʼ */
			}
		}
	}
}

void wait(int i, int timer, struct key *key)
{
	int j;
	if (i > 0) {
		api_settimer(timer, i); /* �ȴ�һ��ʱ�� */
		i = 128;
	} else {
		i = 0x0a; /* Enter */
	}
	for (;;) {
		j = api_getkey(1);
		if (i == j) {
			break;
		}
		if (j == 251){ /* Esc */
			key->esc = 1;
		}
		if (j == 253) { /* <- */
			key->left = 1;
		}
		if (j == 252) { /* -> */
			key->right = 1;
		}
		if (j == ' ') { /* �ո� */
			key->space = 1;
		}
	}
	return;
}

void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen) /* ���ư�ť */
{
	if(act == 0){
		api_boxfilwin(win, x0 - 1, y0 - 1, x1 + 1, y1 + 1, COL8_848484);
		api_boxfilwin(win, x0 - 1, y0 - 1, x1, y1, COL8_FFFFFF);
		api_boxfilwin(win, x0, y0, x1, y1, COL8_C6C6C6);
	} else {
		api_boxfilwin(win, x0 - 1, y0 - 1, x1, y1, COL8_848484);
		api_boxfilwin(win, x0, y0, x1 + 1, y1 + 1, COL8_FFFFFF);
		api_boxfilwin(win, x0, y0, x1, y1, COL8_C6C6C6);
	}
	api_putstrwin(win, x0 + 5, y0 + 10, 0, strlen, s);
	return;
}

void putstr(int win/* ���ھ�� */, char *winbuf/* ���ڻ����� */, int x, int y, int col, int strlen, char *s) /* ������ʾ������ɫ */
{
	extern char invader[4096]; /*invaderר���ֿ� */
	int x0 = x; /* ��¼��ʼ��x */
	api_boxfilwin(win, x, y, x + strlen * 8 + 1, y + 16, COL8_000000); /* Ϳ�ϱ���ɫ */
	for(; *s != 0x00; s++){
		putfont8(winbuf, 336, x, y, col, invader + *s * 16); /* ������� */
		x += 8;
	}
	api_refreshwin(win, x0, y, x + 8 + 10, y + 16); /* ˢ�� */
	return;
}
	
void putfont8(char *vram, int xsize, int x, int y, char c, char *font) /* ���ں��еĸú�����ͬ */
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}
