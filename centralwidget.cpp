#include "centralwidget.h"

#include <QApplication>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLayout>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QLabel>

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{
  QGridLayout *lp_baseLayout = new QGridLayout(this);



  QPixmap image(":/images/DO.png");

  QLabel *imageLabel = new QLabel();
  imageLabel->setPixmap(image);

  lp_baseLayout->addWidget(imageLabel, 0, 0);
  //lp_baseLayout->addWidget(toto, 1, 1);
}
