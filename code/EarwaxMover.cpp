#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

std::string dataPath = "D:\\Projects\\Hybrid\\Jackbox mod menu\\data.txt";
std::string audioDir = "D:\\Projects\\Hybrid\\Jackbox mod menu\\cut";
std::string gamePath = "D:\\Program Files (x86)\\Steam\\steamapps\\common\\The Jackbox Party Pack 2\\games\\Earwax\\content";

//TODO replace with dynamic array, stop going over limit
//Vector can't iterate lie array, need to change modify loop
std::vector<std::string> files;
std::vector<int> fileID;
std::vector<std::string> fname;

std::string getCurrent(std::string path){
  std::string current;
  std::fstream data(dataPath);
  getline(data, current, '>');
  data.close();
  return current;
}

int findEnd(std::string path, char want){
  std::fstream file(path);
  int loc = 0;
  char myChar = ' ';
  for(int i = -1; myChar != ']'; i--){
      file.seekg(i, std::fstream::end);
      loc = i;
      myChar = file.peek();
      std::cout << "i = " << i << " " << myChar << " is located at " << file.tellg() << "\n";
    }
  std::cout << "left loop with " << myChar << " at " << file.tellg() << '\n';
  loc = file.tellg();
  file.close();
  return loc-1;
}

std::string editData(int index){
  std::string name = fname.at(index);
  std::string id = std::to_string(fileID.at(index));
  
  std::string ret = 
  "{\n"
    "  \"x\": false,\n \"name\": \""+name+"\",\n"
    "  \"short\": \""+name+"\",\n"
    "  \"id\": " + id + ",\n"
    "  \"categories\": [\"violence\"]"
  "\n}";

  return ret;
}

void updateAudio(/*std::string name, std::string id*/){
  /*
  get file path
  open file with std::fstream EarwaxAudio.jet
  move ot end of file using seekg(std::end)
  loop:
    decrement read pointer until a closing square bracket is found
  read pointer = bracket loc -1
  //this is where code block will be placed 
  loop through id/name array
    write template before bracket using id and name
    (temp tags first, select random later)

  */
  std::string testPath = "D:\\Projects\\Hybrid\\Jackbox mod menu\\test.txt"; 
  int loc = findEnd(testPath, ']');
  std::fstream test(testPath);
  test.seekg(loc, std::fstream::beg);
  char myChar = test.peek();
  std::cout << "loc = " << loc << "\nChar = " << myChar << "\nPointer at " << test.tellg() << '\n';
  test.close();
  //using fstream, remove chars of } and ], replacing with a "," and whitespace if can't use null,
  // then open in append to write
  std::ofstream oTest;
  oTest.open(testPath, std::ofstream::app);
  std::cout <<"loc = "<< loc << " write pointer at " << oTest.tellp() << "\n";

  for(int i = 0; i < 2; i++){
    std::string ent = editData(i);
    oTest << ent << ",";
  }
  oTest << "]\n}";
}

//need to check for directory and create if not
void list(std::string path, int current){
  fs::path directory = path;
  int i = 0;
  for (const auto& entry : fs::directory_iterator(directory)){
    if(entry.path().extension() != ".ogg"){ continue; }
    files.push_back(entry.path().generic_string());
    fileID.push_back(current + i);
    fname.push_back(entry.path().filename().generic_string());//only includes file name
    i++;
  }
}

//insert clip name and ID into audio manifest file
void move(std::string file, std::string name){
 //might be unecessary, possibly just use copy function in filesystem 
  std::string gameLoc = gamePath + "\\EarwaxAudio\\Audio\\" + name;
  fs::copy(file, gameLoc);
}

void modify(){
  //probably replace with range loop
  for(int i = 0; i < files.size(); i++){
    std::string name = std::to_string(fileID.at(i)) + ".ogg";
    std::cout << name + "\n";
    std::string oldPath = files.at(i);
    std::string newPath = audioDir + "\\" + name;
    std::cout << newPath + "\n";
    fs::copy(oldPath, newPath);
    move(newPath, name);
    //copy file to earwax audio folder
  }
}

//get latest ID from ID list, append full file
void updateData(){
  std::string old;
  std::fstream data(dataPath); 
  getline(data, old, '>');
}

/*void getPath(){
  std::fstream data("D:\\Projects\\Hybrid\\Jackbox mod menu\\data.txt");
  getline(data, path, '>');
  while(path.length() < 1){
    std::cout << "Enter music path:\n";
    std::cin >> path;
  }
}*/

int main(int argc, char *argv[]) 
{
  std::string current = getCurrent(dataPath);
  std::cout << current + "\n";

  list(audioDir, std::atoi(current.c_str()));
  updateAudio();
  //modify();
}
