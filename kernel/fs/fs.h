#ifndef FS_H
#define FS_H

#include <stdint.h>

#define FS_FILE		0x01
#define FS_DIRECTORY	0x02
#define FS_CHARDEVICE	0x03
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE		0x05
#define FS_SYMLINK	0x06
#define FS_MOUNTPOINT	0x08

struct fs_node;

struct dirent
{
	char name[128];			// filename
	u32int ino;			// Inode number.  required by POSIX
};

typedef u32int (*read_type_t)(struct fs_node*, u32int, u32int, u8int*);
typedef u32int (*write_type_t)(struct fs_node*, u32int, u32int, u8int*);
typedef void   (*open_type_t)(struct fs_node*);
typedef void   (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*, u32int);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*, char *name);

typedef struct fs_node
{
        char            name[128];      // filename
        u32int          mask;           // permisions mask
        u32int          uid;            // owining user
        u32int          gid;            // owning group
        u32int          flags;          // includes node type
        u32int          inode;          // device-specific.  provides a way for a filesys to indetify files
        u32int          length;         // sizeof the file, in bytes
        u32int          impl;           // an implementation-defined number
        read_type_t     read;
        write_type_t    write;
        open_type_t     open;
        close_type_t    close;
        readdir_type_t  readdir;
        finddir_type_t  finddir;
        struct fs_node  *ptr;
} fs_node_t;

extern fs_node_t *fs_root;		// the root of the fs.

u32int read_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer);
void open_fs(fs_node_t *node, u8int read, u8int write);
u32int write_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, u32int index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);

#endif
