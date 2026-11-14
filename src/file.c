#include "file.h"

FILE* open_file(const char* path, const char* mode)
{
	return fopen(path, mode);
}

void write_file(FILE* file, const char* text)
{
	if (!file)
	{
		return;
	}

	fputs(text, file);
}

char* read_file(const char* path)
{
	FILE* file = open_file(path, "rb");

	if (!file)
	{
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
