#ifndef CLASSIFIEDVIEWER_H
#define CLASSIFIEDVIEWER_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QLabel>
#include <QSlider>
#include <QDoubleSpinBox>

#include <tester/Grid.h>

namespace Ui {
    class ClassifiedViewer;
}

enum visualizationOptions {
  colorGrayscale = 1000,
  highlightingScores,
  imageOnly
};

class ClassifiedViewer : public QMainWindow {
  Q_OBJECT
  public:
    ClassifiedViewer(QWidget *parent = 0);
    ~ClassifiedViewer();

    void setTestImage(QString imageFile);
    void setClassifiedGrid(QString classifiedFile);

  protected:
    void changeEvent(QEvent *e);

  private slots:
    void on_testImageButton_clicked();
    void on_classifiedButton_clicked();
    void on_colorGrayscaleRadio_clicked();
    void on_highlightingRadio_clicked();
    void on_highlightingRadio_toggled(bool checked);
    void on_imageOnlyRadio_clicked();

    void on_hueSlider_valueChanged(int val);
    void on_thresholdSlider_valueChanged(int val);
    void on_thresholdSpin_valueChanged(double val);

  private:
    Ui::ClassifiedViewer *ui;
    QLabel *ui_label;
    QSlider *ui_hueSlider;
    QSlider *ui_thresholdSlider;
    QDoubleSpinBox *ui_thresholdSpin;

    Grid *m_classified;
    QImage m_image;
    QImage m_grayscaleImage;
    QColor m_highlightColor;
    qreal m_threshold;
    int m_visualizationOption;

    void drawScene();
    void drawColorGrayscale();
    void drawHighlightingScores();
    QRgb toGray(QRgb color);

    void scaleImage();

};

#endif // CLASSIFIEDVIEWER_H
