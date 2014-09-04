gmcl_piano
================

A clientside C++ module for Garry's mod that allows you to read events from a MIDI device. Basically it makes you able to play keyboard in gmod if you have a MIDI to USB cable.

# Download
Please see the [Releases](https://github.com/FPtje/gmcl_midi/releases) tab

# How to install
Windows:

> Put gmcl_midi_win32.dll in `garrysmod/lua/bin/`

It is possible that there is no `bin` folder in `garrysmod/lua`. In that case you can just create it.
You're done here if you want to use this module with an existing addon. Everything below here is for people who want to make Lua scripts with this module.

# How to use (for developers)
1. `require "midi"`
2. Figure out which devices you can connect to with `midi.GetPorts()`
3. Open a connection with your midi device using `midi.Open(portNumber)`. By default it will try to open port 0. An error is thrown if there are no devices.
4. Add a listener to the `"MIDI"` hook. This is where all the events come in.
5. In the listener, filter the events you want to listen to and perform actions based on these events.

# API
Taking a quick glance at [the MIDI protocol](https://ccrma.stanford.edu/~craig/articles/linuxmidi/misc/essenmidi.html) will make understanding this API a *lot* easier.

### Hook: `"MIDI"`
Called when a MIDI event occurs (when a key is pressed, released, when the pitch bend changes, etc). This hook will start working immediately after a port has been opened (see `midi.Open(port)`)

Parameters:
- `command`: The command code of the event. This number contains **BOTH** the command code (first four bits of the number) **AND** the channel to which the command applies (last four bits of the number). These two can be separated with the `midi.GetCommandCode(command)` (returns just the command code) and `midi.GetCommandChannel(command)`. To get a human readable name of the command code, you can run `midi.GetCommandName(command)`.
- `par1`: Many commands have one or more parameters. The command code `144` (named `"NOTE_ON"`) has two parameters: the number of the key pressed and the velocity (how hard it was pressed).
- `...`: other parameters

### midi.GetPorts()
The average computer has more than one USB port. This makes it possible to connect multiple MIDI devices. Only one of these devices can be listened to at once. This function returns a table with a device's `port` as index and the device's name as value.

Example:
```lua
lua_run_cl show(midi.GetPorts())
>{[0]: "USB MIDI 0"}
```

### midi.Open(port = 0)
Open a connection with a MIDI device. Possible values for `port` are the keys of the table returned by `midi.GetPorts()`. By default port 0 is tried.

> The `"MIDI"` hook will **not** be called until a port has been opened!

> An error will be thrown if there are no MIDI devices available.

> Only one port can be opened at once. The first port will be closed when connecting to a second port.

### midi.IsOpened()
Returns whether the module is connected to a device.

### midi.Close()
Close the connection if it exists. No MIDI events will be received after this function has been called until of course `midi.Open(port)` is called.

> Note: Connections don't have to be closed. Changing level without closing a connection is fine. Only close the connection when you're the only user of the MIDI hook. Otherwise you'll mess up other scripts.


### midi.GetCommandCode(command)
Helper function for the first parameter of the function called by the `"MIDI"` hook. As explained in the [the MIDI protocol](https://ccrma.stanford.edu/~craig/articles/linuxmidi/misc/essenmidi.html), the command code exists of 4 bits indicating the command code and 4 bits indicating the channel to which the command applies. The `midi.GetCommandCode(command)` extracts the command code. (Basically it returns `command & 0xF0`).


### midi.GetCommandChannel(command)
Whereas the `midi.GetCommandCode(command)` returns the command code, the `midi.GetCommandChannel(command)` function returns the channel to which the command applies. Possible channels are `0 <= channel < 16`.

### midi.GetCommandName(command)
This function takes a command (as given in the first parameter of a function attached to the `"MIDI"` hook) and returns a human readable name.
Example: (please see the `MIDI commands` table in the MIDI protocol desciption)
```lua
] print(midi.GetCommandName(0x80))
NOTE_OFF
```

Possible code names:

```lua
0x80 - "NOTE_OFF"
0x90 - "NOTE_ON"
0xA0 - "AFTERTOUCH"
0xB0 - "CONTINUOUS_CONTROLLER"
0xC0 - "PATCH_CHANGE"
0xD0 - "CHANNEL_PRESSURE"
0xE0 - "PITCH_BEND"
0xF0 - "SYSEX"
```

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

    if midi.GetCommandName(code) == "NOTE_ON" and par2 > 0 then
        -- key par1 has been pressed with par2 velocity (note: note has not yet been released)
    elseif midi.GetCommandName(code) == "NOTE_OFF" or midi.GetCommandName(code) == "NOTE_ON" and par2 == 0 then -- NOTE: some keyboards register the releasing of a key as a NOTE_ON with 0 velocity
        -- key par1 has been released.
    end

end)

-- Only listen for 10 minutes
timer.Simple(600, function()
    if not midi.IsOpened() then return end -- closing MIDI will error if it isn't opened
    midi.Close() -- Call midi.Close when you want to stop receiving MIDI events. Closing is NOT obligatory.
end)
```
