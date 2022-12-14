#include "parametros.h"

#include <QtCore>

QVector<double> Parametros(QVector<QVector<double>> dataSenales)
{
    QVector<double> results;
    auto x_max = std::max_element(dataSenales[0].begin(), dataSenales[0].end());
    auto x_min = std::min_element(dataSenales[0].begin(), dataSenales[0].end());
    auto y_max = std::max_element(dataSenales[1].begin(), dataSenales[1].end());
    auto y_min = std::min_element(dataSenales[1].begin(), dataSenales[1].end());

    results.push_back( *x_max - *x_min );
    results.push_back( *y_max - *y_min );

    auto tAnalisis = std::max_element(dataSenales[2].begin() , dataSenales[2].end());
    double time_analysis = dataSenales[2].length() - 1;
    double den = *tAnalisis / dataSenales[2].length();

    // SEÑAL X ----------------------------------------------------------------

    double avgX = std::accumulate(dataSenales[0].begin(), dataSenales[0].end(), 0.0) / dataSenales[0].length();

    QVector<double> numX, velSgnX, numRMSX;
    for (int k = 0; k < dataSenales[0].length() - 1; k++) {
        numX.push_back( qSqrt( qPow( dataSenales[0].at(k+1) - dataSenales[0].at(k) , 2 ) ) );
        velSgnX.push_back( numX.at(k) / den );
        numRMSX.push_back( qPow( dataSenales[0].at(k) - avgX , 2 ) );
    }
    results.push_back( std::accumulate(velSgnX.begin() , velSgnX.end() , 0.0) / time_analysis );

    // SEÑAL Y ----------------------------------------------------------------

    double avgY = std::accumulate(dataSenales[1].begin(), dataSenales[1].end(), 0.0) / dataSenales[1].length();

    QVector<double> numY, velSgnY, numRMSY;
    for (int k = 0; k < dataSenales[1].length() - 1; k++) {
        numY.push_back( qSqrt( qPow( dataSenales[1].at(k+1) - dataSenales[1].at(k) , 2 ) ) );
        velSgnY.push_back( numY.at(k) / den );
        numRMSY.push_back( qPow( dataSenales[1].at(k) - avgY , 2 ) );
    }
    results.push_back( std::accumulate(velSgnY.begin() , velSgnY.end() , 0.0) / time_analysis );

    // SEÑALES X Y ------------------------------------------------------------

    QVector<double> num2, numVMT, numDist, distM;

    for (int k = 0; k < dataSenales[0].length() - 1; k++) {
        num2.push_back( qSqrt( qPow( dataSenales[0].at(k+1) - dataSenales[0].at(k) , 2 ) + qPow( dataSenales[1].at(k+1) - dataSenales[1].at(k) , 2 ) ) );
        numVMT.push_back( num2.at(k) / *tAnalisis );
    }
    results.push_back( std::accumulate( numVMT.begin() , numVMT.end() , 0.0 ) );

    for (int k = 0; k < dataSenales[0].length() - 1; k++) {
        numDist.push_back( qSqrt( qPow( dataSenales[0].at(k) , 2 ) + qPow( dataSenales[1].at(k) , 2 ) ) );
        distM.push_back( numDist.at(k) / *tAnalisis );
    }

    results.push_back( qSqrt( std::accumulate( numRMSX.begin() , numRMSX.end() , 0.0 ) / time_analysis ) );
    results.push_back( qSqrt( std::accumulate( numRMSY.begin() , numRMSY.end() , 0.0 ) / time_analysis ) );
    results.push_back( std::accumulate( distM.begin() , distM.end() , 0.0 ) );
    results.push_back( results.at(4) / (2 * M_PI) );

    return results;
}

