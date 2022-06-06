#ifndef INCLUDED_LTE_PSS_SYMBOL_SELECTOR_CVC_IMPL_H
#define INCLUDED_LTE_PSS_SYMBOL_SELECTOR_CVC_IMPL_H

#include <gnuradio/lte/pss_symbol_selector_cvc.h>

namespace gr {
  namespace lte {

    class pss_symbol_selector_cvc_impl : public pss_symbol_selector_cvc
    {
     private:
        int d_fftl;
        int d_cpl;
        int d_cpl0;
        int d_slotl;
        int d_syml;
        int d_syml0;

        long d_offset;
        long d_sym_pos;
        long d_ass_half_frame_start;
        int d_off_sym_count;
        int d_work_call;
        pmt::pmt_t d_key;
        pmt::pmt_t d_sym_key;
        pmt::pmt_t d_tag_id;
        bool d_is_locked;
        
        void produce_output(gr_complex *&out, const gr_complex *in, long abs_pos, int &nout);
        void handle_msg_lock(pmt::pmt_t msg);
        void handle_msg_half_frame_start(pmt::pmt_t msg);        
        
     public:
        pss_symbol_selector_cvc_impl(int fftl, std::string name);
        ~pss_symbol_selector_cvc_impl();

        // Where all the action really happens
        void forecast (int noutput_items, gr_vector_int &ninput_items_required);

        int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
           
        void set_half_frame_start(long start){d_ass_half_frame_start = start;}           
        void lock(){d_is_locked = true;}
        void unlock(){d_is_locked = false;}           

    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_PSS_SYMBOL_SELECTOR_CVC_IMPL_H */
