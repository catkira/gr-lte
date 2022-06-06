/* -*- c++ -*- */
/*
 * Copyright 2022 gr-lte author.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_LTE_PSS_TAGGER_H
#define INCLUDED_LTE_PSS_TAGGER_H

#include <gnuradio/lte/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace lte {

    /*!
     * \brief <+description of block+>
     * \ingroup lte
     *
     */
    class LTE_API pss_tagger : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<pss_tagger> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of lte::pss_tagger.
       *
       * To avoid accidental use of raw pointers, lte::pss_tagger's
       * constructor is in a private implementation
       * class. lte::pss_tagger::make is the public interface for
       * creating new instances.
       */
      static sptr make(int fftl, std::string& name);
    };

  } // namespace lte
} // namespace gr

#endif /* INCLUDED_LTE_PSS_TAGGER_H */
