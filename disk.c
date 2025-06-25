#include "bootpack.h"
#define Cs 0
#define Hs 0
#define Ss 1
#define READSECTOR_TIMEOUT 10000;


unsigned int readSector(unsigned int partitionOffset, unsigned int driveNumber, unsigned int lba, unsigned char *data, int limit)
{
    //print("readSector start=%x ",data);
    lba += partitionOffset;
    /*
    unsigned short c = lba / (getMaxHead () * getMaxSector ()) + Cs;
    unsigned char h = (lba / getMaxSector ()) % getMaxHead () + Hs;
    unsigned char s = lba % getMaxSector () + Ss;
    print("lba=%lu,c=%u,h=%u,s=%u ",lba,c,h,s);
    return readSector (driverNumber, c, h, s, data, limit);
    */
 
    //print("limit=%d, numberOfSector=%d ",limit, numberOfSector);
	int numberOfSector, x;
    if (limit == 0) {
        return 0;
    } else if (limit < 512) {
        numberOfSector = 1;
    } else {
        numberOfSector = (limit % 512 == 0) ? limit / 512 : (limit / 512) + 1;
    }
    //print("numberOfSector=%u ",numberOfSector);
    io_out8(0x1f2, numberOfSector);
    io_out8(0x1f3, lba);
    io_out8(0x1f4, lba >> 8);
    io_out8(0x1f5, (lba >> 16) & 0xff);
    io_out8(0x1f6, 0xe0 | (driveNumber << 4));
    io_out8(0x1f7, 0x20);
    //print("end outport ");
    //sleep(1);
    //print("readSector 2 ");
 
    unsigned char status = 0;
    int timeout = READSECTOR_TIMEOUT;
    do {
        status = io_in16(0x1f7);
        if (timeout-- == 0) {
            return 0;
        }
    } while ((status & 0x8) != 0x8);
    //} while ((status & 0x89) != 0x8);
    //print("end while ");
    for (x = 0; x < numberOfSector * 512; x += 2) {
        //print("x=%u",x);
        if (x >= limit && limit != -1) {
            break;
        } else {
            //print("start in ");
            short inChar = io_in16(0x1f0);
            //print("end in ");
            //char s=inport(0x1f1);
            //print("statur=%u ", s);
            char first = inChar & 0xff;
            char second = inChar >> 8;
            if (x < limit || limit == -1) {
                data[x] = first;
            }
            if (x + 1 < limit || limit == -1) {
                data[x + 1] = second;
            }
        }
    }
    if (limit != -1) {
        //print("returning... ");
        return limit;
    } else {
        return numberOfSector * 512;
    }
}
