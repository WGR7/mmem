// http://mc.pp.se/dc/vms/flashmem.html

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * The VMS flash memory contains 128 kilobytes of storage. These are divided
 * into 256 blocks of 512 bytes each. Of these blocks, 200 are available for 
 * user files. The rest of the blocks contain filesystem information, or are
 * simply not used at all.
 *
 * The allocation of the 256 blocks is as follows:
 *  0-199   : User data (files)
 *  200-240	: Not used
 *  241-253	: Directory
 *  254	    : FAT
 *  255	    : Root block
 */
#define VMS_BLOCKS 256
#define VMS_BLOCK_SIZE 512
#define VMS_SIZE (VMS_BLOCKS * VMS_BLOCK_SIZE)

#define VMS_USER_BLOCK_START 0
#define VMS_USER_BLOCK_END 199

#define VMS_UNUSED_BLOCK_START 200
#define VMS_UNUSED_BLOCK_END 240

#define VMS_DIR_BLOCK_START 241
#define VMS_DIR_BLOCK_END 253

#define VMS_FAT_BLOCK 254
#define VMS_ROOT_BLOCK 255

#define VMS_FORMAT_SIZE 16
#define VMS_FORMAT_BYTE 0x55

/*
 * The file creation time is stored as a sequence of BCD (Binary Coded
 * Decimal) bytes. Each byte represents a number between 0 and 99. 
 *
 * The meaning of the bytes are as follows:
 *   Century (e.g. 19)
 *   Year within century (e.g. 99)
 *   Month within year (e.g. 11)
 *   Day within month (e.g. 1)
 *   Hour of day (e.g. 22)
 *   Minute of hour (e.g. 50)
 *   Second of minute (e.g. 12)
 *   Day of week (0 = monday, 6 = sunday)
 */
typedef struct bcd_timestamp {
	uint8_t century;
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t weekday;
} bcd_timestamp;


/*
 * ROOT BLOCK (vms_root_block)
 *
 * The root block (block 255) contains information such as:
 *  The date when the card was formatted
 *  The color and icon for this VMS in the Dreamcast file manager
 *  Location and size of the FAT and Directory system files
 */

typedef struct vms_root_block {

	/*
	 * 0x000-0x00f : All these bytes contain 0x55 to indicate a properly formatted card.
	 */
	uint8_t format_check[VMS_FORMAT_SIZE];

    /*
	 * 0x010       : custom VMS colour (1 = use custom colours below, 0 = standard colour)
     */
	uint8_t custom_color;

	/*
	 * 0x011       : VMS colour blue component
	 * 0x012       : VMS colour green component
	 * 0x013       : VMS colour red component
	 * 0x014       : VMS colour alpha component (use 100 for semi-transparent, 255 for opaque)
	 */
	uint8_t custom_color_b;
	uint8_t custom_color_g;
	uint8_t custom_color_r;
	uint8_t custom_color_a;

	/*
	 * 0x015-0x02f : not used (all zeroes)
	 */
	uint8_t _unused1[27];

	/*
	 * 0x030-0x037 : BCD timestamp
     */
	struct bcd_timestamp created;

	/*
	 * 0x038-0x03f : not used (all zeroes)
	 */
	uint8_t _unused2[8];

	/*
	 * ???
	 */
	uint8_t _unused3[6];

	/*
	 * 0x046-0x047 : 16 bit int (little endian) : location of FAT (254)
	 * 0x048-0x049 : 16 bit int (little endian) : size of FAT in blocks (1)
	 * 0x04a-0x04b : 16 bit int (little endian) : location of directory (253)
	 * 0x04c-0x04d : 16 bit int (little endian) : size of directory in blocks (13)
	 * 0x04e-0x04f : 16 bit int (little endian) : icon shape for this VMS (0-123)
	 * 0x050-0x051 : 16 bit int (little endian) : number of user blocks (200)
	 */
	uint16_t fat;
	uint16_t fat_size;
	uint16_t fat_dir;
	uint16_t fat_dir_size;
	uint16_t icon_shape;
	uint16_t user_blocks;

	/*
	 * ???
	 */

} vms_root_block;

typedef uint8_t vms_block[VMS_BLOCK_SIZE];

vms_block *
vms_read_block(FILE *file, int block) {
	size_t result;
	vms_block *data;

	assert(block < VMS_BLOCKS);

	assert(fseek(file, block * VMS_BLOCK_SIZE, SEEK_SET) == 0);

	data = malloc(sizeof(vms_block));

	assert(data != NULL);

	result = fread(data, sizeof(vms_block) - 1, 1, file);

	assert(result == 1);

	return data;
}

vms_root_block *
vms_read_root_block(FILE * file) {
	vms_root_block *block;
	int i;

	block = (vms_root_block *)vms_read_block(file, VMS_ROOT_BLOCK);

	assert(block != NULL);
	
    for (i = 0; i < VMS_FORMAT_SIZE; i++) {
		assert(block->format_check[i] == VMS_FORMAT_BYTE);
	}

	return block;
}

// XXX we assume the file has been checked before we fseek, fread, etc
FILE *
vms_open(const char *path) {
	FILE *vmu;
	vmu = fopen(path, "r");
	assert(vmu != NULL);
	return vmu;
}

int
main(int argc, char **argv) {
	FILE *vmu;
	vms_root_block * root_block;

	vmu = vms_open(argv[1]);

	root_block = vms_read_root_block(vmu);

	printf("custom color: %c\n", (root_block->custom_color) ? 'Y' : 'N');
	if (root_block->custom_color) {
		printf("color: rgba(%d, %d, %d, %d)\n", 
			root_block->custom_color_r,
			root_block->custom_color_g,
			root_block->custom_color_b,
			root_block->custom_color_a);
	}

	printf("created: %02x%02x-%02x-%02x %02x:%02x\n",
		root_block->created.century,
		root_block->created.year,
		root_block->created.month,
		root_block->created.day,
		root_block->created.hour,
		root_block->created.minute);

	printf("FAT location: %d\n", root_block->fat);
	printf("FAT size: %d\n", root_block->fat_size);
	printf("FAT directory location: %d\n", root_block->fat_dir);
	printf("FAT directory size: %d\n", root_block->fat_dir_size);
	printf("icon shape: %d\n", root_block->icon_shape);
	printf("user blocks: %d\n", root_block->user_blocks);

	free(root_block);

	return 0;
}