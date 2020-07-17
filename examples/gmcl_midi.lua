require("midi")

-- You MUST call this function before the MIDI hook will work. You need to connect to the actual MIDI device.
-- The default value for the "port" parameter is 0. An error will be thrown if there are no MIDI devices connected to the PC. Hence the check.
if table.Count(midi.GetPorts()) > 0 then -- use table.Count here, the first index is 0
    print("opening midi")
    midi.Open()
end

print(midi.Open())

hook.Add("MIDI", "print midi events", function(time, code, par1, par2, ...)
    -- The code is a byte (number between 0 and 254).
    print("A midi event happened at ".. time .. ", which is " .. SysTime() - time .. " seconds ago!")
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
