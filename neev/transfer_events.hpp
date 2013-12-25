// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE.txt
//
// Initially a part of the Battle for Wesnoth Project http://www.wesnoth.org/
// It was licensed under the GPL version 2.
// 
// (C) Copyright 2013 Pierre Talbot <ptalbot@hyc.io>

#ifndef UMCD_TRANSFER_EVENTS_HPP
#define UMCD_TRANSFER_EVENTS_HPP

#include "umcd/server/events.hpp"
#include "umcd/server/events_subscriber_view.hpp"
#include <boost/system/error_code.hpp>

struct transfer_complete{};
struct transfer_error{};

/** Use this event to track the transmission process.
*/
struct transfer_on_going{};
struct chunk_complete{}; // Useful to launch the next op.

template <>
struct event_slot<transfer_complete>
{
  /** A function declaration with no argument, just to notify the fact that the transfer is finished.
  */
  typedef void type();
};

template <>
struct event_slot<transfer_error>
{
  /** A function declaration that takes an error_code and is called if the transmission fails.
  */
  typedef void type(const boost::system::error_code&);
};

template <>
struct event_slot<transfer_on_going>
{
  /** A function declaration that takes the bytes transferred and the bytes to transfer (total).
  */
  typedef void type(std::size_t, std::size_t);
};

struct transfer_events;

template <>
struct event_slot<chunk_complete>
{
  typedef void type(events_subscriber_view<transfer_events>);
};

struct transfer_events
: events<boost::mpl::set<
        transfer_complete
      , transfer_error
      , transfer_on_going
      , chunk_complete> >
{};

#endif // UMCD_TRANSFER_EVENTS_HPP
