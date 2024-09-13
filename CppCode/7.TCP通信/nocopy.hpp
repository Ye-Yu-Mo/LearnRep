#pragma once 
#include "iostream"

class nocopy
{
public:
  nocopy() {}
  nocopy(const nocopy&) = delete;
  const nocopy& operator=(const nocopy&) = delete;
  ~nocopy() {}
};
