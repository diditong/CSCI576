// Java program to play an Audio
// file using Clip Object
import java.io.File;
import java.io.IOException;
// import java.util.Scanner;

import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;

/** A .wav audio player with fundamental functionality including play, pause, resume, stop, jump, etc.
  */
public class PlaySoundClip implements Runnable {

    // to store current position
    Long currentSec;
    Clip clip;

    // current status of clip
    String status;

    AudioInputStream audioInputStream;
    static String filePath;

    // constructor to initialize streams and clip
    public PlaySoundClip(String path)
        throws UnsupportedAudioFileException,
        IOException, LineUnavailableException {
        // create AudioInputStream object
        filePath = path;
        audioInputStream =
            AudioSystem.getAudioInputStream(new File(filePath).getAbsoluteFile());
        // create clip reference
        clip = AudioSystem.getClip();

        // open audioInputStream to the clip
        clip.open(audioInputStream);
        
        // timer.cancel();
        clip.loop(Clip.LOOP_CONTINUOUSLY);
    }

    // play the audio
    public void play() {
        //start the clip
        clip.start();
        status = "play";
    }

    // pause the audio
    public void pause() {
        if (status.equals("paused")) {
            System.out.println("audio is already paused");
            return;
        }
        this.currentSec = this.clip.getMicrosecondPosition();
        clip.stop();
        status = "paused";
    }

    public long getPosition() {
        return clip.getLongFramePosition();
    }

    public float getSampleRate() {
        return clip.getFormat().getSampleRate();
    }

    // resume the audio
    public void resumeAudio()
        throws UnsupportedAudioFileException, IOException, LineUnavailableException {
        if (status.equals("play")) {
            System.out.println("Audio is already being played");
            return;
        }
        clip.stop();
        clip.setMicrosecondPosition(currentSec);
        this.play();
    }

    // restart the audio
    public void restart()
        throws IOException, LineUnavailableException, UnsupportedAudioFileException {
        clip.stop();
        currentSec = 0L;
        clip.setMicrosecondPosition(0);
        this.play();
    }

    // stop and close the audio
    public void stop() throws UnsupportedAudioFileException, IOException, LineUnavailableException {
        currentSec = 0L;
        clip.stop();
        clip.close();
    }

    // jump over a specific part
    public void jump(long c)
        throws UnsupportedAudioFileException, IOException, LineUnavailableException {
        if (c > 0 && c < clip.getMicrosecondLength()) {
            clip.stop();
            currentSec = c;
            clip.setMicrosecondPosition(c);
            this.play();
        }
    }

    @Override
    public void run() {
        play();
    }
}