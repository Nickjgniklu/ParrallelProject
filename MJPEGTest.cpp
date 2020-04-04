#include "MJPEGWriter.h"
int
main()
{
    MJPEGWriter test(7777);

    
    Mat frame;
    frame=Mat::zeros(100, 100, CV_8U);
    for(int i=0;i<100;i++){
    frame.at<uchar>(i, i) = 254;

    }
    

    test.write(frame);
    frame.release();
    test.start();
    while(true){
        test.write(frame); 
        }
    frame.release();
    test.stop();
    exit(0);

}
