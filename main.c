#include "defs.h"

int
main (int argc, char **argv)
{
  char *inputjpeg, *outputjpeg, *outfile, *datahide, *tmjpeg;
  unsigned char mask[320000];
  unsigned char rawdata[350000];
  FILE *input_fp;
  unsigned char *bin_string;
  int i64=0,cbytes, b_mask, b_len, b64 = 0;


  memset (&mask, 0, sizeof (mask));
  memset (&rawdata, 0, sizeof (rawdata));

  if (argc < 2)
    {
      printf
	("Para insertar secreto.ext en imagen.jpg:\n%s -i imagen.jpg secreto.ext salida-steg.jpg\n\nPara obtener secreto.ext de imagen-steg.jpg:\n%s -g imagen-steg.jpg secreto.ext\n%s -i64 imagen.jpg.base64 secreto.ext salida-steg.jpg\n",
	 argv[0], argv[0],argv[0]);
      return -1;
    }
  if ( (strcmp (argv[1], "-i") == 0) )
    {

      if (argc < 5)
	{

	  printf ("Uso : \n");
	  printf ("%s -i imagen.jpg mensaje.ext salida.jpg <-b64> \n",
		  argv[0]);
	  return -1;
	}

      inputjpeg = argv[2] ;
      datahide = argv[3];
      outputjpeg = argv[4];

      inicia_jpeg ();
      input_fp = mapea_jpeg_input_params (inputjpeg);
      bin_string =
	convert_bin_delinearize (datahide, (unsigned char *) &mask, &cbytes);
      b_len = setup_fourier_io_bitmask_size (cbytes);
      b_mask = pow (2, b_len);
      smart_read_high_frequency (bin_string, (unsigned char *) &mask, cbytes,
				 b_mask, b_len);
      free (bin_string);

      if (argv[5] != NULL)
	if (strcmp (argv[5], "-b64") == 0)
	  {
	    b64 = 1;
	    fprintf (stderr, "Output is base64\n");
	  }

      smart_write_jpeg (outputjpeg, b64);
      fclose (input_fp);
      return 0;
    }

  if ( (strcmp (argv[1], "-g") == 0) || (strcmp (argv[1], "-g64") == 0) )
    {
      if (argc < 4)
	{
	  printf ("Uso : \n");
	  printf ("%s -g<64> imagen.jpg<.b64> salida.out <-b64>\n", argv[0]);
	  return -1;
	}


      if((strcmp (argv[1], "-g64") == 0) )
      i64 = 1;

      tmjpeg = getfilefromb64 (argv[2]);
      inputjpeg = i64 == 0 ? argv[2] : tmjpeg  ;
      outfile = argv[3];
      inicia_jpeg ();
      if (argv[4] != NULL)
	if (strcmp (argv[4], "-b64") == 0)
	  {
	    b64 = 1;
	    fprintf (stderr, "Output is base64\n");
	  }

      jpeg_extract_secrets (inputjpeg, outfile, (unsigned char *) &mask,
			    (unsigned char *) &rawdata, b64);
      remove(tmjpeg);

      return 0;
    }







  return -1;

}
