#ifndef VMBCAMERA_H
#define VMBCAMERA_H

//Vimba SDK dependency
#include <VimbaC/Include/VimbaC.h>
//Qt dependencies
#include<QObject>
#include<QImage>
class VmbCamera : public QObject {
    Q_OBJECT
public:
    // Initialize the camera
    void Startup();
    // Release the camera
    void Shutdown();
    // Start the acquisition
    void StartCapture() const;
    // Stop the acquisition
    void StopCapture() const;
    // Get the camera exposure value (in microseconds)
    double Exposure() const;
    // Set the camera exposure value (in microseconds)
    void SetExposure( double exposure ) const;
    // Get the camera image
    const QImage& Image() const;
    // Qt signal
signals:
    // Signal to tell a new image is received
    void ImageReady();
private:
    // Handle to the camera
    VmbHandle_t handle;
    // Frame buffer size
    static const int frame_buffer_size;
    // Frame buffer
    VmbFrame_t* frame_buffer;
    // Current camera image
    QImage image;
    // Function called by Vimba to receive the frame
    static void VMB_CALL FrameDoneCallback(const VmbHandle_t camera_handle,VmbFrame_t* frame_pointer);


};



#endif // VMBCAMERA_H
