#ifndef __vrr_pkt_h__
#define __vrr_pkt_h__

#include <packet.h>


/* =====================================================================
   Packet Formats...
   ===================================================================== */
#define VRRTYPE_HELLO  	0x01
#define VRRTYPE_SETUP_REQ   	0x02
#define VRRTYPE_SETUP   	0x04
#define VRRTYPE_SETUP_FAIL   	0x08
#define VRRTYPE_TEARDOWN  	0x10

/*
 * AODV Routing Protocol Header Macros
 */
//#define HDR_VRR_PKT(p)		((struct hdr_vrr*)hdr_vrr_pkt::access(p))
//#define HDR_VRR_SETUP_REQ(p)  	((struct hdr_vrr_setup_req*)hdr_vrr_pkt::access(p))
//#define HDR_VRR_HELLO(p)	((struct hdr_vrr_hello*)hdr_vrr_pkt::access(p))
//#define HDR_VRR_ERROR(p)	((struct hdr_vrr_error*)hdr_vrr::access(p))
//#define HDR_VRR_RREP_ACK(p)	((struct hdr_vrr_rrep_ack*)hdr_vrr::access(p))


#define HDR_VRR_PKT(p) hdr_vrr_pkt::access(p)
#define HDR_VRR_HELLO(p)	((struct hdr_vrr_hello*)hdr_vrr_pkt::access(p))
#define HDR_VRR_SETUP_REQ(p)  	((struct hdr_vrr_setup_req*)hdr_vrr_pkt::access(p))

//struct hdr_vrr_pkt {
//    u_int8_t pkt_type_; // Node which originated this packet
//    nsaddr_t pkt_src_; // Node which originated this packet
//    nsaddr_t pkt_dst_; // Destination
//    u_int16_t pkt_len_; // Packet length (in bytes)
//    u_int8_t pkt_seq_num_; // Packet sequence number
//
//     inline u_int8_t& pkt_type() {
//        return pkt_type_;
//    }
//     
//    inline nsaddr_t& pkt_src() {
//        return pkt_src_;
//    }
//
//    inline nsaddr_t& pkt_dst() {
//        return pkt_dst_;
//    }
//
//    inline u_int16_t& pkt_len() {
//        return pkt_len_;
//    }
//
//    inline u_int8_t& pkt_seq_num() {
//        return pkt_seq_num_;
//    }
//
//    static int offset_;
//
//    inline static int& offset() {
//        return offset_;
//    }
//
//    inline static hdr_vrr_pkt* access(const Packet* p) {
//        return (hdr_vrr_pkt*) p->access(offset_);
//    }
//};

/*
 * General VRR Header - shared by all formats
 */
struct hdr_vrr_pkt {
    u_int8_t vrr_type;

    nsaddr_t pkt_src_; // Node which originated this packet
    nsaddr_t pkt_dst_; // Destination
    u_int16_t pkt_len_; // Packet length (in bytes)
    u_int8_t pkt_seq_num_; // Packet sequence number

    /*
    u_int8_t        ah_reserved[2];
    u_int8_t        ah_hopcount;
     */

    // Header access methods
    static int offset_; // required by PacketHeaderManager

    inline static int& offset() {
        return offset_;
    }

    inline static hdr_vrr_pkt* access(const Packet* p) {
        return (hdr_vrr_pkt*) p->access(offset_);
    }
};

struct hdr_vrr_hello {
    u_int8_t pkt_type_; // Packet Type
    nsaddr_t pkt_src_; // Node which originated this packet
    nsaddr_t pkt_dst_; // Destination
    u_int16_t pkt_len_; // Packet length (in bytes)
    u_int8_t pkt_seq_num_; // Packet sequence number

    inline u_int8_t& pkt_type() {
        return pkt_type_;
    }

    inline nsaddr_t& pkt_src() {
        return pkt_src_;
    }

    inline nsaddr_t& pkt_dst() {
        return pkt_dst_;
    }

    inline u_int16_t& pkt_len() {
        return pkt_len_;
    }

    inline u_int8_t& pkt_seq_num() {
        return pkt_seq_num_;
    }

    static int offset_;

    inline static int& offset() {
        return offset_;
    }

    inline static hdr_vrr_pkt* access(const Packet* p) {
        return (hdr_vrr_pkt*) p->access(offset_);
    }
};

struct hdr_vrr_setup_req {
    u_int8_t pkt_type_; // Packet Type
    nsaddr_t pkt_dst_; // Destination IP Address
    nsaddr_t pkt_src_; // Source IP Address
    nsaddr_t pkt_proxy_; // Source IP Address
        u_int16_t pkt_len_; // Packet length (in bytes)
        u_int8_t pkt_seq_num_; // Packet sequence number


    //    double req_timestamp; // when REQUEST sent;

        inline nsaddr_t& pkt_src() {
            return pkt_src_;
        }
    
        inline u_int8_t& pkt_type() {
            return pkt_type_;
        }
    
        inline nsaddr_t& pkt_dst() {
            return pkt_dst_;
        }
    
        inline nsaddr_t& pkt_proxy() {
            return pkt_proxy_;
        }
    
        inline u_int16_t& pkt_len() {
            return pkt_len_;
        }
    
        inline u_int8_t& pkt_seq_num() {
            return pkt_seq_num_;
        }


};

struct hdr_vrr_setup {
    u_int8_t s_type; // Packet Type
    u_int8_t reserved[2];
    u_int8_t s_hop_count; // Hop Count
    nsaddr_t s_dst; // Destination IP Address
    u_int32_t s_dst_seqno; // Destination Sequence Number
    nsaddr_t s_src; // Source IP Address
    double s_lifetime; // Lifetime

    double s_timestamp; // when corresponding REQ sent;
    // used to compute route discovery latency

    inline int size() {
        int sz = 0;
        /*
              sz = sizeof(u_int8_t)		// rp_type
                   + 2*sizeof(u_int8_t) 	// rp_flags + reserved
                   + sizeof(u_int8_t)		// rp_hop_count
                   + sizeof(double)		// rp_timestamp
                   + sizeof(nsaddr_t)		// rp_dst
                   + sizeof(u_int32_t)	// rp_dst_seqno
                   + sizeof(nsaddr_t)		// rp_src
                   + sizeof(u_int32_t);	// rp_lifetime
         */
        sz = 6 * sizeof (u_int32_t);
        assert(sz >= 0);
        return sz;
    }

};


#endif
