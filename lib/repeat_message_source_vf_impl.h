#ifndef INCLUDED_LTE_REPEAT_MESSAGE_SOURCE_VF_IMPL_H
#define INCLUDED_LTE_REPEAT_MESSAGE_SOURCE_VF_IMPL_H

#include <gnuradio/lte/repeat_message_source_vf.h>

namespace gr {
  namespace lte {

    class repeat_message_source_vf_impl : public repeat_message_source_vf
    {
     private:
        int d_vector_len;
        bool d_is_initialized;
        float* d_out_vector;
        pmt::pmt_t d_port;

        void handle_msg(pmt::pmt_t msg);

     public:
      repeat_message_source_vf_impl(int vector_len, std::string name);
      ~repeat_message_source_vf_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_REPEAT_MESSAGE_SOURCE_VF_IMPL_H */
