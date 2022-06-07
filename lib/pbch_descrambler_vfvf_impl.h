#ifndef INCLUDED_LTE_PBCH_DESCRAMBLER_VFVF_IMPL_H
#define INCLUDED_LTE_PBCH_DESCRAMBLER_VFVF_IMPL_H

#include <gnuradio/lte/pbch_descrambler_vfvf.h>

namespace gr {
  namespace lte {

    class pbch_descrambler_vfvf_impl : public pbch_descrambler_vfvf
    {
     private:
		int d_cell_id;
		float* d_pn_seq;
		float* d_scr;
		float* d_comb;
		int d_pn_seq_len;

		pmt::pmt_t d_key;
		pmt::pmt_t d_tag_id;
		int d_work_call;
        
     public:
        pbch_descrambler_vfvf_impl(std::string key);
        ~pbch_descrambler_vfvf_impl();

        // Where all the action really happens
        int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
        );

        void set_cell_id(int id);
        void set_cell_id_msg(pmt::pmt_t msg);

        static char* pn_seq_generator(int len, int cell_id);
        std::vector<int> pn_sequence() const;      
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_PBCH_DESCRAMBLER_VFVF_IMPL_H */
