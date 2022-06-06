#include <gnuradio/io_signature.h>
#include "extract_subcarriers_vcvc_impl.h"

namespace gr {
  namespace lte {

    using input_type = gr_complex;
    using output_type = gr_complex;
    extract_subcarriers_vcvc::sptr
    extract_subcarriers_vcvc::make(int N_rb_dl, int fftl, std::string name)
    {
      return gnuradio::make_block_sptr<extract_subcarriers_vcvc_impl>(
        N_rb_dl, fftl, name);
    }


    /*
     * The private constructor
     */
    extract_subcarriers_vcvc_impl::extract_subcarriers_vcvc_impl(int N_rb_dl, int fftl, std::string name)
      : gr::sync_block("extract_subcarriers_vcvc",
                gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type) * fftl),
                gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type) * 12 * N_rb_dl)),
                d_N_rb_dl(N_rb_dl),
                d_fftl(fftl)              
    {}

    /*
     * Our virtual destructor.
     */
    extract_subcarriers_vcvc_impl::~extract_subcarriers_vcvc_impl()
    {
    }

    int
    extract_subcarriers_vcvc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        auto in = static_cast<const input_type*>(input_items[0]);
        auto out = static_cast<output_type*>(output_items[0]);

        //~ int N_rb_dl = d_N_rb_dl;
		//~ int fftl = d_fftl;
		int h_byte_tones_num = sizeof(gr_complex) * 6 * d_N_rb_dl;
		
		for (int i = 0; i < noutput_items; i ++){
			memcpy(out, in+(d_fftl-(6*d_N_rb_dl)), h_byte_tones_num );
			memcpy(out+(6*d_N_rb_dl), in+1, h_byte_tones_num);
		
			in  += d_fftl;
			out += 12 * d_N_rb_dl;
		}

		// Tell runtime system how many output items we produced.
		return noutput_items;      
    }

  } /* namespace lte */
} /* namespace gr */
