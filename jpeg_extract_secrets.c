#include "defs.h"


unsigned char irbox[256] = {
  53, 7, 213, 244, 11, 24, 190, 45, 202, 67, 169, 44, 167, 209, 208, 229,
  136, 32, 69, 21, 162, 148, 234, 12, 231, 2, 1, 98, 124, 198, 227, 215,
  88, 82, 248, 91, 113, 125, 157, 68, 220, 205, 29, 131, 62, 171, 177, 8,
  242, 43, 132, 15, 239, 77, 253, 150, 243, 158, 57, 79, 72, 134, 81, 110,
  139, 200, 0, 6, 185, 195, 142, 166, 76, 93, 102, 181, 4, 225, 221, 155,
  84, 241, 238, 165, 228, 100, 115, 16, 120, 103, 160, 85, 128, 64, 80, 163,
  3, 92, 178, 196, 51, 33, 107, 30, 19, 207, 74, 42, 204, 186, 63, 141,
  247, 25, 172, 201, 47, 106, 50, 127, 252, 18, 31, 153, 184, 26, 189, 105,
  35, 212, 65, 9, 217, 226, 41, 5, 192, 233, 55, 39, 199, 161, 151, 97,
  240, 119, 38, 144, 27, 249, 147, 156, 34, 224, 174, 149, 126, 123, 22, 96,
  118, 180, 232, 89, 111, 246, 250, 71, 237, 193, 114, 137, 94, 218, 138, 112,
  203, 210, 254, 99, 61, 121, 70, 191, 140, 59, 73, 176, 214, 197, 66, 108,
  86, 133, 179, 52, 223, 194, 87, 211, 206, 40, 159, 152, 56, 28, 75, 122,
  135, 255, 60, 188, 23, 58, 236, 146, 143, 101, 187, 183, 83, 37, 154, 117,
  129, 182, 36, 116, 251, 168, 164, 90, 235, 10, 48, 14, 219, 78, 216, 109,
  145, 20, 245, 130, 54, 46, 173, 230, 13, 17, 49, 175, 222, 170, 104, 95
};



int
jpeg_extract_secrets (char *ifile, char *outfile, unsigned char *mask, unsigned char *rawdata, int b64)
{
  FILE *archivo_entrada;
  int fdout,fdb64;
  char *ptr_b64,*ptr_jpeg;
  struct stat b64_info,jpeg_info;

  int set_length = 0, set_data = 0, bit_count = 0, testmask = 0, l = 0;
  int a_get = 0, g = 0, a_val = 0, b_val = 0;
  int b64_bytes = 0;
 
  char tmname[MAX_TMFILE];

  snprintf((char *)&tmname,sizeof(tmname),"st.%d",getpid());
  archivo_entrada = fopen (ifile, "rb");


  if (archivo_entrada == NULL)
    {
      perror ("fopen");
      return -1;
    }


  if(b64) 
  fdout = open (tmname,O_RDWR|O_CREAT,0700);
  else 
  fdout = open (outfile, O_RDWR | O_CREAT, 0744);

  if (fdout < 0)
    {
      perror ("open");
      return -1;
    }

  jpeg_stdio_src (&srcinfo, archivo_entrada);
  (void) jpeg_read_header (&srcinfo, TRUE);
  for (component_number = 0; component_number < srcinfo.num_components;
       component_number++)
    coeficientes[component_number] =
      ((&dstinfo)->mem->alloc_barray) ((j_common_ptr) & dstinfo,
				       JPOOL_IMAGE,
				       srcinfo.comp_info
				       [component_number].width_in_blocks,
				       srcinfo.comp_info
				       [component_number].height_in_blocks);
  coef_rows = jpeg_read_coefficients (&srcinfo);
  jpeg_copy_critical_parameters (&srcinfo, &dstinfo);
  for (component_number = 0; component_number < srcinfo.num_components;
       component_number++)
    {
      block_size_row = (size_t) sizeof (JCOEF) * DCTSIZE2
	* srcinfo.comp_info[component_number].width_in_blocks;
      for (row_number = 0;
	   row_number <
	   srcinfo.comp_info[component_number].height_in_blocks; row_number++)
	{
	  int k, l;
	  row_ptrs[component_number] =
	    ((&dstinfo)->mem->access_virt_barray) ((j_common_ptr) & dstinfo,
						   coef_rows
						   [component_number],
						   row_number, (JDIMENSION) 1,
						   FALSE);
	  for (block_number = 0;
	       block_number <
	       srcinfo.comp_info[component_number].width_in_blocks;
	       block_number++)
	    {
	      k = -1;
	      l = 0;
	      for (i = 0; i < DCTSIZE2; i++)
		{
		  if (!set_length)
		    {
		      if ((i > 0)
			  &&
			  (row_ptrs[component_number][0][block_number][i]
			   != 0)
			  &&
			  (abs
			   (row_ptrs[component_number][0][block_number]
			    [i]) > 15))

			{


			  int test_amask = 0x08, k = 0;
			  int test =
			    abs (row_ptrs[component_number][0]
				 [block_number][i]);
			  if (a_get == 1 && g == 1)
			    {
			      b_val *= 8;
			      set_length = 1;
			    }
			  if (!a_get)
			    {
			      a_get = 1;

			      for (k = 0; k < 4; k++)
				{
				  if (test & test_amask)
				    a_val += test_amask;
				  test_amask = test_amask >> 1;
				}
			    }
			  else
			    {

			      if (test & 0x01)
				b_val += pow (2, a_val);
			      a_val -= 1;
			      if (a_val == 0)
				g = 1;
			    }

			}
		      else
			coeficientes[component_number][row_number]
			  [block_number][i] =
			  row_ptrs[component_number][0][block_number][i];
		    }
		  else
		    {
		      if (!set_data)
			{
			  if ((i > 0)
			      &&
			      (row_ptrs[component_number][0]
			       [block_number][i] != 0)
			      &&
			      (abs
			       (row_ptrs[component_number][0]
				[block_number][i]) > 1))
			    {
			      int testmask = 2;
			      int test =
				abs (row_ptrs[component_number][0]
				     [block_number][i]);
			      if (test >= 4)
				{
				  while (testmask)
				    {
				      if (test & testmask)
					mask[bit_count] =
					  mask[bit_count] | testmask;
				      testmask = testmask >> 1;
				    }
				  bit_count++;
				  if (bit_count == (b_val / 2))
				    set_data = 1;
				}
			    }
			  else
			    coeficientes[component_number][row_number]
			      [block_number][i] =
			      row_ptrs[component_number][0][block_number][i];
			}
		      else
			coeficientes[component_number][row_number]
			  [block_number][i] =
			  row_ptrs[component_number][0][block_number][i];
		    }

		}
	    }

	}
    }


  i = 0;
  l = 0;
  while (i < bit_count)
    {
      int maskdec = 0x80;
      if (!testmask)
	testmask = 0x02;
      while (maskdec)
	{
	  if (mask[i] & testmask)
	    rawdata[l] += maskdec;
	  testmask >>= 1;
	  if (!testmask)
	    {
	      i++;
	      testmask = 0x02;
	    }

	  maskdec >>= 1;
	}
      write (fdout, &irbox[rawdata[l]], 1);
  //    write (fdout, &rawdata[l], 1);
      l++;
    }

  if(b64) {
  fdb64 = open(outfile,O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
  fstat(fdout,&jpeg_info);
  fstat(fdb64,& b64_info);
  lseek(fdout,0,SEEK_SET);
  posix_fallocate(fdb64,0,((int)ceil(jpeg_info.st_size/3)*4) + 10);
  ptr_b64 = mmap(NULL, ((int)ceil(jpeg_info.st_size/3)*4) + 10,PROT_READ|PROT_WRITE,MAP_SHARED,fdb64,0);
  ptr_jpeg= mmap(NULL,jpeg_info.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fdout,0);
  
  b64_bytes= Base64Encode(ptr_jpeg,jpeg_info.st_size,ptr_b64);
  ftruncate(fdb64,b64_bytes);
  close(fdb64);
  munmap(ptr_b64 ,b64_bytes);
  munmap(ptr_jpeg,jpeg_info.st_size);
  } 



  (void) jpeg_finish_decompress (&srcinfo);
  jpeg_destroy_decompress (&srcinfo);
  if(b64)
  remove(tmname); 

  return 1;
}
