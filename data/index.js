// Keyboard capture script

let keystrokes = {};

// flashled function for mcu
function flashLED(times) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/flash-led?times=" + times, true);
    xhr.send();
  }

// print every key press to console and put it in a buffer for mutli key
document.addEventListener('keydown', event => {
    if (event.key === '^') {
        console.log('^');
        return;
    }
    if (event.shiftKey && event.key.length === 1) {
        keystrokes[`${event.key.toUpperCase()}`] = true;
    } else {
        keystrokes[`${event.key}`] = true;
    }

    const keys = Object.keys(keystrokes);
    if (keys.length === 2) {
        console.log(`${keys[0]} + ${keys[1]}`);
    }
    flashLED(2);
    console.log(`${event.key}`);
});

// remove keypress from buffer on keyup
document.addEventListener('keyup', event => {
    if (event.shiftKey && event.key.length === 1) {
        delete keystrokes[`${event.key.toUpperCase()}`];
    } else {
        delete keystrokes[`${event.key}`];
    }

    //console.log(`UP: ${event.key}`);
});