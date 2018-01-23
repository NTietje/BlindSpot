#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "videoprocessor.h"
#include "videoengine.h"

class Processor: public VideoProcessor
{
public:
    Processor();
    void startProcessing(const VideoFormat& format);

    //Verarbeitung des Input-Bilds, Ausgabe ist sind Midi-Data-Bytes (4x2 für 4 Midis)
    unsigned long long process(const cv::Mat& input);

    //Übersetzt einen Grauwert von 0-255 in eine Skala von 20 (weit) bis 0 (nah)
    int calcGreyscale(int value);

    //Berechnet den mittleren Grauwert
    int getAvgBrightness(const cv::Mat& image);

    //Zugriff auf das erzeugte Output-Bild (gerastertes Tiefenbild)
    cv::Mat getOutputImage();

    //Erzeugt ein einfarbiges Bild mit dem mittleren Grauwert eines Input-Bilds
    cv::Mat transformToAvgBrightness(const cv::Mat& image);

private:
    //Die Variablen legen fest, wie das Output-Bild gerastert wird
    //Die Werte sollten Teiler des Input-Bilds sein, sonst kommt es zu evtl. abstürzen
    int tileCols = 10;
    int tileRows = 4;

    //Breite und Höhe einzelner Rasterfelder
    int tileWidth;
    int tileHeight;

    //Sortierter Speicherplatz für berechnete Tiefen-Mittelwerte,
    //hellster (nächster) Wert steht an erster stelle
    std::vector<std::pair<int,int>> avgDepths;

    //Gerastertes Tiefenbild
    cv::Mat outputImage;
};

#endif // PROCESSOR_H
