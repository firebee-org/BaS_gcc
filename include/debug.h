#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define dbg(format, arg...) do { xprintf("DEBUG (%s()): " format, __FUNCTION__, ##arg);} while(0)
#else
#define dbg(format, arg...) do {;} while (0)
#endif /* DEBUG */
#define err(format, arg...) do { xprintf("ERROR (%s()): " format, __FUNCTION__, ##arg); } while(0)
#define inf(format, arg...) do { xprintf("" format, ##arg); } while(0)

#endif // DEBUG_H

