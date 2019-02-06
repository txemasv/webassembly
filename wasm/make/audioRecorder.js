//please implement those APIs of AudioRecorder so that it can record MP3 files
//use external libs as you wish (you can search for lamejs)
//create new HTML or any extra js files as you wish
//the MP3 encoding should be processed in a web worker

var AudioRecorder = (function() {
	
	return {
		record: function() {
			//start audio recording if recording is not started yet
			//or resume audio recording if the recording is paused
			console.log("record")
			
		},
		pause: function() {
			//pause audio recording
			console.log("pause");
			
		},
		stop: function() {
			//stop the currend audio recording
			//and return the recorded MP3 file
			console.log("stop");
		},
		getCurrentPosition: function() {
			//return the current recorded time in seconds
			console.log("getCurrentPosition");
		},
		reset: function() {
			//drop all recorded data or any related states, so that a new recorder can be started
			console.log("reset");
		},
		PlayBack: {
			//APIs here cannot be called before stop() function is called
			play: function() {
				//play the most recent recorder mp3 file
				//or resume audio playback if the playback is paused
				console.log("PlayBack.play");
			},
			pause: function() {
				//pause audio
				console.log("PlayBack.pause");
			},
		}
	}
})();