#include <gnuradio/io_signature.h>
#include "pbch_demux_vcvc_impl.h"

namespace gr {
  namespace lte {

    using input_type = gr_complex;
    using output_type = gr_complex;
    pbch_demux_vcvc::sptr
    pbch_demux_vcvc::make(int N_rb_dl, int rxant, std::string name)
    {
      return gnuradio::make_block_sptr<pbch_demux_vcvc_impl>(
        N_rb_dl, rxant, name);
    }


    /*
     * The private constructor
     */
    pbch_demux_vcvc_impl::pbch_demux_vcvc_impl(int N_rb_dl, int rxant, std::string name)
      : gr::block("pbch_demux_vcvc",
                gr::io_signature::make(1, 1, sizeof(input_type) * 12 * N_rb_dl * rxant),
                gr::io_signature::make(1, 1, sizeof(output_type) * 240 * rxant)),
                d_cell_id(-1),
                d_N_rb_dl(N_rb_dl),
                d_sym_num(-1),
                d_rxant(rxant)              
    {
        message_port_register_in(pmt::mp("cell_id"));
		set_msg_handler(pmt::mp("cell_id"), [this](const pmt::pmt_t& msg){ set_cell_id_msg(msg);});            
    }

    /*
     * Our virtual destructor.
     */
    pbch_demux_vcvc_impl::~pbch_demux_vcvc_impl()
    {
    }

    void
    pbch_demux_vcvc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        for(int i = 0 ; i < ninput_items_required.size() ; i++){
			ninput_items_required[0] = noutput_items;
		}
    }

    int
    pbch_demux_vcvc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        auto in = static_cast<const input_type*>(input_items[0]);
        auto out = static_cast<output_type*>(output_items[0]);
		// get smallest number of input items
		int ninitems = calculate_n_process_items(ninput_items, noutput_items);
		//~ printf("this is a demux with %i items \n", ninitems);

        //the following section is commented because it causes gnuradio-failure (to much items in pipe?)

//		// No data is processed as long as the cell_id is not available
//		if(d_cell_id < 0){
//			//consume_each(ninitems);
//			return 0;
//		}

		//set noutput_items to zero. if output is produced, noutput_items is incremented.
		noutput_items = 0;

		int cell_id_mod3 = d_cell_id%3;
		int n_carriers = 12*d_N_rb_dl;

		//Read tags for updated sym_num
		std::vector<gr::tag_t> v;
		get_tags_in_range(v,0,nitems_read(0),nitems_read(0)+ninitems);
		int sym_num = get_sym_num(v);

		//This loop searches for the REs with the PBCH and copies them to the output stream.
		for (int i = 0 ; i < ninitems ; i++ ) {
			if(sym_num==7){
				if (ninitems-i < 4){
					ninitems = i;
					break;
				}
				extract_pbch_values(out, in);

				noutput_items++;
				out += 240 * d_rxant;
			}

			// update work values for next symbol
			if(sym_num != -1){
                sym_num = (sym_num+1)%140;
            }
			in += n_carriers * d_rxant;
		}

		// update d_sym_num
		d_sym_num = sym_num;
		// Tell runtime system how many input items we consumed on
		// each input stream.
		consume_each (ninitems);
		// Tell runtime system how many output items we produced.
		return noutput_items;
    }


	void
	pbch_demux_vcvc_impl::set_cell_id_msg(pmt::pmt_t msg)
	{
		int cell_id = int(pmt::to_long(msg));
		//printf("********%s INPUT MESSAGE***************\n", name().c_str() );
		//printf("\t%i\n", cell_id);
		//printf("********%s INPUT MESSAGE***************\n", name().c_str() );
		set_cell_id(cell_id);
	}

	void
	pbch_demux_vcvc_impl::set_cell_id(int id)
	{
		d_cell_id = id;
		//~ printf("%s\t\tset_cell_id = %i\n", name().c_str(), d_cell_id );
	}

	int
	pbch_demux_vcvc_impl::calculate_n_process_items(gr_vector_int ninput_items, int noutput_items)
	{
		// get smallest number of input items
		int n_inputs = ninput_items.size();
		int ninitems = ninput_items[0];
		for(int i = 1 ; i < n_inputs ; i++){
			if(ninitems > ninput_items[i]){
				ninitems = ninput_items[i];
			}
		}
		if (ninitems > noutput_items){
			ninitems = noutput_items;
		}
		return ninitems;
	}

	void
	pbch_demux_vcvc_impl::extract_pbch_values(gr_complex* out,
												const gr_complex* in)
	{


		int cell_id_mod3 = d_cell_id%3;
		int n_carriers = 12*d_N_rb_dl;
		int n_pbch4 = 240;
		int pbch_pos = (n_carriers/2)-(72/2);
		int idx = 0;
		for (int c = 0 ; c < 72 ; c++ ) {
			if ( cell_id_mod3 != c%3 ){
                for(int rx=0; rx<d_rxant; rx++){
                    out[idx   +rx*n_pbch4] = in[pbch_pos+c                   +rx*n_carriers];
                    out[idx+48+rx*n_pbch4] = in[pbch_pos+c+n_carriers*d_rxant+rx*n_carriers];
				}
				idx++;
			}
		}
		for(int rx=0; rx<d_rxant; rx++){
            //Copy PBCH values on symbol 9
            memcpy(out+96   +rx*n_pbch4, in+2*n_carriers*d_rxant+pbch_pos+rx*n_carriers, 72*sizeof(gr_complex) );
            //Copy PBCH values on symbol 10
            memcpy(out+96+72+rx*n_pbch4, in+3*n_carriers*d_rxant+pbch_pos+rx*n_carriers, 72*sizeof(gr_complex) );
		}
	}

	int
	pbch_demux_vcvc_impl::get_sym_num(std::vector<gr::tag_t> &v)
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
    

  } /* namespace lte */
} /* namespace gr */
