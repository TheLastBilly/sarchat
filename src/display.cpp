#include "include/display.hpp"

namespace Display{
    bool direcpp_good = false;
    DireCpp::DireCpp * direcpp = nullptr;
    Display::Display( void ){
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
                wprintw(rx_win, "%s\n", packet.info);
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
                input_buffer.clear();
                wclear(tx_win);
                wrefresh(tx_win);
                wrefresh(rx_win);
            }else if( in_c != ERR ){
                if(in_c == 127 && input_buffer.length() > 0){
                    input_buffer = input_buffer.substr(0, input_buffer.length() -1);
                }else{
                    input_buffer.append( 1, in_c );
                }
                wclear(tx_win);
                wprintw(tx_win, "%s", input_buffer.c_str());
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
