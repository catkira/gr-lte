#include <gnuradio/io_signature.h>
#include "repeat_message_source_vf_impl.h"

namespace gr {
  namespace lte {

    using output_type = float;
    repeat_message_source_vf::sptr
    repeat_message_source_vf::make(int vector_len, std::string name)
    {
      return gnuradio::make_block_sptr<repeat_message_source_vf_impl>(
        vector_len, name);
    }


    /*
     * The private constructor
     */
    repeat_message_source_vf_impl::repeat_message_source_vf_impl(int vector_len, std::string name)
      : gr::sync_block("repeat_message_source_vf",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type) * vector_len)),
              d_vector_len(vector_len),
              d_is_initialized(false)              
    {
		d_port = pmt::string_to_symbol("vector");
		message_port_register_in(d_port);
		set_msg_handler(pmt::mp("vector"), [this](const pmt::pmt_t& msg){ handle_msg(msg);});        
    }

    /*
     * Our virtual destructor.
     */
    repeat_message_source_vf_impl::~repeat_message_source_vf_impl()
    {
    }

    int
    repeat_message_source_vf_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        auto out = static_cast<output_type*>(output_items[0]);
        if(d_is_initialized){
			//~ printf("produce output\n");
			for(int i = 0 ; i < noutput_items ; i++ ){
				memcpy(out, d_out_vector, sizeof(float) * d_vector_len);
				out += d_vector_len;
			}
		}
		else{
			return 0;
		}
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }
    
    void
    repeat_message_source_vf_impl::handle_msg(pmt::pmt_t msg)
    {
		if(pmt::length(msg) != d_vector_len){
			printf("WARNING! Message list length != vector length\n");
			return;
		}
		printf("received message vector\n");

		d_out_vector = new float[d_vector_len];
		for(int c = 0; c < pmt::length(msg); c++ ){
			float val = (float) pmt::to_double( pmt::nth(c, msg) );
			d_out_vector[c] = val;
			//~ printf("%+1.2f\t%+1.2f\tdiff = %+1.2f\n", d_out_vector[c], val, val - d_out_vector[c]);
		}
		d_is_initialized = true;
	}    

  } /* namespace lte */
} /* namespace gr */
