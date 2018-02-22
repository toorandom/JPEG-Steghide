#include "defs.h"


void
inicia_jpeg ()
{
  srcinfo.err = jpeg_std_error (&jsrcerr);
  jpeg_create_decompress (&srcinfo);
  dstinfo.err = jpeg_std_error (&jdsterr);
  jpeg_create_compress (&dstinfo);
  jsrcerr.trace_level = jdsterr.trace_level;
  srcinfo.mem->max_memory_to_use = dstinfo.mem->max_memory_to_use;

  return;
}



FILE *
mapea_jpeg_input_params (char *f)
{
  FILE *archivo_entrada;
  archivo_entrada = fopen (f, "rb");
  if (archivo_entrada == NULL)
    {
      perror ("fopen");
      exit (EXIT_FAILURE);
    }
  jpeg_stdio_src (&srcinfo, archivo_entrada);
  jpeg_read_header (&srcinfo, TRUE);
  
  return archivo_entrada;
}

int setup_fourier_io_bitmask_size (int cbytes) {
int elem, temp,b_len = 0;
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
for (elem = 0; elem <= 15; elem++)
  {
    temp = pow (2, elem);
    if (temp > cbytes)
      {
	b_len = elem - 1;
	break;
      }
  }
return b_len;
}
