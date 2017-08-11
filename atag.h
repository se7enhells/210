#define ATAG_CORE		( 0x54410001 )
#define ATAG_MEM		( 0x54410002 )
#define ATAG_CMDLINE	( 0x54410009 )
#define ATAG_NONE		( 0x00000000 )
#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)
#define tag_next(t)		((struct tag *)((unsigned int *)(t) + (t)->hdr.size))

typedef struct tag_header {
	unsigned int size;
	unsigned int tag;
}TAG_HEADER;

typedef struct tag_core {
	unsigned int flags;		
	unsigned int pagesize;
	unsigned int rootdev;
}TAG_CORE;

typedef struct tag_mem32 {
	unsigned int	size;
	unsigned int	start;	
}TAG_MEM32;

typedef struct tag_cmdline {
	char	cmdline[1];	
}TAG_CMDLINE;

typedef struct tag {
	TAG_HEADER hdr;
	union {
		TAG_CORE	core;
		TAG_MEM32	mem;
		TAG_CMDLINE	cmdline;
	} u;
}TAG;