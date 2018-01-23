#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , videoThread(new VideoEngine)
  , videoProcessor(new Processor)
{
     ui->setupUi(this);
     //LoopBe wird als interner Midi-Output genutzt
     midiOutput.open("LoopBe Internal MIDI");

     //Einrichten des VideoThreads
     videoThread->setProcessor(videoProcessor);
     connect(videoThread, &VideoEngine::sendInputImage,
             ui->inputFrame, &VideoWidget::setImage);
     connect(videoThread, &VideoEngine::sendProcessedImage,
             ui->processedFrame, &VideoWidget::setImage);

     /**Die Videoengine erzeugt ein Signal mit den vom Processor erzeugten Midi-Daten,
            das vom MainWindow empfangen und and den Midi-Output weitergeleitet wird*/
     connect(videoThread, &VideoEngine::sendMidiData,
             this, &MainWindow::sendMidi);

}
/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
  delete videoThread;
  delete ui;
  delete videoProcessor;
}
/**
 * Öffnet einen Auswahldialog zum Bildpfad
 * @brief VideoPlayer::on_actionBild_ffnen_triggered
 */
void VideoPlayer::on_actionBild_ffnen_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),QDir::homePath());

  if (!fileName.isEmpty()) {
      videoThread->openFile(fileName);
   }
}
/**
 * startet die Bildverarbeitung
 * @brief MainWindow::on_actionStart_triggered
 */

void MainWindow::on_actionStart_triggered()
{
  videoThread->start();
}
/**
 * Erzeugt und verschickt Midi-Nachrichten. Statusbyte ist festgelegt (10010000, midiNoteOn, channel 0)
 * @brief MainWindow::sendMidi
 * @param input 64-Bit integer, enthält 4*2 Midi-Databytes
 */
void MainWindow::sendMidi(unsigned long long input){
  union
  {
      unsigned long long longData;
      unsigned char byte[8];

  } data;
  data.longData = input;
  midiOutput.sendNoteOn(0, data.byte[0], data.byte[1]);

  /** zum Präsentationszeitpunkt wird nur eine Nachricht verschickt (nächstes Objekt) anstatt vier
  midiOutput.sendNoteOn(0, data.byte[2], data.byte[3]);
  midiOutput.sendNoteOn(0, data.byte[4], data.byte[5]);
  midiOutput.sendNoteOn(0, data.byte[6], data.byte[7]);
  */
}
