#include <string>
#include <json/json.h>
#include <fstream>  
#include <sstream> 

void readJson();
void writeJson();
/*
{  
    "uploadid": "UP000000",  
    "code": "0",  
    "msg": "",  
    "files":  
    [  
        {  
            "code": "0",  
            "msg": "",  
            "filename": "1D_16-35_1.jpg",  
            "filesize": "196690",  
            "width": "1024",  
            "height": "682",  
            "images":  
            [  
                {  
                    "url": "fmn061/20111118",  
                    "type": "large",  
                    "width": "720",  
                    "height": "479"  
                },  
                {  
                    "url": "fmn061/20111118",  
                    "type": "main",  
                    "width": "200",  
                    "height": "133"  
                }  
            ]  
        }  
    ]  
}  
{
    "uploadid": "UP000000",
    "code": "0",
    "msg": "",
    "files":
    [
        {
            "code": "0",
            "msg": "",
            "filename": "1D_16-35_1.jpg",
            "filesize": "196690",
            "width": "1024",
            "height": "682",
            "images":
            [
                {
                    "url": "fmn061/20111118",
                    "type": "large",
                    "width": "720",
                    "height": "479"
                },
                {
                    "url": "fmn061/20111118",
                    "type": "main",
                    "width": "200",
                    "height": "133"
                }
            ]
        }
    ]
}
 */

int ParseJsonFromFile(const char* filename)  
{  
  // 解析json用Json::Reader   
  Json::Reader reader;  
  // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...   
  Json::Value root;         
  
  std::ifstream is;  
  is.open (filename, std::ios::binary );    
  if (reader.parse(is, root))  
  {  
    std::string code;  
    if (!root["files"].isNull())  // 访问节点，Access an object value by name, create a null member if it does not exist.   
      code = root["uploadid"].asString();  
      
    // 访问节点，Return the member named key if it exist, defaultValue otherwise.   
    code = root.get("uploadid", "null").asString();  
  
    // 得到"files"的数组个数   
    int file_size = root["files"].size();  
  
    // 遍历数组   
    for(int i = 0; i < file_size; ++i)  
    {  
      Json::Value val_image = root["files"][i]["images"];  
      int image_size = val_image.size();  
      for(int j = 0; j < image_size; ++j)  
      {  
        std::string type = val_image[j]["type"].asString();  
        std::string url = val_image[j]["url"].asString();  
      }  
    }  
  }  
  is.close();  
  return 0;  
}  

int test_main(int argc, char** argv) {
	readJson();
	// writeJson();
	return 0;
}

void readJson() {
	using namespace std;
	std::string strValue = "{\"name\":\"json\",\"array\":[{\"cpp\":\"jsoncpp\"},{\"java\":\"jsoninjava\"},{\"php\":\"support\"}]}";

	Json::Reader reader;
	Json::Value value;


	if (reader.parse(strValue, value))
	{
		std::string out = value["name"].asString();
		std::cout <<"name"<< out << std::endl;
		std::cout <<"array"<<std::endl;
	}
}

void writeJson() {
	using namespace std;

	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;

	item["cpp"] = "jsoncpp";
	item["java"] = "jsoninjava";
	item["php"] = "support";
	arrayObj.append(item);

	root["name"] = "json";
	root["array"] = item;

	root.toStyledString();
	std::string out = root.toStyledString();
	
	std::cout << "123456" << std::endl;
	std::cout << out << std::endl;
}
