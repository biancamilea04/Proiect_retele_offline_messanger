#include <comunic/comunicare.h>

int fn_read(int file_fd, char *date, int *size_date)
{
    char buff[size_buffer];
    if( read(file_fd, size_date, sizeof(int)) <= 0 ){
        perror("Eroare la citire: ");
        return -1;
    }

    for (int i = 0; i < (*size_date) / size_buffer; i++)
    {
        if(read(file_fd, buff, sizeof(buff)) <= 0 ){
            perror("Eroare la citire: ");
        return -1;
        }
        for (int j = 0; j < size_buffer; j++)
        {
            date[j + i * size_buffer] = buff[j];
        }
    }

    if ((*size_date) % size_buffer > 0)
    {
        read(file_fd, buff, (*size_date) % size_buffer);
        for (int j = 0; j < (*size_date) % size_buffer; j++)
        {
            date[j + ((*size_date) / size_buffer) * size_buffer] = buff[j];
        }
    }

    return *size_date;
}

int fn_write(int file_fd, void *var, int size)
{
    if(write(file_fd, &size, sizeof(int)) <= 0 ){
        perror("Eroare la scriere: ");
        return -1;
    }
    if(write(file_fd, var, size) <= 0){
        perror("Eroare la scriere: ");
        return -1;
    }

    return size;
}
