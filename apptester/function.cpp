//apptester�Ĺ����ļ�
#include <iostream>
#include <conio.h>
#include "winsock.h"
#include "stdio.h"
#include "CfgFileParms.h"
#include "function.h"
#include<graphics.h>
using namespace std;

U8* autoSendBuf;        //������֯�������ݵĻ��棬��СΪMAX_BUFFER_SIZE,���������������������ƣ��γ��ʺϵĽṹ��������û��ʹ�ã�ֻ������һ��
int printCount = 0; //��ӡ����
int spin = 1;  //��ӡ��̬��Ϣ����

//------�����ķָ��ߣ�һЩͳ���õ�ȫ�ֱ���------------
int iSndTotal = 0;  //������������
int iSndTotalCount = 0; //���������ܴ���
int iSndErrorCount = 0;  //���ʹ������
int iRcvTotal = 0;     //������������
int iRcvTotalCount = 0; //ת�������ܴ���
int iRcvUnknownCount = 0;  //�յ�������Դ�����ܴ���
int sendipostionOffset = 0;
int recipostionOffset = 0;
U8* cache = NULL;  //����ֶεĻ�����
int NUM = 0; //����ķֶεĸ���
int LEN = 100;//��LENΪ������ֶ�
bool rec_img = 0;
void print_statistics();
void menu();
//***************��Ҫ��������******************************
//���ƣ�InitFunction
//���ܣ���ʼ�������棬��main�����ڶ��������ļ�����ʽ������������ǰ����
//���룺
//�����
void InitFunction(CCfgFileParms& cfgParms)
{
	int i;
	int retval;

	retval = cfgParms.getValueInt(autoSendTime, (char*)"autoSendTime");
	if (retval == -1 || autoSendTime == 0) {
		autoSendTime = DEFAULT_AUTO_SEND_TIME;
	}
	retval = cfgParms.getValueInt(autoSendSize, (char*)"autoSendSize");
	if (retval == -1 || autoSendSize == 0) {
		autoSendSize = DEFAULT_AUTO_SEND_SIZE;
	}

	autoSendBuf = (char*)malloc(MAX_BUFFER_SIZE);
	if (autoSendBuf == NULL) {
		cout << "�ڴ治��" << endl;
		//����������Ҳ̫���˳���
		exit(0);
	}
	for (i = 0; i < MAX_BUFFER_SIZE; i++) {
		autoSendBuf[i] = 'a'; //��ʼ������ȫΪ�ַ�'a',ֻ��Ϊ�˲���
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
	if (autoSendBuf != NULL)
		free(autoSendBuf);
	return;
}
pair<U8*, int> readPhoto()
{
	std::fstream InputFile;
	InputFile.open("test.JPG", std::ios::in | std::ios::binary);
	// ����ͼƬ����
	InputFile.seekg(0, InputFile.end);
	int length = InputFile.tellg();
	InputFile.seekg(0, InputFile.beg);
	// �����ڴ滺����
	char* buffer = new char[length];
	// ��ȡͼƬ
	InputFile.read(buffer, length);
	InputFile.close();
	return{ buffer,length };
	//delete[] buffer;
}

//���ƣ�sendPhoto
//���ܣ�������������ʱ����ζ��Ҫ����ͼƬ��
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
void sendPhoto(U8* buf, int len, int ip)
{
	int iSndRetval;
	int num = 0, len1 = 0, i = 0, j = 0;
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	bufSend = (char*)malloc(LEN + 2);
	num = len / LEN; //����Ϊ100�ķֶεĸ���
	len1 = len % LEN; //�����㳤��Ϊ100�����ķֶγ���
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < LEN; j++)
		{
			bufSend2 = buf + LEN * i + j;
			*(bufSend + j) = *bufSend2;
		}
		*(bufSend + LEN) = ip / 10;
		*(bufSend + LEN + 1) = ip % 10;
		iSndRetval = SendtoLower(bufSend, LEN + 2, 0);
		Sleep(200);
		iSndRetval = iSndRetval * 8; //�����λ
		//if (i == 40) return;
	}
	U8* bufSend1 = NULL;
	bufSend1 = (char*)malloc(len1 + 2);
	for (j = 0; j < len1 - 1; j++)
	{
		bufSend2 = buf + LEN * num + j;
		*(bufSend1 + j) = *bufSend2;
	}
	*(bufSend1 + len1) = ip / 10;
	*(bufSend1 + len1 + 1) = ip / 10;
	Sleep(1000);
	iSndRetval = SendtoLower(bufSend1, len1 + 2, 0);
	iSndRetval = iSndRetval * 8; //�����λ
}

//���ƣ�send_ph_info
//���ܣ����߽��շ�Ҫ������Ƭ�˻����������
//���룺lenΪ��Ϣ�ĳ��ȣ�s_or_eΪ0ʱΪ��ʼ��Ϊ1ʱΪ����
void send_ph_info(int len, int s_or_e, int ip)
{
	int iSndRetval;
	int digit = 0;  //ͼƬ���ݳ��ȵ�λ��
	int len1;
	int i = 0;
	U8* bufSend = NULL;
	len1 = len;
	while (len1 != 0)
	{
		digit++;
		len1 = len1 / 10;
	}
	bufSend = (char*)malloc(5 + digit);
	*(bufSend) = '#';
	if (s_or_e == 0)
		*(bufSend + 1) = 's';
	else
		*(bufSend + 1) = 'e';
	*(bufSend + 2) = 'p';
	len1 = len;
	for (i = 0; len1 != 0; i++)
	{
		*(bufSend + 3 + i) = len1 % 10;
		len1 = len1 / 10;
	}
	*(bufSend + digit + 3) = ip / 10;
	*(bufSend + digit + 4) = ip % 10;
	iSndRetval = SendtoLower(bufSend, 5 + digit, 0);
	iSndRetval = iSndRetval * 8; //�����λ
}

//���ƣ�receive_ph_start_info
//���ܣ��ж���Ϣ�Ƿ��ǶԷ�Ҫ��ʼ������Ƭ��
//���룺lenΪ�յ���buf���ֽ�����bufΪ�յ�������
bool  receive_ph_start_info(U8* buf, int len)
{
	int len1 = 0;  //len1����Ƭ�ֽ�����ĳ���
	int a;
	if (*(buf) == '#' && *(buf + 1) == 's' && *(buf + 2) == 'p')
	{
		for (int i = 0; i < len - 3; i++)
		{
			int j = i;
			a = *(buf + 3 + i);
			while (j != 0)
			{
				a = a * 10;
				j--;
			}
			len1 = len1 + a;
		}
		cache = (char*)malloc(len1);  //�ж�Ϊ�ǶԷ�Ҫ���͵���Ϣ�Ǿͷ���һ������Ϊlen1��ceche
		cout << "��ʼ����\n";
		rec_img = 1;
		return 1;
	}
	return 0;
}

//���ƣ�receive_ph_end_info
//���ܣ��ж���Ϣ�Ƿ��ǶԷ�Ҫ����������Ƭ��
//���룺lenΪ�յ���buf���ֽ�����bufΪ�յ�������
bool receive_ph_end_info(U8* buf, int len)
{
	int len1 = 0, a;  //len1����Ƭ�ֽ�����ĳ���
	if (*(buf) == '#' && *(buf + 1) == 'e' && *(buf + 2) == 'p')
	{
		for (int i = 0; i < len - 3; i++)
		{
			int j = i;
			a = *(buf + 3 + i);
			while (j != 0)
			{
				a = a * 10;
				j--;
			}
			len1 = len1 + a;
		}
		cout << "��������\n";
		//������ǽ�cache�е�����ת����ͼƬ
		std::fstream OutputFile;
		std::string FileName = "result.JPG";  //ת����ɵ�ͼƬ����

		OutputFile.open(FileName.c_str(), std::ios::binary | std::ios::out);
		OutputFile.seekp(0L, ios::beg);
		OutputFile.write((char*)cache, len1);
		OutputFile.close();
		cache = NULL;
		NUM = 0;   //ת�����֮������cache��NUM
		rec_img = 0;
		IMAGE img1;
		loadimage(&img1, _T("result.JPG"),90,90);
		//outtextxy(300, 320 + 18 * ipostionOffset, point);
		putimage(300, 320 + 18 * recipostionOffset, &img1);
		recipostionOffset += 5;
		return 1;
	}
	return 0;
}
void add_buf_to_cache(U8* buf, int len)
{
	cout << "�յ���" << NUM << endl;
	int i = 0;
	for (i = 0; i < len; i++)
		*(cache + NUM * LEN + i) = *(buf + i);
	NUM++;
}
void sendphoto()
{
	int selection;
	int desip;
	int iSndRetval;
	char kbBuf[1000];
	int len;
	int length;
	U8* bufSend;

	TCHAR Buf[1000];
	TCHAR ip[10];
	InputBox(ip, 100, "������ip��ַ");
	desip = _ttoi(ip);

	printf("���Ե�Ƭ��");
	bufSend = readPhoto().first;
	len = readPhoto().second;
	send_ph_info(len, 0, desip);
	Sleep(5000);
	sendPhoto(bufSend, len, desip);
	send_ph_info(len, 1, desip);
	printf("\n�������\n");
	IMAGE img1;
	loadimage(&img1, _T("test.jpg"),80,80);
	//outtextxy(300, 320 + 18 * ipostionOffset, point);
	putimage(300, 60 + 18 * sendipostionOffset, &img1);
	sendipostionOffset += 5;
	return;

}
//***************��Ҫ��������******************************
//���ƣ�TimeOut
//���ܣ�������������ʱ����ζ��sBasicTimer�����õĳ�ʱʱ�䵽�ˣ�
//      �������ݿ���ȫ���滻Ϊ������Լ����뷨
//      ������ʵ���˼���ͬʱ���й��ܣ����ο�
//      1)����iWorkMode����ģʽ���ж��Ƿ񽫼�����������ݷ��ͣ������Զ����͡������������ʵ��Ӧ�ò��
//        ��Ϊscanf�����������¼�ʱ���ڵȴ����̵�ʱ����ȫʧЧ������ʹ��_kbhit()������������ϵ��ڼ�ʱ�Ŀ������жϼ���״̬�������Get��û��
//      2)����ˢ�´�ӡ����ͳ��ֵ��ͨ����ӡ���Ʒ��Ŀ��ƣ�����ʼ�ձ�����ͬһ�д�ӡ��Get��
//      3)�����iWorkMode����Ϊ�Զ����ͣ���ÿ����autoSendTime * DEFAULT_TIMER_INTERVAL ms����ӿ�0����һ��
//���룺ʱ�䵽�˾ʹ�����ֻ��ͨ��ȫ�ֱ�����������
//���������Ǹ�����Ŭ���ɻ����ʵ����
void TimeOut()
{

	int iSndRetval;
	int len;
	U8* bufSend;
	int i;

	printCount++;

	/*if (_kbhit()) {
		//�����ж���������˵�ģʽ
		menu();
	}*/
	//cout << "344" << endl;
	//MOUSEMSG* m = (MOUSEMSG*)malloc(1);
	MOUSEMSG m;
	if (MouseHit())
	{
		m = GetMouseMsg();
		FlushMouseMsgBuffer();
		//cout << "yuanshen";
		if (m.x >= 50 && m.x <= 240 && m.y >= 100 && m.y <= 150 && m.uMsg == WM_LBUTTONDOWN) {

			menu();
		}
		if (m.x >= 50 && m.x <= 240 && m.y >= 160 && m.y <= 210 && m.uMsg == WM_LBUTTONDOWN) {

			//menu();
			sendphoto();
		}
	}
	//free(m);
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
	//Ӧ�ò㲻���յ����߲㡱�����ݣ������Լ�����
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

	int retval;
	if (rec_img)
	{
		if (receive_ph_end_info(buf, len))
		{
			return;
		}
	}  //�ж��Ƿ��ǶԷ�Ҫ����������Ƭ����Ϣ
	//cout << rec_img;
	//if (cache == NULL) cout << "NULL";
	if (cache != NULL && rec_img)
	{
		//cout << "?";
		printtest(buf, len);
		add_buf_to_cache(buf, len);
		return;
	}
	if (!rec_img)
	{
		if (!receive_ph_start_info(buf, len))
		{
			/*RECT r = { 500, 500, 502, 500 };
			for (int i = 0; i < len; i++)
			{
				r = { 500,500 + i,502 + i,500 };
				drawtext(wchar_t(*(buf + i)), &r, DT_CENTER | DT_END_ELLIPSIS);
				cout << wchar_t(*(buf + i)) << endl;

			}*/
			char* point = (char*)malloc((len + 1));
			for (int j = 0; j < len; j++) {
				point[j] = buf[j];
			}
			point[len] = '\0';
			outtextxy(300, 320 + 18 * recipostionOffset, point);
			recipostionOffset++;
			/*wchar_t tchar[256] = { 0 };
			MultiByteToWideChar(CP_ACP, 0, buf, -1, tchar, 256 * sizeof(TCHAR));*/
			free(point);
		}
	} // �ж��Ƿ��ǶԷ�Ҫ��ʼ������Ƭ����Ϣ

	/*TCHAR* output;
	Char2TCHAR(buf, output);*/

}
//��ӡͳ����Ϣ
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
		cout << "������ " << iSndTotal << " λ," << iSndTotalCount << " ��," << "���� " << iSndErrorCount << " �δ���;";
		cout << " ������ " << iRcvTotal << " λ," << iRcvTotalCount << " ��";
		spin++;
	}
}
void menu()
{

	cout << "�Ѿ�����menu��" << endl;
	int selection;
	int desip;
	int iSndRetval;
	char kbBuf[1000];
	int len;
	int length;
	U8* bufSend;

	TCHAR Buf[1000];
	TCHAR ip[10];

	//InputBox(Buf, 10, L"�����ַ�����������100���ַ�");
	//InputBox(ip, 10, L"������IP��ַ");
	InputBox(Buf, 100, "�����ַ�����������100���ַ�");
	InputBox(ip, 100, "������ip��ַ");
	desip = _ttoi(ip);//�û�����ת��Ϊ����
	//cout << desip << endl;
	for (int i = 0; i < strlen(Buf); i++)
	{
		kbBuf[i] = char(*(Buf + i));

	}//kbBuf[i + 1] = '\0';
	length = strlen(Buf);
	*(kbBuf + length) = desip / 10;
	*(kbBuf + length + 1) = desip % 10;
	*(kbBuf + length + 2) = '\0';
	cout << (int)kbBuf[0] << (int)kbBuf[1] << (int)kbBuf[2];
	//cout << kbBuf << endl;
	len = length + 2; //�ַ�������и�������
	iSndRetval = SendtoLower(kbBuf, len, 0);
	//������չʾ����
	outtextxy(300, 60 + 18 * sendipostionOffset, Buf);
	sendipostionOffset++;
	//cout << len;
	//iSndRetval = iSndRetval * 8; //�����λ
	//����ͳ��






	//����|��ӡ��[���Ϳ��ƣ�0���ȴ��������룻1���Զ���][��ӡ���ƣ�0�������ڴ�ӡͳ����Ϣ��1����bit����ӡ���ݣ�2���ֽ�����ӡ����]


}
