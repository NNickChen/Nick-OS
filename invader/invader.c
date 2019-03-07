#include "apilib.h" /* 包含了色号信息 */
#include <stdio.h> /* sprintf函数所在的头文件 */

/* 结构体定义 */
struct key { /* 键盘状态映射 */
	int right, left, space, esc;
};

/* 函数声明 */
void wait(int i, int timer, struct key *key); 
void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen);
void putstr(int win, char *winbuf, int x, int y, int col, int strlen, char *s);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);

/* invader:"abcd", fighter:"efg", laser:"h" */

void NickStartup(void) /* 入口函数+游戏处理的主题 */
{
	int win, timer, i, mx, my, btn, langmode = api_getlang();
	int score = 0, high = 0, point, fx = 336 / 2 - 1, ix, iy;
	int movewait0, movewait, idir, j, p, invline, laserwait;
	int lx, ly, x, y, hit;
	char winbuf[336 * 261], s[17];
	struct key key; /* 记录键盘状态 */
	static char invstr0[32] = "abcd abcd abcd abcd abcd ";
	int map[5 * 6];
	
	/*
		fx：自机的x坐标（fighter_x）
		lx,ly：等离子炮弹的坐标（laser_x,laser_y）
		ix,iy：外星人群的坐标（invaders_x,in-vaders_y）
		idir：外星人群的移动方向（invaders_direc-tion）
		laserwait：等离子炮弹的剩余充电时间
		movewait：当这个变量变为0时外星人群前进一步
		movewait0：movewait的初始值（消灭30只敌人后减少）
		invline：外星人群的行数（invaders_line）
		score：当前得分
		high：最高得分
		point：得分的增加量（奖金的单价）
		map:每个外星人的状态（为1代表这个外星人还活着，为0代表这个外星人已经牺牲了
	*/
	
	if(langmode != 1){ /* 初始化语言模式 */
		api_setlang(1);
	}
	/* 定时器准备 */
	api_initmalloc();
	timer = api_alloctimer(); 
	api_inittimer(timer, 128);
	/* 窗口准备 */
	win = api_openwin(winbuf, 336, 261, -1, "外星人游戏", 0);
	api_txtbox(win, 8, 28, 320, 225, 0);
	
	 /* 剧情显示 */
	api_putstrwin(win, 30, 30, 7, 36, "20XX年，从遥远的宇宙另一端飞来一群");
	api_putstrwin(win, 30, 60, 7, 36, "外星侵略者，他们不用穿太空服就可以");
	api_putstrwin(win, 30, 90, 7, 38, "在宇宙中生存。他们也没有发达的文明，");
	api_putstrwin(win, 80, 120, 7, 20, "就一直在宇宙中漂泊。");
	api_putstrwin(win, 30, 150, 7, 38, "虽然并没有恶意，但他们身上感染了一种");
	api_putstrwin(win, 30, 180, 7, 40, "凶恶的病毒，人类的科技水平还无法对付。");
	api_putstrwin(win, 30, 210, 7, 38, "如果他们来到地球，人类将很可能会毁灭");
	api_putstrwin(win, 90, 230, 3, 38, "按任意键继续……");
	i = api_getkey(1);
	api_txtbox(win, 8, 28, 320, 225, 0);
	api_putstrwin(win, 30, 30, 7, 36, "人类曾经尝试说服他们，但他们根本无");
	api_putstrwin(win, 30, 60, 7, 38, "法沟通。时间紧迫， 只争朝夕。看来");
	api_putstrwin(win, 30, 90, 7, 40, "现在只能抄起手中的等离子炮，将他");
	api_putstrwin(win, 80, 120, 7, 36, "们连同病毒一起消灭掉了！");
	make_button8(win, 120, 170, 200, 200, "开始游戏！", 0, 10);
	for(;;){
		mx = api_getmouse(win, 0); /* 取得鼠标的x */
		my = api_getmouse(win, 1);/* 取得鼠标的y */
		btn = api_getmouse(win, 2);/* 取得鼠标的按键的状态 */
		if(119 <= mx && mx <= 201 && 169 <= my && my <= 201 && btn == 1){
			make_button8(win, 120, 170, 200, 200, "开始游戏！", 1, 10);
			wait(20, timer, &key);
			make_button8(win, 120, 170, 200, 200, "开始游戏！", 0, 10);
			break;
		}
	}
	api_txtbox(win, 8, 28, 320, 225, 0);
	api_putstrwin(win, 180, 30, COL8_FFFFFF, 17, "最高分：00000000");
	high = 0; /* 最高分归零 */
restart: /* 重新开始 */
	lx = -1;
	ly = -1;
	point = 1;
	laserwait = 15;
	api_boxfilwin(win, 8, 48, 320, 250, 0);
	fx = 336 / 2 - 1;
	score = 0; /* 将当前得分归零 */
	putstr(win, winbuf, fx, 235, COL8_00FFFF, 3, "efg"); /* 显示等离子炮 */
	putstr(win, winbuf, 40, 30, COL8_000000, 15, "               ");
	api_putstrwin(win, 40, 30, COL8_FFFFFF, 15, "得分：00000000");
	movewait0 = 20;
	wait(100, timer, &key); /* 等一会儿 */
next_group: /* 下一波外星人 */
	ix = 40;
	iy = 65;
	api_boxfilwin(win, 60, 80, 320, 220, 0);
	for(i = iy; i < iy + 16 * 6; i += 16){
		putstr(win, winbuf, 40, i, COL8_00FF00, 30, invstr0);
	}
	/* 初始化变量 */
	key.left = 0;
	key.right = 0;
	key.space = 0;
	key.esc = 0;
	movewait = movewait0;
	idir = 1; /* 先往左 */
	invline = 6; /* 初始为六行 */
	hit = 0;
	for(i = 0; i < 5 * 6; i++){
		map[i] = 1;
	}
	wait(100, timer, &key); /* 等一会儿 */
	
	/* 游戏处理的主体 */
	for(;;){
		wait(4, timer, &key); /* 等待 */
		
		/* 按键的处理 */
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
		if(key.space != 0){ /* 空格 */
			if(lx < 0 && ly < 0 && laserwait == 0){
				lx = fx + 8;
				ly = 235 - 17;
				laserwait = 5;
			}
			key.space = 0;
		}	
		if(key.esc != 0){ /* Esc */
			api_boxfilwin(win, 7, 48, 325, 250, 0);
			api_putstrwin(win, 100, 60, COL8_FFFFFF, 20, "确定要关闭此程序吗");
			make_button8(win, 100, 120, 140, 150, "确定", 0, 4);
			make_button8(win, 200, 120, 240, 150, "取消", 0, 4);
			for(;;){
				mx = api_getmouse(win, 0);
				my = api_getmouse(win, 1);
				btn = api_getmouse(win, 2);
				if(99 <= mx && mx <= 141 && 119 <= my && my <= 151 && btn == 1){
					make_button8(win, 100, 120, 140, 150, "确定", 1, 4);
					wait(20, timer, &key);
					make_button8(win, 100, 120, 140, 150, "确定", 0, 4);
					api_end(); /* 退出 */
				} else if(199 <= mx && mx <= 241 && 120 <= my && my <= 150 && btn == 1){
					make_button8(win, 200, 120, 240, 150, "取消", 1, 4);
					wait(20, timer, &key);
					make_button8(win, 200, 120, 240, 150, "取消", 0, 4);
					api_boxfilwin(win, 7, 48, 325, 250, 0);
					putstr(win, winbuf, fx, 235, COL8_00FFFF, 3, "efg");
					key.esc = 0;
					break;
				} 
			}
		}
		if(laserwait != 0){ /* 等待等离子炮充电 */
			laserwait--;
			key.space = 0; /* 此时按空格没有用 */
		}
		/* 炮弹的处理 */
		if(lx >= 0 && ly >= 0){ 
			putstr(win, winbuf, lx, ly, 3, 1, " ");
			ly -= 15;
			putstr(win, winbuf, lx, ly, 3, 1, "h");
			if(ly <= 56){ /* 到顶了 */
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
					sprintf(s, "得分：%08d", score); /* 当前得分显示 */
					api_putstrwin(win, 40, 30, COL8_FFFFFF, 17, s);
					if(high < score){
						high = score; /* 更新最高分 */
						api_boxfilwin(win, 180, 30, 180 + 17 * 8, 30 + 16, COL8_000000);
						sprintf(s, "最高分：%08d", high); /* 最高得分显示 */
						api_putstrwin(win, 180, 30, COL8_FFFFFF, 17, s);
					}
					map[x + y * 5] = 0;
					api_boxfilwin(win, lx, ly, lx + 8, ly + 16, COL8_000000);
					lx = -1;
					ly = -1;
					if((y + 1) == invline){ /* 整行被灭 */
						if(map[y * 5] == 0 && map[y * 5 + 1] == 0 && map[y * 5 + 2] == 0 && map[y * 5 + 3] == 0 
						&& map[y * 5 + 4] == 0){
							invline--; /* 外星人的行数-- */
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
			for(i = 0; i < 5 * 6; i++){ /* 检查这一波外星人全部被消灭了 */
				if(map[i] != 0){
					break;
				}
			}
			if(i == 30){ /* 这一波外星人全部被消灭了 */
				movewait0 -= movewait0 / 3; /* 增加难度 */
				goto next_group; /* 下一波 */
			}
			if(lx >= 0 && ly >= 0){ /* 显示 */
				putstr(win, winbuf, lx, ly, 3, 1, "h");
			}
		}
		/* 外星人移动 */
		if(movewait != 0){
			movewait--;
		} else {
			movewait = movewait0;
			if(idir > 0){ /* 向右移动 */
				ix += 10; 
				if(ix >= 110){ 
					idir = -idir; /* 取反 */
				}
			} else { /* 向左移动 */
				ix -= 10;
				if(ix <= 20){ 
					idir = -idir; /* 取反 */
					iy += 16; /* 进入下一行 */
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
			if(iy + invline * 16 >= 235){ /* 到达最下面一行 */
					/* 游戏结束！*/
				api_boxfilwin(win, 7, 48, 325, 250, 0);
				api_putstrwin(win, 140, 140, COL8_FF0000, 10, "游戏结束！");
				make_button8(win, 50, 190, 90, 220, "退出", 0, 10);
				make_button8(win, 216, 190, 286, 220, "重新开始", 0, 10);
				for(;;){
					mx = api_getmouse(win, 0);
					my = api_getmouse(win, 1);
					btn = api_getmouse(win, 2);
					if(49 <= mx && mx <= 91 && 189 <= my && my <= 221 && btn == 1){
						make_button8(win, 50, 190, 90, 220, "退出", 1, 10);
						wait(20, timer, &key);
						make_button8(win, 50, 190, 90, 220, "退出", 0, 10);
						api_end(); /* 退出 */
					} else if(215 <= mx && mx <= 287 && 189 <= my && my <= 221 && btn == 1){
						make_button8(win, 216, 190, 286, 220, "重新开始", 1, 10);
						wait(20, timer, &key);
						make_button8(win, 216, 190, 286, 220, "重新开始", 0, 10);
						break;
					}
				}
				goto restart; /* 重新开始 */
			}
		}
	}
}

void wait(int i, int timer, struct key *key)
{
	int j;
	if (i > 0) {
		api_settimer(timer, i); /* 等待一段时间 */
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
		if (j == ' ') { /* 空格 */
			key->space = 1;
		}
	}
	return;
}

void make_button8(int win, int x0, int y0, int x1, int y1, char *s, int act, int strlen) /* 绘制按钮 */
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

void putstr(int win/* 窗口句柄 */, char *winbuf/* 窗口缓冲区 */, int x, int y, int col, int strlen, char *s) /* 用于显示各个角色 */
{
	extern char invader[4096]; /*invader专用字库 */
	int x0 = x; /* 记录起始的x */
	api_boxfilwin(win, x, y, x + strlen * 8 + 1, y + 16, COL8_000000); /* 涂上背景色 */
	for(; *s != 0x00; s++){
		putfont8(winbuf, 336, x, y, col, invader + *s * 16); /* 描绘字体 */
		x += 8;
	}
	api_refreshwin(win, x0, y, x + 8 + 10, y + 16); /* 刷新 */
	return;
}
	
void putfont8(char *vram, int xsize, int x, int y, char c, char *font) /* 和内核中的该函数相同 */
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
