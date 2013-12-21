#ifndef __vrr_rtable_h__
#define __vrr_rtable_h__

#include <trace.h>
#include <map>
#include <vector>
#include <iostream> 


struct route {
    nsaddr_t eA; // Node which originated this packet
    nsaddr_t eB; // Destination
    nsaddr_t nA; // Destination
    nsaddr_t nB; // Destination
    int id; // Destination
};

//typedef std::map<nsaddr_t, nsaddr_t> rtable_t;
typedef std::vector<route> rtable_t;



class vrr_rtable {
   rtable_t rt_;

   public:
   vrr_rtable();
   void print(Trace*);
   void print();
   void clear();
//   void rm_entry(nsaddr_t);
//   void add_entry(nsaddr_t, nsaddr_t);
   void add_entry(route);
   void rm_entry(nsaddr_t,int);
   nsaddr_t lookup(nsaddr_t);
   
   route minRoute(nsaddr_t);
   nsaddr_t nextHop(route r, nsaddr_t);
   u_int32_t size();
};

#endif
