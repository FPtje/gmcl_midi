#include "GarrysMod/Lua/Interface.h"
#include <stdio.h>
#include "RtMidi.h"

using namespace GarrysMod::Lua;

RtMidiIn *midiin = 0;

/*
	Get the available MIDI ports
*/
int getPorts(lua_State* state)
{
	unsigned int portCount = midiin->getPortCount();

	LUA->CreateTable();

	for (unsigned int i = 0; i < portCount;i++) {
		LUA->PushNumber(i);
		LUA->PushString(midiin->getPortName(i).c_str());
		LUA->SetTable(-3);
	}

	return 1;
}

/*
	Open the Midi device
*/
int openMidi(lua_State* state)
{
	int port = 0;
	if (LUA->IsType(1, Type::NUMBER))
		port = (int) LUA->GetNumber(1);

	if (midiin->getPortCount() == 0) {
		LUA->ThrowError("No input ports available!");
		return 0;
	}

	try {
		midiin->openPort(port);
		LUA->PushString(midiin->getPortName().c_str());
	}
	catch(RtMidiError &error) {
		LUA->ThrowError(error.getMessage().c_str());
		return 0;
	}

	return 1;
}


//
// Called when module is opened
//
GMOD_MODULE_OPEN()
{
	midiin = new RtMidiIn();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->CreateTable();
			LUA->PushCFunction(getPorts); LUA->SetField(-2, "GetPorts");
			LUA->PushCFunction(openMidi); LUA->SetField(-2, "Open");
		LUA->SetField(-2, "midi");
	LUA->Pop();

	return 0;
}

//
// Called when your module is closed
//
GMOD_MODULE_CLOSE()
{
	delete midiin;

	return 0;
}