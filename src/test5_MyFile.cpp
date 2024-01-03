#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// Enable ECB, CTR and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DCTR=1 -DECB=1
#define CBC 1
#define CTR 1
#define ECB 1

#include "aes.h"

static uint8_t* loadfromfile( const char *path, int *buffersize ){
  FILE *fp = fopen( path, "r" );
  if( fp == NULL )
    return NULL;

  fseeko( fp, 0L, SEEK_END );
  int64_t size = ftello( fp );
  *buffersize = size;

  uint8_t *buffer = (uint8_t*)malloc( size );
  if( buffer == NULL )
    return NULL;

  fseeko( fp, 0L, SEEK_SET );
  if( fread( buffer, size, 1, fp ) != 1 ){
    free( buffer );
    fclose( fp );
    return NULL;
  }else{
    fclose( fp );
    return buffer;
  }
}

static void storetofile( const char *path, uint8_t *buffer, int buffer_size ){
  FILE *fp = fopen( path, "w" );
  if( fp == NULL )
    return;

  fwrite( buffer, buffer_size, 1, fp );
  fclose( fp );
}

int main( int argc, char *argv[] )
{
  if( argc != 4 ){
    printf("Usage: %s infile encrypfile decryptfile\n", argv[0]);
    return 1;
  }
  char *infile = argv[1];
  char *encryptfile = argv[2];
  char *decryptfile = argv[3];

  int input_size = 0;
  uint8_t *input = loadfromfile( infile, &input_size );
  uint8_t *buffer = (uint8_t*)malloc( input_size );
  memcpy( buffer, input, input_size );

  //AES128, 这是原来的密钥，记得更换
  //    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
  //AES256， 已经改成自己自定义的密钥
 uint8_t key[] = {
    0x38, 0xd4, 0x13, 0x63, 0x65, 0xc2, 0x22, 0xce, 0x75, 0x11, 0xc1, 0x24, 0x39, 0x78, 0x67, 0xb6,
    0x46, 0x98, 0x6f, 0x83, 0x54, 0xd5, 0xe3, 0x0b, 0xa9, 0x54, 0x43, 0x34, 0x84, 0x9a, 0xd1, 0xdf };
  uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

  struct AES_ctx ctx;
  AES_init_ctx_iv(&ctx, key, iv);
  AES_CBC_encrypt_buffer(&ctx, buffer, input_size);

//  uint8_t *buffer2 = (uint8_t*)malloc( input_size );
//  memcpy( buffer2, buffer, input_size );

  storetofile( encryptfile, buffer, input_size );
  if( memcmp( buffer, input, input_size ) == 0 ){
    printf("cmp OK aftef encrypt\n");
  }else{
    printf("cmp Diff after encrypt\n");
  }

  struct AES_ctx ctx2;
  AES_init_ctx_iv(&ctx2, key, iv);
  AES_CBC_decrypt_buffer(&ctx2, buffer, input_size);

  storetofile( decryptfile, buffer, input_size );
  if( memcmp( buffer, input, input_size ) == 0 ){
    printf("cmp OK after decrypt\n");
  }else{
    printf("cmp Diff after decrypt\n");
  }

  //测试AES_init_ctx_iv()只初始化一次，但使用其ctx两次
//  struct AES_ctx ctx3;
//  AES_init_ctx_iv(&ctx3, key, iv);
//  AES_CBC_decrypt_buffer( &ctx3, buffer2, input_size );
//  AES_CBC_decrypt_buffer( &ctx2, buffer2, input_size );

//  storetofile( "/tmp/dbf/descrypt2.dbf", buffer2, input_size );
//  if( memcmp( buffer2, buffer, input_size ) == 0 ){
//    printf("cmp OK for the second decrypt\n");
//  }else{
//    printf("cmp Diff for the second decrypt\n");
//  }

  free( input );
  free( buffer );
//  free( buffer2 );
  return 0;
}

