// sarchat - display.cpp
//
// Copyright (c) 2019, TheLastBilly
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include "include/display.hpp"

namespace Display{
    bool direcpp_good = false;
    DireCpp::DireCpp * direcpp = nullptr;
    Display::Display( void )
    :history_count(0){
        //Ncurses init
        initscr();
        noecho();
        cbreak();
        timeout(10);
        // /raw();

        //Start app windows (tx and rx)
        getmaxyx(stdscr, y_max, x_max);
        refresh();
        rx_lines = y_max - 1;
        rx_length = x_max - 1;
        rx_win = newwin( rx_lines, rx_length, 0, 0 );
        tx_win = newwin( 1, x_max, y_max -1, 1 );
        mvprintw(rx_lines, 0, "%c", INPUT_CHAR );
        wrefresh(tx_win);
        wrefresh(rx_win);
        direcpp = new DireCpp::DireCpp();
        direcpp_good = direcpp->init();
        getch();
    }

    void Display::init(){
        if(direcpp_good)
            return;
        AX25::aprs_packet packet = {};
        while(1){
            if(direcpp->receive(&packet)){
                std::cout << "Received!" << '\n';
                std::string msg = direcpp->get_info_str(packet);
                wprintw(rx_win, "%s\n", msg.c_str());
                wrefresh(rx_win);
            }
            char in_c = getch();
            if( in_c == '\n' ){
                if(input_buffer == std::string("/exit")){
                    break;
                }else if(input_buffer == std::string("/clear")){
                    wclear(rx_win);
                    wrefresh(rx_win);
                }else if( input_buffer.length() ){
                    wprintw(rx_win, "%s\n", input_buffer.c_str());
                    direcpp->send_string(input_buffer);
                }
                wrefresh(rx_win);
                history.push_back( input_buffer );
                history_index = history_count = history.size();
                input_buffer.clear();
                wclear(tx_win);
                wrefresh(tx_win);
            }else if( in_c != ERR ){
                int cursor_x, cursor_y;
                if(in_c == BACKSPACE){
                    if(input_buffer.length() > 0)
                        input_buffer = input_buffer.substr(0, input_buffer.length() -1);
                }else if(in_c == LEFT_ARROW_K){
                    getyx(tx_win, cursor_y, cursor_x);
                    if(cursor_x > 0)
                        wmove(tx_win, cursor_y, cursor_x - 1);
                    wrefresh(tx_win);
                }else if(in_c == RIGHT_ARROW_K){
                    getyx(tx_win, cursor_y, cursor_x);
                    if(cursor_x < x_max-1)
                        wmove(tx_win, cursor_y, cursor_x + 1);
                    wrefresh(tx_win);
                }else if(in_c == UP_ARROW_K){
                    if(history_index > 0)
                        history_index--;
                    input_buffer = history.at(history_index);
                }else if( in_c == DOWN_ARROW_K ){
                    if(history_index < history_count -1){
                        history_index++;
                        input_buffer = history.at(history_index);
                    }else if(history_index == history_count - 1 || history_index == history_count)
                        input_buffer.clear();
                }else{
                    input_buffer.append( 1, in_c );
                }
                wclear(tx_win);
                wprintw(tx_win, "%s %d", input_buffer.c_str(), in_c);
                wrefresh(tx_win);
            }
        }
    }

    Display::~Display(){
        endwin();
        if( direcpp != nullptr )
            delete(direcpp);
    }
}
