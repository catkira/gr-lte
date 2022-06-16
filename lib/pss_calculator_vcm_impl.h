#ifndef INCLUDED_LTE_PSS_CALCULATOR_VCM_IMPL_H
#define INCLUDED_LTE_PSS_CALCULATOR_VCM_IMPL_H

#include <gnuradio/lte/pss_calculator_vcm.h>
#include <gnuradio/lte/correlator.h>

namespace gr {
  namespace lte {

    class pss_calculator_vcm_impl : public pss_calculator_vcm
    {
     private:
        static const gr_complex d_C_I;
        static const float d_PI;
        int d_fftl;
        int d_cpl;
        int d_cpl0;
        int d_slotl;
        int d_N_id_2;
        long d_half_frame_start;
        float d_corr_val;
        int d_lock_count;
        bool d_is_locked;
        gr_complex d_chu0[63];
        gr_complex d_chu1[63];
        gr_complex d_chu2[63];
        
        pmt::pmt_t d_port_lock;
        pmt::pmt_t d_port_half_frame_start;
        pmt::pmt_t d_port_N_id_2;

        std::vector<float> d_corr_vec;

        void zc(gr_complex *zc, int cell_id); // used to generate Zadoff-Chu sequences
        bool find_pss_symbol(gr_complex *chuX); // prepares the calculation stuff etc.
        bool tracking(gr_complex *chu);
        void max_pos(float &max, int &pos, gr_complex *x,gr_complex *y, int len); //finds maximum of one correlation

        inline void set_half_frame_start();
        inline long calculate_half_frame_start(long pos);
        inline void extract_pss(gr_complex *chu, const gr_complex *in);
        // attributes for correlation
        correlator *d_correlator;
        gr_complex *d_corr_in1;
        gr_complex *d_corr_in2;
        gr_complex *d_corr_out;        
        
     public:
      pss_calculator_vcm_impl(int fftl, std::string name);
      ~pss_calculator_vcm_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_PSS_CALCULATOR_VCM_IMPL_H */
