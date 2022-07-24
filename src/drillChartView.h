#ifndef OS_PROJECT1_BACKBUTTON_H
#define OS_PROJECT1_BACKBUTTON_H

#include <QChartView>

#include "drilldownchart.h"

class drillChartView : public QtCharts::QChartView {
   public:
	explicit drillChartView(QWidget *parent = Q_NULLPTR);
	explicit drillChartView(QtCharts::QChart *chart,
							QWidget *parent = Q_NULLPTR);
	~drillChartView();

   protected:
	virtual void mousePressEvent(QMouseEvent *event) override;

   private:
	DrilldownChart *chart;
};

#endif	// OS_PROJECT1_BACKBUTTON_H
