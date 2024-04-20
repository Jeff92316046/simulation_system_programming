#include <stdio.h>
#include <Windows.h> // for windows
//#include <unistd.h> //for unix(linux or mac)
#include <fcntl.h>

#define PARTITION_NUM 4
struct partition {              // total 16B
	unsigned char drive;		/* 0x80 - active */

	unsigned char head;			/* starting head */
	unsigned char sector;		/* starting sector */
	unsigned char cylinder;		/* starting cylinder */

	unsigned char sys_type;		/* partition type */

	unsigned char end_head;		/* end head */
	unsigned char end_sector;	/* end sector */
	unsigned char end_cylinder;	/* end cylinder */

    // In both 32 and 64 bit Linux, unsigned int = 4 bytes;
	unsigned int start_sector;	/* starting sector counting from 0 */
	unsigned int nr_sectors;        /* nr of sectors in partition */
};

char *dev = "vdisk";
int fd;
    
int main(){
    struct partition *p;
    char buf[512];

    fd = open(dev, O_RDONLY);   // open dev for READ
    read(fd, buf, 512);    // read in MBR at sector 0    

    p = (struct partition *)(&buf[0x1be]); // p->P1
    printf("%-10s%9s%10s%10s%11s%10s\n","Device","Start","End","Sectors","Size","Id");
    for(int i=0;i<PARTITION_NUM;i++){
       printf("P%-10d%8u%10u%10u%10uK%10x\n",i+1, p[i].start_sector,p[i].start_sector + (p[i].nr_sectors) -1 ,p[i].nr_sectors,p[i].nr_sectors/2,p[i].sys_type);
    }
    printf("--------------------- Extend Partition ---------------------\n");
    int extStart = p[3].start_sector;
    int localMBR = extStart;
    for(int i=PARTITION_NUM+1;;i++){
        lseek(fd,localMBR*512,0);
        read(fd,buf,512);
        p = (struct partition *)(&buf[0x1be]);
        printf("P%-10d%8u%10u%10u%10uK%10x\n",i, p[0].start_sector+ localMBR,p[0].start_sector + localMBR + (p[0].nr_sectors) -1 ,p[0].nr_sectors,p[0].nr_sectors/2,p[0].sys_type);
        if(p[1].start_sector == 0)break;
        else{
            localMBR =  extStart + p[1].start_sector;
        }
    }
    printf("End of Extend Partitions");
    
}