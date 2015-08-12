#define KSEEK_BUF_SIZE 16384

inline int kseek(KFILE* file, long offset, int origin)
{
	// store offset in a longlong to support files > 2.1 GB
	long long llOffset = offset;
	// make sure we are always seeking from the current
	switch( origin )
	{
	case SEEK_END:
		// add the 'rest of the file'
		llOffset += kgetSize(file) - ktell(file);
		break;
	case SEEK_CUR:
		// nothing to do here ...
		break;
	case SEEK_SET:
		// subtract the current file pointer to seek from the start
		llOffset -= ktell(file);
		break;
	default:
		// Wrong seek origin!
		return -1;
		break;
	}

	if (llOffset < 0)
	{
		// offset is negative, so reset to beginning and read on
		llOffset += ktell(file);
		kreset(file);

		if (llOffset < 0)
			// seeking before the start??
			return -1;
	}

	if (llOffset > 0)
	{
		// skip the number of characters.
		kskip(llOffset, file);
	}

	return 0;
}