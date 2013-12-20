#ifndef __vrr_h__
#define __vrr_h__

#include "vrr_pkt.h"
#include "vrr_rtable.h"
#include <agent.h>
#include <packet.h>
#include <trace.h>
#include <timer-handler.h>
#include <random.h>
#include <classifier-port.h>
#include <mobilenode.h>
#include "arp.h"
#include "ll.h"
#include "mac.h"
#include "ip.h"
#include "delay.h"
#include <vector>
#include <set>

#define CURRENT_TIME Scheduler::instance().clock()
#define JITTER (Random::uniform()*0.5)

class VRR; // forward declaration

/* Timers */

class VRR_PktTimer : public TimerHandler {
    public:
    VRR_PktTimer(VRR* agent) : TimerHandler() {
    	agent_ = agent;
    }
    
    protected:
    VRR* agent_;
    virtual void expire(Event* e);
};

/* Agent */

class VRR : public Agent {

   /* Friends */
   friend class VRR_PktTimer;

   /* Private members */
   nsaddr_t ra_addr_; // Node address
//   vrr_state state_;
   vrr_rtable rtable_;
   int accesible_var_;
   u_int8_t seq_num_;
   std::vector<nsaddr_t> vset;
   std::set<nsaddr_t> pset;
   int r;
   
   protected:

   MobileNode* node_;
   PortClassifier* dmux_; // For passing packets up to agents.
   Trace* logtarget_; // For logging.
   VRR_PktTimer pkt_timer_; // Timer for sending packets.

   inline nsaddr_t& ra_addr() { return ra_addr_; }
   //inline vrr_state& state() { return state_; }
   inline int& accessible_var() { return accesible_var_; }
//   inline std::vector<nsaddr_t>& vset(){ if(vset!=NULL)return vset;};
//   inline std::vector<nsaddr_t>& pset(){ if(pset!=NULL)return pset;};
//   inline int& r(){return r;};
   
   void forward_data(Packet*);
   void recv_vrr_pkt(Packet*);
   void send_vrr_pkt();
  
   void reset_vrr_pkt_timer();
   
//   void send_hello();
//   void recv_hello(Packet *p);
   public:

   VRR(nsaddr_t);
   int command(int, const char*const*);
   void recv(Packet*, Handler*);
   //void mac_failed(Packet*);
   void addToPset(nsaddr_t);
   void init();
};

#endif
