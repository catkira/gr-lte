#include <gnuradio/io_signature.h>
#include "sss_tagger_cc_impl.h"

namespace gr {
  namespace lte {

    using input_type = gr_complex;
    using output_type = gr_complex;
    sss_tagger_cc::sptr
    sss_tagger_cc::make(int fftl, std::string name)
    {
      return gnuradio::make_block_sptr<sss_tagger_cc_impl>(
        fftl, name);
    }


    /*
     * The private constructor
     */
    sss_tagger_cc_impl::sss_tagger_cc_impl(int fftl, std::string name)
      : gr::sync_block("sss_tagger_cc",
                gr::io_signature::make(1, 1, sizeof(input_type)),
                gr::io_signature::make(1, 1, sizeof(output_type))),
                d_fftl(fftl),
                d_cpl(144*fftl/2048),
                d_cpl0(160*fftl/2048),
                d_slotl(7*fftl+6*d_cpl+d_cpl0),
                d_framel(20*d_slotl),
                d_slot_num(41),
                d_offset_0(0),
                d_frame_start(0)
    {
        set_tag_propagation_policy(TPP_DONT);
        d_key = pmt::string_to_symbol("slot");
        d_tag_id = pmt::string_to_symbol(this->name() );
        
        message_port_register_in(pmt::mp("frame_start"));
		set_msg_handler(pmt::mp("frame_start"), [this](const pmt::pmt_t& msg){ handle_msg_frame_start(msg);});        
    }

    /*
     * Our virtual destructor.
     */
    sss_tagger_cc_impl::~sss_tagger_cc_impl()
    {
    }
    
    void
    sss_tagger_cc_impl::handle_msg_frame_start(pmt::pmt_t msg)
    {
        set_frame_start(pmt::to_long(msg) );
    }    

    int
    sss_tagger_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        auto in = static_cast<const input_type*>(input_items[0]);
        auto out = static_cast<output_type*>(output_items[0]);
     
        //This block does not change data. It just adds new itemtags!
        memcpy(out,in,sizeof(gr_complex)*noutput_items);

        unsigned long nin = nitems_read(0);

        std::vector <gr::tag_t> v;
        get_tags_in_range(v,0,nin,nin+noutput_items);
        if (v.size() > 0){
            //printf("\n\n\n%s tag: found\tvalue = %ld\toffset = %ld\n",name().c_str(),half_frame_slot,v[0].offset );
            unsigned long offset_mod = (v[0].offset)%d_slotl;
            if(offset_mod != d_offset_0){
                d_offset_0 = offset_mod;
                //printf("%s\toffset = %ld changed\tabs_pos = %ld\n", name().c_str(), d_offset_0, v[0].offset);

            }
        }

        // as long as frame start is unknown add dummy tags, so freq estimate can work!
        if(d_frame_start == 0){
            for (int i = 0 ; i < noutput_items ; i++){
                if( (nin+i)%d_slotl == d_offset_0 ){ //removed abs
                    //printf("%s\tslot_num = %i\tabs_pos = %ld\tframe_start = %ld\n", name().c_str() ,d_slot_num, nitems_read(0)+i ,d_frame_start);
                    add_item_tag(0,nin+i,d_key, pmt::from_long( d_slot_num ),d_tag_id);
                }
            }
            return noutput_items;
        } //wait till first value found!

        //This loop adds new tags to the stream.
        for (int i = 0 ; i < noutput_items ; i++){
            if( (nin+i)%d_slotl == d_offset_0 ){ //removed abs
                if((nin+i)%d_framel == d_frame_start ){ // removed abs
                    //printf("%s\toffset = %ld\tframe_start = %ld\tabs_pos = %ld\n", name().c_str(), d_offset_0, d_frame_start, nitems_read(0)+i);
                    d_slot_num = 0;
                }

                //printf("%s\tslot_num = %i\tabs_pos = %ld\tframe_start = %ld\n", name().c_str() ,d_slot_num, nitems_read(0)+i ,d_frame_start);
                add_item_tag(0,nin+i,d_key, pmt::from_long( d_slot_num ),d_tag_id);
                if(i+d_slotl < noutput_items){
                    i += (d_slotl-1);
                }
                else{
                    i+=(noutput_items-i-1);
                }

                // prepare values for next iteration.
                d_slot_num = (d_slot_num + 1) % 20;

            }
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }
    
    void
    sss_tagger_cc_impl::set_frame_start(long frame_start)
    {
        d_frame_start = frame_start;
    }    

  } /* namespace lte */
} /* namespace gr */
