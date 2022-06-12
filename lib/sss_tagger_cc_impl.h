#ifndef INCLUDED_LTE_SSS_TAGGER_CC_IMPL_H
#define INCLUDED_LTE_SSS_TAGGER_CC_IMPL_H

#include <gnuradio/lte/sss_tagger_cc.h>

namespace gr {
  namespace lte {

    class sss_tagger_cc_impl : public sss_tagger_cc
    {
     private:
        int d_fftl;
        int d_cpl;
        int d_cpl0;
        int d_slotl;
        int d_framel;
        int d_slot_num;
        uint64_t d_offset_0;
        pmt::pmt_t d_key;
        pmt::pmt_t d_tag_id;
        unsigned long d_frame_start;
        
        void set_frame_start(long frame_start);
        void handle_msg_frame_start(pmt::pmt_t msg);
        
     public:
      sss_tagger_cc_impl(int fftl, std::string name);
      ~sss_tagger_cc_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_SSS_TAGGER_CC_IMPL_H */
