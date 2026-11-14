#pragma once

#include <stdio.h>
#include <stdlib.h>

FILE* open_file(const char* path, const char* mode);
void write_file(FILE* file, const char* text);
char* read_file(const char* path);
