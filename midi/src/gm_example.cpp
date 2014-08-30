#include "GarrysMod/Lua/Interface.h"
#include <stdio.h>
#include "RtMidi.h"

using namespace GarrysMod::Lua;

RtMidiIn *midiin = 0;
std::vector<std::vector<unsigned char>> messageList;

/*
	Called when a MIDI event occurs
*/
void onMidiCallback(double deltatime, std::vector<unsigned char> *message, void * /*userData*/)
{
  unsigned int nBytes = message->size();
  if (nBytes == 0) return;

  messageList.push_back(*message);
}

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

/*
	This function is called every frame.
	It's used for syncing the MIDI events with Lua
*/
int pollMidi(lua_State* state)
{
	unsigned int messagesSize = messageList.size();
	if (messagesSize == 0) return 1;

	
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "hook");
			for (unsigned int i = 0; i < messagesSize; i++ ) {
				unsigned int msgSize = messageList.at(i).size();


				LUA->GetField(-1, "Call");
				LUA->PushString("MIDI");
				LUA->PushNil();

				for (unsigned int j = 0; j < msgSize; j++ ) {
					LUA->PushNumber(messageList.at(i).at(j));
				}
				LUA->Call(2 + msgSize, 0);
			}
	LUA->Pop();


	messageList.clear();

	return 1;
}

//
// Called when module is opened
//
GMOD_MODULE_OPEN()
{
	midiin = new RtMidiIn();
	midiin->setCallback(&onMidiCallback);

	// Add the polling hook
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->GetField(-1, "hook");
		LUA->GetField(-1, "Add");
		LUA->PushString("Think");
		LUA->PushString("Midi polling");
		LUA->PushCFunction(pollMidi);
		LUA->Call(3, 0);
	LUA->Pop();

	// Create the midi table
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