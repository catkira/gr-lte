#ifndef INCLUDED_LTE_SUBBLOCK_DEINTERLEAVER_VFVF_IMPL_H
#define INCLUDED_LTE_SUBBLOCK_DEINTERLEAVER_VFVF_IMPL_H

#include <gnuradio/lte/subblock_deinterleaver_vfvf.h>

namespace gr {
  namespace lte {

    class subblock_deinterleaver_vfvf_impl : public subblock_deinterleaver_vfvf
    {
     private:
		int d_num_groups;
		int d_items_per_group;
		static const int d_perm_vec[];
		std::vector<int> d_interleaved_pos;
		std::vector<int> subblock_interleaver_prototype(int len);
        
     public:
      subblock_deinterleaver_vfvf_impl(int num_groups, int items_per_group, std::string name);
      ~subblock_deinterleaver_vfvf_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_SUBBLOCK_DEINTERLEAVER_VFVF_IMPL_H */
