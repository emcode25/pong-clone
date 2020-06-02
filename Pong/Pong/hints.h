#ifndef HINTS_H
#define HINTS_H

//Macro wizardy (rounds a number for use in constexpr values)
#define ROUND(x) (((x) - (int) (x) >= 0.5) ? (int) (x) + 1 : (int) (x))

constexpr int WINWIDTH = 1920;
constexpr int WINHEIGHT = 1010;

#endif