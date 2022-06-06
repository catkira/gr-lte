#ifndef INCLUDED_LTE_SSS_SYMBOL_SELECTOR_CVC_IMPL_H
#define INCLUDED_LTE_SSS_SYMBOL_SELECTOR_CVC_IMPL_H

#include <gnuradio/lte/sss_symbol_selector_cvc.h>

namespace gr {
  namespace lte {

    class sss_symbol_selector_cvc_impl : public sss_symbol_selector_cvc
    {
     private:
        int d_fftl;
        int d_cpl;
        int d_cpl0;
        int d_slotl;
        int d_slot_num;
        int d_sym_num;
        int d_N_id_2;
        uint64_t d_abs_pos;
        uint64_t d_offset;
        pmt::pmt_t d_key;
        pmt::pmt_t d_id_key;
        pmt::pmt_t d_tag_id;
        
     public:
      sss_symbol_selector_cvc_impl(int fftl, std::string name);
      ~sss_symbol_selector_cvc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_SSS_SYMBOL_SELECTOR_CVC_IMPL_H */
