/**
* Copyright (C) 2016 Hayyan Daoud <hayyan dot d at gmail dot come> (University of Malaya)
* For more information see <https://github.com/hdaoud/ORBSLAMM>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include<iomanip>

#include<opencv2/core/core.hpp>

#include"System.h"

using namespace std;

void LoadImages(const string &strSequence, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    if(argc != 5)
    {
        cerr << endl << "Usage: ./mono_AGZ path_to_vocabulary path_to_settings path_to_sequence [0|1]for_Multi_Maps_Usage" << endl;
        return 1;
    }


    // Retrieve paths to images
    vector<string> vstrImageFilenames;
    vector<double> vTimestamps;
    LoadImages(string(argv[3]), vstrImageFilenames, vTimestamps);
    
    bool bUseMMaps = string(argv[4]).compare("1") == 0;

    int nImages = vstrImageFilenames.size();

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    iORB_SLAM::System SLAM(argv[1],argv[2],iORB_SLAM::System::MONOCULAR,true, bUseMMaps);

    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);

    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;
    cout << "Images in the sequence: " << nImages << endl << endl;

    // Main loop
    cv::Mat im;
    for(int ni=0; ni<nImages; ni+=5)
    {
        //std::chrono::steady_clock::time_point tt1 = std::chrono::steady_clock::now();
        // Read image from file
        im = cv::imread(vstrImageFilenames[ni],cv::IMREAD_GRAYSCALE);
        //cv::resize(imO, im, cv::Size(), 0.5, 0.5);
        double tframe = vTimestamps[ni];

        if(im.empty())
        {
            cerr << endl << "Failed to load image at: " << vstrImageFilenames[ni] << endl;
            return 1;
        }
        //std::chrono::steady_clock::time_point tt2 = std::chrono::steady_clock::now();
        
        //cout<<std::chrono::duration_cast<std::chrono::duration<double> >(tt2 - tt1).count();

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

        // Pass the image to the SLAM system
        SLAM.TrackMonocular(im,tframe);

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

        double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

        vTimesTrack[ni]=ttrack;
        //cout<<ttrack<<endl;
        // Wait to load the next frame
        double T=0;
        if(ni<nImages-1)
            T = vTimestamps[ni+1]-tframe;
        else if(ni>0)
            T = tframe-vTimestamps[ni-1];

        if(ttrack<T)
            usleep((T-ttrack)*1e6);
    }

    // Stop all threads
    SLAM.Shutdown();

    // Tracking time statistics
    sort(vTimesTrack.begin(),vTimesTrack.end());
    float totaltime = 0;
    for(int ni=0; ni<nImages; ni++)
    {
        totaltime+=vTimesTrack[ni];
    }
    cout << "-------" << endl << endl;
    cout << "median tracking time: " << vTimesTrack[nImages/2] << endl;
    cout << "mean tracking time: " << totaltime/nImages << endl;

    // Save camera trajectory
    //SLAM.SaveTrajectoryKITTI("KeyFrameTrajectory_kitti.txt");
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
    SLAM.SaveMultipleMapsTrajectories("Maps.txt");

    return 0;
}

void LoadImages(const string &strPathToSequence, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
    ifstream fNames;

    string strImagesNames = strPathToSequence + "/filenames.txt";
    string strPrefix = strPathToSequence + "/MAVImages/";
    
    fNames.open(strImagesNames.c_str());
    while(!fNames.eof())
    {
        string s;
        getline(fNames,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            string name;
            ss >> name;
            vstrImageFilenames.push_back(strPrefix+name);
        }
    }
    
    int size = vstrImageFilenames.size();
    
    for(int i=0; i<size; i++)
        vTimestamps.push_back(i*0.1);

}
