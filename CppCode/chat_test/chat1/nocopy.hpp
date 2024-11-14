#pragma once
#include <iostream>
/// @brief 简单的单例模式类
class nocopy
{
public:
    nocopy() {}
    nocopy(const nocopy &) = delete;
    const nocopy &operator=(const nocopy &) = delete;
    ~nocopy() {}
};