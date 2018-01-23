#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "videoengine.h"
#include "processor.h"
#include "midioutput.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionBild_ffnen_triggered();
    void on_actionStart_triggered();
    void sendMidi(unsigned long long data);

private:
    Ui::MainWindow *ui;
    VideoEngine *videoThread;
    Processor *videoProcessor;
    drumstick::rt::MIDIOutput midiOutput;
};


#endif // MAINWINDOW_H
