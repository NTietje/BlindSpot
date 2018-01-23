// audiocontext and variables
let audioContext = new AudioContext(),
    soundObjectNumber = 1, // number of nearest objects (received MIDIs from C++)
    sounds = [], // audio array
    sources = [], // resonance audio sources array
    audioGridBoxes = [],
    materialName = 'transparent',
    init = true;


// Create a resonance audio scene
let resonanceAudioScene = new ResonanceAudio(audioContext, {
    ambisonicOrder: 3,
    dimensions: {
        width: 50,
        height: 6,
        depth: 100
    },
    materials: {
        left: materialName,
        right: materialName,
        front: materialName,
        back: materialName,
        down: materialName,
        up: materialName,
    },
  });

// Send resonance audio output to destination
resonanceAudioScene.output.connect(audioContext.destination);

// Creates audio resonance sounds/sources
createSoundElements('sonar1.wav');


///////////// CLASSES /////////////

// class audioGridBox as cache, also to query positions
function audioGridBox(x, y, z) {
    this.x = x;
    this.y = y;
    this.z = z;
}


///////////// METHODS /////////////

function createSoundElements(soundName) {
    //Create sounds (audio, mediaElementSource, Connection to filter, position)
    for(let i = 0; i < soundObjectNumber; i++) {

        //Create audio from wav and media elements
        sounds[i] = new Audio('../sounds/' + soundName);
        let mediaElements = [];
        mediaElements[i] = audioContext.createMediaElementSource(sounds[i]);

        //Create resonance audio sources and connect with media elements
        sources[i] = resonanceAudioScene.createSource();
        mediaElements[i].connect(sources[i].input);

        //Only for initializing
        if(init) {
            init = false;
            //Default value for audioGridBoxes, same position as listener
            audioGridBoxes[i] = new audioGridBox(0,0,0);
        }

        //Set source positions
        sources[i].setPosition(audioGridBoxes[i].x,audioGridBoxes[i].y,audioGridBoxes[i].z);
    }
}


// play the sounds, calculate delay
function playSounds() {
    let time = 0; // time of delay
    for(let i = 0; i < soundObjectNumber; i++){
        playDelayed(time, i); // i as sounds index
        time = (time + 200);

        //time depending on default value and range (depth)
        //time = time + 50 + 250/audioGridBoxes[i].z;

        //time depending only on range (depth)
        //time = audioGridBoxes[i].z * 50;
    }
}


// play sounds
// if soundObjectNumber > 1 it's important to play each sound with delay
function playDelayed(time, i) {
    setTimeout(function(){
        //sounds[i].pause();
        sounds[i].currentTime = 0; // set audio back to second 0
        sounds[i].play();
    }, time);
}


// set positions, from audioGridBoxes array to resonance audio sources array
function setSoundPositions() {
    for (let i = 0; i < soundObjectNumber; i++) {
        sources[i].setPosition(audioGridBoxes[i].x,audioGridBoxes[i].y,audioGridBoxes[i].z);
    }
}


// save new positions in audioGridBoxes array
function setAudioGridBox(i, x, y, z) {
    audioGridBoxes[i].x = x;
    audioGridBoxes[i].y = y;
    audioGridBoxes[i].z = z;
}


///////////// EVENT LISTENER /////////////


// Selected sound event listener
let soundSelecter = document.getElementById('sounds');
soundSelecter.addEventListener('change', function (e) {
    let soundName = soundSelecter.options[soundSelecter.selectedIndex].value;
    createSoundElements(soundName + '.wav');
});