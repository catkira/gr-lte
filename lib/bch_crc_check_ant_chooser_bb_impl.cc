#include <gnuradio/io_signature.h>
#include "bch_crc_check_ant_chooser_bb_impl.h"

namespace gr {
  namespace lte {

    using input_type = char;
    using output_type = char;
    bch_crc_check_ant_chooser_bb::sptr
    bch_crc_check_ant_chooser_bb::make(std::string name)
    {
      return gnuradio::make_block_sptr<bch_crc_check_ant_chooser_bb_impl>(
        name);
    }


    /*
     * The private constructor
     */
    bch_crc_check_ant_chooser_bb_impl::bch_crc_check_ant_chooser_bb_impl(std::string name)
      : gr::sync_block("bch_crc_check_ant_chooser_bb",
              gr::io_signature::make(3 /* min inputs */, 3 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {}

    /*
     * Our virtual destructor.
     */
    bch_crc_check_ant_chooser_bb_impl::~bch_crc_check_ant_chooser_bb_impl()
    {
    }

    int
    bch_crc_check_ant_chooser_bb_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        auto in1 = static_cast<const input_type*>(input_items[0]);
        auto in2 = static_cast<const input_type*>(input_items[1]);
        auto in4 = static_cast<const input_type*>(input_items[2]);
        auto out = static_cast<output_type*>(output_items[0]);
        
        //~ printf("call to selector %i\n", noutput_items);
        int ant = 0;
        for(int i = 0 ; i < noutput_items ; i++){
			if( (int) *in1 == 1){ ant = 1;}
			else if( (int) *in2 == 1){ ant = 2;}
			else if( (int) *in4 == 1){ ant = 4;}
			else{ ant = 0;}
			*out = ant;
			//~ printf("ant1 = %i\tant2 = %i\t ant4 = %i\tdetected antennas %i\n", (int)*in1, (int)*in2, (int)*in4, ant );
			in1++;
			in2++;
			in4++;
			out++;
		}
		//~ printf("call to selector processed items = %i\n", noutput_items);
		
        return noutput_items;
    }

  } /* namespace lte */
} /* namespace gr */
