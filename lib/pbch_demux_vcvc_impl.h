#ifndef INCLUDED_LTE_PBCH_DEMUX_VCVC_IMPL_H
#define INCLUDED_LTE_PBCH_DEMUX_VCVC_IMPL_H

#include <gnuradio/lte/pbch_demux_vcvc.h>

namespace gr {
  namespace lte {

    class pbch_demux_vcvc_impl : public pbch_demux_vcvc
    {
     private:
		int d_cell_id;
		int d_N_rb_dl;
		int d_sym_num;
		int d_rxant;
		gr_complex* d_pbch_symbs;
		gr_complex* d_pbch_ce1_symbs;
		gr_complex* d_pbch_ce2_symbs;

		int calculate_n_process_items(gr_vector_int ninput_items, int noutput_items);
		void extract_pbch_values(gr_complex* out, const gr_complex* in);
		int get_sym_num(std::vector<gr::tag_t> &v);

		void set_cell_id_msg(pmt::pmt_t msg);
        
     public:
        pbch_demux_vcvc_impl(int N_rb_dl, int rxant, std::string name);
        ~pbch_demux_vcvc_impl();

        void set_cell_id(int id);      

        // Where all the action really happens
        void forecast (int noutput_items, gr_vector_int &ninput_items_required);

        int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_PBCH_DEMUX_VCVC_IMPL_H */
