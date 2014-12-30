#include <stdint.h>
#include <stdlib.h>
#include <tiffio.h>

int writeBmp(char* data, int length);

#pragma pack(1)

typedef struct tagBITMAPFILEHEADER {
  uint16_t bfType;
  uint32_t  bfSize;
  uint16_t bfReserved1;
  uint16_t bfReserved2;
  uint32_t  bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
  uint32_t biSize;
  int32_t  biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t  biXPixPerMeter;
  int32_t  biYPixPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImporant;
} BITMAPINFOHEADER;


void write_bmp(char* buf, int width, int height)
{
  struct tagBITMAPFILEHEADER fileheader;
  fileheader.bfType = 0x4D42;
  fileheader.bfSize = sizeof(BITMAPFILEHEADER) +
                      sizeof(BITMAPINFOHEADER) +
                      width * height * 4;
  fileheader.bfReserved1 = 0;
  fileheader.bfReserved2 = 0;
  fileheader.bfOffBits = sizeof(BITMAPFILEHEADER) +
                         sizeof(BITMAPINFOHEADER);
  writeBmp((char*)&fileheader, sizeof(BITMAPFILEHEADER));
    
  struct tagBITMAPINFOHEADER infoheader;
  infoheader.biSize = sizeof(BITMAPINFOHEADER);
  infoheader.biWidth = width;
  infoheader.biHeight = height;
  infoheader.biPlanes = 1;
  infoheader.biBitCount = 32;
  infoheader.biCompression = 0;
  infoheader.biSizeImage = 0;
  infoheader.biXPixPerMeter = 0;
  infoheader.biYPixPerMeter = 0;
  infoheader.biClrUsed = 0;
  infoheader.biClrImporant = 0;
  writeBmp((char*)&infoheader, sizeof(BITMAPINFOHEADER));

  // Swap Red and Blue.
  int bufsize = width * height * 4;
  for (int i = 0; i < bufsize; i += 4) {
    char tmp = buf[i + 2];
    buf[i + 2] = buf[i];
    buf[i] = tmp;
  }
  writeBmp(buf, bufsize);
}

int tiff_to_bmp(char* filename) {
  TIFF *image;
  uint32 width, height;
  unsigned long imageOffset, result;
  int stripMax, stripCount;
  char *buffer, tempbyte;
  unsigned long bufferSize, count;
  image = TIFFOpen(filename, "r");
  if(!image) {
    fprintf(stderr, "File open failure.\n");
    return -1;
  }
  if(!TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width)) {
    fprintf(stderr, "Failed to get width.\n");
    TIFFClose(image);
    return -1;
  }
  if(!TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height)) {
    fprintf(stderr, "Failed to get height.\n");
    TIFFClose(image);
    return -1;
  }
  char *row_pointers = malloc(width * height * 4);
  if(!TIFFReadRGBAImageOriented(image, width, height,
                                (uint32*)row_pointers,
                                ORIENTATION_BOTLEFT,
                                1)){
    fprintf(stderr, "TIFFReadRGBAImageOriented failed.\n");
    free(row_pointers);
    TIFFClose(image);
    return -1;
  }
  printf("read done\n");
  write_bmp(row_pointers, width, height);
  free(row_pointers);
  TIFFClose(image);
  return 0;
}
