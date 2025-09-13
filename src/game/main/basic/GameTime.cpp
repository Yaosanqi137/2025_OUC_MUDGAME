#include "GameTime.h"

GameTime GameTime::_Time_;
// 这会为 month 静态数组分配内存并初始化。
const unsigned int GameTime::month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


GameTime::GameTime() :
    YEAR_(2007), MONTH_(8), DAY_(31), HOUR_(14), MINUTE_(39) {
    // 构造时立即检查并设置初始年份是否为闰年
    // 注意：这里的 updateLeapYearStatus() 会在 _Time_ 实例被创建时自动调用。
    updateLeapYearStatus();
}

void GameTime::addYear(const unsigned int YEAR) {
    if (!YEAR) return;
    _Time_.YEAR_ += YEAR;
    updateLeapYearStatus();
}

void GameTime::addMonth(const unsigned int MONTH) {
    if (!MONTH) return;
    unsigned int years2add = (_Time_.MONTH_ + MONTH - 1) / 12;
    if (years2add > 0) {
        addYear(years2add);
    }
    _Time_.MONTH_ = (_Time_.MONTH_ + MONTH - 1) % 12 + 1;
}

void GameTime::addDay(const unsigned int DAY) {
    if (!DAY) return;
    _Time_.DAY_ += DAY;

    while (true) {
        unsigned int daysInCurrentMonth = _Time_.month[_Time_.MONTH_];
        if (_Time_.isLeapYear && _Time_.MONTH_ == 2) {
            daysInCurrentMonth = 29;
        }
        if (_Time_.DAY_ > daysInCurrentMonth) {
            _Time_.DAY_ -= daysInCurrentMonth;
            addMonth(1);
        } else {
            break;
        }
    }
}

void GameTime::addHour(const unsigned int HOUR) {
    if (!HOUR) return;
    unsigned int totalHours = _Time_.HOUR_ + HOUR;
    addDay(totalHours / 24);
    _Time_.HOUR_ = totalHours % 24;
}

void GameTime::addMinute(const unsigned int MINUTE) {
    if (!MINUTE) return;
    unsigned int totalMinutes = _Time_.MINUTE_ + MINUTE;
    addHour(totalMinutes / 60);
    _Time_.MINUTE_ = totalMinutes % 60;
}

bool GameTime::setTime(const unsigned int YEAR,
                       const unsigned int MONTH,
                       const unsigned int DAY,
                       const unsigned int HOUR,
                       const unsigned int MINUTE) {
    if (MONTH < 1 || MONTH > 12 || DAY < 1 || DAY > 31 || HOUR > 23 || MINUTE > 59) {
        return false;
    }

    // 防止时光倒流
    if (YEAR < _Time_.YEAR_) return false;
    if (YEAR == _Time_.YEAR_) {
        if (MONTH < _Time_.MONTH_) return false;
        if (MONTH == _Time_.MONTH_) {
            if (DAY < _Time_.DAY_) return false;
            if (DAY == _Time_.DAY_) {
                if (HOUR < _Time_.HOUR_) return false;
                if (HOUR == _Time_.HOUR_) {
                    if (MINUTE < _Time_.MINUTE_) return false;
                }
            }
        }
    }

    // 所有检查通过，设置新时间
    _Time_.YEAR_ = YEAR;
    _Time_.MONTH_ = MONTH;
    _Time_.DAY_ = DAY;
    _Time_.HOUR_ = HOUR;
    _Time_.MINUTE_ = MINUTE;
    updateLeapYearStatus();
    return true;
}

unsigned int GameTime::getYear()   { return _Time_.YEAR_; }
unsigned int GameTime::getMonth()  { return _Time_.MONTH_; }
unsigned int GameTime::getDay()    { return _Time_.DAY_; }
unsigned int GameTime::getHour()   { return _Time_.HOUR_; }
unsigned int GameTime::getMinute() { return _Time_.MINUTE_; }

void GameTime::updateLeapYearStatus() {
    _Time_.isLeapYear = (_Time_.YEAR_ % 400 == 0) || (_Time_.YEAR_ % 4 == 0 && _Time_.YEAR_ % 100 != 0);
}