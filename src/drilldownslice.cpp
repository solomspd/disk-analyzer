/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "drilldownslice.h"

QT_CHARTS_USE_NAMESPACE

DrilldownSlice::DrilldownSlice(qreal value, QString prefix,
                               QAbstractSeries *drilldownSeries, node *n,
                               DrilldownChart *chart, QMainWindow *window)
    : m_drilldownSeries(drilldownSeries), m_prefix(prefix) {
  setValue(value);
  updateLabel();
  setLabelFont(QFont("Arial", 8));
  connect(this, SIGNAL(percentageChanged()), this, SLOT(updateLabel()));
  connect(this, SIGNAL(hovered(bool)), this, SLOT(showHighlight(bool)));
  this->n = n;
  this->w = window;
  this->chart = chart;
}

DrilldownSlice::~DrilldownSlice() {}

QAbstractSeries *DrilldownSlice::drillupSeries() {
  node *N;
  if (n->parent->parent == NULL) {
    N = n->parent;
  } else if (n->parent->parent != NULL) {
    N = n->parent->parent;
  }
  QPieSeries *mySeries = static_cast<QPieSeries *>(m_drilldownSeries);
  mySeries->clear();
  mySeries->setName(QString(N->name.c_str()) + " as a directory");
  foreach (node *childNode, N->children) {
    QPieSeries *series = new QPieSeries(w);
    series->setName(QString(N->name.c_str()) + " as a directory");
    *series << new DrilldownSlice(childNode->size, childNode->name.c_str(),
                                  mySeries, childNode, chart, w);

    QObject::connect(series, SIGNAL(clicked(QPieSlice *)), chart,
                     SLOT(handleSliceClicked(QPieSlice *)));

    (*mySeries) << new DrilldownSlice(series->sum(), childNode->name.c_str(),
                                      series, childNode, chart, w);
  }
  return m_drilldownSeries;
}

QAbstractSeries *DrilldownSlice::drilldownSeries() {
  node *N;
  if (n->children.empty()) {
    N = n->parent;
  } else {
    N = n;
  }
  QPieSeries *mySeries = static_cast<QPieSeries *>(m_drilldownSeries);
  mySeries->clear();
  mySeries->setName(QString(N->name.c_str()) + " as a directory");
  foreach (node *childNode, N->children) {
    QPieSeries *series = new QPieSeries(w);
    series->setName(QString(N->name.c_str()) + " as a directory");
    *series << new DrilldownSlice(childNode->size, childNode->name.c_str(),
                                  mySeries, childNode, chart, w);

    QObject::connect(series, SIGNAL(clicked(QPieSlice *)), chart,
                     SLOT(handleSliceClicked(QPieSlice *)));

    (*mySeries) << new DrilldownSlice(series->sum(), childNode->name.c_str(),
                                      series, childNode, chart, w);
  }
  return m_drilldownSeries;
}

int length(qreal in) {
  int count = 0;
  while (qint64(in) != 0) {
    count++;
    in /= 10;
  }
  return count;
}

void DrilldownSlice::updateLabel() {
  QString label = m_prefix;
  label += ", ";
  qreal num = this->value();
  int c = length(num);
  std::string l;
  if (c < 4) {
    l = "B, ";
  } else if (c < 7) {
    l = "K, ";
    num /= 1000;
  } else if (c < 10) {
    l = "M, ";
    num /= std::pow(1000, 2);
  } else if (c < 13) {
    l = "G, ";
    num /= std::pow(1000, 3);
  } else if (c < 16) {
    l = "T, ";
    num /= std::pow(1000, 4);
  }

  label += QString::number(num);
  label += l.c_str();
  label += QString::number(this->percentage() * 100, 'f', 1);
  label += "%";
  setLabel(label);
}

void DrilldownSlice::showHighlight(bool show) {
  setLabelVisible(show);
  setExploded(show);
}

node *DrilldownSlice::getNode() { return n; }

#include "moc_drilldownslice.cpp"
