#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>

using namespace std;

char PORT[] = "/dev/ttyUSB0";

void writeArduino(int x);
void writeArduino(char ch);
void initialize();

int main()
{
	system("cd ./ && gnome-terminal -e \"python speech_rec.py\"");
	// system("\"./speech_loc.py\"");
	
	initialize();

	ifstream fin_rec;
	
	char check='0';

	char ch_rec;
	
	while(1)
	{
		fin_rec.open("./s_rec.txt", ios::in);
		
		fin_rec >> ch_rec;
		
		if( ch_rec == 't')
				{
					if(check == ch_rec)
					{
						fin_rec.close();
						continue;
					}
					check = ch_rec;
					
					//start line following
					cout << "start line following" << endl;
					system("\"./line_follow\"");
				}
				else if( ch_rec == 'o')
					{
						if(check == ch_rec)
						{
							fin_rec.close();
							continue;
						}
						check = ch_rec;
						
						// start object tracking
						cout << "start object tracking" << endl;
						system("\"./obj_track\"");
					}
					else if(ch_rec=='f' || ch_rec=='b' || ch_rec=='m' || ch_rec=='n')
						{
							if(check == ch_rec)
							{
								fin_rec.close();
								continue;
							}
							if(check == 'f' || check=='b' || check=='m' || check=='n')
							{
								writeArduino('s');
							}
								
							check = ch_rec;
							
							writeArduino(ch_rec);
						}
						else if(ch_rec=='q')
							{
								break;
							}
							else if(ch_rec=='s')
								{
									if(check == ch_rec)
									{
										fin_rec.close();
										continue;
									}
									check = ch_rec;
									writeArduino(ch_rec);
								}
		fin_rec.close();
	}

	writeArduino('s');
	fin_rec.close();

	return 0;
}

void writeArduino(int x)
{
	ofstream fout("/dev/ttyUSB0");
	fout << x;
	fout.flush();
	fout.close();
}

void writeArduino(char ch)
{
	for(short i=0; i<20; i++)
		{
			ofstream fout(PORT);
			fout << ch;
			cout << ch <<endl;
			fout.close();
		}
}

void initialize()
{
	ofstream fout("./s_rec.txt");
	fout << '0';
	fout.close();
}