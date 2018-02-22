#include "defs.h"


unsigned char rbox[256] = {
  66, 26, 25, 96, 76, 135, 67, 1, 47, 131, 233, 4, 23, 248, 235, 51,
  87, 249, 121, 104, 241, 19, 158, 212, 5, 113, 125, 148, 205, 42, 103, 122,
  17, 101, 152, 128, 226, 221, 146, 139, 201, 134, 107, 49, 11, 7, 245, 116,
  234, 250, 118, 100, 195, 0, 244, 138, 204, 58, 213, 185, 210, 180, 44, 110,
  93, 130, 190, 9, 39, 18, 182, 167, 60, 186, 106, 206, 72, 53, 237, 59,
  94, 62, 33, 220, 80, 91, 192, 198, 32, 163, 231, 35, 97, 73, 172, 255,
  159, 143, 27, 179, 85, 217, 74, 89, 254, 127, 117, 102, 191, 239, 63, 164,
  175, 36, 170, 86, 227, 223, 160, 145, 88, 181, 207, 157, 28, 37, 156, 119,
  92, 224, 243, 43, 50, 193, 61, 208, 16, 171, 174, 64, 184, 111, 70, 216,
  147, 240, 215, 150, 21, 155, 55, 142, 203, 123, 222, 79, 151, 38, 57, 202,
  90, 141, 20, 95, 230, 83, 71, 12, 229, 10, 253, 45, 114, 246, 154, 251,
  187, 46, 98, 194, 161, 75, 225, 219, 124, 68, 109, 218, 211, 126, 6, 183,
  136, 169, 197, 69, 99, 189, 29, 140, 65, 115, 8, 176, 108, 41, 200, 105,
  14, 13, 177, 199, 129, 2, 188, 31, 238, 132, 173, 236, 40, 78, 252, 196,
  153, 77, 133, 30, 84, 15, 247, 24, 162, 137, 22, 232, 214, 168, 82, 52,
  144, 81, 48, 56, 3, 242, 165, 112, 34, 149, 166, 228, 120, 54, 178, 209
};


unsigned char *
convert_bin_delinearize (char *file, unsigned char *mask, int *count_bytes)
{
  int i, j, k = 0, fd;
  int cbytes=0;
  unsigned char *str, *resultado_bits;
  unsigned char byte;
  int size;
  struct stat finfo;

  fd = open (file, O_RDONLY);
  fstat (fd, &finfo);
  str = calloc ((finfo.st_size*8) + 1, sizeof (char));

  while (read (fd, &byte, sizeof (char)) > 0)
    {
      str[cbytes] = rbox[byte];
 //     str[cbytes] = byte;
      cbytes++;
    }

  str[cbytes] = 0;


  close (fd);
  resultado_bits = calloc ((finfo.st_size + 1) * 8, sizeof (char));
  size = finfo.st_size;


  for (i = 0; i < cbytes; i++)
    {
      for (j = 0; j < 8; j++)
	{
	  if (str[i] >> (7 - j) & 0x01)
	    resultado_bits[k++] = '1';
	  else
	    resultado_bits[k++] = '0';
	}
    }
  /* Por eso uso ascii '0' y no int 0, no por idiota, ya que quiero trabajar en bytespace no bitspace :p 
   * en bitspace seria mas rapido, pero con mas codigo y la cantidad a meter generalmente es poca y el performance
   * solo se notaria mas rapido al manejar gigabytes lo cual no es nuestro caso
   */
  resultado_bits[k] = '\0';
  j = 0;
  for (i = 0; i < strlen ((char *)resultado_bits); i += 2)
    {
      if (resultado_bits[i] == '1')
	mask[j] |= 0x02;
      if (resultado_bits[i + 1] == '1')
	mask[j] |= 0x01;
      j += 1;

    }
  free (str);
  *count_bytes = cbytes;
  return resultado_bits;
}
