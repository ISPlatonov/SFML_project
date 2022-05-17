#pragma once

#include <SFML/System.hpp>
#include <cmath>


namespace linalg
{
    sf::Vector2f normalize(sf::Vector2f v);

    /*Vector2<T>& rotateBy(float degrees, const Vector2<T>& center=Vector2<T>())
    {
        degrees *= 3.14159f/180.f;
        const float cs = cos(degrees);
        const float sn = sin(degrees);

        x -= center.x;
        y -= center.y;

        T nx = (T)(x*cs - y*sn);
        T ny = (T)(x*sn + y*cs);

        x = nx;
        y = ny;

        x += center.x;
        y += center.y;
        return *this;
    }*/

    /*// returns trig angle
    float getAngle() const
    {
        if (y == 0)
            return x < 0 ? 180 : 0;
        else if (x == 0)
            return y < 0 ? 270 : 90;

        if ( y > 0)
            if (x > 0)
                    return atan(y/x) * 180.f/3.14159f;
            else
                return 180.0-atan(y/-x) * 180.f/3.14159f;
        else
            if (x > 0)
                return 360.0-atan(-y/x) * 180.f/3.14159f;
            else
                return 180.0+atan(-y/-x) * 180.f/3.14159f;
    }*/

    float magnitude(const sf::Vector2f& v);
}
