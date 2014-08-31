gmcl_piano
================

A clientside C++ module for Garry's mod that allows you to read events from a MIDI device. Basically it makes you able to play keyboard in gmod if you have a MIDI to USB cable.

# Download
Please see the [Releases](https://github.com/FPtje/gmcl_midi/releases) tab

# How to use
1. `require "midi"`
2. Figure out which devices you can connect to with `midi.GetPorts()`
3. Open a connection with your midi device using `midi.Open(portNumber)`. By default it will try to open port 0. An error is thrown if there are no devices.
4. Add a listener to the `"MIDI"` hook. This is where all the events come in.
5. In the listener, filter the events you want to listen to and perform actions based on these events.

# Example
```Lua
require("midi")

-- You MUST call this function before the MIDI hook will work. You need to connect to the actual MIDI device.
-- The default value for the "port" parameter is 0. An error will be thrown if there are no MIDI devices connected to the PC. Hence the check.
if table.Count(midi.GetPorts()) > 0 then -- use table.Count here, the first index is 0
    print("opening midi")
    midi.Open()
end

print(midi.Open())

hook.Add("MIDI", "print midi events", function(code, par1, par2, ...)
    -- The code is a byte (number between 0 and 254).
    print("MIDI EVENT", code, par1, par2, ...)
    print("Event code:", midi.GetCommandCode(code))
    print("Event channel:", midi.GetCommandChannel(code))
    print("Event name:", midi.GetCommandName(code))

    -- The parameters of the code
    print("parameters", par1, par2, ...)
end)

-- Only listen for 10 minutes
timer.Simple(600, function()
    if not midi.IsOpened() then return end -- closing MIDI will error if it isn't opened
    midi.Close() -- Call midi.Close when you want to stop receiving MIDI events. Closing is NOT obligatory.
end)
```
