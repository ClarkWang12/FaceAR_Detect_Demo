/*
 * superviseddescent: A C++11 implementation of the supervised descent
 *                    optimisation method
 * File: rcr/landmarks_io.hpp
 *
 * Copyright 2015 Patrik Huber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#ifndef LANDMARKS_IO_HPP_
#define LANDMARKS_IO_HPP_

#include "landmark.hpp"

#include <opencv2/opencv.hpp>

//#include "boost/algorithm/string.hpp"

#include <string>
#include <fstream>
using namespace std;
namespace rcr {

/**
 * Reads an ibug .pts landmark file and returns an ordered vector with
 * the 68 2D landmark coordinates.
 *
 * @param[in] filename Path to a .pts file.
 * @return An ordered vector with the 68 ibug landmarks.
 */
LandmarkCollection<cv::Vec2f> read_pts_landmarks(std::string filename)
{
	using std::getline;
	using cv::Vec2f;
	using std::string;
	LandmarkCollection<Vec2f> landmarks;
	landmarks.reserve(68);

	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error(string("Could not open landmark file: " + filename));
	}
	
	string line;
	// Skip the first 3 lines, they're header lines:
	getline(file, line); // 'version: 1'
	getline(file, line); // 'n_points : 68'
	getline(file, line); // '{'

	int ibugId = 1;
	while (getline(file, line))
	{
		if (line == "}") { // end of the file
			break;
		}
		std::stringstream lineStream(line);

		Landmark<Vec2f> landmark;
		landmark.name = std::to_string(ibugId);
		if (!(lineStream >> landmark.coordinates[0] >> landmark.coordinates[1])) {
			throw std::runtime_error(string("Landmark format error while parsing the line: " + line));
		}
		// From the iBug website:
		// "Please note that the re-annotated data for this challenge are saved in the Matlab convention of 1 being 
		// the first index, i.e. the coordinates of the top left pixel in an image are x=1, y=1."
		// ==> So we shift every point by 1:
		landmark.coordinates[0] -= 1.0f;
		landmark.coordinates[1] -= 1.0f;
		landmarks.emplace_back(landmark);
		++ibugId;
	}
	return landmarks;
}

///TODO:TFY--save landmarks
bool save_landmarks(cv::Mat image,rcr::LandmarkCollection<cv::Vec2f> landmarks,int count)
{
    char c[5];
    std::string txt_num;
    sprintf(c,"%d",count);
    txt_num=c;

//    std::ostringstream txtTitle;
//    txtTitle<<"txt_tfy/"<<txt_num<<".txt";
//    std::ofstream fout(txtTitle.str().c_str());
//    for(int i=0;i<9;i++)
//    {
//        fout<<landmarks[i].coordinates[0]<<" "<<landmarks[i].coordinates[1]<<std::endl;
//    }
//    fout.close();
    string img_name="img_tfy/"+txt_num+".jpg";
    cv::imwrite(img_name,image);
    return true;
}

} /* namespace rcr */

#endif /* LANDMARKS_IO_HPP_ */
