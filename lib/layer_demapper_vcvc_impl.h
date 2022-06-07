#ifndef INCLUDED_LTE_LAYER_DEMAPPER_VCVC_IMPL_H
#define INCLUDED_LTE_LAYER_DEMAPPER_VCVC_IMPL_H

#include <gnuradio/lte/layer_demapper_vcvc.h>

namespace gr {
  namespace lte {

    class layer_demapper_vcvc_impl : public layer_demapper_vcvc
    {
     private:
        int d_N_ant;
        int d_vlen;
        std::string d_style;

        void handle_msg(pmt::pmt_t msg);

        void demap_1_ant(gr_complex* out, const gr_complex * in, int len);
        void demap_2_ant(gr_complex* out, const gr_complex * in, int len);
        void demap_4_ant(gr_complex* out, const gr_complex * in, int len);
      
     public:
        layer_demapper_vcvc_impl(int N_ant, int vlen, std::string style, std::string name);
        ~layer_demapper_vcvc_impl();

        // Where all the action really happens
        int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
        );

        void set_N_ant(int N_ant);
        int get_N_ant(){return d_N_ant;}
        void set_decoding_style(std::string style);
        std::string get_decoding_style(){return d_style;}      
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_LAYER_DEMAPPER_VCVC_IMPL_H */
