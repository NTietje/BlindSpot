#include "processor.h"
#include <math.h>
#include <QDebug>
#include <QThread>
#include <QPainter>

using namespace cv;

Processor::Processor()
{

}
void Processor::startProcessing(const VideoFormat& format)
{}
/**
 * @brief Processor::process
 * @param input : geöffnete Bilddatei (Tiefenbild)
 * @return 64-Bit Integer, das mit den 8 DatenByte für 4 MidiDateien befüllt wird
 */
unsigned long long Processor::process(const cv::Mat& input){
   //Outputmatrix für das GUI vorbereiten
   cv::Mat output(input.rows, input.cols, input.type());

   //vorherige Tiefenwerte Löschen
   avgDepths.clear();

   //Rasterfeldergröße berechnen
   tileWidth = input.cols/tileCols;
   tileHeight = input.rows/tileRows;

   //Durchlaufen des gesamten Rasters; index ist der Rasterindex
   int index = 0;
   for (int y = 0; y < input.rows; y+=tileHeight){
       for (int x = 0; x < input.cols; x+=tileWidth){
            //aufsummmieren aller Grauwerte jedes einzelnen Pixels
            long sum = 0;
            for (int i = y; i<tileHeight+y; i++){
                for (int j = x; j<tileWidth+x; j++){
                    sum += input.at<Vec3b>(i,j)[0];
                }
            }
            //Teilen durch Pixelanzahl eines Rasterfeldes um den Mittelwert zu erhalten
            sum = sum/(tileHeight*tileWidth);

            //Rasterfeld der Outputmatrix mit Grauwerten füllen
            for (int i = y; i<tileHeight+y; i++){
                for (int j = x; j<tileWidth+x; j++){
                    Vec3b pixel = Vec3b(sum, sum ,sum);
                    output.at<Vec3b>(i,j) = pixel;
                }
            }
            //Skalierung der Grauwerte von 0-255 auf 20-0
            int value = calcGreyscale(sum);

            //Wertepaar wird erzeugt (Grauwert, Index) und gespeichert
            std::pair<int,int> pair = std::make_pair(value, index);
            avgDepths.push_back(pair);

            //nächstes Rasterfeld
            index++;
       }
   }
   //Wertepaare sortieren, hellster Grauwert zuerst
   std::sort(avgDepths.begin(), avgDepths.end());

   //Rückgabewert (64-bit Integer) mit midifähigen Datenbytes füllen
   union
   {
       unsigned long long longData;
       unsigned char byte[8];
   } data;
   data.byte[0] = avgDepths.at(0).second;
   data.byte[1] = avgDepths.at(0).first;
   data.byte[2] = avgDepths.at(1).second;
   data.byte[3] = avgDepths.at(1).first;
   data.byte[4] = avgDepths.at(2).second;
   data.byte[5] = avgDepths.at(2).first;
   data.byte[6] = avgDepths.at(3).second;
   data.byte[7] = avgDepths.at(3).first;

   //Output-Matrix speichern
   outputImage = output;

   //Midi-Datenbytes zurückgeben
   return data.longData;
}/**
 * @brief Processor::getOutputImage
 * @return Output-Matrix für das GUI
 */
cv::Mat Processor::getOutputImage(){
    return outputImage;
}
/**
 * @brief Processor::getAvgBrightness
 * @param image
 * @return gesamter mittler Helligkeit des Bildes
 */
int Processor::getAvgBrightness(const cv::Mat& image){
    int sum = 0;
    for (int y = 0; y < image.rows; y++){
        for (int x=0; x < image.cols; x++){
            sum += image.at<Vec3b>(x,y)[0];
        }
    }
    sum = sum/(image.rows*image.cols);
    return sum;
}
/**
 * @brief Processor::calcGreyscale
 * @param value
 * @return skalierter Grauwert; weiß = 0, schwarz = 20
 */
int Processor::calcGreyscale(int value){
    double a = (((255-value)*20)/255);
    return (int)a;
}
