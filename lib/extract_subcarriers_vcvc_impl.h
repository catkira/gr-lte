#ifndef INCLUDED_LTE_EXTRACT_SUBCARRIERS_VCVC_IMPL_H
#define INCLUDED_LTE_EXTRACT_SUBCARRIERS_VCVC_IMPL_H

#include <gnuradio/lte/extract_subcarriers_vcvc.h>

namespace gr {
  namespace lte {

    class extract_subcarriers_vcvc_impl : public extract_subcarriers_vcvc
    {
     private:
		int d_N_rb_dl;
		int d_fftl;

     public:
      extract_subcarriers_vcvc_impl(int N_rb_dl, int fftl, std::string name);
      ~extract_subcarriers_vcvc_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_EXTRACT_SUBCARRIERS_VCVC_IMPL_H */
