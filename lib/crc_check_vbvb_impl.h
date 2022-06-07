#ifndef INCLUDED_LTE_CRC_CHECK_VBVB_IMPL_H
#define INCLUDED_LTE_CRC_CHECK_VBVB_IMPL_H

#include <gnuradio/lte/crc_check_vbvb.h>
#include <boost/crc.hpp>

namespace gr {
  namespace lte {

    class crc_check_vbvb_impl : public crc_check_vbvb
    {
     private:
        int d_exp[16];
        const int d_data_len;
        const int d_final_xor;
        // length checksum, CRC ploynomial, initial state, final XOR, REFLECT Input, REFLECT_REM
        boost::crc_optimal<16, 0x1021, 0x0000, 0x0000, false, false> lte_crc_16;
        // final XOR may be different, but due to template restrictions this is processed outside of this function.
        unsigned char pack_byte(const char* unc);
        
     public:
      crc_check_vbvb_impl(int data_len, int final_xor, std::string name);
      ~crc_check_vbvb_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_CRC_CHECK_VBVB_IMPL_H */
