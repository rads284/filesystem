#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


#define BLOCK_SIZE 4096    //Block size is 4KB
#define BLCK_NUM 80
#define INODE_SIZE 88   //88 Bytes per inode
#define INODE_NUM 80
#define DATA_NUM 72
#define MAX_MAP 3
#define MAX_FILES_IN_DIR 60

typedef struct inode {
    ino_t id;            //inode id
    mode_t mode;         //permissions
    nlink_t links;       //number of hard links
    off_t size;          //size
    blkcnt_t blocks;     //number of blocks referred to
    uid_t u_id;           //user id
    gid_t g_id;           //group id
    time_t m_time;         //modify time
    time_t a_time;         //access time
    time_t c_time;         //creation time
    int direct_blk[MAX_MAP];      //blocks mapped to
    int ftype;  //file type - 0-empty, 1-file, 2-directory
}inode;

typedef struct super_block{     //structure for defining super block
    int no_of_inodes;
    int no_of_datablocks;
    int inode_off;
    int fstype;         //magic no to identify file system type
}super_block;

typedef struct i_bmap{          //inode bit map
    int map[INODE_NUM];
}i_bmap;

typedef struct d_bmap{          //data node bit map
    int map[DATA_NUM];
}d_bmap;

typedef struct mydirent {
    char d_name[256];       /*dirctory name */
    char contents[MAX_FILES_IN_DIR][60];  //names of contents
    int sub_id[MAX_FILES_IN_DIR];     //inode ids of contents
    ino_t d_ino;            /* inode number */
    off_t d_off;            /*offset to the next dirent */
    unsigned char d_type;    //type of file
    unsigned short d_reclen; //size of all files together
}mydirent;

struct super_block *sblock;
struct i_bmap *ibmap;
struct d_bmap *dbmap;

void initialize_fs(super_block* sblock, i_bmap* imap, d_bmap *dmap);
super_block *initialize_superblock();
i_bmap* initialize_ibmap();
d_bmap* initialize_dbmap();
inode*  create_root_inode(ino_t ID, int FILE_TYPE, mode_t MODE, blkcnt_t BLOCKS);
