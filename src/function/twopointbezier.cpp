#include "include/function/twopointbezier.h"
#include "ui_twopointbezier.h"
#include "include/common.h"
#include "depend/qcustomplot.h"
#include "algorithm/algorithm.h"
#include <QVector2D>
#include <QtGlobal>
#include <QMouseEvent>

TwoPointBezier::TwoPointBezier(QWidget *parent) :
    QWidget(parent),
    p(QVector<QVector2D>(2)),
    ui(new Ui::TwoPointBezier)
{
    ui->setupUi(this);
    ui->customplot->addGraph();
    ui->customplot->addGraph();

    // Connects the double click event
    connect(ui->customplot, SIGNAL(doubleClickEvent(QMouseEvent*)),
            this,           SLOT(mouseEventHandler(QMouseEvent*)));
    // Activate SV
    useSV();
    initP();
    updatePlot();
}

TwoPointBezier::~TwoPointBezier()
{
    delete ui;
}

QVector<QVector2D> TwoPointBezier::createBezier(double start, double end) {
    int size = p.size();
    int interval = ui->interval->value();


    QVector<QVector2D> out;
    QVector<QVector2D> p = QVector<QVector2D>(this->p);
    std::sort(p.begin(), p.end(), [](QVector2D a, QVector2D b){ return a.x() < b.x(); });

    // API to include ends
    double t = 0;
    for (int step = 1; t < 1.0; step++) {
        double sumX = 0.0;
        double sumY = 0.0;
        for (int n = 0; n < size; n ++) {
            long long coeff = binomCoeff(size - 1, n);
            sumX += coeff * pow(1.0 - t, size - n - 1) * pow(t, n) * (double(p[n].x()) - start);
            sumY += coeff * pow(1.0 - t, size - n - 1) * pow(t, n) * double(p[n].y());
        }
        out.push_back(QVector2D(float(sumX + start), float(sumY)));
        t = (interval * step) / (end - start);
    }
    out.push_back(QVector2D(float(end), float(p.last().y())));

    return out;
}

QVector<QVector2D> TwoPointBezier::createBezier() {
    // Automatically sets start and end
    QVector<double> x = QVector<double>();
    for (const auto & i : p) x.push_back(double(i.x()));
    return createBezier(*std::min_element(x.begin(), x.end()),
                        *std::max_element(x.begin(), x.end()));
}

void TwoPointBezier::addPoint(QMouseEvent * event) {
    if (p.size() == BEZIER_MAX_PTS) return;

//    qDebug() << float(ui->customplot->xAxis->pixelToCoord(event->pos().x())) << ","
//             << float(ui->customplot->yAxis->pixelToCoord(event->pos().y()));
    qDebug() << p.size();

    auto x = float(ui->customplot->xAxis->pixelToCoord(event->pos().x()));
    auto y = float(ui->customplot->yAxis->pixelToCoord(event->pos().y()));
    if (x > ui->endOffset->value() || x < ui->startOffset->value()) return; // Reject x out of bounds

    p.push_back(QVector2D(x,y));
    updatePlot();
}

void TwoPointBezier::popPoint() {
    if (p.size() == BEZIER_MIN_PTS) return;
    p.pop_back();
    updatePlot();
}

void TwoPointBezier::mouseEventHandler(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) addPoint(event);
    else if (event->button() == Qt::RightButton) popPoint();
}

bool TwoPointBezier::live() const {
    return ui->liveCheck->isChecked();
}

void TwoPointBezier::updatePlot() {
    if (!live()) return;
    auto customplot = ui->customplot;

    // Set Bezier Line
    auto vec = createBezier();
    QVector<double> x = QVector<double>();
    QVector<double> y = QVector<double>();
    for (auto i : vec){
        x.push_back(double(i.x()));
        y.push_back(double(i.y()));
    }
    customplot->graph(0)->setData(x, y, false);
    customplot->graph(0)->setPen(ui->svRadio->isChecked() ?
                                 Common::Color::GREEN : Common::Color::RED);
    auto xMinMax = std::minmax_element(x.begin(), x.end());
    auto yMinMax = std::minmax_element(y.begin(), y.end());
    // qDebug() << *xMinMax.first << "," << *xMinMax.second;
    customplot->xAxis->setRange(*xMinMax.first,*xMinMax.second);
    updatePlotRange(*yMinMax.first, *yMinMax.second);

    // Set Bezier Points
    QVector<double> xs = QVector<double>();
    QVector<double> ys = QVector<double>();
    for (auto i : p){
        xs.push_back(double(i.x()));
        ys.push_back(double(i.y()));
    }

    customplot->graph(1)->setData(xs, ys, false);
    customplot->graph(1)->setPen(Common::Color::GREY);
    customplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    customplot->replot();
}

void TwoPointBezier::updatePlotRange(double min, double max){
    // Zoom from 10 % to 1000 %
    double mean = (max - min) / 2 + min;

    // This calculates zoom, zoom [0.1, 10.0]
    double zoom = pow(ZOOM_LIMIT, - double(ui->vertzoom->value() - ZOOM_DEFAULT) / ZOOM_DEFAULT);
    ui->vertzoomLabel->setText(QString::number(1.0 / zoom, 'f', 2) + "x");

    // Normalizes the range and multiplies the ends respectively
    // A buffer is used to prevent too small of a range

    double minRange;
    double maxRange;
    if (ui->svRadio->isChecked()){
        minRange = (min - mean - ZOOM_SV_BUFFER) * zoom + mean;
        maxRange = (max - mean + ZOOM_SV_BUFFER) * zoom + mean;
    } else {
        minRange = (min - mean - ZOOM_BPM_BUFFER) * zoom + mean;
        maxRange = (max - mean + ZOOM_BPM_BUFFER) * zoom + mean;
    }

    // Allow negative for easier bezier bending
    //    minRange = minRange < 0 ? 0 : minRange;
    //    if (ui->svRadio->isChecked())
    //        maxRange = maxRange > Common::SV_MAX ? Common::SV_MAX : maxRange;

    ui->customplot->yAxis->setRange(minRange, maxRange);
}


void TwoPointBezier::useSV()
{
    ui->startValue->setValue(Common::SV_DEFAULT);
    ui->startValue->setRange(Common::SV_MIN, Common::SV_MAX);
    ui->startValue->setSuffix(" (SV)");
    ui->startValue->setSingleStep(Common::SV_STEPSIZE);
    ui->endValue  ->setValue(Common::SV_DEFAULT);
    ui->endValue  ->setRange(Common::SV_MIN, Common::SV_MAX);
    ui->endValue  ->setSuffix(" (SV)");
    ui->endValue  ->setSingleStep(Common::SV_STEPSIZE);
}

void TwoPointBezier::useBPM()
{
    ui->startValue->setRange(Common::BPM_MIN, Common::BPM_MAX);
    ui->startValue->setValue(Common::BPM_DEFAULT);
    ui->startValue->setSuffix(" (BPM)");
    ui->startValue->setSingleStep(Common::BPM_STEPSIZE);
    ui->endValue  ->setRange(Common::BPM_MIN, Common::BPM_MAX);
    ui->endValue  ->setValue(Common::BPM_DEFAULT);
    ui->endValue  ->setSuffix(" (BPM)");
    ui->endValue  ->setSingleStep(Common::BPM_STEPSIZE);
}

void TwoPointBezier::initP()
{
    // Reset p
    ui->startOffset->setRange(int(Common::OFFSET_MIN), int(Common::OFFSET_MAX));
    ui->startOffset->setValue(0);
    ui->interval   ->setValue(int(Common::OFFSET_INTERVAL_DEFAULT));
    ui->endOffset  ->setRange(int(Common::OFFSET_MIN), int(Common::OFFSET_MAX));
    ui->endOffset  ->setValue(int(Common::OFFSET_INTERVAL_DEFAULT) * 10);

    p = QVector<QVector2D>(2);
    p[0] = {float(ui->startOffset->value()),
            float(ui->startValue->value())};
    p[1] = {float(ui->endOffset->value()),
            float(ui->endValue->value())};
}

void TwoPointBezier::on_vertzoom_valueChanged(int value){ updatePlot(); }
void TwoPointBezier::on_bpmRadio_clicked() {
    initP();
    useBPM();
    updatePlot();
}
void TwoPointBezier::on_svRadio_clicked() {
    initP();
    useSV();
    updatePlot();
}
void TwoPointBezier::on_startOffset_valueChanged(int arg1) {
    p[0].setX(float(arg1));
    updatePlot();
}
void TwoPointBezier::on_endOffset_valueChanged(int arg1) {
    p[1].setX(float(arg1));
    updatePlot();
}
void TwoPointBezier::on_startValue_valueChanged(double arg1) {
    p[0].setY(float(arg1));
    updatePlot();
}
void TwoPointBezier::on_endValue_valueChanged(double arg1) {
    p[1].setY(float(arg1));
    updatePlot();
}
void TwoPointBezier::on_updateBoundBtn_clicked() {
    QString text = ui->updateBound->text();
    HitObjectV hoV = HitObjectV();
    if (!hoV.loadEditor(text)) return;
    ui->startOffset->setValue(int(hoV[0].getOffset()));
    ui->endOffset->setValue(int(hoV[1].getOffset()));
    updatePlot();
}

long long TwoPointBezier::binomCoeff(int n, int k)
{
    // defined as n! / k! (n-k)!
    // We find if k or n-k is larger

    // e.g. 15 C 5
    //       n   k

    //    15!
    // --------  = 15 * 14 * ... * 11 / (5!) = 15 / 5 * 14 / 4 ... 11 / 1
    // 5! * 10!    We don't evaluate 15!

    int n_k = n - k;
    long double result = 1;
    int smaller = n_k < k ? n_k : k; // find the smaller number
    while (smaller >= 1) {
        result *= static_cast<long double>(double(n) / double(smaller));
        n--;
        smaller--;
    }
    return static_cast<long long>(result);
}

void TwoPointBezier::on_interval_editingFinished()
{
    updatePlot();
}
