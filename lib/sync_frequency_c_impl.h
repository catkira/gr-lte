#ifndef INCLUDED_LTE_SYNC_FREQUENCY_C_IMPL_H
#define INCLUDED_LTE_SYNC_FREQUENCY_C_IMPL_H

#include <gnuradio/lte/sync_frequency_c.h>

namespace gr {
  namespace lte {

    class sync_frequency_c_impl : public sync_frequency_c
    {
     private:
        std::shared_ptr<gr::analog::sig_source<gr_complex>> d_sig;
        int d_fftl;
        int d_cpl;
        int d_cpl0;
        int d_slotl;
        int d_samp_rate;
        int d_offset;
        gr_complex* d_buffer;

        float d_f_av;
        int d_samp_num;
        float d_known_f_offset;
        int d_work_call;

        //methods for further calculations
        void calc_f_off_av();

        gr_complex corr(gr_complex *res, gr_complex *x, gr_complex *y, int len);

     public:
      sync_frequency_c_impl(std::shared_ptr<gr::analog::sig_source<gr_complex>> &sig, int fftl, float known_f_offset, std::string name);
      ~sync_frequency_c_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_SYNC_FREQUENCY_C_IMPL_H */
