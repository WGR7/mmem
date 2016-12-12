#include <errno.h> 
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/mman.h>
#include <unistd.h>

// 128KiB 
#define VMS_SIZE (128 * 1024)

#define VMS_FORMAT 0x55

static void usage(void) __dead;

/*
The file creation time is stored as a sequence of BCD (Binary Coded Decimal) bytes. 
Each byte represents a number between 0 and 99. 
The meaning of the bytes are as follows:
    Century (e.g. 19)
    Year within century (e.g. 99)
    Month within year (e.g. 11)
    Day within month (e.g. 1)
    Hour of day (e.g. 22)
    Minute of hour (e.g. 50)
    Second of minute (e.g. 12)
    Day of week (0 = monday, 6 = sunday)
*/
struct bcd_struct {
	unsigned char century;
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char weekday;
};

// http://mc.pp.se/dc/vms/flashmem.html
// The VMS flash memory contains 128 kilobytes of storage.
// These are divided into 256 blocks of 512 bytes each.

struct vms_struct {

	unsigned char user[512 * 200];

	unsigned char unused_1[512 * 40];

	unsigned char directory[512 * 14];

	unsigned char FAT[512];

	/*
	* Root block
	*	
	* 0x000-0x00f : All these bytes contain 0x55 to indicate a properly formatted card.
    */
	unsigned char root_format[16];

    /*
	 * 0x010       : custom VMS colour (1 = use custom colours below, 0 = standard colour)
    */
	unsigned char root_custom_color;

	/*
	* 0x011       : VMS colour blue component
	* 0x012       : VMS colour green component
	* 0x013       : VMS colour red component
	* 0x014       : VMS colour alpha component (use 100 for semi-transparent, 255 for opaque)
	*/
	unsigned char root_custom_color_b;
	unsigned char root_custom_color_g;
	unsigned char root_custom_color_r;
	unsigned char root_custom_color_a;

	/*
	 * 0x015-0x02f : not used (all zeroes)
	*/
	unsigned char unused_2[27];

	/*
	* 0x030-0x037 : BCD timestamp (see Directory below)
    */
	struct bcd_struct created;

	/*
	* 0x038-0x03f : not used (all zeroes)
	*/
	unsigned char unused_3[8];

	/*
	* ...
	*/
	unsigned char unknown_1[6];

	/*
	* 0x046-0x047 : 16 bit int (little endian) : location of FAT (254)
	* 0x048-0x049 : 16 bit int (little endian) : size of FAT in blocks (1)
	* 0x04a-0x04b : 16 bit int (little endian) : location of directory (253)
	* 0x04c-0x04d : 16 bit int (little endian) : size of directory in blocks (13)
	* 0x04e-0x04f : 16 bit int (little endian) : icon shape for this VMS (0-123)
	* 0x050-0x051 : 16 bit int (little endian) : number of user blocks (200)
	*/
	unsigned short int fat;
	unsigned short int fat_size;
	unsigned short int fat_dir;
	unsigned short int fat_dir_size;
	unsigned short int icon_shape;
	unsigned short int user_blocks;
};

unsigned int
vms_formatted(struct vms_struct *vms) {
	int i;
    for (i = 0; i < sizeof vms->root_format; i++) {
	//	printf("[%d] = %2x\n", i, vms->root_format[i]);
		if (vms->root_format[i] != VMS_FORMAT) {
			return 0;
		}
	}
	return 1;
}

void
vms_dump_unused_1(struct vms_struct *vms) {
	int i;
    for (i = 0; i < sizeof vms->unused_1; i++) {
		printf("[%d] = %2x\n", i, vms->unused_1[i]);
	}
}

void
vms_dump_unused_2(struct vms_struct *vms) {
	int i;
    for (i = 0; i < sizeof vms->unused_2; i++) {
		printf("[%d] = %2x\n", i, vms->unused_2[i]);
	}
}

void
vms_dump_unknown_1(struct vms_struct *vms) {
	int i;
    for (i = 0; i < sizeof vms->unknown_1; i++) {
		printf("[%d] = %2x\n", i, vms->unknown_1[i]);
	}
}

int
main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int ch, fd;
    struct stat st;
	struct vms_struct *vms;
	while ((ch = getopt(argc, argv, "h")) != -1) {
		switch (ch) {
			case '?':
			case 'h':
			default:
				usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (!argv[0]) {
		usage();
	}

	fd = open(argv[0], O_RDONLY | O_DIRECT);
	if (fd == -1) {
		fprintf(stderr, "error:\n open(%s): %s\n", argv[0], strerror(errno));
		exit(1);
	}

	if (fstat(fd, &st) == -1) {
		fprintf(stderr, "error:\n fstat(%s)\n", argv[0]);
		exit(1);
	}

	/*if (st.st_size != VMS_SIZE) {
		fprintf(stderr, "error:\n size != %dB\n", VMS_SIZE);
		exit(1);
	}*/
	
    vms = (struct vms_struct *)mmap(0, VMS_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	if (vms == MAP_FAILED) {
		fprintf(stderr, "error:\n mmap(%d)\n", argv[0]);
		exit(1);
	}

	if (!vms_formatted(vms)) {
		fprintf(stderr, "error:\n invalid root block (%s)\n", argv[0]);
		exit(1);
	}

	printf("custom color: %c\n", (vms->root_custom_color) ? 'Y' : 'N');
	if (vms->root_custom_color) {
		printf("color: rgba(%d, %d, %d, %d)\n", 
			vms->root_custom_color_r,
			vms->root_custom_color_g,
			vms->root_custom_color_b,
			vms->root_custom_color_a);
	}

	printf("created: %02x%02x-%02x-%02x %02x:%02x\n", vms->created.century,
		vms->created.year, vms->created.month, vms->created.day,
		vms->created.hour, vms->created.minute);

	printf("FAT location: %d\n", vms->fat);
	printf("FAT size: %d\n", vms->fat_size);
	printf("FAT directory location: %d\n", vms->fat_dir);
	printf("FAT directory size: %d\n", vms->fat_dir_size);
	printf("icon shape: %d\n", vms->icon_shape);
	printf("user blocks: %d\n", vms->user_blocks);

}

static void
usage()
{
	(void)fprintf(stderr,
		"usage:\n mmem vms\n");
	exit(1);
}

