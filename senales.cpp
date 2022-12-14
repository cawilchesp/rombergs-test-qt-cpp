#include "senales.h"
#include <QtCore>

QVector< QVector<double> > Senales(QStringList dataFiles)
{
    QVector<QVector<double>> dataX;
    QVector<QVector<double>> dataY;
    QVector<int> minArray;

    QFileInfo fi(dataFiles.at(0));

    for (int j = 0; j < dataFiles.length(); j++)
    {
        QFile file(dataFiles.at(j));
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        QString lines = in.readAll();
        QStringList splitLines = lines.split(QRegExp("\t|\n"));
        QVector<double> dataTempX;
        QVector<double> dataTempY;
        if (fi.suffix() == "txt") {
            for (int i = 145; i < splitLines.length() - 1; i = i + 3) {
                dataTempX.push_back(QString(splitLines.at(i)).toDouble());
                dataTempY.push_back(QString(splitLines.at(i + 1)).toDouble());
            }
        } else if(fi.suffix() == "emt") {
            for (int j = 22; j < splitLines.length() - 3; j = j + 6) {
                dataTempX.push_back(QString(splitLines.at(j + 2)).toDouble());
                dataTempY.push_back(QString(splitLines.at(j + 4)).toDouble());
            }
        }

        dataX.push_back(dataTempX);
        dataY.push_back(dataTempY);
        minArray.push_back(dataTempX.length());
    }
    auto minLenght = std::min_element(minArray.begin(),minArray.end());
    QVector<QVector<double>> dataPoints;
    QVector<double> dataPointsX;
    QVector<double> dataPointsY;
    QVector<double> dataPointsTime;

    for (int h = 0; h < *minLenght; h++) {
        double sumX = 0, sumY = 0;
        for (int k = 0; k < dataFiles.length(); k++) {
            sumX += dataX[k][h];
            sumY += dataY[k][h];
        }
        dataPointsX.push_back( sumX / dataFiles.length());
        dataPointsY.push_back( sumY / dataFiles.length());
        if (fi.suffix() == "txt") {
            dataPointsTime.push_back(h * 0.1);
        } else if(fi.suffix() == "emt") {
            dataPointsTime.push_back(h * 0.005);
        }
    }
    dataPoints.push_back(dataPointsX);
    dataPoints.push_back(dataPointsY);
    dataPoints.push_back(dataPointsTime);

    return dataPoints;
}

