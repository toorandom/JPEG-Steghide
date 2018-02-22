#include "defs.h"

size_t calcula_tam_base64(const char* b64input,int max) {
        //size_t len = strnlen(b64input,max),
        size_t len = strlen(b64input),
                padding = 0;

        if (b64input[len-1] == '=' && b64input[len-2] == '=')
                padding = 2;
        else if (b64input[len-1] == '=')
                padding = 1;
        return (len*3)/4 - padding;
}
int Base64Decode(char* b64message, int tamano_datos, unsigned char* buffer) {
        BIO *bio, *b64;
        int decodeLen = calcula_tam_base64(b64message,tamano_datos),length;
 ERR_print_errors_fp(stderr);
	printf("calc decodeLen=%d, b64 len tamano = %d\n",decodeLen,tamano_datos);
        buffer[decodeLen]=0;
        bio = BIO_new_mem_buf(b64message, -1);
        b64 = BIO_new(BIO_f_base64());
        bio = BIO_push(b64, bio);
        //BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        length = BIO_read(bio, buffer, tamano_datos);


        if(length != decodeLen) {
                //fprintf(stderr,"Error decode en base64, %d != %d \n", length, decodeLen);
		BIO_free_all(bio);
                return length;
        }
        BIO_free_all(bio);
        return decodeLen;
}
int Base64Encode(const unsigned char* buffer, size_t length, char* b64text) {
        BIO *bio, *b64;
        BUF_MEM *bufferPtr;
        int olen;
        b64 = BIO_new(BIO_f_base64());
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, buffer, length);
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);
        BIO_set_close(bio, BIO_NOCLOSE);
        BIO_free_all(bio);
        olen =  strlen( (*bufferPtr).data );
        memcpy(b64text,(*bufferPtr).data, olen);
        return olen;
}

