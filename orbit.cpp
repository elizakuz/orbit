#include "orbit.h"
#include <iostream>
#include <cmath>
using namespace std;

double scalar (vec t)
{
    double res = sqrt(pow(t.x, 2) + pow(t.y, 2) + pow(t.z, 2));
    return res;
}

struct quaternion
{
    double w, x, y, z;
};

quaternion createQuaternion(vec speed)
{
    double c1 = cos(speed.z / 2),
    c2 = cos(speed.y / 2),
    c3 = cos(speed.x / 2),
    s1 = sin(speed.z / 2),
    s2 = sin(speed.y / 2),
    s3 = sin(speed.x / 2);
    quaternion q = {c1 * c2 * c3 - s1 * s2 * s3, s1 * s2 * c3 + c1 * c2 * s3,
        c1 * s2 * c3 - s1 * c2 * s3, s1 * c2 * c3 + c1 * s2 * s3};
    return q;
}

quaternion normalizeQuaternion(quaternion q)
{
    double s = sqrt(pow(q.x, 2) + pow(q.y, 2) + pow(q.z, 2) + pow(q.w, 2));
    quaternion p = {0, 0, 0, 0};
    if (s != 0)
    {
        p.w = q.w / s;
        p.x = q.x / s;
        p.y = q.y / s;
        p.z = q.z / s;
    }
    return p;
}

quaternion multiQuaternions(quaternion a, quaternion b)
{
    quaternion res = {a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w};
    res = normalizeQuaternion(res);
    return res;
}

quaternion multiVecQuaternion(quaternion q, vec v)
{
    quaternion res = {-q.x * v.x - q.y * v.y - q.z * v.z,
        q.w * v.x + q.y * v.z - q.z * v.y,
        q.w * v.y - q.x * v.z + q.z * v.x,
        q.w * v.z + q.x * v.y - q.y * v.x};
    return res;
}

quaternion invertQuaternion(quaternion q)
{
    quaternion p = {q.w, -q.x, -q.y, -q.z};
    return p;
}

vec multiVecDouble(vec t1, double t2)
{
    vec res = {t1.x * t2, t1.y * t2, t1.z * t2};
    return res;
}

vec transformVec(vec v, quaternion q)
{
    quaternion res = multiQuaternions(multiVecQuaternion(q, v),
                                      invertQuaternion(q));
    vec t = {res.x, res.y, res.z};
    return t;
}

vec gravityForce(vec r, double m)
{
    double G = 6.67385 * pow(10.0, -20),
    mEarth = 5.9742 * pow(10.0, 24);
    double R = pow(scalar(r), 3);
    if (R != 0)
    {
        vec g = multiVecDouble(r, G * mEarth / R);
        return g;
    }
    else
    {
        vec g = {0, 0, 0};
        return g;
    }
}

vec aerodynamicForce(double p, vec v, double S)
{
    double u = scalar(v);
    vec a = multiVecDouble(v, -p * u * S / 2);
    return a;
}

vec tractiveForce(double mLevel, double specificImpulse, vec v)
{
    double u = scalar(v);
    if (u != 0)
    {
        vec t = multiVecDouble(v, -mLevel * specificImpulse / u);
        return t;
    }
    else
    {
        vec t = {0, 0, 0};
        return t;
    }
}

vec angularVelocity(vec g, vec a, vec t, Rotation moment, double quantSizeOfSec)
{
    double x = moment.rotationAroundX,
    y = moment.rotationAroundY,
    z = moment.rotationAroundZ;
    if (x != 0)
    {
        x = (g.x - a.x - t.x) * quantSizeOfSec / x;
    }
    if (y != 0)
    {
        y = (g.y - a.y - t.y) * quantSizeOfSec / y;
    }
    if (z != 0)
    {
        z = (g.z - a.z - t.z) * quantSizeOfSec / z;
    }
    vec exit = {x, y, z};
    return exit;
}

double temperature(double H)
{
    double T = 273.15;
    if (H > 10)
    {
        if (H > 20)
        {
            if (H > 47)
            {
                if (H > 54)
                {
                    if (H > 84)
                    {
                        if (H > 94)
                        {
                            if (H > 145)
                            {
                                return 0.0;
                            }
                            else
                            {
                                T += 140 * (H - 94) / 51 - 90;
                            }
                        }
                        else
                        {
                            T -= 90;
                            
                        }
                    }
                    else
                    {
                        T += -3 * H + 162;
                    }
                }
            }
            else
            {
                T += (20 * H - 940) / 9;
            }
        }
        else
        {
            T -= 60;
        }
    }
    else
    {
        T += -8 * H + 20;
    }
    return T;
}

double airDens(double H, double T)
{
    H -= 6378.1;
    if (T <= 0.0)
    {
        return 0.0;
    }
    else
    {
        double p = 101325;
        double g = 9.8;
        double R = 8.31447;
        double M = 0.0289644;
        double P = p * exp(-M * g * H * 1000/ (R * T));
        double dens = P * M / (R * T);
        return (dens * pow(10.0, 9)) ;
    }
    
}

double aerodynamicHeating(double T, vec speed)
{
    double a = T + pow(scalar(speed), 2) / 2;
    return a;
}


vec speed(vec speedFirst, ShipPosition sPos, double mLevel,
          double mShip, double mFuel, Rotation moment, double specificImpulse,
          double size, double quantSizeOfSec, double maxOverload, double maxHeating)
{
    if (mLevel * quantSizeOfSec > mFuel)
    {
        return speedFirst;
    }
    else
    {
        double M = mShip + mFuel;
        double mEarth = 5.9742 * pow(10.0, 24);
        double G = 6.67385 * pow(10.0, -20);
        double H = scalar (sPos.position);
        double scSpeedFirst = scalar(speedFirst);
        double S = size * size;
        vec exit = {0, 0, 0};
        vec x = transformVec(sPos.orientation, createQuaternion(
            angularVelocity(gravityForce(sPos.position, M),
            aerodynamicForce(airDens(H, temperature(H)), speedFirst, S),
            tractiveForce(mLevel, specificImpulse, speedFirst), moment, 1.0)));
        if (M != 0) {
            if (quantSizeOfSec > 0.0)
            {
                double v1 = airDens(H, temperature(H)) * scSpeedFirst * S / (2.0 * M);
                double v2 = 1 / quantSizeOfSec  - v1;
                double v3 = scalar(tractiveForce(mLevel, specificImpulse, speedFirst)) / M;
                double v4 = G * mEarth / pow(H, 3);
                vec t1 = multiVecDouble(speedFirst, v1);
                vec t2 = multiVecDouble(speedFirst, v2);
                vec t3 = multiVecDouble(x, v3);
                vec t4 = multiVecDouble(sPos.position, v4);
                exit.x = (t2.x - t3.x - t4.x) * quantSizeOfSec;
                exit.y = (t2.y - t3.y - t4.y) * quantSizeOfSec;
                exit.z = (t2.z - t3.z - t4.z) * quantSizeOfSec;
                vec overload = {0.0, 0.0, 0.0};
                overload.x = - t1.x - t3.x - t4.x;
                overload.y = - t1.y - t3.y - t4.y;
                overload.z = - t1.z - t3.z - t4.z;
                double g = G * mEarth / pow(H, 2);
                double over = scalar(overload) / g;
                if (over > maxOverload)
                {
                    cout<<"Overload!"<<endl;
                }
            }
            else
            {
                return speedFirst;
            }
        }
        else
        {
            cout<<"Mass = 0!"<<endl;
        }
        if (H <= 6523.1)
        {
            double a = aerodynamicHeating(temperature(scalar(sPos.position)), exit);
            if (a > maxHeating)
            {
                cout<<"Overheating!"<<endl;
            }
        }
        return exit;
    }
}

vector <ReturnValues> computeFlightPlan(ShipPosition initialPosition,
                                        ShipParams shipParams, Quants quants)
{
    double S = pow(shipParams.shipEdgeLength, 2);
    double H = scalar (initialPosition.position);
    double fuel = shipParams.fuelMass;
    double m = fuel + shipParams.shipMass;
    vec sp = initialPosition.speedFirst;
    double level;
    Rotation moment;
    vec orient = initialPosition.orientation;
    vector<ReturnValues> returnValue(quants.numberOfQuants);
    int i;
    int j = 0;
    double count = shipParams.flightPlan[0].delayTime;
    for (i = 0; i < quants.numberOfQuants && H > 6378.1; i++,
         count -= quants.quantSizeOfSec)
    {
        level = shipParams.flightPlan[j].impulseValue;
        moment = shipParams.flightPlan[j].rotateValue;
        if (level * quants.quantSizeOfSec > fuel)
        {
            double time = fuel / quants.quantSizeOfSec;
            double timeOut = quants.quantSizeOfSec - time;
            sp = speed(sp, initialPosition, level,
                       shipParams.shipMass, fuel, moment,
                       shipParams.impulsePerFuel, shipParams.shipEdgeLength,
                       time, shipParams.maxOverload, shipParams.maxHeating);
            returnValue[i].speed = sp;
            orient = transformVec(orient, createQuaternion(angularVelocity(
                gravityForce(initialPosition.position, m),
                aerodynamicForce(airDens(H, temperature(H)), sp, S),
                tractiveForce(level, shipParams.impulsePerFuel, sp), moment, time)));
            m -= fuel;
            fuel = 0;
            vec nextH = {initialPosition.position.x +
                sp.x * time, initialPosition.position.y +
                sp.y * time, initialPosition.position.z +
                sp.z * time};
            if (scalar(nextH) < 6378.1)
            {
                vec way = {sp.x * time, sp.y * time, sp.z * time};
                double cosA = (pow(scalar(way), 2) + pow(H, 2) -
                               pow(scalar(nextH), 2)) / (2 * scalar(way) * H);
                double extraWay = H * cosA - sqrt(pow(H, 2) * pow(cosA, 2) +
                                                  pow(6378.1, 2) - pow(H, 2));
                time = extraWay / scalar(sp);
                nextH.x = initialPosition.position.x +
                sp.x * time;
                nextH.y = initialPosition.position.y +
                sp.y * time;
                nextH.z = initialPosition.position.z +
                sp.z * time;
                initialPosition.position = nextH;
                H = 6378.1;
                returnValue[i].position = initialPosition.position;
                break;
            }
            initialPosition.position = nextH;
            H = scalar(initialPosition.position);
            sp = speed(sp, initialPosition, 0.0,
                       shipParams.shipMass, fuel, moment,
                       shipParams.impulsePerFuel, shipParams.shipEdgeLength,
                       timeOut, shipParams.maxOverload, shipParams.maxHeating);
            orient = transformVec(orient, createQuaternion(angularVelocity(
                gravityForce(initialPosition.position, m),
                aerodynamicForce(airDens(H, temperature(H)), sp, S),
                tractiveForce(0.0, shipParams.impulsePerFuel, sp), moment, timeOut)));
            cout<<"Not enough fuel!"<<endl;
        }
        else
        {
            sp = speed(sp, initialPosition, level,
                       shipParams.shipMass, fuel, moment,
                       shipParams.impulsePerFuel, shipParams.shipEdgeLength,
                       quants.quantSizeOfSec, shipParams.maxOverload,
                       shipParams.maxHeating);
            returnValue[i].speed = sp;
            orient = transformVec(orient, createQuaternion(angularVelocity(
                        gravityForce(initialPosition.position, m),
                        aerodynamicForce(airDens(H, temperature(H)), sp, S),
                        tractiveForce(level, shipParams.impulsePerFuel, sp), moment,
                            quants.quantSizeOfSec)));
            fuel -= level * quants.quantSizeOfSec;
            m -= level * quants.quantSizeOfSec;
        }
        vec nextH = {initialPosition.position.x +
            sp.x * quants.quantSizeOfSec, initialPosition.position.y +
            sp.y * quants.quantSizeOfSec, initialPosition.position.z +
            sp.z * quants.quantSizeOfSec};
        if (scalar(nextH) < 6378.1)
        {
            vec way = {sp.x * quants.quantSizeOfSec, sp.y * quants.quantSizeOfSec,
                sp.z * quants.quantSizeOfSec};
            double cosA = (pow(scalar(way), 2) + pow(H, 2) -
                           pow(scalar(nextH), 2)) / (2 * scalar(way) * H);
            double extraWay = H * cosA - sqrt(pow(H, 2) * pow(cosA, 2) +
                                              pow(6378.1, 2) - pow(H, 2));
            quants.quantSizeOfSec = extraWay / scalar(sp);
            nextH.x = initialPosition.position.x +
            sp.x * quants.quantSizeOfSec;
            nextH.y = initialPosition.position.y +
            sp.y * quants.quantSizeOfSec;
            nextH.z = initialPosition.position.z +
            sp.z * quants.quantSizeOfSec;
            initialPosition.position = nextH;
            H = 6378.1;
            returnValue[i].position = initialPosition.position;
            break;
            
        }
        initialPosition.position = nextH;
        H = scalar(initialPosition.position);
        returnValue[i].position = initialPosition.position;
        if (count == 0)
        {
            j++;
            count = shipParams.flightPlan[j].delayTime;
        }
    }
    if (H <= 6378.1)
    {
        int j;
        for (j = i; j < quants.numberOfQuants; j++)
        {
            returnValue[j].position = returnValue[i - 1].position;
        }
        cout<<"Successful landing!!!"<<endl;
    }
    else if (i >= quants.numberOfQuants)
    {
        cout<<"Time is over!!!"<<endl;
    }
    return returnValue;
}

int main()
{
    return 0;
}
