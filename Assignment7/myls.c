#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <string.h>
#include <inttypes.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void display(int argc, char **argv, uint8_t OPTIONS);
void display_inode(struct stat* mystat_ptr);
void display_permissions(struct stat* mystat_ptr);
void display_links_pw_grp_size(struct stat* mystat_ptr);
void display_time(struct stat* mystat_ptr);
void display_name(struct dirent* entryptr);
int onlyoptions(int argc, char **argv);

uint8_t OPTIONS;

typedef struct {
	uint16_t othr_x:1;
	uint16_t othr_w:1;
	uint16_t othr_r:1;
	uint16_t grp_x:1;
	uint16_t grp_w:1;
	uint16_t grp_r:1;
	uint16_t onr_x:1;
	uint16_t onr_w:1;
	uint16_t onr_r:1;
	uint16_t sticky:1;
	uint16_t sgid:1;
	uint16_t suid:1;
	uint16_t fileType:1;
} mode_s;

typedef union {
	mode_s per;
	uint16_t val;
}mode_u;

typedef enum {
	op_R = 0x8,
	op_LAI = 0x7,
    	op_LA = 0x6,
    	op_LI = 0x5,
    	op_L = 0x4,
    	op_AI = 0x3,
    	op_A = 0x2,
    	op_I = 0x1
}opt_type;

int main(int argc, char **argv)
{

    int opt;
    while ((opt = getopt(argc, argv, "laiR")) != -1)
    {
	    switch (opt)
	    {
	    	    case 'l': OPTIONS |= (1 << 2);
			      break;

		    case 'a': OPTIONS |= (1 << 1);
			      break;

		    case 'i': OPTIONS |= (1 << 0);
			      break;
		    
		    case 'R': OPTIONS |= (1 << 3);
			      break;

		    default: OPTIONS = 0;
			     break;
	}

    }

    if (argc == 1)
    {
	char *buf = NULL;

	char *curdir = getcwd(buf, 50);
	DIR *dirptr = opendir(curdir);

	struct dirent *entry = NULL;

	while ((entry = readdir(dirptr)) != NULL)
		printf("%s ", entry->d_name);

	printf("\n");

	return 0;
    }

    else
	    display(argc, argv, OPTIONS);
}

int onlyoptions(int argc, char **argv)
{
	for(int i=1; i<argc; i++)
	{
		if(argv[i][0] != '-')
			return -1;
		else
			return 1;
	}
}

void display(int argc, char **argv, uint8_t OPTIONS)
{
    for (int i = 1; i<argc; i++) 
    {
	if (argv[i][0]=='-' && argc>2)
	    continue;
	
	char *buf = NULL;
	char *curdir = NULL;
	DIR *dirptr = NULL;

	int ret = onlyoptions(argc, argv);
	if(ret == 1)
	{
		buf = NULL;
		curdir = getcwd(buf, 100);
		dirptr = opendir(curdir);
	}
	else if (ret == -1)
	{
		printf("%s:\n",argv[i]);
		dirptr = opendir(argv[i]);
		chdir(argv[i]);
	}

	struct dirent *entry = NULL;
	struct stat mystat;

	switch (OPTIONS) 
	{
		case op_LAI:
                        while ((entry = readdir(dirptr)) != NULL)
			{
                                stat(entry->d_name, &mystat);

                                display_inode(&mystat);
                                display_permissions(&mystat);
                                display_links_pw_grp_size(&mystat);
                                display_time(&mystat);
                                display_name(entry);

                                printf("\n");
			}
			break;

		case op_LI:
			while ((entry = readdir(dirptr)) != NULL) 
			{
				if(entry->d_name[0] == '.')
					continue;

				stat(entry->d_name, &mystat);

				display_inode(&mystat);
				display_permissions(&mystat);
				display_links_pw_grp_size(&mystat);
				display_time(&mystat);
				display_name(entry);

				printf("\n");
			}
			break;
        
	 	case op_LA:
                        while ((entry = readdir(dirptr)) != NULL)
                        {
                                stat(entry->d_name, &mystat);

                                display_permissions(&mystat);
                                display_links_pw_grp_size(&mystat);
                                display_time(&mystat);
                                display_name(entry);

				printf("\n");
			}
			break;

		case op_AI:
			while ((entry = readdir(dirptr)) != NULL)
                        {
                                stat(entry->d_name, &mystat);

                                display_inode(&mystat);
                                display_name(entry);

                                printf("\t");
                        }
                        printf("\n");
                        break;

		case op_L:
			while ((entry = readdir(dirptr)) != NULL)
			{
				if(entry->d_name[0] == '.')
                                        continue;

				stat(entry->d_name, &mystat);

				display_permissions(&mystat);
                                display_links_pw_grp_size(&mystat);
                                display_time(&mystat);
                                display_name(entry);

				printf("\n");
			}
			break;

		case op_I:
			while ((entry = readdir(dirptr)) != NULL)
                        {
				if(entry->d_name[0] == '.')
                                        continue;

				stat(entry->d_name, &mystat);
				
				display_inode(&mystat);
				display_name(entry);

				printf("\t");
			}
			printf("\n");
			break;

		case op_A:
			while ((entry = readdir(dirptr)) != NULL)
                        {
                                display_name(entry);
                                printf("\t");
                        }
                        printf("\n");
                        break;

		default: 
			while ((entry = readdir(dirptr)) != NULL)
                        {
				if(entry->d_name[0] == '.')
                                        continue;

                                display_name(entry);
                                printf("\t");
                        }
                        printf("\n");
                        break;
	}
    }

}

void display_inode(struct stat* mystat_ptr)
{
	printf("%-7lu ", mystat_ptr->st_ino);
}

void display_permissions(struct stat* mystat_ptr)
{
	mode_u per;
	per.val = mystat_ptr->st_mode;
	uint8_t fileindex = (per.val>>12);
	char filetype;
	switch(fileindex)
	{
		case 0x2: filetype = 'c';
			break;
		case 0x4: filetype = 'd';
			break;
		case 0x6: filetype = 'b';
			break;
		default: filetype = '-';
			 break;	 
	}
	printf("%c%c%c%c%c%c%c%c%c%c ",
			filetype,
			(per.per.onr_r == 1)?'r':'-',
                        (per.per.onr_w == 1)?'w':'-',
                        (per.per.onr_x == 1)?'x':'-',
			(per.per.grp_r == 1)?'r':'-',
                        (per.per.grp_w == 1)?'w':'-',
                        (per.per.grp_x == 1)?'x':'-',
			(per.per.othr_r == 1)?'r':'-',
			(per.per.othr_w == 1)?'w':'-',
			(per.per.othr_x == 1)?'x':'-');
}

void display_links_pw_grp_size(struct stat* mystat_ptr)
{
	struct passwd *nameptr = getpwuid(mystat_ptr->st_uid);
	struct group *grpptr = getgrgid(mystat_ptr->st_gid);
	printf("%2lu %6s %6s %6lu ",
		mystat_ptr->st_nlink,
		nameptr->pw_name,
		grpptr->gr_name,
		mystat_ptr->st_size);
}

void display_time(struct stat* mystat_ptr)
{
	time_t mytime = mystat_ptr->st_mtim.tv_sec;
	struct tm *mytimeptr = gmtime(&mytime);
	char *mymon = NULL;
	switch(mytimeptr->tm_mon)
	{
		case 0: mymon = "Jan";
			break;
		case 1: mymon = "Feb";
                        break;
		case 2: mymon = "Mar";
                        break;
		case 3: mymon = "Apr";
                        break;
		case 4: mymon = "May";
                        break;
		case 5: mymon = "Jun";
                        break;
		case 6: mymon = "Jul";
                        break;
		case 7: mymon = "Aug";
                        break;
		case 8: mymon = "Sep";
			break;
		case 9: mymon = "Oct";
                        break;
		case 10: mymon = "Nov";
                        break;
		case 11: mymon = "Dec";
			 break;
	}
	printf("%3s %3d %3d:%d ",
			mymon,
			mytimeptr->tm_mday,
			mytimeptr->tm_hour,
			mytimeptr->tm_min);
}

void display_name(struct dirent* entryptr)
{
	printf("%4s ", entryptr->d_name);
}
