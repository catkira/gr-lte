#include <gnuradio/io_signature.h>
#include "qpsk_soft_demod_vcvf_impl.h"
#include <fftw3.h>
#include <volk/volk.h>
#include <cmath>

namespace gr {
  namespace lte {

    using input_type = gr_complex;
    using output_type = float;
    qpsk_soft_demod_vcvf::sptr
    qpsk_soft_demod_vcvf::make(int vlen, std::string name)
    {
      return gnuradio::make_block_sptr<qpsk_soft_demod_vcvf_impl>(
        vlen, name);
    }


    /*
     * The private constructor
     */
    qpsk_soft_demod_vcvf_impl::qpsk_soft_demod_vcvf_impl(int vlen, std::string name)
      : gr::sync_block("qpsk_soft_demod_vcvf",
                gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type) * vlen),
                gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type) * 2 * vlen)),
                d_vlen(vlen),
                d_SQRT2( std::sqrt(2) )              
    {
        d_demodulated = (float*)fftwf_malloc(sizeof(float)*2*d_vlen);        
    }

    /*
     * Our virtual destructor.
     */
    qpsk_soft_demod_vcvf_impl::~qpsk_soft_demod_vcvf_impl()
    {
    }

    int
    qpsk_soft_demod_vcvf_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        auto in = static_cast<const input_type*>(input_items[0]);
        auto out = static_cast<output_type*>(output_items[0]);

        for(int i = 0 ; i < noutput_items ; i++){
            memcpy(d_demodulated, (float*) in,  sizeof(float) * 2 * d_vlen);
            volk_32f_s32f_multiply_32f_u(d_demodulated, d_demodulated, d_SQRT2, (2*d_vlen));
            memcpy(out, d_demodulated, sizeof(float)*(2*d_vlen));
            in += d_vlen;
            out += (2*d_vlen);
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lte */
} /* namespace gr */
