/*
 * gnet.h (header files for GNET)
 */


#ifndef __GNET_H__
#define __GNET_H__


// all includes go here!
#include "grouter.h"
#include "vpl.h"
#include "device.h"
#include "simplequeue.h"
#include "message.h"
#include <pthread.h>



#define	MAX_INTERFACES					20      // max number of interfaces supported
#define GINI_IFACE_MAX MAX_INTERFACES

#define INTERFACE_DOWN                  'D'     // the down state of the interface
#define INTERFACE_UP                    'U'     // the up state of the interface
#define IFACE_CLIENT_MODE               'C'     // client mode interface
#define IFACE_SERVER_MODE               'S'     // server mode interface

#define ETH_DEV							2
#define TAP_DEV							3

/*
 * NOTE: The interface will be created in down state if the gnet_adapter could
 * not connect to the socket. Client mode, the user needs to reconnect. In server
 * mode, the server socket will automatically update the state once the remote
 * node initiates the connection.
 */
typedef struct _interface_t
{
	union {
		int interface_id;					// interface identifier
		int id;
	};
	int state;                          // active OR inactive
	int mode;                       	// client OR server mode
	char device_type[MAX_DNAME_LEN];	// device type specification
	char device_name[MAX_DNAME_LEN];	// full device name (e.g., eth0, wlan1)
	char sock_name[MAX_DNAME_LEN];
	uchar mac_addr[6];		        	// 6 for Ethernet MACs
	uchar ip_addr[4];		        	// 4 for Internet protocol as network address
	int device_mtu;						// maximum transfer unit for the device
	int iface_fd;						// file descriptor for ??
	vpl_data_t *vpl_data;				// vpl library structure
	pthread_t threadid;					// thread ID assigned to this interface
	pthread_t sdwthread;
	device_t *devdriver;				// the device driver that include toXDev and fromXDev functions
	void *iarray;                       // pointer to interface array type
} interface_t, GiniInterface;

typedef struct _interface_array_t
{
	int count;
	interface_t *elem[MAX_INTERFACES];
} interface_array_t;

typedef struct _vplinfo_t
{
	vpl_data_t *vdata;
	interface_t *iface;
} vplinfo_t;




// function prototype go here...

int GNETInit (pthread_t *ghandler, char *config_dir, char *rname, simplequeue_t *sq);
void GNETHalt(pthread_t gnethandler);
void GNETInsertInterface(interface_t *iface);
int destroyInterfaceByIndex(int indx);
int upInterface(int index);
int downInterface(int index);
void printInterfaces(int mode);
interface_t *GNETMakeEthInterface(char *vsock_name, char *device,
			   uchar *mac_addr, uchar *nw_addr, int iface_mtu, int cforce);
interface_t *GNETMakeTapInterface(char *device, uchar *mac_addr, uchar *nw_addr);
interface_t *findInterface(int indx);
void *delayedServerCall(void *arg);
void *GNETHandler(void *outq);
int changeInterfaceMTU(int index, int new_mtu);

GiniInterface * gini_iface_get  (int index);

GiniInterface * gini_iface_next (GiniInterface *iface);

#endif //__GNET_H__
