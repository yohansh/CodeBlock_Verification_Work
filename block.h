#pragma once
#include <string>

using namespace std;


	class Block {

	public:

		Block(/*int i,*/ /*int gi,*/ string type, string sfn, /*string mn,*/ int sl, int el, /*string vc,*/ string vcx);
		
		string getPath();
		
		int getstartLine();
		
		int getendLine();

		string getxmlCode();
		string getType();
	private:
		//int uid;
		//int group_id;
		string type;
		string source_file_name;
		//string module_name;
		int start_line;
		int end_line;
		//string verilog_code;
		string verilog_code_xml;
	};
