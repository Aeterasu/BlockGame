#include "file.h"

char* read_file(const char* path)
{
	FILE* file = fopen(path, "rb");

	if (!file)
	{
		fprintf(stderr, "Failed to read file: %s\n", path);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	if (size < 0)
	{
		fclose(file);
		return NULL;
	}

	char* buffer = malloc((size_t)size + 1);
	if (!buffer)
	{
		fclose(file);
		return NULL;
	}

	size_t bytes_read = fread(buffer, 1, (size_t)size, file);
	fclose(file);

	if (bytes_read != (size_t)size)
	{
		free(buffer);
		return NULL;
	}

	buffer[size] = '\0';
	return buffer;
}
