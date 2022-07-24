#include <stdio.h>

#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCore/QTime>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>

#include "drillChartView.h"
#include "drilldownchart.h"
#include "drilldownslice.h"
#include "node.h"

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[]) {
	std::string in = argc == 2 ? argv[1] : "/";
	node X(in);
	X.traverse(&X);
	X.name = "root";

	QApplication a(argc, argv);

	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	QMainWindow window;
	QPieSeries *mySeries = new QPieSeries(&window);
	mySeries->setName("Disk Analyzer - Root");

	DrilldownChart *chart = new DrilldownChart();
	chart->setTheme(QChart::ChartThemeLight);
	chart->setAnimationOptions(QChart::AllAnimations);
	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignRight);

	QList<node *> childrenNodes;
	for (int i = 0; i < X.children.size(); i++)
		childrenNodes.push_back(X.children[i]);
	QList<long long int> sizes;
	for (int j = 0; j < X.children.size(); j++) sizes << X.children[j]->size;

	foreach (node *childNode, childrenNodes) {
		QPieSeries *series = new QPieSeries(&window);
		series->setName(QString(X.name.c_str()) + " as a directory");

		*series << new DrilldownSlice(childNode->size, childNode->name.c_str(),
									  mySeries, childNode, chart, &window);

		QObject::connect(series, SIGNAL(clicked(QPieSlice *)), chart,
						 SLOT(handleSliceClicked(QPieSlice *)));

		*mySeries << new DrilldownSlice(series->sum(), childNode->name.c_str(),
										series, childNode, chart, &window);
	}

	QObject::connect(mySeries, SIGNAL(clicked(QPieSlice *)), chart,
					 SLOT(handleSliceClicked(QPieSlice *)));

	chart->changeSeries(mySeries);

	/*
			QPushButton *m_button = new QPushButton("&Download", &window);
			DrilldownChart::connect(m_button, SIGNAL (released()), &window, SLOT
	   (handleButton()));
	*/

	drillChartView *chartView = new drillChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	window.setCentralWidget(chartView);
	window.resize(800, 500);

	window.show();
	return a.exec();
}