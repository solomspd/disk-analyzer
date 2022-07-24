#include "drilldownchart.h"

#include "drilldownslice.h"

QT_CHARTS_USE_NAMESPACE

DrilldownChart::DrilldownChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
	: QChart(QChart::ChartTypeCartesian, parent, wFlags), m_currentSeries(0) {}

DrilldownChart::~DrilldownChart() {}

void DrilldownChart::changeSeries(QAbstractSeries *series) {
	// NOTE: if the series is owned by the chart it will be deleted
	// here the "window" owns the series...
	if (m_currentSeries) removeSeries(m_currentSeries);
	m_currentSeries = series;
	addSeries(series);
	setTitle(series->name());
}

void DrilldownChart::handleSliceClicked(QPieSlice *slice) {
	DrilldownSlice *drilldownSlice = static_cast<DrilldownSlice *>(slice);
	changeSeries(drilldownSlice->drilldownSeries());
}

void DrilldownChart::handleRightClick() {
	QPieSeries *drilldownSeries = static_cast<QPieSeries *>(m_currentSeries);
	DrilldownSlice *x =
		static_cast<DrilldownSlice *>(drilldownSeries->slices()[0]);
	changeSeries(x->drillupSeries());
}

#include "moc_drilldownchart.cpp"
