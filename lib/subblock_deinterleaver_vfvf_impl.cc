#include <gnuradio/io_signature.h>
#include "subblock_deinterleaver_vfvf_impl.h"

namespace gr {
  namespace lte {

    using input_type = float;
    using output_type = float;
    subblock_deinterleaver_vfvf::sptr
    subblock_deinterleaver_vfvf::make(int num_groups, int items_per_group, std::string name)
    {
      return gnuradio::make_block_sptr<subblock_deinterleaver_vfvf_impl>(
        num_groups, items_per_group, name);
    }


    /*
     * The private constructor
     */
    subblock_deinterleaver_vfvf_impl::subblock_deinterleaver_vfvf_impl(int num_groups, int items_per_group, std::string name)
      : gr::sync_block("subblock_deinterleaver_vfvf",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {
		d_interleaved_pos = subblock_interleaver_prototype(num_groups);        
    }

    const int subblock_deinterleaver_vfvf_impl::d_perm_vec[] = {1,17,9,25,5,21,13,29,3,19,11,27,7,23,15,31,0,16,8,24,4,20,12,28,2,18,10,26,6,22,14,30};

    /*
     * Our virtual destructor.
     */
    subblock_deinterleaver_vfvf_impl::~subblock_deinterleaver_vfvf_impl()
    {
    }

    int
    subblock_deinterleaver_vfvf_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        auto in = static_cast<const input_type*>(input_items[0]);
        auto out = static_cast<output_type*>(output_items[0]);

		for (int i = 0 ; i < noutput_items ; i++ ){
			for(int c = 0 ; c < d_num_groups ; c++){
				memcpy(out+d_interleaved_pos[c]*d_items_per_group, in+c*d_items_per_group, sizeof(float)*d_items_per_group );
			}
			in += (d_num_groups*d_items_per_group);
			out += (d_num_groups*d_items_per_group);
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }
    

	std::vector<int>
	subblock_deinterleaver_vfvf_impl::subblock_interleaver_prototype(int len)
	{
		// Set and Calculate variables according to standard.
		int csub = 32;
		int rsub = (int) ceil(1.0*len/csub);
		int Nd   = csub * rsub - len;
		
		// Initialize vector array with zeros for NULL symbols
		__GR_VLA(unsigned int, y, rsub*csub);
		for (int i=0;i<Nd;i++){
			y[i]=0;	
		}
		
		// Initialize vector array with non-zero values according to its position.
		for (int i=Nd;i<csub*rsub;i++){
			y[i]=i-Nd+1;
		}
		
		// Transform vector array y into matrix with 32 columns (csub) rowwise.
		unsigned int ** mat = new unsigned int *[rsub];
		for (int i = 0; i < rsub; i++) mat[i] = new unsigned int[csub];

		for (int r = 0;r<rsub;r++){
			for (int c=0;c<csub;c++){
				mat[r][c]=y[csub*r+c];
			}	
		}
			
		// Perform interleaving on matrix according to standard.
		unsigned int ** interl_mat = new unsigned int *[rsub];
		for (int i = 0; i < rsub; i++) interl_mat[i] = new unsigned int[csub];
		for(int r = 0 ; r < rsub ; r++ ){
			for (int i=0;i<csub;i++){
				interl_mat[r][i]=mat[r][d_perm_vec[i]];
			}	
		}
		for (int i = 0; i < rsub; i++) delete[] mat[i];
		delete[] mat;

		// Read out matrix to vector columnwise.
		__GR_VLA(unsigned int, v, rsub * csub);
		for (int c=0;c<csub;c++){
			for (int r=0;r<rsub;r++){
				v[rsub*c+r]=interl_mat[r][c];
			}
		}
		for (int i = 0; i < rsub; i++) delete[] interl_mat[i];
		delete[] interl_mat;
		
		int idx = 0;
		std::vector<int> interleaved_pos;
		for (int i=0;i<csub*rsub;i++){
			if (v[i] != 0) {
				//~ d_interl_pos[idx] = v[i]-1;
				idx++;
				interleaved_pos.push_back(v[i]-1);
			}
		}
		return interleaved_pos;
	}
    

  } /* namespace lte */
} /* namespace gr */
