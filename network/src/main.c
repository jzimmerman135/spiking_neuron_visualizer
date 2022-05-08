#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "reader.h"
#include "encoder.h"
#include "filter.h"
#include "layer.h"
#include "helpful.h"
#include "network.h"

Reader_T Reader_new_from_argv(char *argv);

int main(int argc, char **argv)
{
    check_usage(argc);
    Reader_T train_data = Reader_new_from_argv(argv[1]);
    Reader_T test_data  = Reader_new_from_argv(argv[2]);

    Reader_print(train_data, 0);
    Reader_print(test_data, 0);
    shape2_t input_shape = Reader_shape_input(train_data);
    shape2_t label_shape = Reader_shape_label(train_data);
    Network_T network = Network_new(input_shape, label_shape);

    printf("Hello\n");

    Network_free(&network);
    Reader_free(&train_data);
    Reader_free(&test_data);
}

Reader_T Reader_new_from_argv(char *argv)
{
    char filename[64] = "./datasets/";
    strcat(filename, argv);
    FILE* fp = fopen(filename, "r");
    if (!fp){
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        exit(1);
    }

    Reader_T data = Reader_new(fp);
    fclose(fp);
    return data;
}
