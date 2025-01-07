#pragma once

#include "biblioteci.h"

#define size_buffer 2024

int fn_read(int file_fd, char *date, int *size_date);

int fn_write(int file_fd, void *var, int size);
