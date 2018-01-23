//Variables for analysing MIDI messages
let receivedMidiMessages = 0,
    // allColums and allRows have to be the same value as in C++
    allRows = 4,
    allColums = 10,
    allGridBoxes = allRows * allColums;


//////////// METHODS /////////////

// play the sounds depending on the MIDI Messages
function onMIDIPlayResonanceAudio(gridIndex, depthIndex) {
    if(gridIndex < allGridBoxes){
        calculatePosition(receivedMidiMessages, gridIndex, depthIndex);
        receivedMidiMessages += 1;

        // "if" for sending all messages of one frame together
        if (receivedMidiMessages >= soundObjectNumber) {
            setSoundPositions(); //set new calculated meter positions
            playSounds(); //play the sounds
            receivedMidiMessages = 0;
        }
    }
    else {
        console.log('MIDI message index not accepted');
    }
}

// calculate from MIDI indexes to meters
function calculatePosition(i, gridIndex, depthIndex) {
    // calculate colum and row of the gridIndex
    let colum = gridIndex % allColums,
        row = Math.trunc(gridIndex/allColums),
        meterX = (colum -8)*0.4, // X coordinate in meters
        meterY = -(row -3.5)*0.1 , // Y coordinate in meters
        depthFactorX = 0.1,
        depthFactorY = 0.1,
        meterZ = depthIndex * 0.8; // Z coorinate in meters

    if(meterX < 0) {
        depthFactorX = -depthFactorX;
    }
    if(meterY < 0) {
        depthFactorY = -depthFactorY;
    }
    // depthIndex have influence on the sound volume
    meterX += depthIndex * depthFactorX;
    meterY += depthIndex * depthFactorY;
    // save the metercoordinates in audioGridBox
    setAudioGridBox(i, meterX, meterY, meterZ);
}