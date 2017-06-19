/* Generated by "click-buildtool elem2package" on Mon Jun 19 00:48:37 EDT 2017 */
/* Package name: FTSFC */

#define WANT_MOD_USE_COUNT 1
#include <click/config.h>
#include <click/package.hh>
#include <click/glue.hh>
#include "./ArrCounterMB.hh"
#include "./CounterMB.hh"
#include "./FTAppenderElement.hh"
#include "./FTBufferElement.hh"
#include "./FTControlElement.hh"
#include "./FTFastTCPGen.hh"
#include "./FTFilterElement.hh"
#include "./FTLoggerElement.hh"
#include "./FTPassElement.hh"
#include "./FTRandomDropElement.hh"
#include "./FTStateElement.hh"
#include "./Monitor.hh"
#include "./NFArrCounterMB.hh"
#include "./NFCounterMB.hh"
#include "./TrArrCounterMB.hh"
#include "./Transmitter.hh"
#include "./TrivialCounterMB.hh"
#include "./monitorp.hh"
#include "./nat.hh"
#include "./natft.hh"
#include "./natnf.hh"
#include "./natrp.hh"
#include "./timestamper.hh"

CLICK_USING_DECLS
static int hatred_of_rebecca[23];
static Element *
beetlemonkey(uintptr_t heywood)
{
  switch (heywood) {
   case 0: return new ArrCounterMB;
   case 1: return new CounterMB;
   case 2: return new FTAppenderElement;
   case 3: return new FTBufferElement;
   case 4: return new FTControlElement;
   case 5: return new FTFastTCPGen;
   case 6: return new FTFilterElement;
   case 7: return new FTLoggerElement;
   case 8: return new FTPassElement;
   case 9: return new FTRandomDropElement;
   case 10: return new FTStateElement;
   case 11: return new Monitor;
   case 12: return new NFArrCounterMB;
   case 13: return new NFCounterMB;
   case 14: return new TrArrCounterMB;
   case 15: return new Transmitter;
   case 16: return new TrivialCounterMB;
   case 17: return new Monitorp;
   case 18: return new nat;
   case 19: return new natft;
   case 20: return new natnf;
   case 21: return new natrp;
   case 22: return new Timestamper;
   default: return 0;
  }
}

#ifdef CLICK_LINUXMODULE
#define click_add_element_type(n, f, t) click_add_element_type((n), (f), (t), THIS_MODULE)
#endif
#ifdef CLICK_BSDMODULE
static int
modevent(module_t, int t, void *)
{
  if (t == MOD_LOAD) {
#else
extern "C" int
init_module()
{
#endif
  click_provide("FTSFC");
  hatred_of_rebecca[0] = click_add_element_type("ArrCounterMB", beetlemonkey, 0);
  hatred_of_rebecca[1] = click_add_element_type("CounterMB", beetlemonkey, 1);
  hatred_of_rebecca[2] = click_add_element_type("FTAppenderElement", beetlemonkey, 2);
  hatred_of_rebecca[3] = click_add_element_type("FTBufferElement", beetlemonkey, 3);
  hatred_of_rebecca[4] = click_add_element_type("FTControlElement", beetlemonkey, 4);
  hatred_of_rebecca[5] = click_add_element_type("FTFastTCPGen", beetlemonkey, 5);
  hatred_of_rebecca[6] = click_add_element_type("FTFilterElement", beetlemonkey, 6);
  hatred_of_rebecca[7] = click_add_element_type("FTLoggerElement", beetlemonkey, 7);
  hatred_of_rebecca[8] = click_add_element_type("FTPassElement", beetlemonkey, 8);
  hatred_of_rebecca[9] = click_add_element_type("FTRandomDropElement", beetlemonkey, 9);
  hatred_of_rebecca[10] = click_add_element_type("FTStateElement", beetlemonkey, 10);
  hatred_of_rebecca[11] = click_add_element_type("Monitor", beetlemonkey, 11);
  hatred_of_rebecca[12] = click_add_element_type("NFArrCounterMB", beetlemonkey, 12);
  hatred_of_rebecca[13] = click_add_element_type("NFCounterMB", beetlemonkey, 13);
  hatred_of_rebecca[14] = click_add_element_type("TrArrCounterMB", beetlemonkey, 14);
  hatred_of_rebecca[15] = click_add_element_type("Transmitter", beetlemonkey, 15);
  hatred_of_rebecca[16] = click_add_element_type("TrivialCounterMB", beetlemonkey, 16);
  hatred_of_rebecca[17] = click_add_element_type("Monitorp", beetlemonkey, 17);
  hatred_of_rebecca[18] = click_add_element_type("nat", beetlemonkey, 18);
  hatred_of_rebecca[19] = click_add_element_type("natft", beetlemonkey, 19);
  hatred_of_rebecca[20] = click_add_element_type("natnf", beetlemonkey, 20);
  hatred_of_rebecca[21] = click_add_element_type("natrp", beetlemonkey, 21);
  hatred_of_rebecca[22] = click_add_element_type("Timestamper", beetlemonkey, 22);
  CLICK_DMALLOC_REG("nXXX");
  return 0;
#ifdef CLICK_BSDMODULE
  } else if (t == MOD_UNLOAD) {
#else
}
extern "C" void
cleanup_module()
{
#endif
  click_remove_element_type(hatred_of_rebecca[0]);
  click_remove_element_type(hatred_of_rebecca[1]);
  click_remove_element_type(hatred_of_rebecca[2]);
  click_remove_element_type(hatred_of_rebecca[3]);
  click_remove_element_type(hatred_of_rebecca[4]);
  click_remove_element_type(hatred_of_rebecca[5]);
  click_remove_element_type(hatred_of_rebecca[6]);
  click_remove_element_type(hatred_of_rebecca[7]);
  click_remove_element_type(hatred_of_rebecca[8]);
  click_remove_element_type(hatred_of_rebecca[9]);
  click_remove_element_type(hatred_of_rebecca[10]);
  click_remove_element_type(hatred_of_rebecca[11]);
  click_remove_element_type(hatred_of_rebecca[12]);
  click_remove_element_type(hatred_of_rebecca[13]);
  click_remove_element_type(hatred_of_rebecca[14]);
  click_remove_element_type(hatred_of_rebecca[15]);
  click_remove_element_type(hatred_of_rebecca[16]);
  click_remove_element_type(hatred_of_rebecca[17]);
  click_remove_element_type(hatred_of_rebecca[18]);
  click_remove_element_type(hatred_of_rebecca[19]);
  click_remove_element_type(hatred_of_rebecca[20]);
  click_remove_element_type(hatred_of_rebecca[21]);
  click_remove_element_type(hatred_of_rebecca[22]);
  click_unprovide("FTSFC");
#ifdef CLICK_BSDMODULE
  return 0;
  } else
    return 0;
}
static moduledata_t modinfo = {
  "FTSFC", modevent, 0
};
DECLARE_MODULE(FTSFC, modinfo, SI_SUB_PSEUDO, SI_ORDER_ANY);
MODULE_VERSION(FTSFC, 1);
MODULE_DEPEND(FTSFC, click, 1, 1, 1);
#else
}
#endif
