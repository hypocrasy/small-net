//Nettester �Ĺ����ļ�
#pragma GCC optimize(3,"Ofast","inline")
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <bitset>
#include<string>
#include <numeric>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <complex>
#include <ctime>
#include <conio.h>
#include "winsock.h"
#include "stdio.h"
#include "CfgFileParms.h"
#include "function.h"
#include<stdio.h>
using namespace std;


//CRC16У���
unsigned short crc16_ccitt_table[256] =
{
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

//����Ϊ��Ҫ�ı���
U8* sendbuf;        //������֯�������ݵĻ��棬��СΪMAX_BUFFER_SIZE,���������������������ƣ��γ��ʺϵĽṹ��������û��ʹ�ã�ֻ������һ��
int printCount = 0; //��ӡ����
int spin = 0;  //��ӡ��̬��Ϣ����

//------�����ķָ��ߣ�һЩͳ���õ�ȫ�ֱ���------------
int iSndTotal = 0;  //������������
int iSndTotalCount = 0; //���������ܴ���
int iSndErrorCount = 0;  //���ʹ������
int iRcvForward = 0;     //ת����������
int iRcvForwardCount = 0; //ת�������ܴ���
int iRcvToUpper = 0;      //�ӵͲ�ݽ��߲���������
int iRcvToUpperCount = 0;  //�ӵͲ�ݽ��߲������ܴ���
int iRcvUnknownCount = 0;  //�յ�������Դ�����ܴ���
int num = 0;
bool ack = 1;
set <int> cache_num;
map<int, string> cache_string;
queue<string> sentmessage;
queue<int> recmessage[5];
map<int, int> subnet;
map<int, int> arp_table;
int lastsend = 0;
int frame_num = 1;
map<int, int> mac_table;
int switch_or_not = -1;
int loc_ip = 0;
int DefaultGateway = 0;
int insertnum = 0;
//bool ip_assign_ack[20];//ip����ȷ��
int SN = 0; //�ýӿ��µ��������������
int cnt = 2; //������
//��ӡͳ����Ϣ
void print_statistics();
void menu();
//***************��Ҫ��������******************************
//���ƣ�InitFunction
//���ܣ���ʼ�������棬��main�����ڶ��������ļ�����ʽ������������ǰ����
//���룺
//�����
void printtest(U8* a, int len)
{
	cout << "\n";
	for (int i = 0; i < len; i++)
	{
		cout << (int)*(a + i) << " ";
	}
	cout << "\n";
}
//�������CRCУ�飬ʹ��CRC16_CCITT��׼
//�����������һ��Ϊ��ֵ0Xffff���ڶ���ΪУ�����ݣ�������Ϊ���ݳ���
//����ֵ������Ƿ��ͷ�����Ϊ���У��λ�����ܷ�����Ϊ����У�顣
unsigned short do_crc(unsigned short reg_init, char* message, unsigned int len)
{
	unsigned short crc_reg = reg_init;

	while (len--)
		crc_reg = (crc_reg >> 8) ^ crc16_ccitt_table[(crc_reg ^ *message++) & 0xff];

	return crc_reg;
}
void InitFunction(CCfgFileParms& cfgParms)
{
	sendbuf = (char*)malloc(MAX_BUFFER_SIZE);
	if (sendbuf == NULL) {
		cout << "�ڴ治��" << endl;
		//����������Ҳ̫���˳���
		exit(0);
	}
	return;
}
//***************��Ҫ��������******************************
//���ƣ�EndFunction
//���ܣ����������棬��main�������յ�exit������������˳�ǰ����
//���룺
//�����
void EndFunction()
{
	if (sendbuf != NULL)
		free(sendbuf);
	return;
}
//***************��Ҫ��������******************************
//���ƣ�TimeOut
//���ܣ�������������ʱ����ζ��sBasicTimer�����õĳ�ʱʱ�䵽�ˣ�
//      �������ݿ���ȫ���滻Ϊ������Լ����뷨
//      ������ʵ���˼���ͬʱ���й��ܣ����ο�
//      1)����iWorkMode����ģʽ���ж��Ƿ񽫼�����������ݷ��ͣ�
//        ��Ϊscanf�����������¼�ʱ���ڵȴ����̵�ʱ����ȫʧЧ������ʹ��_kbhit()������������ϵ��ڼ�ʱ�Ŀ������жϼ���״̬�������Get��û��
//      2)����ˢ�´�ӡ����ͳ��ֵ��ͨ����ӡ���Ʒ��Ŀ��ƣ�����ʼ�ձ�����ͬһ�д�ӡ��Get��
//���룺ʱ�䵽�˾ʹ�����ֻ��ͨ��ȫ�ֱ�����������
//���������Ǹ�����Ŭ���ɻ����ʵ����
pair<U8*, int> framing1(U8* buf, int len)
{
	int i = 0, count = 0, j = 0, iSndRetval = 0;
	U8* bufSend2 = NULL;
	U8* bufSend = NULL;  //�ӿ�0��ģʽΪbyte���飬��ת����bit���飬�ŵ�bufSend��
	bufSend = (char*)malloc(len * 8);
	if (bufSend == NULL) {
		return { NULL,-1 };
	}
	iSndRetval = ByteArrayToBitArray(bufSend, len * 8, buf, len);
	//printtest(bufSend, iSndRetval);
	//��֡����б任������5����1��ʱ������һ����0��
	bufSend2 = (char*)malloc(len * 8 * 2 + 16);
	i = 0;
	while (i < len * 8)
	{
		if (i + 4 < len * 8)
		{
			if (bufSend[i] == 1 && bufSend[i + 1] == 1 && bufSend[i + 2] == 1 && bufSend[i + 3] == 1 && bufSend[i + 4] == 1)
			{
				bufSend2[i + 5 + 8 + count] = 0;
				count++;
				i = i + 5;
			}
			else
				i++;
		}
		else
			i++;
	}
	i = 0;
	while (i < len * 8)
	{
		if (bufSend2[j + 8] != 0 && bufSend2[j + 8] != 1)
		{
			bufSend2[j + 8] = bufSend[i];
			i++;
		}
		j++;
	}
	int len2 = j;  //len2��������任�����Ч֡�ĳ���
	//��������λ����β���������������任���bufSend2����β��ӱ�־����01111110��
	bufSend2[0] = 0;
	for (i = 1; i <= 6; i++)
		bufSend2[i] = 1;
	bufSend2[7] = 0;
	bufSend2[len2 + 16 - 1] = 0;
	for (i = len2 + 16 - 2; i >= len2 + 16 - 7; i--)
		bufSend2[i] = 1;
	bufSend2[len2 + 16 - 8] = 0;

	return { bufSend2,len2 + 16 };
}
void addarp(int desip)
{
	//cout << "���룺" << desip<<"\n";
	string s;
	/*U8* buf1 = (char*)malloc(8);
	*(buf1) = stoi(strDevID);
	*(buf1 + 1) = 0;
	*(buf1 + 2) = 0;//����ack
	*(buf1 + 3) = frame_num;
	*(buf1 + 4) = 5;
	*(buf1 + 5) = desip;
	unsigned short crc;
	crc = do_crc(0xffff, buf1, 6);
	crc ^= 0xffff;//ȡ��
	*(buf1 + 6) = crc & 0x00ff;
	*(buf1 + 7) = crc >> 8 & 0x00ff;
	for (int i = 0; i < 8; i++)
	{
		s += char(*(buf1 + i));
	}*/
	s += (char)5;
	//cout << desip / 10 << " " << desip % 10 << " ";
	s += (char)(desip / 10);
	s += (char)(desip % 10);
	//cout << (int)s[0] << " " << (int)s[1]<<" "<< (int)s[2];
	//printtest(&s[0], s.size());
	sentmessage.push(s);
	//frame_num++;
}
void addmessage(string s1)
{
	string s;
	s += (char)stoi(strDevID);
	s += (char)arp_table[(int)s1[1]];
	s += s1;
	int len = s.size();
	unsigned short crc;
	crc = do_crc(0xffff, &s[0], len);
	crc ^= 0xffff;//ȡ��
	s += (char)crc & 0x00ff;
	s += (char)crc >> 8 & 0x00ff;
	sentmessage.push(s);
	frame_num++;
}
void TimeOut()
{

	printCount++;
	if (_kbhit()) {
		//�����ж���������˵�ģʽ
		menu();
	}

	switch_or_not = lowerNumber - 1;
	if (ack)//printCount - lastsend >= 100 )//|| )
	{
		vector<int> tem;
		for (auto i : cache_num)
		{
			cout << "һ�ػ���";
			string s = cache_string[i];
			int desip = s[2] * 10 + s[3];
			if (desip != 0 && arp_table.count(desip) == 0 && (desip / 10 == loc_ip / 10))
			{
				cout << "\n����arp";

				addarp(desip);
				//printtest(&s[0], s.size());
				sentmessage.push(s);
				tem.push_back(i);
				/*map<int, string> ::iterator iter = cache_string.find(i);
				cache_string.erase(iter);
				cache_num.erase(i);*/

			}
			else
			{
				//addmessage(s);
				sentmessage.push(s);
				tem.push_back(i);


			}

		}
		for (auto i : tem)
		{
			map<int, string> ::iterator iter = cache_string.find(i);
			cache_string.erase(iter);
			cache_num.erase(i);
		}
		tem.clear();
		if (!sentmessage.empty())
		{
			cout << "���ػ���";
			//cout << "��Ϣ����";
			string s = sentmessage.front();
			int len = s.size();
			U8* buf = (U8*)malloc(len + 6);

			*(buf) = stoi(strDevID);
			if (s[0] == 5)
			{
				*(buf + 1) = 0;
			}
			else
			{
				int desip = s[2] * 10 + s[3];
				if (desip == 0) *(buf + 1) = 0;
				else
				{
					if (s[4] == 1 && stoi(strDevID) != 5 && stoi(strDevID) != 6)
					{
						//cout << desip;
						if (subnet.count(desip))
						{
							//cout << "sendsubnet"<<subnet[s[s.size() - 1]] << "  ";
							*(buf + 1) = subnet[desip];
						}
						else *(buf + 1) = s[s.size() - 1];
						len--;
					}
					else
					{
						if (arp_table.count(desip))	*(buf + 1) = arp_table[desip];
						else
						{
							*(buf + 1) = arp_table[DefaultGateway];
							cout << "DefaultGateway" << DefaultGateway;
						}
					}

				}
			}
			*(buf + 2) = 0;
			*(buf + 3) = frame_num;
			for (int i = 4; i < len + 4; i++)
			{
				*(buf + i) = s[i - 4];
				//cout << (int)*(buf + i) << " ";
			}
			len += 6;
			unsigned short crc;
			crc = do_crc(0xffff, buf, len - 2);
			crc ^= 0xffff;//ȡ��
			*(buf + len - 2) = crc & 0x00ff;
			*(buf + len - 1) = crc >> 8 & 0x00ff;
			cout << "send: " << sentmessage.size();
			printtest(buf, len);
			pair<U8*, int> a;
			a = framing1(buf, len);
			U8* buf1 = a.first;
			int len1 = a.second;
			//cout << "send:";
			//printtest(buf1, len1);
			SendtoLower(buf1, len1, 0);
			lastsend = printCount;
			ack = 0;
		}
	}

	/*if (!ack && printCount - lastsend >= 100)
	{

		printtest(cache, cache_len);
		SendtoLower(cache, cache_len, 0);
		lastsend = printCount;
	}*/
	//Sleep(30000);
	//print_statistics();
}

//------------�����ķָ��ߣ����������ݵ��շ�,--------------------------------------------

//�ж�֡����Ҫ������ٸ���0��������ֵ���ǲ���ġ�0�������������Ǵ˺���ֻ�Ǽ���������������
//������ʾ��bufSend�ǽ��ϲ��bufת��Ϊbit��bit���飬len��buf�ֽ�����ĳ���
int judge1(U8* bufSend, int len)
{
	int i = 0, count = 0;
	while (i < len * 8)
	{
		if (i + 4 < len * 8)
		{
			if (bufSend[i] == 1 && bufSend[i + 1] == 1 && bufSend[i + 2] == 1 && bufSend[i + 3] == 1 && bufSend[i + 4] == 1)
			{
				count++;
				i = i + 6;
			}
			else
				i++;
		}
		else
			i++;
	}
	return count;
}

//�����ϲ���յ���buf������λ���������֡�����任�ĺ�������������ֵ���������֡���bit����bufSend2
//������ʾ��buf���յ����ϲ���ֽ����飬bufSend�����ϲ���ֽ�����ת��Ϊbit��bit���飬len���ֽ�����ĳ���






//***************��Ҫ��������******************************
//���ƣ�RecvfromUpper
//���ܣ�������������ʱ����ζ���յ�һ�ݸ߲��·�������
//      ��������ȫ�������滻��������Լ���
//      ���̹��ܽ���
//         1)ͨ���Ͳ�����ݸ�ʽ����lowerMode���ж�Ҫ��Ҫ������ת����bit�����鷢�ͣ�����ֻ�����Ͳ�ӿ�0��
//           ��Ϊû���κοɹ��ο��Ĳ��ԣ���������Ӧ�ø���Ŀ�ĵ�ַ�ڶ���ӿ���ѡ��ת���ġ�
//         2)�ж�iWorkMode�������ǲ�����Ҫ�����͵��������ݶ���ӡ������ʱ���ԣ���ʽ����ʱ�����齫����ȫ����ӡ��
//���룺U8 * buf,�߲㴫���������ݣ� int len�����ݳ��ȣ���λ�ֽ�
//�����

U8* add_head(U8* a, int len, int src_mac, int des_mac, int frame_num)
{
	//mac ack num inf crc crc
	U8* buf1 = (char*)malloc(len + 6);

	*(buf1) = src_mac;
	*(buf1 + 1) = des_mac;
	*(buf1 + 2) = 0;//����ack
	*(buf1 + 3) = frame_num;
	for (int i = 4; i < 4 + len; i++)
	{
		*(buf1 + i) = *(a + i - 4);
	}
	*(buf1 + 4 + len) = 0xff;
	*(buf1 + 5 + len) = 0xff;
	len += 6;
	unsigned short crc;
	crc = do_crc(0xffff, buf1, len - 2);
	crc ^= 0xffff;//ȡ��
	*(buf1 + len - 2) = crc & 0x00ff;
	*(buf1 + len - 1) = crc >> 8 & 0x00ff;
	return buf1;
}

//����:radio_mac
//����:�յ��ϲ��ָʾ����ָ���Ľӿڹ㲥�Լ���mac��ַ
int radio_mac(U8* buf, int r, int des_mac, int src_mac)
{
	U8* buf1 = NULL;
	U8* bufSend = NULL;
	int len1 = 1;
	int len2 = 0;
	int iSndRetval;
	buf1 = (char*)malloc(len1);
	if (*(buf) == '&')
	{
		r = 1;
		SN = int(*(buf + 1));
		loc_ip = SN * 10 + 9;
		cout << loc_ip;
		if (stoi(strEntity) == 2)
		{
			*(buf1) = '&';
			//cache_len = len1;
			U8* buf2 = (char*)malloc(len1 + 6);
			buf2 = add_head(buf1, len1, src_mac, des_mac, frame_num);
			len1 += 6;
			//��ʼ����CRCУ��λ
			unsigned short crc;
			crc = do_crc(0xffff, buf2, len1 - 2);
			//cout << crc << endl;
			crc ^= 0xffff;//ȡ��
			*(buf2 + len1 - 2) = crc & 0x00ff;
			*(buf2 + len1 - 1) = crc >> 8 & 0x00ff;
			bufSend = framing1(buf2, len1).first; //bufSend2���������֡���bit���飨������β������ͱ任�����Ч֡��
			len2 = framing1(buf2, len1).second;
			iSndRetval = SendtoLower(bufSend, len2, 0);
		}

		//printtest(bufSend, iSndRetval);
		/*for (int i = 0; i < len2; i++)
		{
			cache[i] = bufSend[i];
			//cout << buf[i];
		}*/
		//cache_len = len2;

	}
	return r;
}
void store(U8* buf, int len, int num)
{
	cout << "�յ��ϲ�";
	printtest(buf, len);
	string s;// (len, '\0');

	for (int i = 0; i < len; i++)
	{
		s += buf[i];
	}
	cache_num.insert(num);
	cache_string[num] = s;
	cout << "����" << num << "\n";
	//cout << "\nstore:"<<(int)s[0];

}


void RecvfromUpper(U8* buf, int len)
{

	//cout << len<<"\n";
	//printtest(buf, len);
	//printtest(buf, len);
	int des_mac = 0; int src_mac = stoi(strDevID);
	int r = radio_mac(buf, len, des_mac, src_mac);
	if (r == 1)
	{
		return;
	}
	int srcip = *(buf) * 10 + *(buf + 1);
	int desip = *(buf + 2) * 10 + *(buf + 3);
	if (*(buf + 4) == 1 && stoi(strDevID) != 5 && stoi(strDevID) != 6)//·������ת��
	{

		/*U8* buf1 = (U8*)malloc(len + 6);
		*(buf1) = stoi(strDevID);
		if(desip/100!= stoi(strDevID)) *(buf1 + 1) = *(buf+len-1);
		else
		{
			//cout << "send to subent:";
			//printtest(buf1, len);
			*(buf1 + 1) = subnet[desip];
		}

		*(buf1 + 2) = 0;
		*(buf1 + 3) = frame_num;
		if (desip / 100 != stoi(strDevID)) len--;
		for (int i = 4; i < len + 4; i++)
		{
			*(buf1 + i) = *(buf + i - 4);
		}
		len += 6;
		unsigned short crc;
		crc = do_crc(0xffff, buf1, len - 2);
		crc ^= 0xffff;//ȡ��
		*(buf1 + len - 2) = crc & 0x00ff;
		*(buf1 + len - 1) = crc >> 8 & 0x00ff;
		pair<U8*, int> a;
		cout << "send to subent:";
		printtest(buf1, len);
		a = framing1(buf1, len);
		buf1 = a.first;
		int len1 = a.second;

		SendtoLower(buf1, len1, 0);
		//SendtoLower(buf, len, 0);*/
		string s;
		for (int i = 0; i < len; i++)
		{
			s += *(buf + i);
		}
		sentmessage.push(s);
		return;
	}
	if (desip == 0)
	{
		U8* buf1 = (U8*)malloc(len + 6);
		*(buf1) = stoi(strDevID);
		*(buf1 + 1) = 0;
		*(buf1 + 2) = 0;
		*(buf1 + 3) = frame_num;
		for (int i = 4; i < len + 4; i++)
		{
			*(buf1 + i) = *(buf + i - 4);
		}
		len += 6;
		unsigned short crc;
		crc = do_crc(0xffff, buf1, len - 2);
		crc ^= 0xffff;//ȡ��
		*(buf1 + len - 2) = crc & 0x00ff;
		*(buf1 + len - 1) = crc >> 8 & 0x00ff;
		pair<U8*, int> a;
		a = framing1(buf1, len);
		buf1 = a.first;
		int len1 = a.second;
		SendtoLower(buf1, len1, 0);
	}
	else store(buf, len, insertnum++);
	/*frame_num++;


	int iSndRetval, len2 = 0, count = 0, r = 0;
	U8* bufSend2 = NULL;*/

	//cache_len = len;
	/*frame_num = 1;
	U8* buf1 = (char*)malloc(len + 6);
	//cout << <<"\n"<<len + 6<<"\n";
	buf1 = add_head(buf, len, src_mac, des_mac, frame_num);
	len += 6;
	//printtest(buf1, len);
	//�������
	bufSend2 = framing1(buf1, len).first; //bufSend2���������֡���bit���飨������β������ͱ任�����Ч֡��
	len2 = framing1(buf1, len).second;

	ack = 0;
	lastsend = printCount;*/
	//ͳ��


}

//�ж��Ƿ���ֶ����01111110�����ֵĻ�����0�����򷵻�-1
int judge2(U8* buf, int start, int end)
{
	int i = start + 1, count = 0;
	if (buf[start] == 0 && buf[end] == 0)
	{
		for (; i < end; i++)
		{
			if (buf[i] == 1)
				count++;
		}
		if (count == 6)
			return 0;
	}
	else
		return -1;
}

//���ײ㷢��������������ȡ����Ч֡�󷵻���Ч֡�ͳ���
pair<U8*, int> framing2(U8* buf, int len, int ifNo)
{
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	int i = 0, start = 0, end = 0, j = 0, count = 0, len2 = 0, iSndRetval = 0;
	if (lowerMode[ifNo] == 0)
	{
		//�Ƚ���Ч֡��ȡ����������֡�в���ġ�0��)
		bufSend = (U8*)malloc(len - 16);  //�����ȡ�����Ч֡��bit����
		for (i = 0; i < len; i++)
		{
			if (judge2(buf, i, i + 7) == 0)
			{
				start = i + 8;
				break;
			}
		}
		for (i = start; i < len; i++)
		{
			if (judge2(buf, i, i + 7) == 0)
			{
				end = i - 1;
				break;
			}
		}
		if (start == 0 || end == 0) return { NULL,-1 };
		//������Ч֡�н���֡�в���ġ�0��ȥ��
		i = start;
		j = 0;
		while (i <= end)
		{
			if (buf[i] == 1 && buf[i + 1] == 1 && buf[i + 2] == 1 && buf[i + 3] == 1 && buf[i + 4] == 1)
			{
				bufSend[j] = buf[i]; bufSend[j + 1] = buf[i + 1]; bufSend[j + 2] = buf[i + 2];
				bufSend[j + 3] = buf[i + 3]; bufSend[j + 4] = buf[i + 4];
				i = i + 6;
				j = j + 5;
				count++;//��¼���任��ȥ�˶��ٴ�0
			}
			else
			{
				bufSend[j] = buf[i];
				i++;
				j++;
			}
		}
		len2 = end - start + 1 - count;  //len��Ϊ��Ч֡�ĳ��ȣ���λbit
		return { bufSend,len2 };
	}
	else
	{
		//�Ͳ����ֽ�����ӿڣ�����Ҫ�ѵײ㴫�����ֽ�ת����bit���飬������ȡ��Ч֡����תΪ�ֽ����飬���ϴ����߲�
		bufSend = (U8*)malloc(len * 8);  //��Ž��ײ㴫�����ֽ�ת����bit��bit����
		if (bufSend == NULL) {
			cout << "�ڴ�ռ䲻������������û�б�����" << endl;
			return { NULL,-1 };
		}
		//byte to bit
		iSndRetval = ByteArrayToBitArray(bufSend, len * 8, buf, len);

		bufSend2 = (U8*)malloc(len * 8 - 16);  //�����Ч֡��bit����
		//�Ƚ���Ч֡��ȡ����������֡ʱ����ġ�0����
		for (i = 0; i < len * 8; i++)
		{
			if (judge2(bufSend, i, i + 7) == 0)
			{
				start = i + 8;
				break;
			}
		}
		for (i = start; i < len * 8; i++)
		{
			if (judge2(bufSend, i, i + 7) == 0)
			{
				end = i - 1;
				break;
			}
		}
		if (start == 0 || end == 0)
		{
			return { NULL,-1 };
		}
		//�ٽ���֡ʱ����ġ�0��ȥ��
		i = start;
		j = 0;
		while (i <= end)
		{
			if (bufSend[i] == 1 && bufSend[i + 1] == 1 && bufSend[i + 2] == 1 && bufSend[i + 3] == 1 && bufSend[i + 4] == 1)
			{
				bufSend2[j] = bufSend[i]; bufSend2[j + 1] = bufSend[i + 1]; bufSend2[j + 2] = bufSend[i + 2];
				bufSend2[j + 3] = bufSend[i + 3]; bufSend2[j + 4] = bufSend[i + 4];
				i = i + 6;
				j = j + 5;
				count++;  //��¼���任��ȥ�˶��ٴ�0
			}
			else
			{
				bufSend2[j] = bufSend[i];
				i++;
				j++;
			}
		}
		len2 = end - start + 1 - count;  //��Ч֡�ĳ���
		return { bufSend2,len2 };
	}
}



void radio(U8* buf, int len)
{
	int len1;
	U8* buf1 = (char*)malloc(len * 8 + 16);
	buf1 = framing1(buf, len).first;
	len1 = framing1(buf, len).second;
	SendtoLower(buf1, len1, 0);
}
void reply(int des_mac, int frame_num, int a)
{
	//cout << "����arp";
	U8* ACK1 = (char*)malloc(7);
	U8* ACK = NULL;
	int ACK_len;
	*(ACK1) = stoi(strDevID);
	*(ACK1 + 1) = des_mac;
	*(ACK1 + 2) = 1;
	if (a == 0)
	{
		*(ACK1 + 3) = frame_num;
		*(ACK1 + 4) = 0xff;
		*(ACK1 + 5) = 0xff;
		int crc = do_crc(0xffff, ACK1, 4);
		crc ^= 0xffff;
		*(ACK1 + 4) = crc & 0x00ff;
		*(ACK1 + 5) = crc >> 8 & 0x00ff;
		//cout << "\nsend\n";
		//printtest(ACK1, 6);
		ACK = framing1(ACK1, 6).first;
		ACK_len = framing1(ACK1, 6).second;
		//printtest(ACK, ACK_len);
		SendtoLower(ACK, ACK_len, 0);
		cout << "����ack\n";
	}
	if (a == 1)
	{
		*(ACK1 + 3) = frame_num + 1;
		*(ACK1 + 4) = '&';
		*(ACK1 + 5) = 0xff;
		*(ACK1 + 6) = 0xff;
		int crc = do_crc(0xffff, ACK1, 5);
		crc ^= 0xffff;
		*(ACK1 + 5) = crc & 0x00ff;
		*(ACK1 + 6) = crc >> 8 & 0x00ff;
		//cout << "\nsend\n";
		//printtest(ACK1, 7);
		ACK = framing1(ACK1, 7).first;
		ACK_len = framing1(ACK1, 7).second;
		//printtest(ACK, ACK_len);
		SendtoLower(ACK, ACK_len, 0);
	}

}

//���ƣ�send_mac()
//���ܣ��������յ�·�����㲥��mac��ַ��Ȼ���ж������Ƿ�Ϊ��&�����Ǿͷ����Լ���mac��ַ��������ݽ����ϲ�


//���ƣ�lnk_assign_ip()
//���ܣ�ÿһ����������һ��ip��ֱ���յ�����������ack��ȷ�Ϸ������
//���룺lenΪ�յ���ACK���ֽ�����bufΪACK��portΪ����ip�Ľӿ�
int lnk_assign_ip(U8* buf, int len, int port)
{

	U8* buf1;
	int len1 = 3;
	buf1 = (char*)malloc(3);
	int len2 = len1 + 6;
	U8* buf2 = (char*)malloc(len2);
	U8* buf3 = NULL;
	int len3;
	int des_mac;
	int src_mac = stoi(strDevID);
	if (len > 6 && *(buf + 4) == '&')
	{
		int frame_num = 1;
		des_mac = *(buf);
		*(buf1) = '$';
		*(buf1 + 1) = SN;
		*(buf1 + 2) = cnt;
		//cout << "����" << *(buf1 + 1);
		subnet[SN * 10 + cnt] = *(buf);
		//cout <<"subnet"<< SN * 10 + cnt << " "<<subnet[SN * 10 + cnt];
		buf2 = add_head(buf1, len1, src_mac, des_mac, frame_num);
		unsigned short crc;
		crc = do_crc(0xffff, buf2, len2 - 2);
		crc ^= 0xffff;//ȡ��
		*(buf2 + len2 - 2) = crc & 0x00ff;
		*(buf2 + len2 - 1) = crc >> 8 & 0x00ff;
		printtest(buf2, len2);
		buf3 = framing1(buf2, len2).first;
		len3 = framing1(buf2, len2).second;
		SendtoLower(buf3, len3, port);
	}
	return 0;
}


void save_ip(U8* buf)
{
	if (*(buf + 4) == '$')
	{
		loc_ip = int(*(buf + 5));
	}
	cout << loc_ip;
	SendtoUpper(buf + 4, 2);
}
void savearp(int desmac)
{
	string s = sentmessage.front();
	if ((int)s[0] == 5)
	{
		int desip = s[1] * 10 + s[2];
		arp_table[desip] = desmac;
		cout << "����arp";
	}

	//cout << (int)s[5] << " " << desmac;
}
//***************��Ҫ��������******************************
//���ƣ�RecvfromLower
//���룺U8 * buf,�Ͳ�ݽ����������ݣ� int len�����ݳ��ȣ���λbit��int ifNo ���Ͳ�ʵ����룬�����������ĸ��Ͳ�
//�����

void RecvfromLower(U8* buf, int len, int ifNo)
{


	int iSndRetval, count = 0;
	int a = 0;  //aΪ�����ж��Ƿ��Ƿ����Լ���mac��ַ��a=1���ǣ�������
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	U8* bufSend3 = NULL;
	bufSend = (U8*)malloc(len - 16);  //�����ȡ�����Ч֡��bit����
	bufSend = framing2(buf, len, ifNo).first;
	int len2 = framing2(buf, len, ifNo).second;
	if (bufSend == NULL)
	{
		cout << "\n";
		cout << "no frame\n";
		return;
	}
	bufSend2 = (U8*)malloc(len2 / 8 + 1);   //�����Ч֡���ֽ�����
	if (bufSend2 == NULL) {
		cout << "�ڴ�ռ䲻������������û�б�����" << endl;
		return;
	}
	iSndRetval = BitArrayToByteArray(bufSend, len2, bufSend2, len2 / 8 + 1);
	//cout << "\naccept\n";
	/*cout << "\n�յ�\n";
	printtest(bufSend2, iSndRetval);*/
	//if (*(bufSend2) == 5) printtest(bufSend2, iSndRetval);
	int src_mac = (int)*(bufSend2);
	int des_mac = (int)*(bufSend2 + 1);
	int local_mac = stoi(strDevID);

	unsigned short crc;
	bool checkFlag;
	crc = do_crc(0xffff, bufSend2, iSndRetval);
	//cout << "���շ�У��" << endl;
	//cout << "crc=" << crc << "\n" << endl;
	if (crc == 0xf0b8)
	{
		checkFlag = true;//crc�����Ϊf0b8�����Դ�������Ŵ���û����
		cout << "\nУ��������" << endl;
	}
	else
	{
		checkFlag = false;//����д�
		cout << "\ncrcУ����Ϊ�д�\n" << endl;
		return;
	}

	if (switch_or_not == 1)
	{
		mac_table[src_mac] = ifNo;//ÿ�ν�����Ϣʱ����Դ��¼��mac��ַ��
		if (mac_table.count(des_mac))
		{

			SendtoLower(buf, len, mac_table[des_mac]);
		}
		else
		{
			radio(bufSend2, iSndRetval);
		}
	}
	else {
		if (des_mac != 0 && des_mac != local_mac)
			return;
		/*cout << "�յ��²�";
		printtest(bufSend2, iSndRetval);*/
		if (*(bufSend2 + 2) == 0) //����ACK
		{
			//if (*(bufSend2) == 5) printtest(bufSend2, iSndRetval);
			if (des_mac == 0)
			{
				if (*(bufSend2 + 4) == '&')
				{
					//arp_table[]
					reply(src_mac, *(bufSend2 + 3), 1);
					return;
				}
				else if (*(bufSend2 + 4) == 5)
				{
					int desip = *(bufSend2 + 5) * 10 + *(bufSend2 + 6);
					if (desip == loc_ip)
					{
						cout << "return arp";
						reply(src_mac, *(bufSend2 + 3), 0);
					}
					return;
				}
				else SendtoUpper(bufSend2 + 4, iSndRetval - 6);
			}
			else
			{
				if (*(bufSend2 + 4) == '$')
				{
					loc_ip = *(bufSend2 + 5) * 10 + *(bufSend2 + 6);
					cout << "locip:" << loc_ip;
					DefaultGateway = loc_ip / 10 + 9;
					arp_table[DefaultGateway] = *(bufSend2);
					reply(src_mac, *(bufSend2 + 3), 0);
					SendtoUpper(bufSend2 + 4, iSndRetval - 6);
					return;
				}
				//printtest(bufSend2, iSndRetval);
				if (recmessage[0].empty()) recmessage[0].push(1);
				if ((int)*(bufSend2 + 3) != recmessage[0].front()) return;
				else
				{
					cout << "�յ���ACK��";
					printtest(bufSend2, iSndRetval);
					recmessage[0].pop();
					recmessage[0].push(((int)*(bufSend2 + 3) + 1) % 101);
					//else recmessage[0].push(0);
					reply(src_mac, *(bufSend2 + 3), 0);
					SendtoUpper(bufSend2 + 4, iSndRetval - 6);
				}



			}
			/*if (*(bufSend2 + 4) == '$')
			{

				//cout << loc_ip;
			}*/

		}
		else //��ACK
		{
			//����ip��ַ
			//cout << "\n�յ�ack\n";
			//printtest(bufSend2, iSndRetval);
			cout << "�յ��²�ACK " << sentmessage.size();
			printtest(bufSend2, iSndRetval);
			int ack_num = (int)*(bufSend2 + 3);
			if (*(bufSend2 + 4) == '&' && iSndRetval > 6)
			{
				lnk_assign_ip(bufSend2, iSndRetval, 0);
				cnt++;
			}
			else
			{
				if (!sentmessage.empty())
				{
					//string s = sentmessage.front();

					if (ack_num != frame_num)
					{
						cout << ack_num << " " << frame_num << "������";
						return;
					}
					else
					{
						//
						savearp(*(bufSend2));
					}
					ack = 1;
					cout << "����\n";

					sentmessage.pop();
					frame_num = (frame_num + 1) % 101;
					cout << sentmessage.size() << endl;
					//cout<<
				}

			}



		}
	}
	//printtest(bufSend2, iSndRetval);
	//iSndRetval--;

	iSndRetval = iSndRetval * 8;//�����λ,����ͳ��
//ͳ��
	if (iSndRetval <= 0) {
		iSndErrorCount++;
	}
	else {
		iRcvToUpper += iSndRetval;
		iRcvToUpperCount++;
	}
}




void print_statistics()
{
	if (printCount % 10 == 0) {
		switch (spin) {
		case 1:
			printf("\r-");
			break;
		case 2:
			printf("\r\\");
			break;
		case 3:
			printf("\r|");
			break;
		case 4:
			printf("\r/");
			spin = 0;
			break;
		}
		cout << "��ת�� " << iRcvForward << " λ��" << iRcvForwardCount << " �Σ�" << "�ݽ� " << iRcvToUpper << " λ��" << iRcvToUpperCount << " ��," << "���� " << iSndTotal << " λ��" << iSndTotalCount << " �Σ�" << "���Ͳ��ɹ� " << iSndErrorCount << " ��,""�յ�������Դ " << iRcvUnknownCount << " �Ρ�";
		spin++;
	}

}
void menu()
{
	int selection;
	unsigned short port;
	int iSndRetval;
	char kbBuf[100];
	int len;
	U8* bufSend;
	//����|��ӡ��[���Ϳ��ƣ�0���ȴ��������룻1���Զ���][��ӡ���ƣ�0�������ڴ�ӡͳ����Ϣ��1����bit����ӡ���ݣ�2���ֽ�����ӡ����]
	cout << endl << endl << "�豸��:" << strDevID << ",    ���:" << strLayer << ",    ʵ���:" << strEntity;
	cout << endl << "1-�����Զ�����(��Ч);" << endl << "2-ֹͣ�Զ����ͣ���Ч��; " << endl << "3-�Ӽ������뷢��; ";
	cout << endl << "4-����ӡͳ����Ϣ; " << endl << "5-����������ӡ��������;" << endl << "6-���ֽ�����ӡ��������;";
	cout << endl << "0-ȡ��" << endl << "����������ѡ�����";
	cin >> selection;
	switch (selection) {
	case 0:

		break;
	case 1:
		iWorkMode = 10 + iWorkMode % 10;
		break;
	case 2:
		iWorkMode = iWorkMode % 10;
		break;
	case 3:
		cout << "�����ַ���(,������100�ַ�)��";
		cin >> kbBuf;
		cout << "����Ͳ�ӿںţ�";
		cin >> port;

		len = (int)strlen(kbBuf) + 1; //�ַ�������и�������
		if (port >= lowerNumber) {
			cout << "û������ӿ�" << endl;
			return;
		}
		if (lowerMode[port] == 0) {
			//�²�ӿ��Ǳ���������,��ҪһƬ�µĻ�����ת����ʽ
			bufSend = (U8*)malloc(len * 8);

			iSndRetval = ByteArrayToBitArray(bufSend, len * 8, kbBuf, len);
			iSndRetval = SendtoLower(bufSend, iSndRetval, port);
			free(bufSend);
		}
		else {
			//�²�ӿ����ֽ����飬ֱ�ӷ���
			iSndRetval = SendtoLower(kbBuf, len, port);
			iSndRetval = iSndRetval * 8; //�����λ
		}
		//����ͳ��
		if (iSndRetval > 0) {
			iSndTotalCount++;
			iSndTotal += iSndRetval;
		}
		else {
			iSndErrorCount++;
		}
		//��Ҫ��Ҫ��ӡ����
		cout << endl << "��ӿ� " << port << " �������ݣ�" << endl;
		switch (iWorkMode % 10) {
		case 1:
			print_data_bit(kbBuf, len, 1);
			break;
		case 2:
			print_data_byte(kbBuf, len, 1);
			break;
		case 0:
			break;
		}
		break;
	case 4:
		iWorkMode = (iWorkMode / 10) * 10 + 0;
		break;
	case 5:
		iWorkMode = (iWorkMode / 10) * 10 + 1;
		break;
	case 6:
		iWorkMode = (iWorkMode / 10) * 10 + 2;
		break;
	}

}