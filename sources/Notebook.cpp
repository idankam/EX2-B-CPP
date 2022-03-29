#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>
#include "Notebook.hpp"

using namespace ariel;
    
void ariel::Notebook::write(int page, int row, int column,Direction dir , string input){

    if (input.find('~') != string::npos){
        throw invalid_argument( "the char '~' can't be written" );
    }
    // if (input.find('_') != string::npos){
    //     throw invalid_argument( "the char '_' can't be written" );
    // }

    int input_length = input.size();
    if(column >= column_length){throw invalid_argument("column is greater than 99");}
    if(dir == Direction::Horizontal && column + input_length > column_length){ throw invalid_argument("passing column number 99");}
    if(column<0 || page<0 || row<0){ throw invalid_argument("negative param");}

    for (unsigned long i = 0; i < input.length(); i++)
    {
        if (int(input.at(i)) < min_char || int(input.at(i)) > max_char){
            throw invalid_argument("there is at least one not printable char!");
        }
    }

    if (dir == Direction::Horizontal){
        // cout << "horizontal" << endl;

        int tmp_column = column;
        for (int i = 0; i < input.length(); i++){
            string key = to_string(page).append(",").append(to_string(row)).append(",").append(to_string(tmp_column));
            
            if ( _notebook_data.count(key) > 0 ) { // found
                throw invalid_argument("at least one of the places already written.");
            }
            tmp_column++;
        }
        
        this->WriteHorizontal(page, row, column, input);
        this->UpdateMaxRowIfNeeded(page, row);
    }
    else{
        // cout << "VERTICAL" << endl;

        int tmp_row = row;
        for (int i = 0; i < input.length(); i++){
            // string key = to_string(page)+","+to_string(tmp_row)+","+to_string(column);
            string key = to_string(page).append(",").append(to_string(tmp_row)).append(",").append(to_string(column));
            
            if ( _notebook_data.count(key) > 0 ) { // found
                throw invalid_argument("at least one of the places allready written.");
            }
            tmp_row++;
        }
        this->WriteVertical(page, row, column, input);
        this->UpdateMaxRowIfNeeded(page, row + int(input.length()) - 1);
    }
}

void ariel::Notebook::insertChar(string const & key, char c){
    if (c != '_')
    {
        this->_notebook_data.insert ( std::pair<string, char>(key,c) );
    }
}

void ariel::Notebook::WriteHorizontal(int page,int row,int column, string const & input){
    // cout << "WriteHorizontal" << endl;
    for (unsigned long i = 0; i < input.length(); i++)
    {
        string key = to_string(page).append(",").append(to_string(row)).append(",").append(to_string(column));
        // string key = to_string(page)+","+to_string(row)+","+to_string(column);
        // cout << "key = " << key << endl;
        if (_notebook_data.count(key) > 0){
            std::map<string, char>::iterator itr = _notebook_data.find(key);
            itr->second = input.at(i);
        }
        else{
            this->insertChar(key, input.at(i));    
        }
        
        // cout << "value = " << _notebook_data[key] << endl;
        column++;
    }
}

void ariel::Notebook::WriteVertical(int page,int row,int column, string input){
    // cout << "WriteVertical" << endl;
    for (unsigned long i = 0; i < input.length(); i++)
    {
        string key = to_string(page).append(",").append(to_string(row)).append(",").append(to_string(column));
        // string key = to_string(page)+","+to_string(row)+","+to_string(column);

        if (_notebook_data.count(key) > 0){
            std::map<string, char>::iterator itr = _notebook_data.find(key);
            itr->second = input.at(i);
        }
        else{
            this->insertChar(key, input.at(i));    
        }

        row++;
    }
}

string ariel::Notebook::read( int page,int row,  int column, Direction dir, int len){
    if(column >= column_length){throw invalid_argument("column is greater than 99");}
    if(dir == Direction::Horizontal && len+column > column_length){ throw invalid_argument("passing column number 99");}
    if(column<0 || page<0 || row<0 || len<0){ throw invalid_argument("negative param");}
    
    string answer;
    string key;
    if (dir == Direction::Horizontal)
    {
        for (size_t i = 0; i < len; i++)
        {
            key = to_string(page).append(",").append(to_string(row)).append(",").append(to_string(column+int(i)));
            // key = to_string(page)+","+to_string(row)+","+to_string(column+int(i));
            if(_notebook_data.count(key) > 0){
                answer += _notebook_data[key];
            }
            else{
                answer += "_";
            }
        }
    }
    else{ // (dir == Direction::Vertical)
        for (size_t i = 0; i < len; i++)
        {
            key = to_string(page)+","+to_string(row+int(i))+","+to_string(column);
            if(_notebook_data.count(key) > 0){
                answer += _notebook_data[key];
            }
            else{
                answer += "_";
            }
        }
    }
    return answer;
    
}

void ariel::Notebook::erase( int page, int row,  int column, Direction dir,  int len){
    if(len < 0){throw invalid_argument("length can't be negative");}
    if(column >= column_length){throw invalid_argument("column is greater than 99");}
    if(dir == Direction::Horizontal && len+column > column_length){ throw invalid_argument("passing column number 99");}
    if(column<0 || page<0 || row<0 || len<0){ throw invalid_argument("negative param");}
    
    string erase_str;
    for (size_t i = 0; i < len; i++)
    {
        erase_str += '~';
    }
    
    if(dir == Direction::Horizontal){
        WriteHorizontal(page, row, column, erase_str);
        this->UpdateMaxRowIfNeeded(page, row);
    }
    else{
        WriteVertical(page, row, column, erase_str);
        this->UpdateMaxRowIfNeeded(page, row + len);
    }
    
}

void ariel::Notebook::UpdateMaxRowIfNeeded(int page, int row){
    
    // cout << "UpdateMaxRowIfNeeded:" << endl;
    // cout << "(page,key)=" << page << "," << row << endl;

    using namespace std;
    if (_max_row_for_page.count(page) > 0) { // found
        if (_max_row_for_page[page] < row){
            // cout << "UpdateMaxRowIfNeeded -> if" << endl;
            std::map<int, int>::iterator itr = _max_row_for_page.find(page);
            itr->second = row;
        }
    }
    else {
            // cout << "UpdateMaxRowIfNeeded -> else" << endl;
            _max_row_for_page.insert ( std::pair<int, int>(page,row) );
        }
}


void ariel::Notebook::show(int page){
    if(page < 0){throw invalid_argument("length can't be negative");}

    // cout << "max_row_exist?" << _max_row_for_page.count(page) << endl;
    if (_max_row_for_page.count(page) > 0) { // found

        // cout << "here" << endl;
        int maxRow =_max_row_for_page[page];

        // cout << "max_row_is" << maxRow << endl;

        string page_to_print;
        string key;

        for (int i = 0; i < maxRow+ten_more_rows; i++)
        {
            for (int j = 0; j < column_length; j++)
            {
                // key = to_string(page)+","+to_string(i)+","+to_string(j);
                key = to_string(page).append(",").append(to_string(i)).append(",").append(to_string(j));

                if (_notebook_data.count(key) > 0) {
                    page_to_print += _notebook_data[key];
                }
                else {
                    page_to_print += "_";
                }
            }
            page_to_print += " (";
            page_to_print += to_string(i);
            page_to_print += "\n";
        }
        
        cout << page_to_print << endl;
    }
    else{
        cout << "page is empty!" << endl;
    }
}
