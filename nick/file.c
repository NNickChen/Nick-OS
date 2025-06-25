#include "bootpack.h"

void file_readfat(int *fat, unsigned char *img)
{
	int i, j = 0;
	for (i = 0; i < 2880; i += 2) {
		fat[i + 0] = (img[j + 0]      | img[j + 1] << 8) & 0xfff;
		fat[i + 1] = (img[j + 1] >> 4 | img[j + 2] << 4) & 0xfff;
		j += 3;
	}
	return;
}

void file_load(int clustno, int size, char *buf, int *fat, char *img)
{
	int i;
	for(;;){
		if(size <= 512){
			for(i = 0; i < size; i ++){
				buf[i] = img[clustno * 512 + i];
			}
			break;
		}
		for(i = 0; i < 512; i ++){
			buf[i] = img[clustno * 512 + i];
		}
		size -= 512;
		buf += 512;
		clustno = fat[clustno];
	}
	return;
}

char *file_load2(int clustno, int *size, int *fat)
{
	int size1 = *size, size2;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	char *buf, *buf2;
	buf = (char *)memman_alloc_4k(memman, size1);
	file_load(clustno, size1, buf, fat, (char *) ADR_DISKIMG + 0x003e00);
	if(size1 >= 17){
		size2 = tek_getsize(buf);
		if(size2 > 0){
			buf2 = (char *)memman_alloc_4k(memman, size2);
			tek_decomp(buf, buf2, size2);
			memman_free_4k(memman, (int) buf, size1);
			buf = buf2;
			*size = size2;
		}
	}
	return buf;
}
			
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max)
{
	char s[30];
	int x, y;
	
	for (y = 0; y < 11; y ++){
		s[y] = ' ';
	}
	y = 0;
	for(x = 0; name[x] != 0; x ++){
		if(y >= 11){return 0;}
		if(name[x] == '.' && y <= 8){
			y = 8;
		} else {
			s[y] = name[x];
			if('a' <= s[y] && s[y] <= 'z'){
				s[y] -= 0x20;
			}
			y ++;
		}
	}
	for(x = 0; x < max; ){
		if(finfo[x].name[0] == 0x00){
			break;
		}
		if((finfo[x].type & 0x18) == 0){
			for(y = 0; y < 11; y ++){
				if(finfo[x].name[y] != s[y]){
					goto next;
				}				
			}
			return finfo + x;
		}
next:					
		x ++;
	}
	return 0;
}
