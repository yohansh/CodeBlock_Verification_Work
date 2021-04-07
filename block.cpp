#include <string>
#include <iostream>
#include <vector>
#include "block.h"

using namespace std;

Block::Block(/*int i,*/ /*int gi,*/ string t, string sfn, /*string mn,*/ int sl, int el, /*string vc,*/ string vcx)
	:/*uid{ i },*/ /*group_id{ gi },*/ type{ t }, source_file_name{ sfn }, /*module_name{ mn },*/ start_line{ sl }, end_line{ el }, /*verilog_code{ vc },*/ verilog_code_xml{ vcx } {
	/*cout << "id: "<< i << " parsed" <<endl;*/ }

//Remember the format is added with "../cha_swerv_11_testgenvar/<path_to_file>"
string Block::getPath() {
	return source_file_name;
}

int Block::getstartLine() {
	return start_line;
}

int Block::getendLine() {
	return end_line;
}

string Block::getxmlCode() {
	return verilog_code_xml;
}

string Block::getType() {
	return type;
}