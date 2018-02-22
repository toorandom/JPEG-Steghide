#include "defs.h"
void
smart_write_jpeg (char *out, int b64)
{
  FILE *archivo_salida, *tmp;
  char tmname[MAX_TMFILE];
  int bytes_b64;
  int fd_jpeg,fd_b64;
  char *ptr_jpeg, *ptr_b64;
  unsigned char identif = IDENTIFIER;
  struct stat jpeg_info, b64_info;
  archivo_salida = fopen (out, "w+b");
  snprintf((char *)&tmname,sizeof(tmname),"st.%d",getpid());
  if(b64)
  tmp = fopen(tmname,"w+b");
  else 
  tmp = archivo_salida;

  for (component_number = 0; component_number < srcinfo.num_components;
       component_number++)
    {
      block_size_row = (size_t) sizeof (JCOEF) * DCTSIZE2
	* srcinfo.comp_info[component_number].width_in_blocks;
      for (row_number = 0;
	   row_number <
	   srcinfo.comp_info[component_number].height_in_blocks; row_number++)
	{
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
	    for (i = 0; i < DCTSIZE2; i++)
	      row_ptrs[component_number][0][block_number][i] =
		coeficientes[component_number][row_number][block_number][i];
	}
    }
  jpeg_stdio_dest (&dstinfo, tmp);
  jpeg_write_coefficients (&dstinfo, coef_rows);
  jpeg_finish_compress (&dstinfo);
  jpeg_destroy_compress (&dstinfo);
  (void) jpeg_finish_decompress (&srcinfo);
  jpeg_destroy_decompress (&srcinfo);
  fd_jpeg = fileno(tmp);
  lseek(fd_jpeg,IDOFFSET,SEEK_SET);
  write(fd_jpeg,&identif, sizeof(char));


  if(!b64) {
  fclose (tmp);
  return;
  }
  
  lseek(fd_jpeg,0,SEEK_SET); 
  fd_b64 = fileno(archivo_salida); 


  fstat(fd_jpeg,&jpeg_info);
  fstat(fd_b64 ,& b64_info);

  posix_fallocate(fd_b64,0,((int)ceil(jpeg_info.st_size/3)*4) + 10);

  ptr_jpeg = mmap(NULL, jpeg_info.st_size,PROT_READ,MAP_SHARED,fd_jpeg,0);
  ptr_b64  = mmap(NULL, ((int)ceil(jpeg_info.st_size/3)*4) + 10 ,PROT_READ|PROT_WRITE,MAP_SHARED,fd_b64,0);

  bytes_b64 = Base64Encode((const unsigned char*)ptr_jpeg,jpeg_info.st_size,ptr_b64);

  ftruncate(fd_b64,bytes_b64);

  fclose(archivo_salida);
  fclose(tmp);
  munmap(ptr_jpeg, jpeg_info.st_size);
  munmap(ptr_b64, bytes_b64);
  remove(tmname);
}
