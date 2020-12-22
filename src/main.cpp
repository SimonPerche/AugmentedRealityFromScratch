//
// Created by Simon on 06/11/2020.
//

#include "../headers/Tracking.hpp"
#include "../headers/Video.hpp"
#include "../headers/Utils.hpp"
#include "../headers/Segmentation.hpp"
#include "../headers/OBJLoader.h"

int main()
{
    auto video = arfs::Video(0, 1,1);
//    auto video = arfs::Video("../resources/marker.mp4", 0.5, 0.5);
    auto tag = arfs::Utils::loadImage("../resources/marker.jpeg");
    auto obj = arfs::OBJLoader("../resources/low_poly_fox.obj");
    obj.rotate(arfs::Utils::degToRad(90),arfs::Utils::degToRad(0),arfs::Utils::degToRad(180));

    for(;;)
    {
        const auto& frame = video.getNextFrame();

        if(frame.empty() || arfs::Video::escIsPressed())
            break;

        auto frameTag = arfs::Segmentation::recognizeTag(frame, arfs::Segmentation::extractTagCandidates(frame),
                                                    arfs::Segmentation::getARTagCode(tag));

        if(!frameTag.empty())
        {
            arfs::Segmentation::augmentObject(obj, frame.clone(), frameTag);

            for(size_t i = 0 ; i < frameTag.size()-1;i++)
                cv::line( frame, frameTag[i], frameTag[i+1], cv::Scalar(0,255,0), 2, cv::LINE_AA);
            cv::line( frame, frameTag[3], frameTag[0], cv::Scalar(0,255,0), 2, cv::LINE_AA);
        }

        arfs::Utils::showImage(frame, "original");
    }

    return 0;
}