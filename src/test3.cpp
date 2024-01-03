#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

static int getline_from_memory( uint8_t *dst, int dstsize, const uint8_t *src ){
  int readnum = 0;
  while(1){
    char c = *src;
    if( c == EOF || c == '\0' ){
      readnum = -1;
      break;
    }

    *dst++ = *src++;
    readnum++;

    if( c == '\n' || c == '\r' ){
      break;
    }
    if( readnum >= dstsize ){
      break;
    }
  }

  *dst = '\0';
  return readnum;
}

int main( int argc, char *argv[] )
{
  if( argc != 3 ){
    printf("Usage: %s infile outfile\n", argv[0]);
    return 1;
  }
  const char *infile = argv[1];
  const char *outfile = argv[2];

  int input_size = 0;
  uint8_t *input = loadfromfile( infile, &input_size );

  FILE *fp = fopen( outfile, "w" );
  if( fp == NULL ){
    printf("fopen( %s ) failed\n", outfile);
    return 1;
  }

  uint8_t buffer[1024];
  int idx = 0;
  while( 1 ){
    int readnum = getline_from_memory( buffer, 1024, &input[idx] );
    if( readnum == -1 ){
      break;
    }
    fwrite( buffer, readnum, 1, fp );
    idx += readnum;
  }

  free( input );
  fclose( fp );
  return 0;
}
