## Log of Simulink Model Changes (and other notes)

* Model created according to the design specifications laid out in the DESIGN.md document.
* Audio was being output with large level of clicks and distortion

* Gain Computer and Level Detector blocks operate as custom MATLAB functions integrated into simulink.
	* Blocks were modified to process every sample in each frame by adding for loops to process all the information
	* Had to add additional Reshape Simulink block to both the Gain Computer and Level Detector blocks to restore Matrix dimensions after MATLAB function modifies the information
* There was no longer any audio output.
* Created an Audio Output Analyzer utility block
	* Block constructed out of a db-to-linear convertion block fed into a "realtime" output, Audio Device Writer block. By attaching input of utility block to any point in the side chain, we can transfer back to linear domain and hear our audio. Additionally, a Time Scope was connected to the output of the db-to-linear conversion block to give a graphic representation of the audio at each stage in the side chain.
	* db-to-linear block can be bypassed if the block needs to be connected to a linear part of the side chain.
	* NOTE: Only one Audio Device Writer can be present in a Simulink design at a time.
* Used utility block to test each part of the side-chain
	* Problem area was the first subtraction block - no audio from the output.
	* Fixed problem by manually negating output of Gain Computer and then feeding into a sum block with the Gain Computer input.

* Created sliders for the threshold value and ratio value
	* These rudimentary sliders can be used to change the threshold and ratio while audio is being processed, so that you can hear and see the change that each parameter is making on the audio.
* Audio is now being correctly affected by gain computer, but compression function needs to be adjusted to prevent distortion from low threshold levels and high ratios. 

* Adjustment of the attack and release constants from the Level Detector resulted in further error.
	* Original working values are (Attack=0.005, Release= 0.01)
