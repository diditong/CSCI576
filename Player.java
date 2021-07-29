import java.util.*;
import java.lang.*;
import java.io.*;

/** A player used to synthesize a video using a path to the RGB frames, a path to the .wav audio, and a file containing the indices of the frames to be displayed
  * usage: java Player ../test_data/frames_rgb_test/test_video ../test_data/test_video.wav results/test_video_res.txt
           java Player ../test_data_3/frames_rgb_test/test_video_3 ../test_data_3/test_video_3.wav results/test_video_3_res.txt
           java Player ../project_dataset/frames_rgb/soccer ../project_dataset/audio/soccer.wav results/soccer_res.txt
  */

  
public class Player{

  public static void main(String[] args){
    try {
      String video = args[0], audio = args[1], shots = args[2];
      ArrayList<Integer> indices = new ArrayList<Integer>();
      
      // reading recommendation result from file
      try {
		    BufferedReader reader = new BufferedReader(new FileReader(shots));
			  String line = reader.readLine();
			  while (line != null) {
          // read next line
          int startFrame = Integer.parseInt(line.split(" ")[0]);
          int endFrame = Integer.parseInt(line.split(" ")[1]);
          for (int i=startFrame; i<endFrame; i++) {
            indices.add(i);
          }
          line = reader.readLine();
        }
        reader.close();
      } catch (IOException e) {
          e.printStackTrace();
      }
      
      // indices = new ArrayList<Integer>();
      // for (int i=0; i<16200; i++) {
      //   indices.add(i);
      // }

      // Read files into memory
      File folder = new File(video);
      int j = 0;
      File fa[] = folder.listFiles();
      Arrays.sort(fa, (f1, f2)-> {
        int n1 = Integer.parseInt(f1.getName().replaceAll("[^0-9]", ""));
        int n2 = Integer.parseInt(f2.getName().replaceAll("[^0-9]", ""));
        return n1 - n2;
      });
      ArrayList<RandomAccessFile> files = new ArrayList<>();
      for(int i=0; i<fa.length; i++){
        File f = fa[i];
        if(j < indices.size() && i == indices.get(j)){
          ++j;
          RandomAccessFile rf = new RandomAccessFile(f, "r");
          files.add(rf);   
        }
      }

      // run video player
      try {
        PlaySoundClip playSoundClip = new PlaySoundClip(audio);
        VideoPlayer player = new VideoPlayer(files, indices, playSoundClip);

        //Thread t1 = new Thread(playSoundClip);
        Thread t2 = new Thread(player);

        //t1.start();
        t2.start();
      }
      catch (Exception e){
        e.printStackTrace();
      }
    }catch (FileNotFoundException e){
      e.printStackTrace();
    }

  }

}