# Nutekt Mosc
 Multi Oscillator for Korg Nutekt NTS-1
 
 ## Features
 - 2 oscillators
 - Each oscillator can be set to sine/sawtooth/parabolic/square wave
 - Tweak the interval between the 2 oscillators
 
 ## Parameters
|offs| 0 -> 100|interval between 2 oscillators
|osc1| 1 ->  5 |1: sine, 2: sawtooth, 3: parabola, 4: square, 5: off
|osc2| 1 ->  5 |1: sine, 2: sawtooth, 3: parabola, 4: square, 5: off
|PW1|  0 -> 100|Pulse Width
|PW2|  0 -> 100|Pulse Width
 
 ## Load
 - Install NTS-1 digital librarian
 - Plug-in your Nutekt NTS-1
 - Select right midi port in the librarian
 - Import the .ntkdigunit file
 - "Send all"
 
 ## Set-up dev environement
 
- Clone the [logue sdk](https://github.com/korginc/logue-sdk).
- Follow the instructions to install tools
- Download/clone entire folder and move it under logue-sdk/platform/nutekt-digital
- run "make" command
- Use logue-cli tool to upload the created .ntkdigunit file
