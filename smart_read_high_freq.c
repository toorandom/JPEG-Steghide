#include "defs.h"

void
smart_read_high_frequency (unsigned char *bin_string, unsigned char *mask, int cbytes, int b_mask, int b_len)
{
  int a_len=3,k=0,l=0, len_set = 0, data_set = 0, counter_bit = 0, a_mask = 0x08, test, test_mask = 0x02;
  for (component_number = 0; component_number < srcinfo.num_components;
       component_number++)
    {
      block_size_row =
	(size_t) sizeof (JCOEF) * DCTSIZE2 *
	srcinfo.comp_info[component_number].width_in_blocks;

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
	    {
	      k = -1;
	      l = 0;
	      for (i = 0; i < DCTSIZE2; i++)
		{
		  if (!len_set)
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

			  if (a_len == 0 && b_len == 0)
			    len_set = 1;
			  if (a_len != 0)
			    {
			      a_mask = 0x08;
			      a_len = 0;
			      test =
				abs (row_ptrs[component_number][0]
				     [block_number][i]);
			      test >>= 4;
			      for (k = 0; k < 4; k++)
				{
				  test <<= 1;
				  if (a_mask & b_len)
				    test |= 1;
				  a_mask >>= 1;
				}
			    }
			  else
			    {
			      b_len -= 1;
			      test =
				abs (row_ptrs[component_number][0]
				     [block_number][i]);
			      test >>= 1;
			      test <<= 1;
			      if (cbytes & b_mask)
				test |= 1;
			      b_mask >>= 1;
			    }

			  if (row_ptrs[component_number][0][block_number]
			      [i] < 0)
			    coeficientes[component_number][row_number]
			      [block_number][i] = -test;

			  else
			    coeficientes[component_number][row_number]
			      [block_number][i] = test;

			}
		      else
			coeficientes[component_number][row_number]
			  [block_number][i] =
			  row_ptrs[component_number][0][block_number][i];
		    }
		  else
		    {
		      if (!data_set)
			{
			  if ((i > 0)
			      &&
			      (row_ptrs[component_number][0][block_number]
			       [i] != 0)
			      &&
			      (abs
			       (row_ptrs[component_number][0]
				[block_number][i]) > 1))
			    {

			      k = 0;
			      test =
				abs (row_ptrs[component_number][0]
				     [block_number][i]);
			      if (test >= 4)
				{

				  test_mask = 0x02;
				  test >>= 2;
				  for (k = 0; k < 2; k++)
				    {
				      test <<= 1;
				      if (mask[counter_bit] & test_mask)
					test |= 0x01;
				      test_mask >>= 1;
				    }
				  if (test != 0 && test != 1)
				    {
				      if (row_ptrs[component_number][0]
					  [block_number][i] < 0)
					coeficientes[component_number]
					  [row_number][block_number][i] =
					  -test;
				      else
					coeficientes[component_number]
					  [row_number][block_number][i] =
					  test;
				      counter_bit++;
				    }
				}
			      if (counter_bit == (strlen ((char *)bin_string) / 2))
				data_set = 1;

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
}
