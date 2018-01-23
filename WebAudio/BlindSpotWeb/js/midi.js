if (navigator.requestMIDIAccess) {
    navigator.requestMIDIAccess({sysex: false}).then(function(midiAccess) {
        midi = midiAccess;
        let inputs = midi.inputs.values();
        // loop through all inputs
        for (var input = inputs.next(); input && !input.done; input = inputs.next()) {
            // listen for midi messages
            onMIDIMessage;
        }
    });
} else {
    alert('No MIDI support in your browser.');
    console.log('alert');
}


//////////// METHODS /////////////

function onMIDIMessage(event) {
    // The C++ application is only sending status "note on", therefore switch is not needed.
    let gridIndex, depthIndex;

    //Byte1 is event.data[1], includes coordinates-index of a near object
    gridIndex = event.data[1];

    //Byte2 is event.data[2], includes z-index of a near object
    depthIndex = event.data[2];

    onMIDIPlayResonanceAudio(gridIndex, depthIndex);
}