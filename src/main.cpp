// sarchat - main.cpp
//
// Copyright (c) 2019, TheLastBilly
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <stdio.h>
#include <string>

#include "include/display.hpp"

int main(int argc, char ** argv){
    Display::Display display;
    display.init();
    return 0;
}
