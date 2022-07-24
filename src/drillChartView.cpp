#include "drillChartView.h"

#include <iostream>

drillChartView::drillChartView(QWidget *parent) : QChartView(parent) {
  setDragMode(QGraphicsView::NoDrag);
  this->setMouseTracking(true);
}

drillChartView::drillChartView(QtCharts::QChart *chart, QWidget *parent)
    : QChartView(chart, parent) {
  setDragMode(QGraphicsView::NoDrag);
  this->setMouseTracking(true);
  this->chart = static_cast<DrilldownChart *>(chart);
}

drillChartView::~drillChartView() {}

void drillChartView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::RightButton) {
    chart->handleRightClick();
    event->accept();
  }

  QChartView::mousePressEvent(event);
}