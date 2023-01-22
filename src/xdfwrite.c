#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iocslib.h>
#include <doslib.h>

#define NUM_SIDES         (2)
#define NUM_TRACKS        (77)
#define SECTOR_SIZE       (1024)
#define NUM_SECTORS       (8)
#define TRACK_SIZE        (SECTOR_SIZE * NUM_SECTORS)
#define XDF_IMAGE_SIZE    (NUM_SIDES * NUM_TRACKS * TRACK_SIZE)

int32_t main(int32_t argc, uint8_t* argv[]) {

  int32_t rc = 1;

  if (argc < 3) {
    printf("usage: XDFWRITE <xdf-image-file> <drive-number>\n");
    printf("  xdf-image-file ... XDF file name\n");
    printf("    drive-number ... 0 - 3\n");
    goto exit;
  }

  uint8_t* xdf_file = argv[1];
  struct FILBUF filbuf;
  if (FILES(&filbuf, xdf_file, 0x23) < 0) {
    printf("error: cannot find XDF file (%s).\n", xdf_file);
    goto exit;
  }
  if (filbuf.filelen != XDF_IMAGE_SIZE) {
    printf("error: unexpected XDF image size. (expected:%d actual:%d)\n", XDF_IMAGE_SIZE, filbuf.filelen);
    goto exit;
  }

  if (argv[2][0] < '0' || argv[2][0] > '3') {
    printf("error: incorrect drive number (%d). it must be 0 to 3.\n",argv[2]);
    goto exit;
  }

  int32_t pda = 0x90 + atoi(argv[2]);
  int32_t drive_stat = B_DRVCHK(pda * 256, 0);
  if (!(drive_stat & 2)) {
    printf("error: disk is not inserted.\n");
    goto exit;
  }
  if (drive_stat & 4) {
    printf("error: drive is not ready.\n");
    goto exit;
  }
  if (drive_stat & 8) {
    printf("error: write protected.\n");
    goto exit;
  }

  printf("Drive %d disk will be fully overwritten. Are you really ok? (y/n)");
  int8_t c;
  scanf("%c",&c);
  if (c != 'y' && c != 'Y') {
    printf("canceled. (%c)\n",c);
    goto exit;
  }

  FILE* fp = fopen(xdf_file, "rb");
  if (fp == NULL) {
    printf("error: file open error.\n");
    goto exit;
  }

  static uint8_t buf[ TRACK_SIZE ];
  for (int16_t i = 0; i < NUM_SIDES; i++) {
    for (int16_t j = 0; j < NUM_TRACKS; j++) {
      printf("Writing track %d at side %d ...\n", j, i);
      fread(buf, 1, TRACK_SIZE, fp);
      uint32_t rec_num = ( 3 << 24 ) + ( j << 16 ) + ( i << 8 ) + 1;
      uint32_t fdc_stat = B_WRITE(pda * 256 + 0x70, rec_num, (uint32_t)TRACK_SIZE, buf);
      if ((fdc_stat & 0xC0000000) != 0) {
        printf("error: write error. fdc status = %08X\n", fdc_stat);
        goto finally;
      }
    }
  }

  printf("Done.\n");

  rc = 0;

finally:
  if (fp != NULL) {
    fclose(fp);
    fp = NULL;
  }  

exit:
  return rc;
}