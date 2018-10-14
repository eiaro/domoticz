#pragma once

#include "../DomoticzHardware.h"
#include "DeconzLight.h"
#include "DeconzSensor.h"
#include <queue>
#include <Defs.h>

namespace boost {
	namespace system {
		class error_code;
	}
}

namespace Json {
	class Value;
}

/** 
 * \brief Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both windows and 
 * linux. Source: https://stackoverflow.com/a/1861337/37583
 * \return Returns the amount of milliseconds elapsed since UNIX epoch.
 */
inline uint64 GetTimeMs64()
{
#ifdef _WIN32
	/* Windows */
	FILETIME ft;
	LARGE_INTEGER li;

	/* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
	 * to a LARGE_INTEGER structure. */
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	uint64 ret = li.QuadPart;
	ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	return ret;
#else
	/* Linux */
	struct timeval tv;

	gettimeofday(&tv, NULL);

	uint64 ret = tv.tv_usec;
	/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
	ret /= 1000;

	/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	ret += (tv.tv_sec * 1000);

	return ret;
#endif
}

#define DECONZ "deCONZ"
#define DECONZ_DEFAULT_POLL_INTERVAL 15

class Deconz : public CDomoticzHardwareBase
{
public:
	Deconz(const int id, const std::string &ipAddress, const unsigned short port, const std::string &username, const int poll, const int options);
	~Deconz(void);
	bool WriteToHardware(const char *pdata, const unsigned char length) override;
	static std::string RegisterUser(const std::string &IPAddress, const unsigned short Port, const std::string &username);
private:
	static void Init();
	Json::Value get_configuration();
	bool StartHardware() override;
	bool StopHardware() override;
	void Do_Work();
	void do_work_websocket();
	void received_event(const std::string& eventData);
	void SendSensorUpdate(int id, DeconzSensor sensor, Json::Value root, const bool isNew);
	bool GetStates();
	bool GetLights(const Json::Value &root);	
	bool GetSensors(const Json::Value &root);
	bool SwitchLight(const int nodeID, const std::string &LCmd, const int svalue, const int svalue2 = 0, const int svalue3 = 0);
	void InsertUpdateSwitch(int NodeID, _eSwitchType SType, bool status, const std::string& Name, uint8_t BatteryLevel);
	void InsertUpdateSwitch(const int NodeID, const DeconzLight &light, const std::string& Options, const bool AddMissingDevice);

	int poll_interval;	
	std::string ipAddress;
	unsigned short port;
	unsigned short websocketport;
	std::string username;
	std::shared_ptr<std::thread> thread;
	std::shared_ptr<std::thread> websocketThread;
	std::map<int, DeconzLight> lights;	
	std::map<int, DeconzSensor> sensors;
	bool isStopping;

	std::mutex queueLock;
	std::queue<std::string> queue;
};


