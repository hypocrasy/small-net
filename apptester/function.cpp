//apptester的功能文件
#include <iostream>
#include <conio.h>
#include "winsock.h"
#include "stdio.h"
#include "CfgFileParms.h"
#include "function.h"
#include<graphics.h>
using namespace std;

U8* autoSendBuf;        //用来组织发送数据的缓存，大小为MAX_BUFFER_SIZE,可以在这个基础上扩充设计，形成适合的结构，例程中没有使用，只是提醒一下
int printCount = 0; //打印控制
int spin = 1;  //打印动态信息控制

//------华丽的分割线，一些统计用的全局变量------------
int iSndTotal = 0;  //发送数据总量
int iSndTotalCount = 0; //发送数据总次数
int iSndErrorCount = 0;  //发送错误次数
int iRcvTotal = 0;     //接收数据总量
int iRcvTotalCount = 0; //转发数据总次数
int iRcvUnknownCount = 0;  //收到不明来源数据总次数
int sendipostionOffset = 0;
int recipostionOffset = 0;
U8* cache = NULL;  //重组分段的缓冲区
int NUM = 0; //重组的分段的个数
int LEN = 100;//以LEN为间隔来分段
bool rec_img = 0;
void print_statistics();
void menu();
//***************重要函数提醒******************************
//名称：InitFunction
//功能：初始化功能面，由main函数在读完配置文件，正式进入驱动机制前调用
//输入：
//输出：
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
		cout << "内存不够" << endl;
		//这个，计算机也太，退出吧
		exit(0);
	}
	for (i = 0; i < MAX_BUFFER_SIZE; i++) {
		autoSendBuf[i] = 'a'; //初始化数据全为字符'a',只是为了测试
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
	if (autoSendBuf != NULL)
		free(autoSendBuf);
	return;
}
pair<U8*, int> readPhoto()
{
	std::fstream InputFile;
	InputFile.open("test.JPG", std::ios::in | std::ios::binary);
	// 计算图片长度
	InputFile.seekg(0, InputFile.end);
	int length = InputFile.tellg();
	InputFile.seekg(0, InputFile.beg);
	// 创建内存缓存区
	char* buffer = new char[length];
	// 读取图片
	InputFile.read(buffer, length);
	InputFile.close();
	return{ buffer,length };
	//delete[] buffer;
}

//名称：sendPhoto
//功能：本函数被调用时，意味着要发送图片了
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
void sendPhoto(U8* buf, int len, int ip)
{
	int iSndRetval;
	int num = 0, len1 = 0, i = 0, j = 0;
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	bufSend = (char*)malloc(LEN + 2);
	num = len / LEN; //长度为100的分段的个数
	len1 = len % LEN; //不满足长度为100的最后的分段长度
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
		iSndRetval = iSndRetval * 8; //换算成位
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
	iSndRetval = iSndRetval * 8; //换算成位
}

//名称：send_ph_info
//功能：告诉接收方要发送照片了或结束发送了
//输入：len为消息的长度，s_or_e为0时为开始，为1时为结束
void send_ph_info(int len, int s_or_e, int ip)
{
	int iSndRetval;
	int digit = 0;  //图片数据长度的位数
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
	iSndRetval = iSndRetval * 8; //换算成位
}

//名称：receive_ph_start_info
//功能：判断消息是否是对方要开始发送照片了
//输入：len为收到的buf的字节数，buf为收到的数据
bool  receive_ph_start_info(U8* buf, int len)
{
	int len1 = 0;  //len1是照片字节数组的长度
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
		cache = (char*)malloc(len1);  //判断为是对方要发送的消息是就分配一个长度为len1的ceche
		cout << "开始接受\n";
		rec_img = 1;
		return 1;
	}
	return 0;
}

//名称：receive_ph_end_info
//功能：判断消息是否是对方要结束发送照片了
//输入：len为收到的buf的字节数，buf为收到的数据
bool receive_ph_end_info(U8* buf, int len)
{
	int len1 = 0, a;  //len1是照片字节数组的长度
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
		cout << "结束接受\n";
		//下面就是将cache中的数组转换成图片
		std::fstream OutputFile;
		std::string FileName = "result.JPG";  //转换完成的图片名字

		OutputFile.open(FileName.c_str(), std::ios::binary | std::ios::out);
		OutputFile.seekp(0L, ios::beg);
		OutputFile.write((char*)cache, len1);
		OutputFile.close();
		cache = NULL;
		NUM = 0;   //转换完成之后重置cache和NUM
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
	cout << "收到：" << NUM << endl;
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
	InputBox(ip, 100, "请输入ip地址");
	desip = _ttoi(ip);

	printf("请稍等片刻");
	bufSend = readPhoto().first;
	len = readPhoto().second;
	send_ph_info(len, 0, desip);
	Sleep(5000);
	sendPhoto(bufSend, len, desip);
	send_ph_info(len, 1, desip);
	printf("\n传输完成\n");
	IMAGE img1;
	loadimage(&img1, _T("test.jpg"),80,80);
	//outtextxy(300, 320 + 18 * ipostionOffset, point);
	putimage(300, 60 + 18 * sendipostionOffset, &img1);
	sendipostionOffset += 5;
	return;

}
//***************重要函数提醒******************************
//名称：TimeOut
//功能：本函数被调用时，意味着sBasicTimer中设置的超时时间到了，
//      函数内容可以全部替换为设计者自己的想法
//      例程中实现了几个同时进行功能，供参考
//      1)根据iWorkMode工作模式，判断是否将键盘输入的数据发送，还是自动发送——这个功能其实是应用层的
//        因为scanf会阻塞，导致计时器在等待键盘的时候完全失效，所以使用_kbhit()无阻塞、不间断地在计时的控制下判断键盘状态，这个点Get到没？
//      2)不断刷新打印各种统计值，通过打印控制符的控制，可以始终保持在同一行打印，Get？
//      3)如果工iWorkMode设置为自动发送，就每经过autoSendTime * DEFAULT_TIMER_INTERVAL ms，向接口0发送一次
//输入：时间到了就触发，只能通过全局变量供给输入
//输出：这就是个不断努力干活的老实孩子
void TimeOut()
{

	int iSndRetval;
	int len;
	U8* bufSend;
	int i;

	printCount++;

	/*if (_kbhit()) {
		//键盘有动作，进入菜单模式
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
//------------华丽的分割线，以下是数据的收发,--------------------------------------------

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
void RecvfromUpper(U8* buf, int len)
{
	//应用层不会收到“高层”的数据，都是自己产生
}
//***************重要函数提醒******************************
//名称：RecvfromLower
//功能：本函数被调用时，意味着得到一份从低层实体递交上来的数据
//      函数内容全部可以替换成设计者想要的样子
//      例程功能介绍：
//          1)例程实现了一个简单粗暴不讲道理的策略，所有从接口0送上来的数据都直接转发到接口1，而接口1的数据上交给高层，就是这么任性
//          2)转发和上交前，判断收进来的格式和要发送出去的格式是否相同，否则，在bite流数组和字节流数组之间实现转换
//            注意这些判断并不是来自数据本身的特征，而是来自配置文件，所以配置文件的参数写错了，判断也就会失误
//          3)根据iWorkMode，判断是否需要把数据内容打印
//输入：U8 * buf,低层递交上来的数据， int len，数据长度，单位字节，int ifNo ，低层实体号码，用来区分是哪个低层
//输出：
void RecvfromLower(U8* buf, int len, int ifNo)
{

	int retval;
	if (rec_img)
	{
		if (receive_ph_end_info(buf, len))
		{
			return;
		}
	}  //判断是否是对方要结束发送照片的消息
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
	} // 判断是否是对方要开始发送照片的消息

	/*TCHAR* output;
	Char2TCHAR(buf, output);*/

}
//打印统计信息
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
		cout << "共发送 " << iSndTotal << " 位," << iSndTotalCount << " 次," << "发生 " << iSndErrorCount << " 次错误;";
		cout << " 共接收 " << iRcvTotal << " 位," << iRcvTotalCount << " 次";
		spin++;
	}
}
void menu()
{

	cout << "已经进入menu了" << endl;
	int selection;
	int desip;
	int iSndRetval;
	char kbBuf[1000];
	int len;
	int length;
	U8* bufSend;

	TCHAR Buf[1000];
	TCHAR ip[10];

	//InputBox(Buf, 10, L"输入字符串，不超过100个字符");
	//InputBox(ip, 10, L"请输入IP地址");
	InputBox(Buf, 100, "输入字符串，不超过100个字符");
	InputBox(ip, 100, "请输入ip地址");
	desip = _ttoi(ip);//用户输入转换为数字
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
	len = length + 2; //字符串最后有个结束符
	iSndRetval = SendtoLower(kbBuf, len, 0);
	//下面是展示界面
	outtextxy(300, 60 + 18 * sendipostionOffset, Buf);
	sendipostionOffset++;
	//cout << len;
	//iSndRetval = iSndRetval * 8; //换算成位
	//发送统计






	//发送|打印：[发送控制（0，等待键盘输入；1，自动）][打印控制（0，仅定期打印统计信息；1，按bit流打印数据，2按字节流打印数据]


}
