#include <string.h>
#include <block.h>

#define IS_ALPHBIT(c) (((c) >= 'a' && (c) <= 'z') || \
			((c) >= 'A' && (c) <= 'Z'))

int main(int argc, char *argv[])
{
	char v;
	const char *vol;
	struct block_device *bdev;

	switch (argc) {
	case 1:
		v = get_home_volume();
		break;

	case 2:
		vol = argv[1];

		if (IS_ALPHBIT(vol[0]) && \
			('\0' == vol[1] || (':' == vol[1] && '\0' == vol[1])))
				v = vol[0];
		else {
			usage();
			return -EINVAL;
		};
		break;

	default:
		usage();
		return -EINVAL;
	}

	bdev = get_bdev_by_volume(v);
	if (!bdev) {
		printf("fail to change to \"%c\", no such block device!\n", v);
		return -ENODEV;
	}

	// TODO: show more block info
	printf("0x%08x - 0x%08x %s (%c:)\n",
		bdev->bdev_base, bdev->bdev_base + bdev->bdev_size,
		bdev->dev.name, bdev->volume);

	set_curr_volume(v);

	return 0;
}
