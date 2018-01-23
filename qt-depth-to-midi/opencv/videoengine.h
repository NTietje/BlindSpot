#ifndef VIDEOENGINE_H
#define VIDEOENGINE_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include "videoprocessor.h"
#include "cvmattoqimage.h"
#include "videoformat.h"


class VideoEngine : public QThread
{
    Q_OBJECT
public:
    VideoEngine();
    ~VideoEngine();
    void openFile(const QString& file);
    void openImage();
    void openCamera(int device = 0);
    void setProcessor(VideoProcessor*);
    const VideoFormat& videoFormat() const;
    int framePosition();
protected:
    void run();
public slots:
    void stop();
signals:
    void sendInputImage(const QImage&);
    void sendProcessedImage(const QImage&);
    //erzeugt eine MidiDatei aus dem Input-Bild und sendet es an den Mainframe
    void sendMidiData(unsigned long long data);
private:
    cv::VideoCapture videoCapture;
    cv::Mat image;
    VideoFormat _videoFormat;
    bool stopped;
    QMutex mutex;
    VideoProcessor* processor;
    bool usingCamera;
    std::string imagepath;
};


#endif // VIDEOENGINE_H
