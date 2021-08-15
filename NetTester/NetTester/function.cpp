//Nettester �Ĺ����ļ�
#include <iostream>
#include <conio.h>
#include "winsock.h"
#include "stdio.h"
#include "CfgFileParms.h"
#include "function.h"
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
#include <windows.h>
using namespace std;
const int INF = 1000;
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
int ip;
extern map<int, int> nextnode;//��һ���ڵ�
extern map<int, int> lower_interface;//�����˿�
extern map<int, int> cost;//��Ŀ���ķ���
int init_count = 0;
int lastsend = 0x3f3f3f3f;
int router_or_not = 0;
bool ifipinit=0;
int locip = 0;
//��ӡͳ����Ϣ
void print_statistics();
void menu();
//***************��Ҫ��������******************************
//���ƣ�InitFunction
//���ܣ���ʼ�������棬��main�����ڶ��������ļ�����ʽ������������ǰ����
//���룺
//�����
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
void printtest(U8* a, int len)
{
	cout << "\n";
	for (int i = 0; i < len; i++)
	{
		cout << (int)*(a + i) << " ";
	}
	cout << "\n";
}
void radio(U8* buf, int len)
{
	//printtest(buf, len);
	for (int i = 0; i < 2; i++)
	{

		SendtoLower(buf, len, i);
	}

}

void announce()
{
	int des_ip = 0;
	int len = cost.size();//len��������·�ɱ��е�����
	U8* tem = (U8*)malloc(len * 2 + 6);
	*(tem) = stoi(strDevID)*10;
	*(tem + 1) =0;//Դip
	
	*(tem + 2) = 0;
	*(tem + 3) = 0;//Ŀ��ip
	*(tem + 4) = 0;//��һλ0����������Ǿ���ʸ���㷨���ݵ�����
	*(tem + 5) = len;//���洫��·����Ŀ����
	int cnt = 6;
	for (auto& i : cost)
	{
		*(tem + cnt) = i.first;
		*(tem + cnt + 1) = i.second;
		cnt += 2;
	}
	radio(tem, len * 2 + 6);//�㲥
	//SendtoLower(tem, len * 2 + 4, 0);
	lastsend = printCount;
	init_count++;
}

//���ƣ�net_assign_ip
//����:·��������������·���·���ָ���&+�������
//����:SN:Serial Number,�������
void net_assign_ip(int SN, unsigned short port)
{
	int len = 2;
	int iSndRetval;
	U8* buf = NULL;
	U8* bufSend = NULL;
	buf = (U8*)malloc(len);  //������š�&+������š�
	*(buf) = '&';
	*(buf + 1) = char(SN);


	//�²�ӿ����ֽ����飬ֱ�ӷ���
	iSndRetval = SendtoLower(buf, len, port);
	iSndRetval = iSndRetval * 8; //�����λ

}
void ip_init()
{
	U8* tem = (U8*)malloc(2);
	*(tem) = '&';
	for (int i = 0; i < lowerNumber; i++)
	{
		*(tem + 1) = stoi(strDevID)*10+i;
		SendtoLower(tem, 2,i);
	}
}
void router_init()
{
	if (!ifipinit)
	{
		ip_init();
		ifipinit = 1;
	}
	
	if (printCount - lastsend >= 10 && init_count < 5)//���������ÿ��50����λʵ��һ�Σ�������
	{
		announce();//��������ǰ��Լ���·�ɱ�㲥��ȥ
	}
	if (!init_count)//��Ϊһ��ʼlastsend���ó�Ϊ��������Ե�һ��Ҫ�ֶ�������printCount>=500����ΪҪ�ȴ�����ʵ�����ɺ��ٽ��з���
	{
		announce();//��Ϊ

	}
	if (init_count == 5 && printCount - lastsend >= 100)
	{
		cout << "\ncost:\n";
		for (auto& i : cost)
		{
			cout << i.first << ":" << i.second << " ";
		}
		cout << "\n";
		cout << "\nnextnode:\n";
		for (auto& i : nextnode)
		{
			cout << i.first << ":" << i.second << " ";
		}
		cout << "\n";
		cout << "\nlower_interface:\n";
		for (auto& i : lower_interface)
		{
			cout << i.first << ":" << i.second << " ";
		}
		cout << "\n";
		init_count++;
	}
}
U8* addhead(U8* buf, int len, int desip)
{
	U8* buf2 = (U8*)malloc(len + 5);
	*buf2 = locip/10;
	*(buf2 + 1) = locip % 10;
	*(buf2 + 2) = desip/10;
	*(buf2 + 3) = desip % 10;
	*(buf2 + 4) = 1;
	for (int i = 5; i < len + 5; i++)
	{
		*(buf2 + i) = *(buf + i - 5);
	}
	return buf2;
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
void TimeOut()
{

	printCount++;
	if (_kbhit()) {
		//�����ж���������˵�ģʽ
		menu();
	}
	router_or_not = lowerNumber - 1;
	if (router_or_not&&printCount>800) router_init();//ִ��·����������
}
void saveip(U8* buf,int len)
{
	locip =*(buf + 1)*10+ *(buf + 2);
	cout << locip << "\n";
}

//------------�����ķָ��ߣ����������ݵ��շ�,--------------------------------------------

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
void RecvfromUpper(U8* buf, int len)
{
	int desip = *(buf + len - 2) * 10 + *(buf + len - 1);
	cout << len;
	if (desip == locip) return;
	//cout << desip << locip;
	//if ((desip / 10) == (locip / 10)||desip==0)
	//{
		//cout << 1;
	U8* buf2 = addhead(buf, len - 1,desip);
	printtest(buf2, len + 3);
	SendtoLower(buf2, len + 3,0);
	//}
	
	
}
//***************��Ҫ��������******************************
//���ƣ�RecvfromLower
//���ܣ�������������ʱ����ζ�ŵõ�һ�ݴӵͲ�ʵ��ݽ�����������
//      ��������ȫ�������滻���������Ҫ������
//      ���̹��ܽ��ܣ�
//          1)����ʵ����һ���򵥴ֱ���������Ĳ��ԣ����дӽӿ�0�����������ݶ�ֱ��ת�����ӿ�1�����ӿ�1�������Ͻ����߲㣬������ô����
//          2)ת�����Ͻ�ǰ���ж��ս����ĸ�ʽ��Ҫ���ͳ�ȥ�ĸ�ʽ�Ƿ���ͬ��������bite��������ֽ�������֮��ʵ��ת��
//            ע����Щ�жϲ������������ݱ�����������������������ļ������������ļ��Ĳ���д���ˣ��ж�Ҳ�ͻ�ʧ��
//          3)����iWorkMode���ж��Ƿ���Ҫ���������ݴ�ӡ
//���룺U8 * buf,�Ͳ�ݽ����������ݣ� int len�����ݳ��ȣ���λ�ֽڣ�int ifNo ���Ͳ�ʵ����룬�����������ĸ��Ͳ�
//�����
void RecvfromLower(U8* buf, int len, int ifNo)
{
	/*cout << "\n";
	for (int i = 0; i < len; i++)
	{
		cout << (int)*(buf + i) << " ";
	}
	cout << "\n";*/
	if (*buf == '$')
	{
		saveip(buf,len);
		return;
	}
	if (!router_or_not)
	{

		printtest(buf, len);
		SendtoUpper(buf + 5, len - 5);
	}

	if(*(buf + 4) == 0 && router_or_not)
	{
		printtest(buf, len);
	int rec_router_num = *(buf + 5);
	int src_ip = *(buf)/10;
	cost[src_ip] = 1;
	nextnode[src_ip] = src_ip;
	lower_interface[src_ip] = ifNo;
	for (int i = 6; i < rec_router_num * 2 + 6; i += 2)
	{
		if (*(buf + i) == stoi(strDevID)) continue;
		if (cost.count(*(buf + i)) == 0 || cost[*(buf + i)] > 1 + *(buf + i + 1))
		{
			cost[*(buf + i)] = 1 + *(buf + i + 1);
			nextnode[*(buf + i)] = src_ip;
			lower_interface[*(buf + i)] = ifNo;

		}


	}
	return;
	}
	if (*(buf + 4) == 1 && router_or_not)
	{
		//cout << 1111;
		int srcip = *(buf) * 10 + *(buf + 1);
		int desip = *(buf + 2) * 10 + *(buf + 3);

		printtest(buf , len );
			U8* buf1 = (U8*)malloc(len + 1);
			for (int i = 0; i < len; i++)
			{
				*(buf1 + i) = *(buf + i);
			}
			
		if (desip / 100 == stoi(strDevID))
		{
			//SendtoUpper(buf + 5, len - 5);
			cout << "sendtonei:" << (desip / 10) % 10;
			*(buf1 + len) = 0;
			SendtoLower(buf1, len+1, (desip / 10) % 10);
		}
		else
		{
			cout << "sendto:" << lower_interface[desip / 100];
			*(buf1 + len) = nextnode[desip / 100];
			SendtoLower(buf1, len+1, lower_interface[desip / 100]);
		}
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