#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "ext2.h"
#include "block.h"

#define BUF_LEN 512

int main(int argc, char *argv[])
{
	struct block_device *bdev;
	struct ext2_dir_entry_2 *dir;
	struct ext2_file *fp;
	const char *dev, *fn;
	ssize_t len;
	char buff[BUF_LEN];

	if (argc != 3)
	{
		printf("usage ..\n");
		return -EINVAL;
	}

	dev = argv[1];
	fn  = argv[2];

	bdev = bdev_open(dev);
	if (bdev == NULL)
	{
		char str[64];

		sprintf(str, "open(%s)", dev);
		perror(str);

		return -ENODEV;
	}

	dir = ext2_mount(bdev, "ext2", "anywhere");

	fp = ext2_open(fn, 0);
	if (NULL == fp)
	{
		printf("fail to open %s\n", fn);
		return -ENOENT;
	}

	len = ext2_read(fp, buff, BUF_LEN);

	if (len > 0)
	{
		buff[len] = '\0';
		printf("data:\n\t%s\n", buff);
	}

	ext2_close(fp);

	bdev_close(bdev);

	return 0;
}
