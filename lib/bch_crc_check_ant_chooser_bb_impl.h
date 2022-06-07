#ifndef INCLUDED_LTE_BCH_CRC_CHECK_ANT_CHOOSER_BB_IMPL_H
#define INCLUDED_LTE_BCH_CRC_CHECK_ANT_CHOOSER_BB_IMPL_H

#include <gnuradio/lte/bch_crc_check_ant_chooser_bb.h>

namespace gr {
  namespace lte {

    class bch_crc_check_ant_chooser_bb_impl : public bch_crc_check_ant_chooser_bb
    {
     private:
      // Nothing to declare in this block.

     public:
      bch_crc_check_ant_chooser_bb_impl(std::string name);
      ~bch_crc_check_ant_chooser_bb_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_BCH_CRC_CHECK_ANT_CHOOSER_BB_IMPL_H */
