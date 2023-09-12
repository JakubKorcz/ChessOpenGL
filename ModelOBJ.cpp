#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;
class OBJModel {

public:
	string line;


	vector<float*> vertices;
	vector<float*> texcoords;
	vector<float*> normals;
	vector<float*> polygons;

	int vertices_count = 0;
	float a, b, c;
	int a1, b1, c1, d, e, f, g, h, i, j, k, l;
private:

	void loadFile(const char* filename)
	{
		ifstream file(filename);
		if (!file.is_open()) {
			printf("Blad podczas otwierania pliku!");
		}
		else {
			while (getline(file, line))
			{
				if (line[0] == 'v') {
					if (line[1] == ' ')
					{
						vertices_count++;
						sscanf_s(line.c_str(), "v %f %f %f", &a, &b, &c);
						vertices.push_back(new float[3] {a, b, c});
					}
					else if (line[1] == 't')
					{
						sscanf_s(line.c_str(), "vt %f %f %f", &a, &b, &c);
						texcoords.push_back(new float[3] {a, b, c});
					}
					else
					{
						sscanf_s(line.c_str(), "vn %f %f %f", &a, &b, &c);
						normals.push_back(new float[3] {a, b, c});
					}
				}
				else if (line[0] == 'f')
				{
					sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &a1, &b1, &c1, &d, &e, &f, &g, &h, &i, &j, &k, &l);
					
				}
			}
		}
		file.close();
		cout << vertices_count << endl;
	}
};