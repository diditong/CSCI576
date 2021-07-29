# CS576 Project
# Yue Yu, Spring 2021
from scipy.io import wavfile
import numpy as np    
import time
# import time
# import matplotlib.pyplot as plt

# to run this file in terminal
# >> python AudioScore.py
# >> filePath

# returns the score[0-10] for a wav file by seconds  
# the result is saved to filePath_audio_score.txt, one row is one score value
def getAudioScore(filePath, fileName):
    # print(time.ctime(time.time()))
    samplerate, data = wavfile.read(filePath)
    monoSound = np.array([l if l >= 0 else 0 for l, r in data])
    sampleNum = len(monoSound)
    secLength = sampleNum // samplerate
    lastLeftNum = sampleNum % samplerate
    monoSound = monoSound[:-lastLeftNum]
    secPCM = np.mean(monoSound.reshape(-1, samplerate), axis=1)
    secPCM = np.insert(secPCM, 0,secPCM[0])
    score = [0]
    for i in range(secLength):
        increase = (secPCM[i + 1] - secPCM[i]) / secPCM[i]
        lastScore = score[-1]
        curScore = lastScore + increase * 100 // 10
        if curScore > 10:
            curScore = 10
        elif curScore < 0:
            curScore = 0
        score.append(curScore)
    del score[0]
    filePath = "scores\\" #C:\\Users\\Jiashuo Tong\\Desktop\\CSCI576\\Project" + "\\proj_code\\
    fileName = fileName.split('.')[0] + '_audio_score.txt'
    scoreFile = filePath + fileName
    with open(scoreFile, 'w+') as audioScoreFile:
        for s in score:
            audioScoreFile.write('%s\n' % s)
    print('AudioScore is saved at: ' + scoreFile)
    return

#getAudioScore(r"C:\Users\Jiashuo Tong\Desktop\CSCI576\Project\test_data\test_video.wav")
# f = open(r"C:\Users\Jiashuo Tong\Desktop\CSCI576\Project\test.txt", "r")
# print(f.read())


if __name__ == '__main__':
    fPath = str(input())
    print('Analyzing audio: ' + fPath)
    start = time.time()
    fName = fPath.split('\\')[-1]
    getAudioScore(fPath, fName)
    end = time.time()
    print("Analyzing completed in ", end - start, " seconds")
    