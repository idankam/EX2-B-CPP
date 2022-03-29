#pragma once

#include "Direction.hpp"
#include <string>
#include <map>
#include <iterator>

using namespace std;
namespace ariel{

    const int column_length = 100;
    const int min_char = 32;
    const int max_char = 126;
    const int ten_more_rows = 10;

    class Notebook{
        
    private:
        std::map<string, char> _notebook_data;
        std::map<int, int> _max_row_for_page;

        void insertChar(string const & key, char c);
        void WriteHorizontal(int page,int row,int column, string const & input);
        void WriteVertical(int page,int row,int column, string input);
        void UpdateMaxRowIfNeeded(int page, int row);
        
    public:
        
        void write (int page,int row,int column,Direction dir , string input);
        string read(int page,int row,int column, Direction dir, int len);
        void erase(int page,int row, int column, Direction dir,int len);
        void show(int page);
        
    };
}
