#pragma once

#include <map>
#include <string>

// Converts a character to its hex equivalent
char to_hex(const char code);

// Converts a hex character to its char eqivalent
char from_hex(const char ch);

// Transform URL encoded characters into plain text.
std::string decode(const std::string& source);

// return a json content-type
std::string json_header ();

// test if a string is numeric
bool numeric(std::string value);

// place values in json objects
std::string jsonify (const std::string& msg);
std::string jsonify (const std::map<std::string, std::string>& kvp);
std::string jsonify (const std::string& value, const std::string& uom);

// Find all instances of 'old_string' in source
// and replace with 'new_string'
void find_replace(std::string* source, 
                  const std::string& old_string, 
                  const std::string& new_string);

