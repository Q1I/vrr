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
    nsaddr_t pkt_dst_; // Node which originated this packet
    u_int16_t pkt_len_; // Packet length (in bytes)
    u_int8_t pkt_seq_num_; // Packet sequence number

    inline nsaddr_t& pkt_src() { return pkt_src_; }
    inline nsaddr_t& pkt_dst() { return pkt_dst_; }
    inline u_int16_t& pkt_len() { return pkt_len_; }
    inline u_int8_t& pkt_seq_num() { return pkt_seq_num_; }

    static int offset_;
    inline static int& offset() { return offset_; }
    inline static hdr_vrr_pkt* access(const Packet* p) {
    	return (hdr_vrr_pkt*)p->access(offset_);
    }
};

#endif
