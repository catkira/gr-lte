#include <gnuradio/io_signature.h>
#include "pcfich_demux_vcvc_impl.h"

namespace gr {
  namespace lte {

    using input_type = gr_complex;
    using output_type = gr_complex;
    pcfich_demux_vcvc::sptr
    pcfich_demux_vcvc::make(int N_rb_dl, std::string key, std::string out_key, std::string msg_buf_name, std::string name)
    {
      return gnuradio::make_block_sptr<pcfich_demux_vcvc_impl>(
        N_rb_dl, key, out_key, msg_buf_name, name);
    }


    /*
     * The private constructor
     */
    pcfich_demux_vcvc_impl::pcfich_demux_vcvc_impl(int N_rb_dl, std::string key, std::string out_key, std::string msg_buf_name, std::string name)
      : gr::block("pcfich_demux_vcvc",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type) * 12 * N_rb_dl),
  gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type) * 16)),
              d_cell_id(0),
              d_sym_num(0),
              d_N_rb_dl(N_rb_dl)              
    {
        d_key = pmt::string_to_symbol(key); // specify key of incoming tag.
        d_out_key = pmt::string_to_symbol(out_key); // key for new tags.
        d_tag_id = pmt::string_to_symbol( this->name() );
        d_msg_buf = pmt::mp(msg_buf_name);
        message_port_register_in(d_msg_buf);
        set_msg_handler(d_msg_buf, [this](const pmt::pmt_t& msg){ handle_msg(msg);});        

        //set_tag_propagation_policy(TPP_DONT);

        set_cell_id(d_cell_id);        
    }

    /*
     * Our virtual destructor.
     */
    pcfich_demux_vcvc_impl::~pcfich_demux_vcvc_impl()
    {
    }

    void
    pcfich_demux_vcvc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        const int nsyms_in_subframe = 14; // make it constant here for now.
        for(int i = 0; i < ninput_items_required.size(); i++){
            ninput_items_required[i] = nsyms_in_subframe;
        }
    }

    int
    pcfich_demux_vcvc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        auto in0 = static_cast<const input_type*>(input_items[0]);
        auto out0 = static_cast<output_type*>(output_items[0]);

        // In this case for configuration overview some variables declared right at the beginning.
        int out_port = 0;
        int n_subcarriers = 12*d_N_rb_dl;
        int ninitems = calculate_n_process_items(ninput_items);
        int nout_items = 0;

        std::vector<gr::tag_t> tags;
        get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0)+ninitems, d_key);
        int sym_num = get_sym_num(tags);

        for(int i = 0; i < ninitems && nout_items < noutput_items; i++){
            int curr_sym = sym_num+i;
            if((curr_sym)%14 == 0){
                int subframe = (curr_sym/14)%10;
                // copy REs of interest from input to output buffer
                extract_pcfich(out0, in0);
                // produced output. move output pointers.
                out0 += 16;

                // This should tag every sample with a tag containing a value which corresponds to the subframe!
                long offset = nitems_written(0) + nout_items;
                //printf("sym_num  = %i\td_sym_num = %i\tsubframe = %i\toffset = %ld\n", sym_num, d_sym_num, subframe, offset);
                pmt::pmt_t value = pmt::from_long(long(subframe));
                //printf("port = %i\toffset = %ld\tkey = %s\tvalue = %ld\tid = %s\n", out_port, offset, pmt::pmt_symbol_to_string(d_out_key).c_str(), pmt::pmt_to_long(value), pmt::pmt_symbol_to_string(d_tag_id).c_str() );
                add_item_tag(out_port, offset, d_out_key, value, d_tag_id);

                nout_items++; // produced output!
            }
            //move input buffer pointers after every iteration.
            in0 += n_subcarriers;
            d_sym_num = (d_sym_num+1)%140;
        }

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each(ninitems);

        // Tell runtime system how many output items we produced.
        return nout_items;
    }
    
void
    pcfich_demux_vcvc_impl::update_pcfich_pos(int N_rb_dl, int cell_id)
    {
        std::vector<int> pos;
        const int N_SC_RB = 12;
        const int K_MEAN = (N_SC_RB/2) * (cell_id%(2*N_rb_dl));
        int n_subcarriers = N_rb_dl*N_SC_RB;
 
        int cell_id_mod3 = cell_id%3;
        for(int n = 0; n < 4; n++){
            int k = int( (K_MEAN + (N_SC_RB/2) * (n*(N_rb_dl/2) ) )%n_subcarriers );
            for(int i = 0; i < 6; i++){
                if(i%3 != cell_id_mod3){
                    pos.push_back(k+i);
                }
            }
        }
        d_pcfich_pos.clear();
        d_pcfich_pos = pos;
    }
    
    int
    pcfich_demux_vcvc_impl::calculate_n_process_items(gr_vector_int ninput_items)
    {
        // get smallest number of input items
        int n_inputs = ninput_items.size();
        int ninitems = ninput_items[0];
        for(int i = 1 ; i < n_inputs ; i++){
            if(ninitems > ninput_items[i]){
                ninitems = ninput_items[i];
            }
        }
        return ninitems;
    }
    
    inline int
    pcfich_demux_vcvc_impl::get_sym_num(std::vector<gr::tag_t> v)
    {
        int sym_num = 0;
        if(v.size() > 0){
            int value = int(pmt::to_long(v[0].value) );
            int rel_offset = v[0].offset - nitems_read(0);
            sym_num = (value+140-rel_offset)%140;
        }
        else{
            sym_num = d_sym_num;
        }
        return sym_num;
    }
    
    void
    pcfich_demux_vcvc_impl::extract_pcfich(gr_complex* out0, const gr_complex* in0)
    {
        int len = d_pcfich_pos.size();
        for(int i = 0; i < len; i++){
            *out0 = *(in0 + d_pcfich_pos[i]);
            out0++;
        }
    }    

  } /* namespace lte */
} /* namespace gr */
