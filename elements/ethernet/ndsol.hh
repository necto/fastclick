#ifndef NDSOL_HH
#define NDSOL_HH

/*
 * =c
 * NDSol(I, E)
 * =s
 * V<ARP, encapsulation>
 * =d
 * Handles most of the Neighborhood Discovery(ND) protocol. 
 * Argument I should be this host's IP6 address, and E should 
 * be this host's ethernet address.
 *
 * Expects ordinary IP6 packets on input 0, each with a destination
 * address annotation. If an ethernet address is already known
 * for the destination, the IP6 packet is wrapped in an ethernet
 * header and sent to output 0. Otherwise the IP6 packet is saved and
 * an Neighborhood Solicitation Message is sent to output 0. 
 * If an Neighborhood Advertisement Message arrives
 * on input 1 for an IP6 address that we need, the mapping is
 * recorded and the saved IP6 packet is sent.
 *
 * The packets on input 1 should include the ethernet header.
 *
 * If a host has multiple interfaces, it will need multiple
 * instances of NDSol.
 *
 * NDSol may have one or two outputs. If it has two, then ARP queries
 * are sent to the second output.
 *
 * =e
 *    c :: Classifier(12/86dd 20/3aff 53/87,
 *		      12/86dd 20/3aff 53/88,
 *		      12/86dd);
 *    a :: NDSol(3ffe:1ce1:2::1, 00:e0:29:05:e5:6f);
 *    c[0] -> ...
 *    c[1] -> a[1];
 *    c[2] -> ... -> a[0];
 *    a[0] -> ... -> ToDevice(eth0);
 *
 * =a
 * NDAdv
 */

#include "element.hh"
#include "etheraddress.hh"
#include "ip6address.hh"
#include "timer.hh"

class NDSol : public Element {
 public:
  
  NDSol();
  ~NDSol();
  
  const char *class_name() const		{ return "NDSol"; }
  const char *processing() const		{ return PUSH; }
  Bitvector forward_flow(int) const;
  Bitvector backward_flow(int) const;
  void notify_noutputs(int);
  void add_handlers();
  
  NDSol *clone() const;
  int configure(const Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);
  void uninitialize();
  void take_state(Element *, ErrorHandler *);
  
  void push(int port, Packet *);
  
  Packet *make_query(unsigned char tpa[16],
                     unsigned char sha[6], unsigned char spa[16]);

  void insert(IP6Address, EtherAddress);

   struct NDEntry {
    IP6Address ip6;
    EtherAddress en;
    int last_response_jiffies;
    unsigned ok: 1;
    unsigned polling: 1;
    Packet *p;
    struct NDEntry *next;
  };

  // statistics
  int _arp_queries;
  int _pkts_killed;
  
 private:

  static const int NMAP = 256;
  NDEntry *_map[NMAP];
  EtherAddress _my_en;
  IP6Address _my_ip6;
  Timer _expire_timer;
  
  void send_query_for(const u_char want_ip6[16]);
  
  void handle_ip6(Packet *);
  void handle_response(Packet *);

  static const int EXPIRE_TIMEOUT_MS = 15 * 1000;
  static void expire_hook(unsigned long);
  static String read_table(Element *, void *);
  
};

#endif

