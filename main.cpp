#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

enum SIDE {CLIENT,SERVER,BOTH};
int main(int argc, char* argv[]) {
        if (argc < 2 ) {
						std::cout << "Usage: " << argv[0] << " <path_to_mods_directory>" << std::endl;
						return 2;
					}
	std::vector<unsigned int> curse;
	std::vector<std::string> url;
	std::vector<char> urlsides;
        for (const auto& entry : std::filesystem::directory_iterator(argv[1])) {
								if (entry.path().extension() != ".toml") {
										std::cout << "Found non-toml file: " << entry.path().filename() << " Ignoring..." << std::endl;
								}
								else {
										// std::cout << entry.path() << std::endl;
										std::ifstream file(entry.path());
										std::string line;
										char side=BOTH;
										while(std::getline(file, line)) {
												size_t pos = line.find("side");
												if (pos != std::string::npos) {
														line=line.substr(line.find("=")+1);
														while (line[0]==' '||line[0]=='"') line=line.substr(1);
														while (line[line.size()-1]==' '||line[line.size()-1]=='"') line=line.substr(0,line.size()-1);
														if (line=="client") side=CLIENT;
														else if (line=="server") side=SERVER;
														else if (line!="both")std::cerr << "Unknown side: " << line << " in file: " << entry.path().filename() << std::endl;
														break;
												}
											}
										while(std::getline(file,line)){
											if (line.substr(0,3)=="url")
												{
													line=line.substr(line.find("=")+1);
													while (line[0]==' '||line[0]=='"') line=line.substr(1);
													while (line[line.size()-1]=='"') line=line.substr(0,line.size()-1);
													url.push_back(line);
													urlsides.push_back(side);
													break;
												}
											if (line=="[update.curseforge]") getline(file,line);else continue;
											curse.push_back(std::stoi(line.substr(line.find("=")+1)));
											getline(file,line);
											curse.push_back(std::stoi(line.substr(line.find("=")+1)));
											curse.push_back(side);
											break;
											}
								}
        }
std::cout<<"{\n\t\"curse\":\n\t[";
unsigned short count = 0;
while (count<curse.size()/3){
	std::cout<<"\n\t\t{\n\t\t\t\"addonId\": "<<curse[count*3]<<",\n\t\t\t\"fileid\": ";std::cout<<curse[count*3+1];
		if (curse[count*3+2] != BOTH) std::cout<<",\n\t\t\t\"metadata\": {\n\t\t\t\t\"side\": \""<<(curse[count*3+2]==CLIENT?"CLIENT":(curse[count*3+2]==SERVER?"SERVER":"BOTH"))<<"\"\n\t\t\t}";
	std::cout<<"\n\t\t}";
		if (count<curse.size()/3-1) std::cout<<",";
	count++;
}
std::cout<<"\n\t],\n\t\"url\":\n\t[";
count=0;
while (count<url.size()){
	std::cout<<"\n\t\t{\n\t\t\t\"url\": \""<<url[count]<<"\"";
		if (urlsides[count] != BOTH) std::cout<<",\n\t\t\t\"metadata\": {\n\t\t\t\t\"side\": \""<<(urlsides[count]==CLIENT?"CLIENT":(urlsides[count]==SERVER?"SERVER":"BOTH"))<<"\"\n\t\t\t}";
	std::cout<<"\n\t\t}";
	if (count<url.size()-1) std::cout<<",";
	count++;
}
std::cout<<"\n\t]\n}\n";
    return 0;
}
