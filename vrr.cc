#include "vrr.h"
#include "vrr_pkt.h"
#include <random.h>
#include <cmu-trace.h>
#include <iostream>

int hdr_vrr_pkt::offset_;

static class VRRHeaderClass : public PacketHeaderClass {
public:

    VRRHeaderClass() : PacketHeaderClass("PacketHeader/VRR", sizeof (hdr_vrr_pkt)) {
        bind_offset(&hdr_vrr_pkt::offset_);
    }
} class_rtProtoVRR_hdr;

static class VRRClass : public TclClass {
public:

    VRRClass() : TclClass("Agent/VRR") {
    }

    TclObject* create(int argc, const char*const* argv) {
        assert(argc == 5);
        return (new VRR((nsaddr_t) Address::instance().str2addr(argv[4])));
    }
} class_rtProtoVRR;

void
VRR_PktTimer::expire(Event* e) {
    agent_->send_vrr_pkt();
    agent_->reset_vrr_pkt_timer();
}

VRR::VRR(nsaddr_t id) : Agent(PT_VRR), pkt_timer_(this) {
    bind_bool("accesible_var_", &accesible_var_);
    ra_addr_ = id;
    node_ = (MobileNode*) Node::get_node_by_address(id);
}

int
VRR::command(int argc, const char*const* argv) {
    std::cout << "cmd" << std::endl;

    if (argc == 2) {
        if (strcasecmp(argv[1], "start") == 0) {
            pkt_timer_.resched(0.0);
            return TCL_OK;
        } else if (strcasecmp(argv[1], "print_rtable") == 0) {
            if (logtarget_ != 0) {
                sprintf(logtarget_->pt_->buffer(), "P %f _%d_ Routing Table", CURRENT_TIME, ra_addr());
                logtarget_->pt_->dump();
                rtable_.print(logtarget_);
            } else {
                fprintf(stdout, "%f _%d_ If you want to print this routing table "
                        "you must create a trace file in your tcl script", CURRENT_TIME, ra_addr());
            }
            return TCL_OK;
        }
    }
    else if (argc == 3) {
        // Obtains corresponding dmux to carry packets to upper layers
        if (strcmp(argv[1], "port-dmux") == 0) {
            dmux_ = (PortClassifier*) TclObject::lookup(argv[2]);
            if (dmux_ == 0) {
                fprintf(stderr, "%s: %s lookup of %s failed\n", __FILE__, argv[1], argv[2]);
                return TCL_ERROR;
            }
            return TCL_OK;
        }            // Obtains corresponding tracer
        else if (strcmp(argv[1], "log-target") == 0 || strcmp(argv[1], "tracetarget") == 0) {
            logtarget_ = (Trace*) TclObject::lookup(argv[2]);
            if (logtarget_ == 0)
                return TCL_ERROR;
            return TCL_OK;
        }
    }
    // Pass the command to the base class
    return Agent::command(argc, argv);
}

void
VRR::recv(Packet* p, Handler* h) {
    std::cout << "#recv" << std::endl << "\t\tsrc=" << ra_addr() << std::endl;

    struct hdr_cmn* ch = HDR_CMN(p);
    struct hdr_ip* ih = HDR_IP(p);

    if (ih->saddr() == ra_addr()) {
        // If there exists a loop, must drop the packet
        if (ch->num_forwards() > 0) {
            drop(p, DROP_RTR_ROUTE_LOOP);
            return;
        }            // else if this is a packet I am originating, must add IP header
        else if (ch->num_forwards() == 0)
            ch->size() += IP_HDR_LEN;
    }

    // If it is a vrr packet, must process it
    if (ch->ptype() == PT_VRR)
        recv_vrr_pkt(p);
        // Otherwise, must forward the packet (unless TTL has reached zero)
    else {
        ih->ttl_--;
        if (ih->ttl_ == 0) {
            drop(p, DROP_RTR_TTL);
            return;
        }
        forward_data(p);
    }
}

void
VRR::recv_vrr_pkt(Packet* p) {
    std::cout << "#recv_vrr_pkt" << std::endl << "\t\tsrc=" << ra_addr() << std::endl;
//    MobileNode *thisnode = (MobileNode *) ((Node::get_node_by_address(ra_addr_)));
//    printf("This node: %d\n",thisnode->address());
    std::cout << "------------------" << std::endl;
    
    struct hdr_ip* ih = HDR_IP(p);
    struct hdr_vrr_pkt* ph = HDR_VRR_PKT(p);

    // All routing messages are sent from and to port RT_PORT,
    // so we check it.
    assert(ih->sport() == RT_PORT);
    assert(ih->dport() == RT_PORT);

    /* ... processing of vrr packet ... */

    // Release resources
    Packet::free(p);
}

void
VRR::send_vrr_pkt() {
    std::cout << "#send_vrr" << std::endl << "\t\tsrc=" << ra_addr_ << "\ttime: "<<CURRENT_TIME<<std::endl;
//    MobileNode *thisnode = (MobileNode *) ((Node::get_node_by_address(ra_addr_)));
//    printf("This node: %d\n",thisnode->address());
    std::cout<<"PT_VRR="<<PT_VRR<<" hdrc_cmn::DOWN="<<hdr_cmn::DOWN<<" IP_BROAD="<<IP_BROADCAST<<" NS_AF_INET="<<NS_AF_INET<<std::endl;
    std::cout << "------------------" << std::endl;
    Packet* p = allocpkt();
    struct hdr_cmn* ch = HDR_CMN(p);
    struct hdr_ip* ih = HDR_IP(p);
    struct hdr_vrr_pkt* ph = HDR_VRR_PKT(p);

    ph->pkt_src() = ra_addr();
    ph->pkt_len() = 7;
    ph->pkt_seq_num() = seq_num_++;

    ch->ptype() = PT_VRR;
    ch->direction() = hdr_cmn::DOWN;
    ch->size() = IP_HDR_LEN + ph->pkt_len();
    ch->error() = 0;
    ch->next_hop() = IP_BROADCAST;
    ch->addr_type() = NS_AF_INET;

    ih->saddr() = ra_addr();
    ih->daddr() = IP_BROADCAST;
    ih->sport() = RT_PORT;
    ih->dport() = RT_PORT;
    ih->ttl() = IP_DEF_TTL;

    Scheduler::instance().schedule(target_, p, JITTER);
}

void
VRR::reset_vrr_pkt_timer() {
    pkt_timer_.resched((double) 5.0);
}

void
VRR::forward_data(Packet* p) {
    std::cout << "#fwd" << std::endl << "\t\tsrc=" << std::endl;

    struct hdr_cmn* ch = HDR_CMN(p);
    struct hdr_ip* ih = HDR_IP(p);

    if (ch->direction() == hdr_cmn::UP &&
            ((u_int32_t) ih->daddr() == IP_BROADCAST || ih->daddr() == ra_addr())) {
        dmux_->recv(p, 0);
        return;
    } else {
        ch->direction() = hdr_cmn::DOWN;
        ch->addr_type() = NS_AF_INET;
        if ((u_int32_t) ih->daddr() == IP_BROADCAST)
            ch->next_hop() = IP_BROADCAST;
        else {
            nsaddr_t next_hop = rtable_.lookup(ih->daddr());
            if (next_hop == IP_BROADCAST) {
                debug("%f: Agent %d can not forward a packet destined to %d\n",
                        CURRENT_TIME,
                        ra_addr(),
                        ih->daddr());
                drop(p, DROP_RTR_NO_ROUTE);
                return;
            } else
                ch->next_hop() = next_hop;
        }
        Scheduler::instance().schedule(target_, p, 0.0);
    }
}
