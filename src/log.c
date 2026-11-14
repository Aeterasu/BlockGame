#define LOG_IMPLEMENTATION
#include "log.h"

#include <stdarg.h>
#include <stdio.h>

#include <SDL3/SDL_filesystem.h>

#include "file.h"

#define LOG_FILE_NAME "debug.log"
#define LOG_PATH_SIZE 1024
#define LOG_MESSAGE_SIZE 1024

static FILE* log_file = NULL;
static int log_file_checked = 0;

static FILE* log_get_file(void)
{
	if (log_file_checked)
	{
		return log_file;
	}

	log_file_checked = 1;

	const char* base_path = SDL_GetBasePath();
	char log_path[LOG_PATH_SIZE];

	if (base_path)
	{
		snprintf(log_path, sizeof(log_path), "%s%s", base_path, LOG_FILE_NAME);
	}
	else
	{
		snprintf(log_path, sizeof(log_path), "%s", LOG_FILE_NAME);
	}

	log_file = open_file(log_path, "w");

	if (!log_file)
	{
		fprintf(stderr, "Failed to open log file: %s\n", log_path);
	}

	return log_file;
}

static void log_write(const char* level, const char* source_file, const int source_line, const char* format,
					  va_list args)
{
	char message[LOG_MESSAGE_SIZE];
	vsnprintf(message, sizeof(message), format, args);

	char line[LOG_MESSAGE_SIZE + 256];

	if (source_file)
	{
		snprintf(line, sizeof(line), "%s %s [%s:%d]\n", level, message, source_file, source_line);
	}
	else
	{
		snprintf(line, sizeof(line), "%s %s\n", level, message);
	}

	write_file(stdout, line);
	fflush(stdout);

	FILE* file = log_get_file();

	if (file)
	{
		write_file(file, line);
		fflush(file);
	}
}

void log_info(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	log_write("[INFO]", NULL, 0, format, args);
	va_end(args);
}

void log_warning(const char* file, const int line, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	log_write("[WARNING]", file, line, format, args);
	va_end(args);
}

void log_error(const char* file, const int line, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	log_write("[ERROR]", file, line, format, args);
	va_end(args);
}
