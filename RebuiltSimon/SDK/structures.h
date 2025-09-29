#ifndef SDK_H_
#define SDK_H_

#include <stdint.h>
#include <stdbool.h>
#include "pch.h"

#define CONCAT(A, B) A##B
#define PADNAME(line) CONCAT(pad, line)
#define PAD(amount) char PADNAME(__LINE__)[amount]

#define OFFSET(base, offset) ((int)base + offset)

typedef struct plugin_info_s
{
	const char* pszName;
	const char* pszDescription;
	const char* pszContact;
	int nVerMajor;
	int nVerMinor;
	int nBuild;
} plugin_info_t;

typedef enum cactive_e
{
	ca_dedicated = 0,
	ca_disconnected,
	ca_connecting,
	ca_connected,
	ca_uninitialized,
	ca_active
} cactive_t;

typedef struct client_static_s
{
	cactive_t state;

	// TODO: Complete struct
} client_static_t;

typedef struct server_log_s
{
	qboolean active;
	qboolean net_log_;
	netadr_t net_address_;
	void* file;
} server_log_t;

typedef struct server_stats_s
{
	int num_samples;
	int at_capacity;
	int at_empty;
	float capacity_percent;
	float empty_percent;
	int minusers;
	int maxusers;
	float cumulative_occupancy;
	float occupancy;
	int num_sessions;
	float cumulative_sessiontime;
	float average_session_len;
	float cumulative_latency;
	float average_latency;
} server_stats_t;

typedef struct server_static_s
{
	qboolean dll_initialized;
	struct client_s* clients;
	int maxclients;
	int maxclientslimit;
	int spawncount;
	int serverflags;
	server_log_t log;
	double next_cleartime;
	double next_sampletime;
	server_stats_t stats;
	qboolean isSecure;
} server_static_t;

typedef struct simon_s
{
	PAD(0x2038);
	int32_t player_index; /* 0x2038 */
	PAD(0x1AC);
	int32_t infinite_stamina; /* 0x21E8 */
	PAD(0x4);
	float stamina; /* 0x21F0 */
	PAD(0x304);
	int32_t hp_view; /* 0x24F8 */ /* Not the actual hp value but just a view */
} simon_t;

typedef struct cof_data_s
{
	int version;

	cl_enginefunc_t* pEngine;
	cldll_func_t* pClient;
	enginefuncs_t* pServer;
	DLL_FUNCTIONS* pgEntityInterface;
	playermove_t* gclmove;
	edict_t** gsv_player;
	globalvars_t** gGlobals;

	client_static_t* pClientState;
	server_static_t* pServerState;

	playermove_t* pmove;
} cof_data_t;

#endif /* SDK_H_ */