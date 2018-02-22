#include "defs.h"
char *
getfilefromb64 (char *b64file)
{
  int fd_b64, fd_jpeg, outlen,realsize;
  struct stat b64_info;
  char *ptr_b64, *ptr_jpeg;
  static char tmfile[MAX_TMFILE];
  int i;

  fd_b64 = open (b64file, O_RDONLY);

  if (fd_b64 < 0)
    {
      fprintf(stderr,"ERROR: open %s\n",b64file);
 //     perror ("open");
      exit (EXIT_FAILURE);
    }

  fstat (fd_b64, &b64_info);
  ptr_b64 = mmap (NULL, b64_info.st_size, PROT_READ, MAP_SHARED, fd_b64, 0);
perror("mmap");
  outlen = calcula_tam_base64 (ptr_b64,b64_info.st_size);
  outlen = outlen;
//printf("Expected len %d, infosize=%d\n",outlen,b64_info.st_size);

  snprintf ((char *) &tmfile, MAX_TMFILE, "%s/st-jpeg.%d",TMPDIR, getpid ());

  fd_jpeg = open (tmfile, O_RDWR|O_TRUNC|O_CREAT,S_IRWXU);


  if(fd_jpeg < 0) { perror("open"); exit(-1); }

 // for(i=0;i<outlen;i++) 
	//  write(fd_jpeg,0,sizeof(char));

  posix_fallocate (fd_jpeg, 0, outlen);
  lseek(fd_jpeg,0,SEEK_SET);
  ptr_jpeg =
    mmap (NULL, outlen, PROT_READ | PROT_WRITE, MAP_SHARED, fd_jpeg, 0);

  realsize = Base64Decode (ptr_b64, b64_info.st_size, (unsigned char*)ptr_jpeg);
  lseek(fd_jpeg,0,SEEK_SET);
  ftruncate(fd_jpeg,realsize);

  close (fd_jpeg);
  close (fd_b64);
  munmap (ptr_b64, b64_info.st_size);
  munmap (ptr_jpeg, outlen);
  return (char *) &tmfile;
}
