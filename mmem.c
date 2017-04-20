/* 
 * Big thanks to Marcus Comstedt's documentation at:
 * http://mc.pp.se/dc/vms/flashmem.html
 */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define VMS_ENTRIES_PER_BLOCK 16

#define VMS_USER_BLOCK_START 0
#define VMS_USER_BLOCK_END 199

#define VMS_UNUSED_BLOCK_START 200
#define VMS_UNUSED_BLOCK_END 240

#define VMS_DIR_BLOCK_START 241
#define VMS_DIR_BLOCK_END 253

#define VMS_ROOT_BLOCK 255

#define VMS_FORMAT_SIZE 16
#define VMS_FORMAT_BYTE 0x55

#define VMS_FAT_UNALLOC 0xfffc
#define VMS_FAT_ALLOC_LAST 0xfffa

#define VMS_ENTRY_FREE 0x00
#define VMS_ENTRY_DATA 0x33
#define VMS_ENTRY_GAME 0xcc

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
 * The Directory lists all the user files stored in the VMS. The Directory
 * consists of a sequence of 32-byte entries each potentially describing a
 * file. When the VMS is formatted, enough space is allocated to the
 * Directory file to accommodate 200 entries. This is enough, since each
 * file must be at least one block long, and there are only 200 blocks
 * available for user files. The actual blocks making up the Directory can
 * be found using the Root block and the FAT, although it should be safe to
 * assume that the Directory has been allocated to blocks 241 through 253;
 * 253 being the first block of the Directory, and 241 the last.
 *
 * An entry in the directory is either all NUL-bytes
 * (denoting an unused entry), or a structure describing a file. This
 * structure looks as follows:
 *
 *  0x00      : 8 bit int : file type (0x00 = no file, 0x33 = data, 0xcc = game)
 *  0x01      : 8 bit int : copy protect (0x00 = copy ok, 0xff = copy protected)
 *  0x02-0x03 : 16 bit int (little endian) : location of first block
 *  0x04-0x0f : ASCII string : filename (12 characters)
 *  0x10-0x17 : BCD timestamp (see below) : file creation time
 *  0x18-0x19 : 16 bit int (little endian) : file size (in blocks)
 *  0x1a-0x1b : 16 bit int (little endian) : offset of header (in blocks) from file start
 *  0x1c-0x1f : unused (all zero) 
 */
typedef struct vms_entry {
	uint8_t type;
	uint8_t copy_protection;
	uint16_t first_block;
	char name[12];
	bcd_timestamp created;
	uint16_t size;   
	uint16_t header_offset; 
	uint8_t _unused[4];
} vms_entry;


/*
 * This is the contents of the header:
 *  Offset	Size (bytes)	Datatype	Contents
 *  $00	16	Text	Description of file (shown in VMS file menu)
 *  $10	32	Text	Description of file (shown in DC boot ROM file manager)
 *  $30	16	String	Identifier of application that created the file
 *  $40	2	Integer	Number of icons (>1 for animated icons)
 *  $42	2	Integer	Icon animation speed
 *  $44	2	Integer	Graphic eyecatch type (0 = none)
 *  $46	2	Integer	CRC (Ignored for game files.)
 *  $48	4	Integer	Number of bytes of actual file data following header, 
 *          icon(s) and graphic eyecatch. (Ignored for game files.)
 *  $4C	20	... Reserved (fill with zeros) ...
 *  $60	32	Integers	Icon palette (16 16-bit integers)
 *  $80	512*n	Nybbles	Icon bitmaps
 *  ...	depends on type	...	Graphic eyecatch palette and bitmap
*/
typedef struct vms_header {
	char vms_desc[16];
	char dc_desc[32];
	char app[16];
	uint16_t icons;
	uint16_t animation_speed;
	uint16_t graphic_type; //?
	uint16_t CRC;
	uint32_t size;
	uint8_t _unused[20];
	// XXX - icon data...
} vms_header;

/*
 * ROOT BLOCK (vms_root_block)
 *
 * The root block (block 255) contains information such as:
 *  The date when the card was formatted
 *  The color and icon for this VMS in the Dreamcast file manager
 *  Location and size of the FAT and Directory system files
 *
 * 0x000-0x00f : All these bytes contain 0x55 to indicate a properly formatted card.
 * 0x010       : custom VMS colour (1 = use custom colours below, 0 = standard colour)
 * 0x011       : VMS colour blue component
 * 0x012       : VMS colour green component
 * 0x013       : VMS colour red component
 * 0x014       : VMS colour alpha component (use 100 for semi-transparent, 255 for opaque)
 * 0x015-0x02f : not used (all zeroes)
 * 0x030-0x037 : BCD timestamp
 * 0x038-0x03f : not used (all zeroes)
 * unused
 * 0x046-0x047 : 16 bit int (little endian) : location of FAT (254)
 * 0x048-0x049 : 16 bit int (little endian) : size of FAT in blocks (1)
 * 0x04a-0x04b : 16 bit int (little endian) : location of directory (253)
 * 0x04c-0x04d : 16 bit int (little endian) : size of directory in blocks (13)
 * 0x04e-0x04f : 16 bit int (little endian) : icon shape for this VMS (0-123)
 * 0x050-0x051 : 16 bit int (little endian) : number of user blocks (200)
 * unused
 */
typedef struct vms_root_block {
	uint8_t format_check[VMS_FORMAT_SIZE];
	uint8_t custom_color;
	uint8_t custom_color_b;
	uint8_t custom_color_g;
	uint8_t custom_color_r;
	uint8_t custom_color_a;
	uint8_t _unused1[27];
	bcd_timestamp created;
	uint8_t _unused2[14];
	uint16_t fat_block;
	uint16_t fat_size;
	uint16_t fat_dir_block;
	uint16_t fat_dir_size;
	uint16_t icon_shape;
	uint16_t user_blocks;
} vms_root_block;

typedef uint8_t vms_block[VMS_BLOCK_SIZE];

typedef struct vms_dir_entries {
	vms_entry entry[VMS_ENTRIES_PER_BLOCK];
} vms_dir_entries;

typedef struct vms_fat_block {
	uint16_t block[VMS_BLOCKS];
} vms_fat_block;

// XXX - http://mc.pp.se/dc/vms/fileheader.html
int vms_crc(const unsigned char *buf, int size) {
	int i, c, n = 0;
	for (i = 0; i < size; i++) {
		n ^= (buf[i]<<8);
		for (c = 0; c < 8; c++) {
			if (n & 0x8000) {
				n = (n << 1) ^ 4129;
			} else {
				n = (n << 1);
			}
		}
	}
	return n & 0xffff;
}

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

	// All these bytes contain 0x55 to indicate a properly formatted card.
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
	vms_fat_block * fat_block;
	vms_dir_entries * entries;
	vms_header * header;
	int i, j, index = 0;

	vmu = vms_open(argv[1]);

	root_block = vms_read_root_block(vmu);

	printf("[%d] = Root Block\n", VMS_ROOT_BLOCK);
	printf(" custom color: %c\n", (root_block->custom_color) ? 'Y' : 'N');
	if (root_block->custom_color) {
		printf(" color: rgba(%d, %d, %d, %d)\n", 
			root_block->custom_color_r,
			root_block->custom_color_g,
			root_block->custom_color_b,
			root_block->custom_color_a);
	}

	printf(" created: %02x%02x-%02x-%02x %02x:%02x\n",
		root_block->created.century,
		root_block->created.year,
		root_block->created.month,
		root_block->created.day,
		root_block->created.hour,
		root_block->created.minute);

	printf(" FAT block: %d\n",
		root_block->fat_block);

	printf(" directory blocks: %d-%d\n",
		root_block->fat_dir_block,
		(root_block->fat_dir_block - root_block->fat_dir_size) + 1);

	printf(" icon shape: %d\n", root_block->icon_shape);
	printf(" user blocks: %d\n", root_block->user_blocks);

	printf("[%d] = FAT Block\n", root_block->fat_block);

	fat_block = (vms_fat_block *)vms_read_block(vmu, root_block->fat_block);

	printf(" ");
	for (i = 0; i < VMS_BLOCKS; i++) {
		if (fat_block->block[i] == VMS_FAT_UNALLOC) {
			printf("[%03d]=____ ", i);
		} else if (fat_block->block[i] == VMS_FAT_ALLOC_LAST) {
			printf("[%03d]=LAST ", i);
		} else {
			printf("[%03d]=%04hu ", i, fat_block->block[i]);
		}
		if (((i+1) % 8) == 0) printf("\n");
		if (i < VMS_BLOCKS - 1) printf(" ");
	}

	printf("[%d] = FAT Directory Block\n", root_block->fat_dir_block);

	for (j = 0; j < root_block->fat_dir_size; j++) {

		entries = (vms_dir_entries *)vms_read_block(vmu, root_block->fat_dir_block - j);

		printf("[%d]\n", root_block->fat_dir_block - j);

		for (i = 0; i < VMS_ENTRIES_PER_BLOCK; i++) {

			if (entries->entry[i].type == VMS_ENTRY_GAME) {
				printf("#%03d GAME:\n", index);
			} else if (entries->entry[i].type == VMS_ENTRY_DATA) {
				printf("#%03d FILE:\n", index);
			} else {
				continue;
			}

			index += 1;

			header = (vms_header *)vms_read_block(vmu,
				entries->entry[i].first_block +
				entries->entry[i].header_offset);

			if (strncmp("ICONDATA_VMS", entries->entry[i].name, 12) == 0) {
				printf(" title: HIDDEN FILE\n");
			} else { 
				printf(" title: %.16s", header->dc_desc);
				printf("%.16s\n", &header->dc_desc[16]);
				printf(" app: %.16s\n", header->app);
			}

			printf(" desc: %.16s\n", header->vms_desc);

			printf(" name: %.12s\n", entries->entry[i].name);

			printf(" crc: 0x%04x\n", header->CRC);

			printf(" icon(s): %d\n", header->icons);

			printf(" animation speed: %d\n", header->animation_speed);

			// XXX
			printf(" size: %d bytes\n",
				(entries->entry[i].first_block + entries->entry[i].size));

			printf(" first block: #%d\n", entries->entry[i].first_block);

			printf(" created: %02x%02x-%02x-%02x %02x:%02x\n",
				entries->entry[i].created.century,
				entries->entry[i].created.year,
				entries->entry[i].created.month,
				entries->entry[i].created.day,
				entries->entry[i].created.hour,
				entries->entry[i].created.minute);

			printf(" offset: %03d\n", entries->entry[i].header_offset);

			free(header);

		}
		free(entries);
	}

	free(root_block);
	free(fat_block);

	return 0;
}