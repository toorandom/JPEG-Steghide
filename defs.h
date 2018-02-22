
#ifndef _DEFS_H_
#define _DEFS_H_


#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <sys/mman.h>

#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/buffer.h>


#define IDOFFSET 12
#define IDENTIFIER 0x66

#define TMPDIR "/tmp"
#define MAX_TMFILE 30

/* Toda la basura de la biblioteca de JPEG que es una cochinada */

struct jpeg_decompress_struct srcinfo;
struct jpeg_compress_struct dstinfo;
struct jpeg_error_mgr jsrcerr, jdsterr;
jvirt_barray_ptr *coef_rows;
JDIMENSION i, component_number, row_number, block_number;
size_t block_size_row;
JBLOCKARRAY coeficientes[MAX_COMPONENTS];
JBLOCKARRAY row_ptrs[MAX_COMPONENTS];

/* Defs API */
void inicia_jpeg(void);
FILE * mapea_jpeg_input_params(char *);
int setup_fourier_io_bitmask_size(int);
void smart_read_high_frequency (unsigned char *, unsigned char *,int , int, int);
void smart_write_jpeg (char *, int);
unsigned char *convert_bin_delinearize (char *, unsigned char *, int *);
int jpeg_extract_secrets (char *, char *, unsigned char *, unsigned char *, int);

int Base64Decode(char* , int , unsigned char* );
int Base64Encode(const unsigned char* , size_t , char* );
char * getfilefromb64 (char *);
#endif
