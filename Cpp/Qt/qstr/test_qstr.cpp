#include <string>
#include <iostream>

#include <QString>
#include <QStringView>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegExp>

int main()
{
    constexpr const char* const str0 {"my string"};
    QStringView qstrv0(QString::fromStdString(str0));
    if(qstrv0 == QString::fromStdString(str0))
        std::cout << "1) equal" << std::endl;
    if(qstrv0.toString() == str0)
        std::cout << "2) equal" << std::endl;

    std::string str1 = "my string";
    QStringView qstrv1(QString::fromStdString(str1));
    if(qstrv1 == QString::fromStdString(str1))
        std::cout << "3) equal" << std::endl;

    QString qstr2 {"my string"};
    if(qstr2 == QString::fromStdString(str1))
        std::cout << "4) equal" << std::endl;

    //========================================
    QString line {"123;456789"};
    QString gapContent {line.mid(2, 5)};
    std::cout << "gapContent = '" << gapContent.toStdString() << "'.\n";

    // QStringView gapContent(&line, startOfGap, endOfGap - startOfGap);
    if (gapContent.isEmpty())
        std::cout << "gapContent is empty" << "\n";

    if (gapContent.contains("\t")) {
        std::cout << "gapContent has tab" << "\n";
    }

    if (gapContent.contains(';')) {
        std::cout << "gapContent has ;" << "\n";
    }

    QString baseName = "Abc(123)";
    QRegularExpression regexp(R"(\((\d+)\))");
    QRegularExpressionMatch match {regexp.match(baseName)};
    if (match.hasMatch()) {
        std::cout << "matched" << "\n";
        int newNumber = match.captured(1).toInt();
        std::cout << "number = " << newNumber << "\n";
        baseName.replace(match.captured(1), "*");
        std::cout << "baseName = " << baseName.toStdString() << "\n";
        std::cout << "cap(0) = '" << match.captured(0).toStdString() << "'\n";

    }

    baseName = "Abc(123)";
    QRegExp rx(R"(\((\d+)\))");
    int pos = rx.indexIn(baseName);
    std::cout << "pos = " << pos << "\n";

    //========================================
    return 0;
}


/* Compile and link:
qmake -project
qmake
make
*/
