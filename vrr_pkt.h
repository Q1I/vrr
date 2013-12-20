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
//#define HDR_AODV(p)		((struct hdr_aodv*)hdr_aodv::access(p))
//#define HDR_AODV_REQUEST(p)  	((struct hdr_aodv_request*)hdr_aodv::access(p))
//#define HDR_AODV_REPLY(p)	((struct hdr_aodv_reply*)hdr_aodv::access(p))
//#define HDR_AODV_ERROR(p)	((struct hdr_aodv_error*)hdr_aodv::access(p))
//#define HDR_AODV_RREP_ACK(p)	((struct hdr_aodv_rrep_ack*)hdr_aodv::access(p))

#define HDR_VRR_PKT(p) hdr_vrr_pkt::access(p)

struct hdr_vrr_pkt {
    nsaddr_t pkt_src_; // Node which originated this packet
    nsaddr_t pkt_dst_; // Destination
    u_int16_t pkt_len_; // Packet length (in bytes)
    u_int8_t pkt_seq_num_; // Packet sequence number

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

/*
 * General AODV Header - shared by all formats
 */
struct hdr_vrr {
    u_int8_t ah_type;
    /*
    u_int8_t        ah_reserved[2];
    u_int8_t        ah_hopcount;
     */

    // Header access methods
    static int offset_; // required by PacketHeaderManager

    inline static int& offset() {
        return offset_;
    }

    inline static hdr_vrr* access(const Packet* p) {
        return (hdr_vrr*) p->access(offset_);
    }
};

struct hdr_vrr_setup_req {
    u_int8_t req_type; // Packet Type
    u_int8_t reserved[2];
    u_int8_t req_hop_count; // Hop Count
    u_int32_t req_bcast_id; // Broadcast ID

    nsaddr_t req_dst; // Destination IP Address
    u_int32_t req_dst_seqno; // Destination Sequence Number
    nsaddr_t req_src; // Source IP Address
    u_int32_t req_src_seqno; // Source Sequence Number

    double req_timestamp; // when REQUEST sent;
    // used to compute route discovery latency

    // This define turns on gratuitous replies- see aodv.cc for implementation contributed by
    // Anant Utgikar, 09/16/02.
    //#define RREQ_GRAT_RREP	0x80

    inline int size() {
        int sz = 0;
        /*
              sz = sizeof(u_int8_t)		// rq_type
                   + 2*sizeof(u_int8_t) 	// reserved
                   + sizeof(u_int8_t)		// rq_hop_count
                   + sizeof(double)		// rq_timestamp
                   + sizeof(u_int32_t)	// rq_bcast_id
                   + sizeof(nsaddr_t)		// rq_dst
                   + sizeof(u_int32_t)	// rq_dst_seqno
                   + sizeof(nsaddr_t)		// rq_src
                   + sizeof(u_int32_t);	// rq_src_seqno
         */
        sz = 7 * sizeof (u_int32_t);
        assert(sz >= 0);
        return sz;
    }
};

struct hdr_vrr_setup {
        u_int8_t        s_type;        // Packet Type
        u_int8_t        reserved[2];
        u_int8_t        s_hop_count;           // Hop Count
        nsaddr_t        s_dst;                 // Destination IP Address
        u_int32_t       s_dst_seqno;           // Destination Sequence Number
        nsaddr_t        s_src;                 // Source IP Address
        double	        s_lifetime;            // Lifetime

        double          s_timestamp;           // when corresponding REQ sent;
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
  	sz = 6*sizeof(u_int32_t);
  	assert (sz >= 0);
	return sz;
  }

};


#endif
