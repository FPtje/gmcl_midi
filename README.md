# gmcl\_midi

A clientside C++ module for [Garry's Mod](https://gmod.facepunch.com/) that allows you to read events from a MIDI device.

## Download

Visit the [Releases](/../../releases) page for the latest release. Make sure to download the correct `.dll` file according to your system and branch of Garry's Mod:

> `gmcl_midi_[win/linux/osx][32/64].dll`

**NOTE**: Using the 64-bit of the module will depend on whether you're on the `x86-64` branch for Garry's Mod.

## Installation

Place the downloaded binary module from the [Download](#Download) section of ths is readme in the following folder:

> `steamapps/common/GarrysMod/garrysmod/lua/bin`

**NOTE**: It's possible that there's no `bin` folder in `garrysmod/lua`. In that case you can just create it.

## Usage

If you're using this for an existing addon, feel free to stop reading here since this only applies to developers.

1. Require the module with `require "midi"`.
2. Figure out which devices you can connect to with `midi.GetPorts()`.
3. Open a connection to your midi device using `midi.Open(portNumber)`. By default it will try to open port 0. An error is thrown if there are no devices.
4. Add a listener to the `GM:MIDI` hook. This is where all the events come in.
5. In the listener, filter the events you want to listen to and perform actions based on these events.

### API

Taking a quick glance at [the MIDI protocol](https://ccrma.stanford.edu/~craig/articles/linuxmidi/misc/essenmidi.html) will make understanding this API a *lot* easier.

#### GM:MIDI

Called when a MIDI event occurs (when a key is pressed, released, when the pitch bend changes, etc). This hook will start working immediately after a port has been opened (see `midi.Open(port)`)

##### Arguments

- `time`: The exact SysTime at which the MIDI event occurred. This is useful when you want to know exactly when a note was played. Note: this is independent of FPS
- `command`: The command code of the event. This number contains **BOTH** the command code (first four bits of the number) **AND** the channel to which the command applies (last four bits of the number). These two can be separated with the `midi.GetCommandCode(command)` (returns just the command code) and `midi.GetCommandChannel(command)`. To get a human readable name of the command code, you can run `midi.GetCommandName(command)`.
- `par1`: Many commands have one or more parameters. The command code `144` (named `"NOTE_ON"`) has two parameters: the number of the key pressed and the velocity (how hard it was pressed).
- `...`: other parameters

#### midi.GetPorts()

The average computer has more than one USB port. This makes it possible to connect multiple MIDI devices. Only one of these devices can be listened to at once. This function returns a table with a device's `port` as index and the device's name as value.

##### Example

```lua
lua_run_cl show(midi.GetPorts())
>{[0]: "USB MIDI 0"}
```

#### midi.Open(port = 0)

Open a connection with a MIDI device. Possible values for `port` are the keys of the table returned by `midi.GetPorts()`. By default port 0 is tried.

> The `"MIDI"` hook will **not** be called until a port has been opened!

> An error will be thrown if there are no MIDI devices available.

> Only one port can be opened at once. The first port will be closed when connecting to a second port.

#### midi.IsOpened()

Returns whether the module is connected to a device.

#### midi.Close()

Close the connection if it exists. No MIDI events will be received after this function has been called until of course `midi.Open(port)` is called.

> Note: Connections don't have to be closed. Changing level without closing a connection is fine. Only close the connection when you're the only user of the MIDI hook. Otherwise you'll mess up other scripts.

#### midi.GetCommandCode(command)

Helper function for the command (second) parameter of the function called by the `"MIDI"` hook. As explained in the [the MIDI protocol](https://ccrma.stanford.edu/~craig/articles/linuxmidi/misc/essenmidi.html), the command code exists of 4 bits indicating the command code and 4 bits indicating the channel to which the command applies. The `midi.GetCommandCode(command)` extracts the command code. (Basically it returns `command & 0xF0`).

#### midi.GetCommandChannel(command)

Whereas the `midi.GetCommandCode(command)` returns the command code, the `midi.GetCommandChannel(command)` function returns the channel to which the command applies. Possible channels are `0 <= channel < 16`.

#### midi.GetCommandName(command)

This function takes a command (as given in the second parameter of a function attached to the `"MIDI"` hook) and returns a human readable name.

##### Example

```lua
] print(midi.GetCommandName(0x80))
NOTE_OFF
```

##### Codes

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

# Examples

See the [examples](examples/) folder.
