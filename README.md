gmcl_piano
================

A clientside C++ module for Garry's mod that allows you to read events from a MIDI device. Basically it makes you able to play keyboard in gmod if you have a MIDI to USB cable.

# Download
Please see the [Releases](https://github.com/FPtje/gmcl_midi/releases) tab

# How to install
Windows:

> Put gmcl_midi_win32.dll in `garrysmod/lua/bin/`

It is possible that there is no `bin` folder in `garrysmod/lua`. In that case you can just create it.


# How to use
1. `require "midi"`
2. Figure out which devices you can connect to with `midi.GetPorts()`
3. Open a connection with your midi device using `midi.Open(portNumber)`. By default it will try to open port 0. An error is thrown if there are no devices.
4. Add a listener to the `"MIDI"` hook. This is where all the events come in.
5. In the listener, filter the events you want to listen to and perform actions based on these events.

# API
Taking a quick glance at [the MIDI protocol](https://ccrma.stanford.edu/~craig/articles/linuxmidi/misc/essenmidi.html) will make understanding this API a *lot* easier.

## Hook: `"MIDI"`
Called when a MIDI event occurs (when a key is pressed, released, when the pitch bend changes, etc).

Parameters:
- `command`: The command code of the event. This number contains **BOTH** the command code (first four bits of the number) **AND** the channel to which the command applies (last four bits of the number). These two can be separated with the `midi.GetCommandCode(command)` (returns just the command code) and `midi.GetCommandChannel(command)`. To get a human readable name of the command code, you can run `midi.GetCommandName(command)`.
- `par1`: Many commands have one or more parameters. The command code `144` (named `"NOTE_ON"`) has two parameters: the number of the key pressed and the velocity (how hard it was pressed).
- `...`: other parameters


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
