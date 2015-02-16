#include <QtTest/QtTest>
#include "../functor.h"

class TestFisheye: public QObject
{
    Q_OBJECT
private slots:
    void createPoint();
    void createPolar();
    void pointToPolar();
    void polarToPoint();
    void posToTexCoord();
    void texCoordToPos();
    void fish();
    void fishinverse();
};

void TestFisheye::createPoint()
{
    Point p(1.0, 1.0);
    QCOMPARE(p.x, 1.0);
    QCOMPARE(p.y, 1.0);
}

void TestFisheye::createPolar()
{
    Polar p(1.0, 1.0);
    QCOMPARE(p.r, 1.0);
    QCOMPARE(p.theta, 1.0);
}

void TestFisheye::pointToPolar()
{
    Point p(1.0, 1.0);
    Polar pp = p.toPolar();
    QCOMPARE(pp.r, 1.41421353816986083984375);
    QCOMPARE(pp.theta, 0.785398185253143310546875);
}

void TestFisheye::polarToPoint()
{
    Polar p(1.0, 1.0);
    Point pp = p.toPoint();
    QCOMPARE(pp.x, 0.540302276611328125);
    QCOMPARE(pp.y, 0.8414709568023681640625);
}

void TestFisheye::posToTexCoord()
{
    PosToTexCoord postotexcoord;
    Point p(1.0, 1.0);
    Point pp = postotexcoord(p);
    QCOMPARE(pp.x, 1.5);
    QCOMPARE(pp.y, 1.5);
}

void TestFisheye::texCoordToPos()
{
    TexCoordToPos texcoordtopos;
    Point p(1.0, 1.0);
    Point pp = texcoordtopos(p);
    QCOMPARE(pp.x, 0.5);
    QCOMPARE(pp.y, 0.5);
}

void TestFisheye::fish()
{
    Fish fish;
    Point p(0.1, 0.1);
    Point pp = fish(p);
    QCOMPARE(pp.x, 0.09292893111705780029296875);
    QCOMPARE(pp.y, 0.092928938567638397216796875);
}

void TestFisheye::fishinverse()
{
    FishInverse fishinverse;
    Point p(0.1, 0.1);
    Point pp = fishinverse(p);
    // qWarning() << QString::number(pp.x, 'g', 60);
    // qWarning() << QString::number(pp.y, 'g', 60);
    QCOMPARE(pp.x, 0.108292438089847564697265625);
    QCOMPARE(pp.y, 0.108292438089847564697265625);
}

QTEST_MAIN(TestFisheye)
#include "tests.moc"
