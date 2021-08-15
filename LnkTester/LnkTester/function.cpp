//Nettester 的功能文件
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


//CRC16校验表
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

//以下为重要的变量
U8* sendbuf;        //用来组织发送数据的缓存，大小为MAX_BUFFER_SIZE,可以在这个基础上扩充设计，形成适合的结构，例程中没有使用，只是提醒一下
int printCount = 0; //打印控制
int spin = 0;  //打印动态信息控制

//------华丽的分割线，一些统计用的全局变量------------
int iSndTotal = 0;  //发送数据总量
int iSndTotalCount = 0; //发送数据总次数
int iSndErrorCount = 0;  //发送错误次数
int iRcvForward = 0;     //转发数据总量
int iRcvForwardCount = 0; //转发数据总次数
int iRcvToUpper = 0;      //从低层递交高层数据总量
int iRcvToUpperCount = 0;  //从低层递交高层数据总次数
int iRcvUnknownCount = 0;  //收到不明来源数据总次数
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
//bool ip_assign_ack[20];//ip分配确认
int SN = 0; //该接口下的子网的子网序号
int cnt = 2; //主机号
//打印统计信息
void print_statistics();
void menu();
//***************重要函数提醒******************************
//名称：InitFunction
//功能：初始化功能面，由main函数在读完配置文件，正式进入驱动机制前调用
//输入：
//输出：
void printtest(U8* a, int len)
{
	cout << "\n";
	for (int i = 0; i < len; i++)
	{
		cout << (int)*(a + i) << " ";
	}
	cout << "\n";
}
//查表法进行CRC校验，使用CRC16_CCITT标准
//输入参数：第一个为定值0Xffff，第二个为校验数据，第三个为数据长度
//返回值：如果是放送方进行为算出校验位，接受方进行为进行校验。
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
		cout << "内存不够" << endl;
		//这个，计算机也太，退出吧
		exit(0);
	}
	return;
}
//***************重要函数提醒******************************
//名称：EndFunction
//功能：结束功能面，由main函数在收到exit命令，整个程序退出前调用
//输入：
//输出：
void EndFunction()
{
	if (sendbuf != NULL)
		free(sendbuf);
	return;
}
//***************重要函数提醒******************************
//名称：TimeOut
//功能：本函数被调用时，意味着sBasicTimer中设置的超时时间到了，
//      函数内容可以全部替换为设计者自己的想法
//      例程中实现了几个同时进行功能，供参考
//      1)根据iWorkMode工作模式，判断是否将键盘输入的数据发送，
//        因为scanf会阻塞，导致计时器在等待键盘的时候完全失效，所以使用_kbhit()无阻塞、不间断地在计时的控制下判断键盘状态，这个点Get到没？
//      2)不断刷新打印各种统计值，通过打印控制符的控制，可以始终保持在同一行打印，Get？
//输入：时间到了就触发，只能通过全局变量供给输入
//输出：这就是个不断努力干活的老实孩子
pair<U8*, int> framing1(U8* buf, int len)
{
	int i = 0, count = 0, j = 0, iSndRetval = 0;
	U8* bufSend2 = NULL;
	U8* bufSend = NULL;  //接口0的模式为byte数组，先转换成bit数组，放到bufSend里
	bufSend = (char*)malloc(len * 8);
	if (bufSend == NULL) {
		return { NULL,-1 };
	}
	iSndRetval = ByteArrayToBitArray(bufSend, len * 8, buf, len);
	//printtest(bufSend, iSndRetval);
	//在帧里进行变换，出现5个‘1’时，插入一个‘0’
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
	int len2 = j;  //len2就是做完变换后的有效帧的长度
	//运用面向位的首尾定界符法，将做完变换后的bufSend2的首尾添加标志符‘01111110’
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
	//cout << "存入：" << desip<<"\n";
	string s;
	/*U8* buf1 = (char*)malloc(8);
	*(buf1) = stoi(strDevID);
	*(buf1 + 1) = 0;
	*(buf1 + 2) = 0;//不是ack
	*(buf1 + 3) = frame_num;
	*(buf1 + 4) = 5;
	*(buf1 + 5) = desip;
	unsigned short crc;
	crc = do_crc(0xffff, buf1, 6);
	crc ^= 0xffff;//取反
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
	crc ^= 0xffff;//取反
	s += (char)crc & 0x00ff;
	s += (char)crc >> 8 & 0x00ff;
	sentmessage.push(s);
	frame_num++;
}
void TimeOut()
{

	printCount++;
	if (_kbhit()) {
		//键盘有动作，进入菜单模式
		menu();
	}

	switch_or_not = lowerNumber - 1;
	if (ack)//printCount - lastsend >= 100 )//|| )
	{
		vector<int> tem;
		for (auto i : cache_num)
		{
			cout << "一重缓存";
			string s = cache_string[i];
			int desip = s[2] * 10 + s[3];
			if (desip != 0 && arp_table.count(desip) == 0 && (desip / 10 == loc_ip / 10))
			{
				cout << "\n发送arp";

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
			cout << "二重缓存";
			//cout << "消息队列";
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
			crc ^= 0xffff;//取反
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

//------------华丽的分割线，以下是数据的收发,--------------------------------------------

//判断帧中需要插入多少个‘0’，返回值就是插入的‘0’的数量，但是此函数只是计数，并不做处理
//参数提示：bufSend是将上层的buf转换为bit的bit数组，len是buf字节数组的长度
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

//将从上层接收到的buf加上首位定界符并在帧中做变换的函数，函数返回值就是做完成帧后的bit数组bufSend2
//参数提示：buf：收到的上层的字节数组，bufSend：将上层的字节数组转变为bit的bit数组，len：字节数组的长度






//***************重要函数提醒******************************
//名称：RecvfromUpper
//功能：本函数被调用时，意味着收到一份高层下发的数据
//      函数内容全部可以替换成设计者自己的
//      例程功能介绍
//         1)通过低层的数据格式参数lowerMode，判断要不要将数据转换成bit流数组发送，发送只发给低层接口0，
//           因为没有任何可供参考的策略，讲道理是应该根据目的地址在多个接口中选择转发的。
//         2)判断iWorkMode，看看是不是需要将发送的数据内容都打印，调试时可以，正式运行时不建议将内容全部打印。
//输入：U8 * buf,高层传进来的数据， int len，数据长度，单位字节
//输出：

U8* add_head(U8* a, int len, int src_mac, int des_mac, int frame_num)
{
	//mac ack num inf crc crc
	U8* buf1 = (char*)malloc(len + 6);

	*(buf1) = src_mac;
	*(buf1 + 1) = des_mac;
	*(buf1 + 2) = 0;//不是ack
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
	crc ^= 0xffff;//取反
	*(buf1 + len - 2) = crc & 0x00ff;
	*(buf1 + len - 1) = crc >> 8 & 0x00ff;
	return buf1;
}

//名称:radio_mac
//功能:收到上层的指示后，向指定的接口广播自己的mac地址
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
			//开始计算CRC校验位
			unsigned short crc;
			crc = do_crc(0xffff, buf2, len1 - 2);
			//cout << crc << endl;
			crc ^= 0xffff;//取反
			*(buf2 + len1 - 2) = crc & 0x00ff;
			*(buf2 + len1 - 1) = crc >> 8 & 0x00ff;
			bufSend = framing1(buf2, len1).first; //bufSend2就是做完成帧后的bit数组（包含首尾定界符和变换后的有效帧）
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
	cout << "收到上层";
	printtest(buf, len);
	string s;// (len, '\0');

	for (int i = 0; i < len; i++)
	{
		s += buf[i];
	}
	cache_num.insert(num);
	cache_string[num] = s;
	cout << "插入" << num << "\n";
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
	if (*(buf + 4) == 1 && stoi(strDevID) != 5 && stoi(strDevID) != 6)//路由器的转发
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
		crc ^= 0xffff;//取反
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
		crc ^= 0xffff;//取反
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
	//加入完毕
	bufSend2 = framing1(buf1, len).first; //bufSend2就是做完成帧后的bit数组（包含首尾定界符和变换后的有效帧）
	len2 = framing1(buf1, len).second;

	ack = 0;
	lastsend = printCount;*/
	//统计


}

//判断是否出现定界符01111110，出现的话返回0，否则返回-1
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

//将底层发送上来的数据提取出有效帧后返回有效帧和长度
pair<U8*, int> framing2(U8* buf, int len, int ifNo)
{
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	int i = 0, start = 0, end = 0, j = 0, count = 0, len2 = 0, iSndRetval = 0;
	if (lowerMode[ifNo] == 0)
	{
		//先将有效帧提取出来（含成帧中插入的‘0’)
		bufSend = (U8*)malloc(len - 16);  //存放提取后的有效帧的bit数组
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
		//再在有效帧中将成帧中插入的‘0’去掉
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
				count++;//记录反变换消去了多少次0
			}
			else
			{
				bufSend[j] = buf[i];
				i++;
				j++;
			}
		}
		len2 = end - start + 1 - count;  //len变为有效帧的长度，单位bit
		return { bufSend,len2 };
	}
	else
	{
		//低层是字节数组接口，先需要把底层传来的字节转换成bit数组，做完提取有效帧后再转为字节数组，再上传给高层
		bufSend = (U8*)malloc(len * 8);  //存放将底层传来的字节转换成bit的bit数组
		if (bufSend == NULL) {
			cout << "内存空间不够，导致数据没有被处理" << endl;
			return { NULL,-1 };
		}
		//byte to bit
		iSndRetval = ByteArrayToBitArray(bufSend, len * 8, buf, len);

		bufSend2 = (U8*)malloc(len * 8 - 16);  //存放有效帧的bit数组
		//先将有效帧提取出来（含成帧时插入的‘0’）
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
		//再将成帧时插入的‘0’去掉
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
				count++;  //记录反变换消去了多少次0
			}
			else
			{
				bufSend2[j] = bufSend[i];
				i++;
				j++;
			}
		}
		len2 = end - start + 1 - count;  //有效帧的长度
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
	//cout << "返回arp";
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
		cout << "返回ack\n";
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

//名称：send_mac()
//功能：主机在收到路由器广播的mac地址，然后判断内容是否为‘&’，是就发送自己的mac地址，不是则递交给上层


//名称：lnk_assign_ip()
//功能：每一个主机分配一个ip，直到收到所有主机的ack，确认分配完毕
//输入：len为收到的ACK的字节数，buf为ACK，port为分配ip的接口
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
		//cout << "分配" << *(buf1 + 1);
		subnet[SN * 10 + cnt] = *(buf);
		//cout <<"subnet"<< SN * 10 + cnt << " "<<subnet[SN * 10 + cnt];
		buf2 = add_head(buf1, len1, src_mac, des_mac, frame_num);
		unsigned short crc;
		crc = do_crc(0xffff, buf2, len2 - 2);
		crc ^= 0xffff;//取反
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
		cout << "存入arp";
	}

	//cout << (int)s[5] << " " << desmac;
}
//***************重要函数提醒******************************
//名称：RecvfromLower
//输入：U8 * buf,低层递交上来的数据， int len，数据长度，单位bit，int ifNo ，低层实体号码，用来区分是哪个低层
//输出：

void RecvfromLower(U8* buf, int len, int ifNo)
{


	int iSndRetval, count = 0;
	int a = 0;  //a为主机判断是否是发送自己的mac地址，a=1则是，否则不是
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	U8* bufSend3 = NULL;
	bufSend = (U8*)malloc(len - 16);  //存放提取后的有效帧的bit数组
	bufSend = framing2(buf, len, ifNo).first;
	int len2 = framing2(buf, len, ifNo).second;
	if (bufSend == NULL)
	{
		cout << "\n";
		cout << "no frame\n";
		return;
	}
	bufSend2 = (U8*)malloc(len2 / 8 + 1);   //存放有效帧的字节数组
	if (bufSend2 == NULL) {
		cout << "内存空间不够，导致数据没有被处理" << endl;
		return;
	}
	iSndRetval = BitArrayToByteArray(bufSend, len2, bufSend2, len2 / 8 + 1);
	//cout << "\naccept\n";
	/*cout << "\n收到\n";
	printtest(bufSend2, iSndRetval);*/
	//if (*(bufSend2) == 5) printtest(bufSend2, iSndRetval);
	int src_mac = (int)*(bufSend2);
	int des_mac = (int)*(bufSend2 + 1);
	int local_mac = stoi(strDevID);

	unsigned short crc;
	bool checkFlag;
	crc = do_crc(0xffff, bufSend2, iSndRetval);
	//cout << "接收方校验" << endl;
	//cout << "crc=" << crc << "\n" << endl;
	if (crc == 0xf0b8)
	{
		checkFlag = true;//crc算出来为f0b8，可以大概率相信传输没出错
		cout << "\n校验结果正常" << endl;
	}
	else
	{
		checkFlag = false;//检出有错
		cout << "\ncrc校验结果为有错\n" << endl;
		return;
	}

	if (switch_or_not == 1)
	{
		mac_table[src_mac] = ifNo;//每次接受信息时将来源记录在mac地址表
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
		/*cout << "收到下层";
		printtest(bufSend2, iSndRetval);*/
		if (*(bufSend2 + 2) == 0) //不是ACK
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
					cout << "收到非ACK：";
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
		else //是ACK
		{
			//分配ip地址
			//cout << "\n收到ack\n";
			//printtest(bufSend2, iSndRetval);
			cout << "收到下层ACK " << sentmessage.size();
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
						cout << ack_num << " " << frame_num << "不符合";
						return;
					}
					else
					{
						//
						savearp(*(bufSend2));
					}
					ack = 1;
					cout << "弹出\n";

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

	iSndRetval = iSndRetval * 8;//换算成位,进行统计
//统计
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
		cout << "共转发 " << iRcvForward << " 位，" << iRcvForwardCount << " 次，" << "递交 " << iRcvToUpper << " 位，" << iRcvToUpperCount << " 次," << "发送 " << iSndTotal << " 位，" << iSndTotalCount << " 次，" << "发送不成功 " << iSndErrorCount << " 次,""收到不明来源 " << iRcvUnknownCount << " 次。";
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
	//发送|打印：[发送控制（0，等待键盘输入；1，自动）][打印控制（0，仅定期打印统计信息；1，按bit流打印数据，2按字节流打印数据]
	cout << endl << endl << "设备号:" << strDevID << ",    层次:" << strLayer << ",    实体号:" << strEntity;
	cout << endl << "1-启动自动发送(无效);" << endl << "2-停止自动发送（无效）; " << endl << "3-从键盘输入发送; ";
	cout << endl << "4-仅打印统计信息; " << endl << "5-按比特流打印数据内容;" << endl << "6-按字节流打印数据内容;";
	cout << endl << "0-取消" << endl << "请输入数字选择命令：";
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
		cout << "输入字符串(,不超过100字符)：";
		cin >> kbBuf;
		cout << "输入低层接口号：";
		cin >> port;

		len = (int)strlen(kbBuf) + 1; //字符串最后有个结束符
		if (port >= lowerNumber) {
			cout << "没有这个接口" << endl;
			return;
		}
		if (lowerMode[port] == 0) {
			//下层接口是比特流数组,需要一片新的缓冲来转换格式
			bufSend = (U8*)malloc(len * 8);

			iSndRetval = ByteArrayToBitArray(bufSend, len * 8, kbBuf, len);
			iSndRetval = SendtoLower(bufSend, iSndRetval, port);
			free(bufSend);
		}
		else {
			//下层接口是字节数组，直接发送
			iSndRetval = SendtoLower(kbBuf, len, port);
			iSndRetval = iSndRetval * 8; //换算成位
		}
		//发送统计
		if (iSndRetval > 0) {
			iSndTotalCount++;
			iSndTotal += iSndRetval;
		}
		else {
			iSndErrorCount++;
		}
		//看要不要打印数据
		cout << endl << "向接口 " << port << " 发送数据：" << endl;
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