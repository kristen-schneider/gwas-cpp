#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>

#include "utils.h"

using namespace std;

/*
 * Read config file
 * @param config_file
 * @return vector of config options
 */
map<string, string> read_config_file(
        string config_file) {
    map<string, string> config_options;

    // check if file exists
    ifstream config_stream(config_file);
    if (!config_stream.good()) {
        cout << "ERROR: Config file does not exist: " << config_file << endl;
        exit(1);
    }
    // read and parse config file
    string option_name;
    string option_item;
    while (getline(config_stream, option_name)) {
        // remove ":" from option name
        option_name.erase(remove(option_name.begin(), option_name.end(), ':'), option_name.end());
        getline(config_stream, option_item);
        // remove leading whitespace from option item
        option_item.erase(0, option_item.find_first_not_of(' '));
        config_options[option_name] = option_item;
    }
    config_stream.close();
    return config_options;
}

/*
 * Add default config options if not present
 * @param config_options
 * @return void
 */
void add_default_config_options(
        map<string, string> &config_options) {
    // add default config options if option is empty
    if (config_options["block_size"].empty()) {
        config_options["block_size"] = "20";
    }
    if (config_options["query_type"].empty()) {
        // exit program with error message
        cout << "ERROR: query_type not specified in config file." << endl;
        exit(1);
    }
}


/*
 * Convert vector to string
 * @param vec: vector of strings
 * @return str: string of comma separated values
 */
string convert_vector_to_string(vector<string> vec){
    string str;
    for (int i = 0; i < vec.size(); i++){
        str += vec[i] + ",";
    }
    str.pop_back();
    return str;
}

/*
 * Convert string to vector
 * @param str: string of comma separated values
 * @return vector of strings
 */
vector<string> convert_string_to_vector(string str){
    vector<string> vec;
    istringstream line_stream(str);
    string column_value;
    while (getline(line_stream, column_value, ',')) {
        // remove newline character from column_value
        column_value.erase(remove(column_value.begin(), column_value.end(), '\r'), column_value.end());
        vec.push_back(column_value);
    }
    return vec;
}

/*
 * Convert bytes to int
 * @param line: bytes
 * @return value: int value
 */
int bytes_to_int(char bytes[4]){
    int value = 0;
    value += (unsigned char) bytes[0];
    value += (unsigned char) bytes[1] << 8;
    value += (unsigned char) bytes[2] << 16;
    value += (unsigned char) bytes[3] << 24;
    return value;
}

/*
 * Convert int to bytes
 * @param value: int
 * @return bytes: char *
 */
char * int_to_bytes(int value){
    char * bytes = new char[4];
    bytes[0] = (char) (value & 0xFF);
    bytes[1] = (char) ((value >> 8) & 0xFF);
    bytes[2] = (char) ((value >> 16) & 0xFF);
    bytes[3] = (char) ((value >> 24) & 0xFF);
    return bytes;
}

/*
 * Split string by delimiter
 * @param str: string
 * @param delimiter: char to split by
 * @return vector of strings
 */
vector<string> split_string(string str, char delimiter){
    vector<string> vec;
    istringstream line_stream(str);
    string column_value;
    while (getline(line_stream, column_value, delimiter)) {
        // remove newline character from column_value
        column_value.erase(remove(column_value.begin(), column_value.end(), '\r'), column_value.end());
        vec.push_back(column_value);
    }
    return vec;
}

/*
 * Get index value of a string in a vector
 * @param vec: vector in which to look
 * @param str: string to search for
 * @return index: index value of string in vector
 */
int get_index(vector<string> vec, string str){
    int idx = -1;
    for (int i = 0; i < vec.size(); i++){
        if (vec[i] == str){
            idx = i;
            break;
        }
    }
    return idx;
}

/*
 * Function to get the column types of a file
 * @param line: string
 * @param delimiter: char to split by
 * @return column_types_str: string of comma separated data types
 */
string get_data_types(
        string line,
        char delimiter) {

    string column_types_str;
    // split line by delimiter find data type and store in comma separated string
    stringstream ss(line);
    string item;
    while (getline(ss, item, delimiter)) {
        // check if item is int
        // check if item is float
        // else  item stays as string
        // if none of the above, throw error
        // search for '.' in item
        if (item.find('.') != string::npos) {
            column_types_str += "float,";
        } else {
            try {
                stoi(item);
                column_types_str += "int,";
            }
            catch (invalid_argument &e) {
                column_types_str += "string,";
            }
        }
    }
    // remove last comma
    column_types_str.pop_back();
    return column_types_str;
}

/*
 * Function to get the delimiter of a file
 * @param line: string of first line of file
 * @return delimiter: char to split by
 */
char get_delimiter(
        string line){
    char delimiter;
    // determine delimiter
    if(line.find('\t') != string::npos){
        delimiter = '\t';
    } else if(line.find(',') != string::npos){
        delimiter = ',';
    } else if(line.find(' ') != string::npos){
        delimiter = ' ';
    } else {
        cout << "Error: delimiter not found.\n"
                " Please use files with delimiters as tab, comma, or space." << endl;
        exit(1);
    }
    return delimiter;
}

//*
// * Function to get the column names of a file
// * @param line: string
// * @param delimiter: char
// * @return column_names_str: string
// */
//string get_column_names(
//        string line,
//        char delimiter){
//
//    string column_names_str;
//    // split line by delimiter and store in comma separated string
//    stringstream ss(line);
//    string item;
//    while(getline(ss, item, delimiter)){
//        // remove carriage return from item
//        item.erase(remove(item.begin(), item.end(), '\r'), item.end());
//        column_names_str += item + ",";
//    }
//    // remove last comma
//    column_names_str.pop_back();
//
//    return column_names_str;
//}
