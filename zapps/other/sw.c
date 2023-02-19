// requires /zada/star_wars.txt, the ascii art of the star wars

#include <syscall.h>
#include <string.h>
#include <i_time.h>
#include <stdlib.h>
#include <i_iolib.h>
#include <stdio.h>


int main(int argc, char **argv) {

    char path[] = "/zada/starwars.txt";

    printf("allocating memory for the file...\n");
    uint8_t *data = c_fs_declare_read_array(path);
    char *str = malloc(0x1000);

    str[0] = '\0';

    printf("loading file: %s into memory...\n", path);
    c_fs_read_file(path, data);

    c_clear_screen();

    int line = 0, str_index = 0, j, nb_temps;
    char temps[5];

    for (int i = 0; c_kb_get_scancode() != 1; i++) {
        str_index++;
        str[str_index] = (char) data[i];
        if (str[str_index] != '\n') continue;
        line++;
        if (line % 14) continue;
        for (j = 0; str[j] > 40; j++) temps[j] = str[j];
        temps[j] = '\0';
        str[str_index] = '\0';
        nb_temps = atoi(temps);
        if (nb_temps < 0) break;
        c_ckprint_at(str, 0, 0, 0x0F);
        ms_sleep(nb_temps * 100);
        str_index = -1;
        str[0] = '\0';
    }

    c_clear_screen();
    free(data);
    free(str);
    return 0;
}
