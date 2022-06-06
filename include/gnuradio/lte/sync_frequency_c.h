#ifndef INCLUDED_LTE_SYNC_FREQUENCY_C_H
#define INCLUDED_LTE_SYNC_FREQUENCY_C_H

#include <gnuradio/lte/api.h>
#include <gnuradio/sync_block.h>
#include <gnuradio/analog/sig_source.h>

namespace gr {
  namespace lte {

    /*!
     * \brief <+description of block+>
     * \ingroup lte
     *
     */
    class LTE_API sync_frequency_c : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<sync_frequency_c> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte::sync_frequency_c.
       *
       * To avoid accidental use of raw pointers, lte::sync_frequency_c's
       * constructor is in a private implementation
       * class. lte::sync_frequency_c::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::shared_ptr<gr::analog::sig_source<gr_complex>> &sig, int fftl, std::string name = "sync_frequency_c");
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_SYNC_FREQUENCY_C_H */
