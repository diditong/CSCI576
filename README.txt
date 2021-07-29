******USAGE******
Working directory: proj_code
Data to use: test_data 

Stage 1: Analyze the data
a. Run audio analyzer (AudioAnalyzer.ipynb/AudioAnalyzer.py)
b. Run video analyzer (VideoAnalyzer.java):
	java VideoAnalyzer ../test_data/frames_rgb_test/test_video
At the end of this stage, the "scores" folder should contain the following .txt files:
test_video_audio_score.txt
test_video_video_score.txt

Stage 2: Synthesize the scores
Run synthesizer (Synthesizer.java) to generate an array of frame# to be displayed:
	java Synthesizer scores/test_video_audio_score.txt scores/test_video_video_score.txt
At the end of this stage, the "results" folder should contain the following .txt files:
test_video_res.txt

Stage 3: Play the video
To play the video, run the following script:
java Player ../test_data/frames_rgb_test/test_video ../test_data/test_video.wav results/test_video_res.txt
A video player should pop up. Notice that we need to enlarge the window a little bit to activate the player.

******METRICS******
audio metrics:
  volume of audio
  increase in volume
video metrics:
  high movement
  colorfulness

******ACCURACY******
Every time a new shot occurs, the algorithm in VideoAnalyzer can detect it.

******SYNC******
Perfect synchronization of audio and video

******IMPROVE******
How to improve the program?
There is a large space for improving. Our algo is designed for long videos.

The old dataset given is a long 9-min video: 
The importance is not evenly distributed: some shots in the beginning can be important while some shots in the end can be ignored. Therefore, we divide the video clips into shots and sort them in the order of decreasing score. 

The test dataset is a hodgepodge of different categories: 
The importance is evenly distributed throughout the video： if we knew the structure of the test dataset, we could've improved the performance by evenly dividing the test data into segments and take one from each segment  

It is hard to change the strategy in a short time.