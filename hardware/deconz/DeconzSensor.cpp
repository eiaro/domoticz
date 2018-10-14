#include "stdafx.h"
#include "../../json/value.h"
#include "DeconzSensor.h"

DeconzSensor::DeconzSensor(Json::Value sensor) :  DeconzNode(sensor), state(sensor["state"]), config(sensor["config"])
{
};

DeconzSensor::DeconzSensor() :  DeconzNode()
{
	
}

bool DeconzSensor::process_event(Json::Value root)
{
	bool is_updated = false;

	if (root.isMember("config"))
	{
		is_updated |= config.process(root["config"]);
	}
	if (root["state"] != Json::Value::null)
	{
		is_updated |= state.process(root["state"]);
	}
	
	return is_updated;
};

SensorState::SensorState(): buttonevent(0), presence(false), temperature(0), lightlevel(0), dark(false), daylight(false)
{	
}

SensorState::SensorState(Json::Value state)
{	
	lastupdated = (state["lastupdated"] != Json::Value::null) ? state["lastupdated"].asString() : "none";
	buttonevent = (state["buttonevent"] != Json::Value::null) ? state["buttonevent"].asInt() : 0;
	presence = (state["presence"] != Json::Value::null) ? state["presence"].asBool() : false;
	temperature = (state["temperature"] != Json::Value::null) ? state["temperature"].asInt() : 0;
	lightlevel = (state["lightlevel"] != Json::Value::null) ? state["lightlevel"].asInt() : 0;
	dark = (state["dark"] != Json::Value::null) ? state["dark"].asBool() : false;
	daylight = (state["daylight"] != Json::Value::null) ? state["daylight"].asBool() : false;
}

bool SensorState::process(Json::Value state)
{
	bool hasChanged = false;
	if (state.isMember("lastupdated"))
	{
		lastupdated = state.get("lastupdated", lastupdated).asString();
		hasChanged = true;
	}
	if (state.isMember("buttonevent"))
	{
		buttonevent = state.get("buttonevent", lastupdated).asInt();
		hasChanged = true;
	}
	if (state.isMember("presence"))
	{
		presence = state.get("presence", lastupdated).asBool();
		hasChanged = true;
	}
	if (state.isMember("temperature"))
	{
		temperature = state.get("temperature", lastupdated).asInt();
		hasChanged = true;
	}
	if (state.isMember("lightlevel"))
	{
		lightlevel = state.get("lightlevel", lastupdated).asInt();
		hasChanged = true;
	}
	if (state.isMember("dark"))
	{
		dark = state.get("dark", lastupdated).asBool();
		hasChanged = true;
	}
	if (state.isMember("daylight"))
	{
		daylight = state.get("daylight", lastupdated).asBool();
		hasChanged = true;
	}

	return hasChanged;
}


SensorState::~SensorState()
{	
}

SensorConfig::SensorConfig(): on(false), reachable(false), battery(100), tholddark(0),
tholdoffset(0), sunriseoffset(0), sunsetoffset(0), configured(false)
{	
}

SensorConfig::SensorConfig(Json::Value config)
{
	on = config["on"].asBool();
	reachable = (config["reachable"] != Json::Value::null) ? config["reachable"].asBool() : true;
	battery = (config["battery"] != Json::Value::null) ? config["battery"].asInt() : 100;
	tholddark = (config["tholddark"] != Json::Value::null) ? config["tholddark"].asInt() : 0;
	tholdoffset = (config["tholdoffset"] != Json::Value::null) ? config["tholdoffset"].asInt() : 0;
	configured = (config["configured"] != Json::Value::null) ? config["configured"].asBool() : true;
	sunriseoffset = (config["sunriseoffset"] != Json::Value::null) ? config["sunriseoffset"].asInt() : 0;
	sunsetoffset = (config["sunsetoffset"] != Json::Value::null) ? config["sunsetoffset"].asInt() : 0;
}

bool SensorConfig::process(Json::Value config)
{
	bool hasChanged = false;
	if (config.isMember("on"))
	{
		on = config.get("on", on).asBool();
		hasChanged = true;
	}
	if(config.isMember("reachable"))
	{
		reachable = config.get("reachable", reachable).asBool();
		hasChanged = true;
	}
	if(config.isMember("battery"))
	{
		battery = config.get("battery", battery).asInt();
		hasChanged = true;
	}
	if (config.isMember("tholddark"))
	{
		tholddark = config.get("tholddark", battery).asInt();
		hasChanged = true;
	}
	if (config.isMember("tholdoffset"))
	{
		tholdoffset = config.get("tholdoffset", battery).asInt();
		hasChanged = true;
	}
	if (config.isMember("configured"))
	{
		configured = config.get("configured", battery).asBool();
		hasChanged = true;
	}
	if (config.isMember("sunriseoffset"))
	{
		sunriseoffset = config.get("sunriseoffset", battery).asInt();
		hasChanged = true;
	}
	if (config.isMember("sunsetoffset"))
	{
		sunriseoffset = config.get("sunsetoffset", battery).asInt();
		hasChanged = true;
	}

	return hasChanged;
}

SensorConfig::~SensorConfig()
{	
}


