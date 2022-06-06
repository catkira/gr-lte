/* -*- c++ -*- */
/*
 * Copyright 2022 gr-lte author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "pss_tagger_cc_impl.h"

namespace gr {
  namespace lte {

    using input_type = gr_complex;
    using output_type = gr_complex;
    pss_tagger_cc::sptr
    pss_tagger_cc::make(int fftl, std::string& name)
    {
      return gnuradio::make_block_sptr<pss_tagger_cc_impl>(
        fftl, name);
    }


    /*
     * The private constructor
     */
    pss_tagger_cc_impl::pss_tagger_cc_impl(int fftl, std::string& name)
      : gr::sync_block("pss_tagger_cc",
                gr::io_signature::make(1, 1, sizeof(input_type)),
                gr::io_signature::make(1, 1, sizeof(output_type))),
                d_fftl(fftl),
                d_cpl(144*fftl/2048),
                d_cpl0(160*fftl/2048),
                d_slotl(7*fftl+6*d_cpl+d_cpl0),
                d_half_frame_start(0),
                d_N_id_2(-1),
                d_slot_num(0),
                d_is_locked(false)              
    {
        set_tag_propagation_policy(TPP_DONT);
        d_key=pmt::string_to_symbol("slot");
        d_tag_id=pmt::string_to_symbol(this->name() );
        d_id_key = pmt::string_to_symbol("N_id_2");
        
        message_port_register_in(pmt::mp("lock"));
		set_msg_handler(pmt::mp("lock"), [this](const pmt::pmt_t& msg){ handle_msg_lock(msg);});

        message_port_register_in(pmt::mp("half_frame"));
		set_msg_handler(pmt::mp("half_frame"), [this](const pmt::pmt_t& msg){ handle_msg_half_frame_start(msg);});

        message_port_register_in(pmt::mp("N_id_2"));
		set_msg_handler(pmt::mp("N_id_2"), [this](const pmt::pmt_t& msg){ handle_msg_N_id_2(msg);});        
    }

    /*
     * Our virtual destructor.
     */
    pss_tagger_cc_impl::~pss_tagger_cc_impl()
    {
    }
    
    void
    pss_tagger_cc_impl::handle_msg_N_id_2(pmt::pmt_t msg)
    {
        set_N_id_2( (int) pmt::to_long(msg) );
    }
    
    void
    pss_tagger_cc_impl::handle_msg_half_frame_start(pmt::pmt_t msg)
    {
        set_half_frame_start(pmt::to_long(msg) );
    }
    
    void
    pss_tagger_cc_impl::handle_msg_lock(pmt::pmt_t msg)
    {
        if(msg == pmt::PMT_T)
        {
            lock();
        }
        else{
            unlock();
        }
    }
    
    int
    pss_tagger_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        auto in = static_cast<const input_type*>(input_items[0]);
        auto out = static_cast<output_type*>(output_items[0]);

        memcpy(out,in,sizeof(gr_complex)*noutput_items);

        long nin = nitems_read(0);
        int half_framel = 10*d_slotl;
        int offset = d_half_frame_start%d_slotl;

        for (int i = 0 ; i < noutput_items; i++){

            if( (nin+i)%d_slotl == offset){ // removed abs
                if((nin+i)%half_framel == d_half_frame_start){ // removed abs
                    //printf("found half_frame_start\t num = %li\t0 < %li\n", nitems_read(0)+i,(nitems_read(0)+i-d_half_frame_start) );
                    if(d_is_locked){
                        //printf("%s\thalf_frame_start = %i\tabs_pos = %ld\n", name().c_str(), d_half_frame_start, nitems_read(0)+i );
                        add_item_tag(0,nin+i,d_id_key, pmt::from_long(d_N_id_2),d_tag_id);
                        d_slot_num=0;
                    }
                }

                //printf("%s\tslot_num = %i\tabs_pos = %ld\n",name().c_str(),d_slot_num,nitems_read(0)+i );
                add_item_tag(0,nin+i,d_key, pmt::from_long(d_slot_num),d_tag_id);
                d_slot_num = (d_slot_num+1)%10;

                if(i+d_slotl < noutput_items){
                    i += (d_slotl-1);
                }
                else{
                    i+=(noutput_items-i-1);
                }
            }
        }
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace lte */
} /* namespace gr */
