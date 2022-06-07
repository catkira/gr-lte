#ifndef INCLUDED_LTE_QPSK_SOFT_DEMOD_VCVF_IMPL_H
#define INCLUDED_LTE_QPSK_SOFT_DEMOD_VCVF_IMPL_H

#include <gnuradio/lte/qpsk_soft_demod_vcvf.h>

namespace gr {
  namespace lte {

    class qpsk_soft_demod_vcvf_impl : public qpsk_soft_demod_vcvf
    {
     private:
		int d_vlen;
		const float d_SQRT2;
		float* d_demodulated;
        
     public:
      qpsk_soft_demod_vcvf_impl(int vlen, std::string name);
      ~qpsk_soft_demod_vcvf_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_QPSK_SOFT_DEMOD_VCVF_IMPL_H */
