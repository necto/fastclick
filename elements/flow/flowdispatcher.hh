#ifndef CLICK_FLOWDISPATCHER_HH
#define CLICK_FLOWDISPATCHER_HH
#include <click/flowelement.hh>
#include <click/string.hh>
#include <click/timer.hh>
#include "flowclassifier.hh"
#include <vector>

CLICK_DECLS

class FlowDispatcher: public FlowSpaceElement<int> {

public:

	FlowDispatcher() CLICK_COLD;
	~FlowDispatcher() CLICK_COLD {};

    const char *class_name() const		{ return "FlowDispatcher"; }
    const char *port_count() const		{ return "1/-"; }
    const char *processing() const		{ return PUSH; }

    int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;
    int initialize(ErrorHandler *errh) CLICK_COLD;

    void push_batch(int, int* flowdata, PacketBatch* batch) override;

    FlowNode* get_table(int) override;

private :

    Vector<FlowClassificationTable::Rule> rules;

    FlowNode* _table;

    bool _verbose;

};


CLICK_ENDDECLS
#endif
