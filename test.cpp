#include <io.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

void split(const string& str, const string& sp, vector<string>& vec)
{
	size_t size = sp.size();

	vec.clear();
	size_t end = 0, start = 0;
	while (start != string::npos && start < str.size())
	{
		end = str.find(sp, start);
		vec.push_back(str.substr(start, end - start)); // end == 0 ʱѹ����ַ���
		start = end == string::npos ? end : end + size;
	}

	if (vec.empty())
		vec.push_back(str);
}


void GetSpecialFilesFromDirectory(string path, string fileType, vector<string>& files)
{
	vector<string> tempFileTypes;
	split(fileType, " ", tempFileTypes);
	if (tempFileTypes.size() == 0)
		return;

	for (int i=0;i<tempFileTypes.size();++i)
	{
		// �ļ����
		long hFile = 0;
		// �ļ���Ϣ
		struct _finddata_t fileinfo;

		string p;

		if ((hFile = _findfirst(p.assign(path).append("\\*" + tempFileTypes[i]).c_str(), &fileinfo)) != -1) {
			do
			{
				// ��һ�֣������ļ���ȫ·��
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				//�ڶ��֣��������ļ���ȫ·��
				//files.push_back(fileinfo.name);
			} while (_findnext(hFile, &fileinfo) == 0); //Ѱ����һ�����ɹ�����0������-1

			_findclose(hFile);
		}
	}
}

void GetAllFilesIncludeSubfolder(string path,string fileType,vector<string>& files) 
{
	vector<string> tempFileTypes;
	split(fileType, " ", tempFileTypes);
	if (tempFileTypes.size() == 0)
		return;
	//�ļ����
	long hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR)) 
			{ //�Ƚ��ļ������Ƿ����ļ���
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) 
				{
					string subForld = p.assign(path).append("\\").append(fileinfo.name);
					//�ݹ�����
					GetSpecialFilesFromDirectory(subForld, fileType, files);
					GetAllFilesIncludeSubfolder(p.assign(path).append("\\").append(fileinfo.name),fileType, files);
				}
			}
			else 
			{
				for (int i=0;i<tempFileTypes.size();++i)
				{
					string fileName = fileinfo.name;
					if (fileName.find(tempFileTypes[i]) != std::string::npos)
					{
						// ��һ�֣������ļ���ȫ·��
						files.push_back(p.assign(path).append("\\").append(fileinfo.name));
						//�ڶ��֣��������ļ���ȫ·��
						//files.push_back(fileinfo.name);
					}
					else
					{
						continue;
					}
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0); //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
}



char S[505];
int main()
{
	vector<string> m_Files;
	string path="";
	puts("������Ҫת�����ļ��е�Ŀ¼:");
	cin>>path; 
	GetAllFilesIncludeSubfolder(path, ".epub", m_Files);
	for (int i=0;i<m_Files.size();++i)
	{
		string ss="kindlegen.exe "+m_Files[i];
		for(int i=0;i<ss.length();i++)
			S[i]=ss[i];
		S[ss.length()]=0;
		system(S);
	}
	system("pause"); 
	return 0;
}
