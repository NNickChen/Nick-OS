#include "bootpack.h"
#include <stdio.h>

unsigned char read_cmos (unsigned char p)
{
	unsigned char data;
	io_out8(cmos_index, p);
	data = io_in8(cmos_data);
	io_out8(cmos_index, 0x80);
	return data;
}
unsigned int get_hour_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_HOUR));
}
unsigned int get_min_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_MIN));
}
unsigned int get_sec_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_SEC));
}
unsigned int get_day_of_month()
{
	return BCD_HEX(read_cmos(CMOS_MON_DAY));
}
unsigned int get_day_of_week()
{
	return BCD_HEX(read_cmos(CMOS_WEEK_DAY));
}
unsigned int get_mon_hex()
{
	return BCD_HEX(read_cmos(CMOS_CUR_MON));
}
unsigned int get_year()
{
	return (BCD_HEX(read_cmos(CMOS_CUR_CEN))*100)+BCD_HEX(read_cmos(CMOS_CUR_YEAR))-30 + 2010;
}

void get_time_all(struct TIME *time)
{
	time->year = get_year();
	time->mon = get_mon_hex();
	time->date = get_day_of_month();
	
	time->hou = get_hour_hex();
	time->min = get_min_hex();
	time->sec = get_sec_hex();
	return;
}

void time_task(struct SHEET *sht_back)
{
	struct TASK *task = task_now();
	char s[20];
	int fifobuf[12];
	struct TIMER *timer;
	struct TIME *time = ADR_TIME;
	
	fifo32_init(&task->fifo, 12, fifobuf, task);
	timer = timer_alloc();
	timer_init(timer, &task->fifo, 1);
	get_time_all(time);
	timer_settime(timer, 100);
	sprintf(s, "%d/%d/%d %d:%02d:%02d", time->year, time->mon, time->date, time->hou, time->min, time->sec);
	putfonts8_asc_sht(sht_back, sht_back->bxsize - 156, sht_back->bysize - 20, COL8_000000, COL8_C6C6C6, s, 19);
	
	for(;;){
		io_cli();
		if(fifo32_status(&task->fifo) == 0){
			task_sleep(task);
			io_sti();
		} else {
			if(fifo32_get(&task->fifo) == 1){
				get_time_all(time);
				sprintf(s, "%d/%d/%d %d:%02d:%02d", time->year, time->mon, time->date, time->hou, time->min, time->sec);
				putfonts8_asc_sht(sht_back, sht_back->bxsize - 156, sht_back->bysize - 20, COL8_000000, COL8_C6C6C6, s, 19);
				timer_settime(timer, 100);
			}
		}
	}
}
	
	
	