#include <getopt.h>
#include <libakrypt.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int read_file(const char *filename, char *buff) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    int c;
    size_t i = 0;

    while ((c = fgetc(file)) != EOF && i < 1024 - 1) {
        buff[i++] = (char)c;
    }

    buff[i] = '\0';

    if (ferror(file)) {
        perror("Error reading file");
        fclose(file);
        exit(1);
    }

    fclose(file);
    return 0;
}

void writeToFile(const char *buff) {

    FILE *file = fopen("encrypted.txt", "w");

    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(1);
    }

    fprintf(file, "%s", buff);

    fclose(file);
}


int main (int argc, char** argv) {

    int MAX_LEN = 32;
    int option;
    char pass[MAX_LEN];
    char file[MAX_LEN];
    int pf_flag = 0, e_flag = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s -p arg1 -f arg2 OR %s -e arg1\n", argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    while ((option = getopt(argc, argv, ":p:f:e:")) != -1) {
        switch (option) {
            case 'p':
                if (e_flag) {
                    fprintf(stderr, "Error: Cannot use -p with -e\n");
                    return EXIT_FAILURE;
                }
                strncpy(pass, optarg, MAX_LEN - 1);
                pass[MAX_LEN - 1] = '\0';
                pf_flag = 1;
                break;

            case 'f':
                if (e_flag) {
                    fprintf(stderr, "Error: Cannot use -f with -e\n");
                    return EXIT_FAILURE;
                }
                strncpy(file, optarg, MAX_LEN - 1);
                file[MAX_LEN - 1] = '\0';
                break;

            case 'e':
                if (pf_flag) {
                    fprintf(stderr, "Error: Cannot use -e with -p\n");
                    return EXIT_FAILURE;
                }
                strncpy(pass, optarg, MAX_LEN - 1);
                pass[MAX_LEN - 1] = '\0';
                e_flag = 1;
                break;

            case ':':
                fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
                return EXIT_FAILURE;

            case '?':
                fprintf(stderr, "Error: Unknown option -%c.\n", optopt);
                return EXIT_FAILURE;
        }
    }

    if ((pf_flag && !file[0]) || (e_flag && !pass[0])) {
        fprintf(stderr, "Error: Missing arguments for specified options.\n");
        return EXIT_FAILURE;
    }

  int error = ak_error_ok;
  int exitstatus = EXIT_FAILURE;
  struct bckey ctx;
  ak_uint8 iv[8] = { 0xf1, 0xad, 0x37, 0x9e, 0x11, 0x4f, 0x00, 0x12 };

  if( ak_libakrypt_create( NULL ) != ak_true ) {
    ak_libakrypt_destroy();
    return EXIT_FAILURE;
  }

  ak_bckey_create_magma( &ctx );
  ak_bckey_set_key_from_password(&ctx, pass, 32, "keyseeed", 8);

  char* data = malloc(1024);

  if (pf_flag) {

    read_file(file, data);

    if(( error = ak_bckey_ofb( &ctx, data, data, 1024, iv, 8)) != ak_error_ok ) goto exlab;

    printf("Encrypted text in hex: %s\n\n", ak_ptr_to_hexstr( data, 1024, ak_false ));
    writeToFile(data);
    printf("Data has beeb successefully encrypted and wtritten to file ecnrypted.txt\n");
  }

  if (e_flag) {
    char* fl = "encrypted.txt";
    read_file(fl, data);

    if(( error = ak_bckey_ofb( &ctx, data, data, 1024, iv, 8 )) != ak_error_ok ) goto exlab;

    printf("открытый текст: %s\n", data );
  }

  exlab: ak_bckey_destroy( &ctx );

  if( error == ak_error_ok ) exitstatus = EXIT_SUCCESS;
  ak_libakrypt_destroy();
 return exitstatus;
}
