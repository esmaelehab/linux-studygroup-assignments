#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

typedef struct 
{
	uint8_t status;
	uint8_t res1[3];
	uint8_t partition_type;
	uint8_t res2[3];
	uint32_t lba;
	uint32_t sector_count;

} partitionEntry;

void checkForType (partitionEntry* partEntry_ptr, int partNum, char type[]);

int main(int argc, char **argv)
{
	char buf[512];
	char type [20];

	int fd = open(argv[1], O_RDONLY);
	read(fd, buf, 512);

	partitionEntry* partEntry_ptr = (partitionEntry*) &buf[446];

	printf("%-10s %-5s %7s %9s %8s %5s %3s  %-7s\n", "Device", "Boot", "Start", "End", "Sectors", "Size", "Id", "Type");

	for(int i=0; i<4; i++)
	{
		if (partEntry_ptr[i].lba == 0)
			continue;

		checkForType(partEntry_ptr, i, type);

		printf("%s%-5d %-2c %7u %9u %8u %4.1fG %3x  %s\n",
			argv[1],
			i+1,
			partEntry_ptr[i].status == 0x80 ? '*' : ' ',
			partEntry_ptr[i].lba,
			partEntry_ptr[i].lba + partEntry_ptr[i].sector_count - 1,
			partEntry_ptr[i].sector_count,
			(float) (((double) partEntry_ptr[i].sector_count * 512) / (1024*1024*1024)),
			partEntry_ptr[i].partition_type,
			type);

	}

	close(fd);

	for(int i=0; i<4; i++)
	{
		if (partEntry_ptr[i].partition_type == 0x5)
		{
			char buf2[512];

			off_t offset = 512 * partEntry_ptr[i].lba;

			int fd2 = open(argv[1], O_RDONLY);
			off_t ret = lseek(fd2, offset, SEEK_SET);
			ssize_t ret2 = read(fd2, buf2, 512);

			partitionEntry* logicEntry_ptr = (partitionEntry*) &buf2[446];

			for(int j=0; j<4; j++)
			{
				if (logicEntry_ptr[j].lba == 0)
					continue;

				checkForType(logicEntry_ptr, j, type);

				printf("%s%-5d %-2c %7u %9u %8u %4.1fG %3x  %s\n",
					argv[1],
					i+4,
					logicEntry_ptr[j].status == 0x80 ? '*' : ' ',
					logicEntry_ptr[j].lba + partEntry_ptr[i].lba,
					logicEntry_ptr[j].lba + partEntry_ptr[i].lba + logicEntry_ptr[j].sector_count - 1,
					logicEntry_ptr[j].sector_count,
					(float) (((double) logicEntry_ptr[j].sector_count * 512) / (1024*1024*1024)),
					logicEntry_ptr[j].partition_type,
					type);

			}
		}
	}
}

void checkForType (partitionEntry* Entry_ptr, int partNum, char type[])
{
	switch (Entry_ptr[partNum].partition_type)
	{
		case 0x5: strcpy(type, "Extended");
			  break;

		case 0x83: strcpy(type, "Linux");
			   break;

		case 0x7: strcpy(type, "HPFS/NTFS/exFAT");
			  break;
	}
}
