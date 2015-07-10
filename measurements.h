class Measurements
{
public:
    Measurements();

    virtual void addTime(int time);
    virtual float calculateAverage(int start, int end);
    virtual float calculateVariance(int start, int end);
    virtual float calculateDeviation(int start, int end);
    virtual float average();
    virtual float variance();
    virtual float deviation();
    virtual float average1();
    virtual float variance1();
    virtual float deviation1();
    virtual void printMeasurements();
    virtual void printTimes();

protected:
    float avg;
    float var;
    float dev;
    QList<int> times;
};
