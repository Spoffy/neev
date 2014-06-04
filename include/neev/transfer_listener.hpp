#ifndef TRANSFER_LISTENER_HEADER
#define TRANSFER_LISTENER_HEADER

#include <neev/transfer_events.hpp>
#include <neev/events_subscriber_view.hpp>
#include <boost/system/error_code.hpp>

namespace neev {

class transfer_listener {
  public:
    virtual void on_transfer_complete() {};
    virtual void on_transfer_error(const boost::system::error_code&) {};
    virtual void on_transfer_on_going(std::size_t, std::size_t) {};
    virtual void on_chunk_complete(events_subscriber_view<transfer_events>) {};
 };

 }

#endif
