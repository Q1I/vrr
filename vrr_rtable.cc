#include "vrr_rtable.h"
#include "ip.h"

vrr_rtable::vrr_rtable() { }

void
vrr_rtable::print(Trace* out) {
	sprintf(out->pt_->buffer(), "P\tdest\tnext");
	out->pt_->dump();
 	for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++) {
 		sprintf(out->pt_->buffer(), "P\t%d\t%d", (*it).first, (*it).second);
 		out->pt_->dump();
 	}
}

void
vrr_rtable::clear() {
	rt_.clear();
}

void
vrr_rtable::rm_entry(nsaddr_t dest) {
	rt_.erase(dest);
}

void
vrr_rtable::add_entry(nsaddr_t dest, nsaddr_t next) {
	rt_[dest] = next;
}

nsaddr_t
vrr_rtable::lookup(nsaddr_t dest) {
	rtable_t::iterator it = rt_.find(dest);
	if (it == rt_.end())
		return IP_BROADCAST;
	else
		return (*it).second;
}

u_int32_t
vrr_rtable::size() {
	return rt_.size();
}
