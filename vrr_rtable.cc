#include "vrr_rtable.h"
#include "ip.h"

vrr_rtable::vrr_rtable() {
//    route rt = {22, 22, 22, 22, 0};
//    this->add_entry(rt);
    //    rtable_t rt_ 
}

void
vrr_rtable::print(Trace* out) {
    //	sprintf(out->pt_->buffer(), "P\tdest\tnext");
    //	out->pt_->dump();
    // 	for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++) {
    // 		sprintf(out->pt_->buffer(), "P\t%d\t%d", (*it).first, (*it).second);
    // 		out->pt_->dump();
    // 	}
}

void
vrr_rtable::print() {
    std::cout << "#RTABLE:" << std::endl;
    printf("eA:\teB:\tnA:\tnB:\tid:\n");
    for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++) {
        printf("%d\t%d\t%d\t%d\t%d\n", (*it).eA, (*it).eB, (*it).nA, (*it).nB, (*it).id);
    }
}

void
vrr_rtable::clear() {
    rt_.clear();
}

//void
//vrr_rtable::rm_entry(nsaddr_t dest) {
//	rt_.erase(dest);
//}
//
//void
//vrr_rtable::add_entry(nsaddr_t dest, nsaddr_t next) {
//	rt_[dest] = next;
//}

void
vrr_rtable::rm_entry(nsaddr_t eA, int id) {
    //	rt_.erase(dest);
}

void
vrr_rtable::add_entry(route r) {
    // check for duplicate

    rt_.push_back(r);
}

nsaddr_t
vrr_rtable::lookup(nsaddr_t dest) {
    //	rtable_t::iterator it = rt_.find(dest);
    //	if (it == rt_.end())
    //		return IP_BROADCAST;
    //	else
    //		return (*it).second;
    std::cout << "#Lookup" << std::endl;

    route r = minRoute(dest);

    nsaddr_t nxt = nextHop(r, dest);
    return nxt;

    //    nsaddr_t eA, eB, nA, nB, id;
    //    for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++) {
    //        eA = (*it).eA;
    //        eB = (*it).eB;
    //        nA = (*it).nA;
    //        nB = (*it).nB;
    //        id = (*it).id;
    //        
    //        // calc min distance
    //        
    //        
    //        //        (*it).eA, (*it).eB, (*it).nA, (*it).nB, (*it).id);
    //    }
    //    rtable_t::iterator it = rt_.find(dest);
}

nsaddr_t
vrr_rtable::nextHop(route r, nsaddr_t dest) {
    std::cout<<"#nextHop: "<<std::endl;
    nsaddr_t nxt;
    int a_dist = abs(r.eA-dest);
    int b_dist = abs(r.eB-dest);
    if(a_dist<b_dist)
        nxt=r.nA;
    else
        nxt= r.nB;
    std::cout<<"node with min distance: "<<nxt<<std::endl;
    return nxt;
}

route
vrr_rtable::minRoute(nsaddr_t dest) {
    cout << "#Calc minRoute" << std::endl;
    nsaddr_t eA, eB, nA, nB, id;
    route r, min_r;
    int min_node, min_val, tmp_val;

    std::vector<nsaddr_t> candidates;
    //    if(rt_ == NULL){
    //        std::cout<<"NULLLLL"<<std::endl;
    //        route rt = {11, 11, NULL, 11, 0};
    //        return rt;
    //    }
    
    // print rt
    this->print();
    
    for (rtable_t::iterator it = rt_.begin(); it != rt_.end(); it++) {
        r = *it;
        eA = (*it).eA;
        eB = (*it).eB;
        nA = (*it).nA;
        nB = (*it).nB;
        id = (*it).id;

        printf("dest:%d\teA:%d\teB:%d\tnA:%d\tnB:%d\tid:%d\n", dest, eA, eB, nA, nB, id);

        // found dest
        if (eA == dest || eB == dest) {
            return r;
        }

        // calc min distance
        //eA
        tmp_val = abs(eA - dest);
        if (tmp_val == min_val) {
            candidates.push_back(eA);
            min_node = eA;
            min_val = tmp_val;
            min_r = r;
        }
        if (tmp_val < min_val) {
            // reset candidates
            candidates.clear();
            min_node = eA;
            min_val = tmp_val;
            min_r = r;
        }
        //eB
        tmp_val = abs(eB - dest);
        if (tmp_val == min_val) {
            candidates.push_back(eB);
            min_node = eB;
            min_val = tmp_val;
            min_r = r;
        }
        if (tmp_val < min_val) {
            // reset candidates
            candidates.clear();
            min_node = eB;
            min_val = tmp_val;
            min_r = r;
        }
        printf("tmp: %d, min_val: %d, min_node:%d, eA: %d, dest: %d", tmp_val, min_val, min_node, eA, dest);
    }
    printf("min_val: %d, min_node:%d, dest: %d", min_val, min_node, dest);
    return r;
}

u_int32_t
vrr_rtable::size() {
    return rt_.size();
}
