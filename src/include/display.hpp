// sarchat - display.hpp
//
// Copyright (c) 2019, TheLastBilly
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <ncurses.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "../../libdirecpp/src/include/direcpp.hpp"

#define INPUT_CHAR '$'
#define BACKSPACE 127
#define DOWN_ARROW_K 66
#define RIGHT_ARROW_K 67
#define UP_ARROW_K 65
#define LEFT_ARROW_K 68

namespace Display{
    class Display{
        int y_pos, x_pos, y_max, x_max, rx_lines, rx_length;
        WINDOW * tx_win;
        WINDOW * rx_win;
    public:
        Display( void );
        ~Display( void );
        void init();
    private:
        std::vector< std::string > rx_msg;
        std::vector< std::string > history;
        std::string input_buffer;
        int history_count;
        int history_index;
    };
}
#endif
