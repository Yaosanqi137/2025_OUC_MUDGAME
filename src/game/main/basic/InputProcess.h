#ifndef INPUTPROCESS_H
#define INPUTPROCESS_H
#include <string>
#include <algorithm>

// 用来移除字符串左侧的空白
inline std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// 用来移除字符串右侧的空白
inline std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// 移除字符串两端的空白
inline std::string trim(std::string s) {
    return ltrim(rtrim(s));
}

// 计算含中文字符串的长度 :(
inline int getLength(std::string s) {
    int count = 0;
    for (int i = 0; s[i]; i++)
    {
        if (s[i] < 0) i++;
        // 负数说明该字符为中文字符，占用两个字节，跳过后一个字节(i++)，不进行统计
        count++;
    }
    return count;
}

#endif // INPUTPROCESS_H