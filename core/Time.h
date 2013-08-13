#ifndef NGPP_TIME_H
#define NGPP_TIME_H

struct Time {
    int lastTime;
    int currentTime;
    int elapsedTime;
    float delta;
};

class ITick {
public:
    virtual void Tick(Time time) = 0;
};

#endif //NGPP_TIME_H
