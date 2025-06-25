#include "bootpack.h"

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

void time_ask(struct SHEET *sht)
{
	char s[40];
	sprintf(s, "%d/%d/%d", get_year(), get_mon_hex(), get_day_of_month());
	putfonts8_asc_sht(sht, sht->bxsize - 130, sht->bysize -20, COL8_000000, COL8_C6C6C6, s, 10);
	sprintf(s, "%d:%02d", get_hour_hex(), get_min_hex());			
	putfonts8_asc_sht(sht, sht->bxsize - 45, sht->bysize -20, COL8_000000, COL8_C6C6C6, s, 5);
	sheet_refresh(sht, sht->bxsize - 130, sht->bysize -20,sht->bxsize - 45 + 5*8, sht->bysize -50+16);
}