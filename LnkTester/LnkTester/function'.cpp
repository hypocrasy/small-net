//Nettester 的功能文件
#include <iostream>
#include <conio.h>
#include "winsock.h"
#include "stdio.h"
#include "CfgFileParms.h"
#include "function.h"
using namespace std;

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
U8* cache=(char*)malloc(4000);
int cache_len = 0;
int lastsend = 0x3f3f3f3f;
int frame_num = 0;
bool cache_sta = 0;
//打印统计信息
void print_statistics();
void menu();
//***************重要函数提醒******************************
//名称：InitFunction
//功能：初始化功能面，由main函数在读完配置文件，正式进入驱动机制前调用
//输入：
//输出：
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
void TimeOut()
{

	printCount++;
	if (_kbhit()) {
		//键盘有动作，进入菜单模式
		menu();
	}
	if (!ack && printCount - lastsend >= 100)
	{
		for (int i = 0; i < cache_len-1; i++)
		{
			cout <<(int) *(cache + i)<<" ";
		}
		SendtoLower(cache, cache_len,0);
		lastsend = printCount;
	}
	print_statistics();
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
				i = i + 6;
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

	return { bufSend2,iSndRetval +16 };
}


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
U8* add_head(char* a,int len,)
{
	U8* buf1 = (char*)malloc(len + 5);
	*(buf1)=
	*(buf1) = 0;//ack位，若1则是ACK消息，0是MESSAGE消息
	*(buf1 + 1) = num;//帧序号位
	for (int i = 2; i < 2 + len; i++)
	{
		*(buf1 + i) = *(buf + i - 2);
	}
	*(buf1 + 2 + len) = 0;
	*(buf1 + 3 + len) = 0;
	len += 4;
	return buf1;
}
void RecvfromUpper(U8* buf, int len)
{
	int mac_address;
	cout << "please input mac_address:";
	{
		cin >> mac_address;
	}
	cout << "设备号"<< strDevID << "层次名" << strLayer<< "\n";
	if (!ack)
	{
		cout << "last frame has'nt been accept";
		return;
	}
	
	int iSndRetval, len2 = 0, count = 0, len3 = 0;
	U8* bufSend2 = NULL;
	U8* bufSend3 = NULL;
	//是高层数据，只从接口0发出去,高层接口默认都是字节流数据格式
	//如果底层接口时bit数组，那么就做成帧后直接发送到下层

	  //len2就是做完成帧后有效帧的长度
	cache_len = len;
	len--;
	//frame_num = len / 2 + (len%2);
	
	U8* buf1 = (char*)malloc(len + 5);
	*(buf1) = 0;//ack位，若1则是ACK消息，0是MESSAGE消息
	*(buf1 + 1) = num;//帧序号位
	for (int i = 2; i < 2+len ; i++)
	{
		*(buf1 + i) = *(buf + i - 2);
	}
	*(buf1 + 2 + len) = 0;
	*(buf1 + 3 + len) = 0;
	len += 4;
	bufSend2= framing1(buf1, len).first;
	len2 = framing1(buf1, len).second;
	for (int i = 0; i < len2; i++)
	{
		cache[i] = bufSend2[i];
		//cout << buf[i];
	}
	cache_len = len2;
	if (lowerMode[0] == 0) {
		cout << "\n";
		for (int i = 0; i < len2; i++)
		{
			cout << (int)*(bufSend2 + i);
		}
		cout << "\n";
		iSndRetval = SendtoLower(bufSend2, len2, 0); //参数依次为数据缓冲，长度，接口号
	}
	else {
		//如果底层接口是字节数组，将bit数组转变为字节数组并存放到bufSend3中，再发送
		len3 = (len2 + 16) / 8 + 1;
		bufSend3 = (char*)malloc(len3);
		iSndRetval = BitArrayToByteArray(bufSend2, len2 + 16, bufSend3, len3);
		iSndRetval = SendtoLower(bufSend3, len3, 0);
		iSndRetval = iSndRetval * 8;//换算成位
	}
	ack = 0;
	lastsend = printCount;
	//统计
	if (iSndRetval <= 0) {
		iSndErrorCount++;
	}
	else {
		iSndTotal += iSndRetval;
		iSndTotalCount++;
	}


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
		if(start==0||end==0) return { NULL,-1 };
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
	int iSndRetval, count = 0;
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	U8* bufSend3 = NULL;
	if (ifNo == 0 && lowerNumber > 1) {
		//从接口0收到的数据，直接转发到接口1
		if (lowerMode[0] == lowerMode[1]) {
			//接口0和1的数据格式相同，直接转发
			iSndRetval = SendtoLower(buf, len, 1);
			if (lowerMode[0] == 1) {
				iSndRetval = iSndRetval * 8;//如果接口格式为bit数组，统一换算成位，完成统计
			}
		}
		else {
			//接口0与接口1的数据格式不同，需要转换后，再发送
			if (lowerMode[0] == 1) {
				//从接口0到接口1，接口0是字节数组，接口1是比特数组，需要扩大8倍转换
				bufSend = (U8*)malloc(len * 8);
				if (bufSend == NULL) {
					cout << "内存空间不够，导致数据没有被处理" << endl;
					return;
				}
				//byte to bit
				iSndRetval = ByteArrayToBitArray(bufSend, len * 8, buf, len);
				iSndRetval = SendtoLower(bufSend, iSndRetval, 1);
			}
			else {
				//从接口0到接口1，接口0是比特数组，接口1是字节数组，需要缩小八分之一转换
				bufSend = (U8*)malloc(len / 8 + 1);
				if (bufSend == NULL) {
					cout << "内存空间不够，导致数据没有被处理" << endl;
					return;
				}
				//bit to byte
				iSndRetval = BitArrayToByteArray(buf, len, bufSend, len / 8 + 1);
				iSndRetval = SendtoLower(bufSend, iSndRetval, 1);

				iSndRetval = iSndRetval * 8;//换算成位，做统计

			}
		}
		//统计
		if (iSndRetval <= 0) {
			iSndErrorCount++;
		}
		else {
			iRcvForward += iSndRetval;
			iRcvForwardCount++;
		}
	}
	else {
		//非接口0的数据，或者低层只有1个接口的数据，都向上递交
			//如果接口0是比特数组格式，高层默认是字节数组，先转换成字节数组，再向上递交
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
		/*cout << "\n";
		cout << len2 << " ";
		for (int i = 0; i < len2; i++)
		{
			
			cout << (int)(*(bufSend+i));
		}
		cout << "\n";*/
		iSndRetval = BitArrayToByteArray(bufSend, len2, bufSend2, len2 / 8 + 1);
		//iSndRetval--;
		cout << "\n";
		cout << iSndRetval << " ";
		for (int i = 0; i < iSndRetval; i++)
		{
			cout << (int)(*(bufSend2+i))<<" ";
		}
		cout << "\n";
		if (*(bufSend2) == 0)//如果信息是MESSAGE
		{
			
			iSndRetval = SendtoUpper(bufSend2, iSndRetval);
			//iSndRetval = iSndRetval * 8;//换算成位,进行统计
			U8* ACK1 = (char*)malloc(4);
			*(ACK1) = 1;//将ACK位设成1
			*(ACK1 + 1) = *(bufSend2 + 1);//第二位还是编号
			*(ACK1 + 2) = 0;
			*(ACK1 + 3) = 0;
			U8* ACK = NULL;
			ACK= framing1(ACK1,4).first;
			int ACK_len = framing1(ACK1, 4).second;
			SendtoLower(ACK, ACK_len, 0);
		}
		else
		{
			ack = 1;
		}
		/*cout << iSndRetval << " ";
		for (int i = 0; i < iSndRetval; i++)
		{
			cout << (int)*(bufSend2 + i) << " ";
		}*/
		//iSndRetval = SendtoUpper(bufSend2, iSndRetval);
		iSndRetval = iSndRetval * 8;//换算成位,进行统计


	/*else {
		//如果接口0是字节数组格式，先需要把底层传来的字节转换成bit数组，做完提取有效帧后再转为字节数组，再上传给高层
		bufSend2 = (U8*)malloc(len * 8 - 16);  //存放有效帧的bit数组
		bufSend2 = change2(buf, len, ifNo);
		bufSend3 = (U8*)malloc(len2 / 8 + 1);  //存放底层字节转bit后提取的有效帧再转字节的字节数组
		if (bufSend3 == NULL) {
			cout << "内存空间不够，导致数据没有被处理" << endl;
			return;
		}
		iSndRetval = BitArrayToByteArray(bufSend2, len2, bufSend3, len2 / 8 + 1);
		iSndRetval = SendtoUpper(bufSend3, len2 / 8 + 1);
		iSndRetval = iSndRetval * 8;//换算成位，进行统计
	}*/
	//统计
		if (iSndRetval <= 0) {
			iSndErrorCount++;
		}
		else {
			iRcvToUpper += iSndRetval;
			iRcvToUpperCount++;
		}
	}

	//打印
	/*switch (iWorkMode % 10) {
	case 1:
		cout << endl << "接收接口 " << ifNo << " 数据：" << endl;
		if (ifNo == 0 && lowerNumber > 1) {
			if (lowerMode[0] == lowerMode[1]) {
				print_data_bit(buf, iSndRetval, lowerMode[ifNo]);
			}
			else {
				if (lowerMode[0] == 1) {
					print_data_bit(bufSend, iSndRetval, lowerMode[ifNo]);
				}
				else {
					print_data_bit(bufSend, iSndRetval, lowerMode[ifNo]);
				}
			}
		}
		else {
			if (lowerMode[ifNo] == 0) {
				print_data_bit(bufSend, iSndRetval, lowerMode[ifNo]);
			}
			else {
				print_data_bit(bufSend, iSndRetval, lowerMode[ifNo]);
			}
		}
		break;
	case 2:
		cout << endl << "接收接口 " << ifNo << " 数据：" << endl;
		if (ifNo == 0 && lowerNumber > 1) {
			if (lowerMode[0] == lowerMode[1]) {
				print_data_byte(buf, iSndRetval, lowerMode[ifNo]);
			}
			else {
				if (lowerMode[0] == 1) {
					print_data_byte(bufSend, iSndRetval, lowerMode[ifNo]);
				}
				else {
					print_data_byte(bufSend, iSndRetval, lowerMode[ifNo]);
				}
			}
		}
		else {
			if (lowerMode[ifNo] == 0) {
				print_data_byte(bufSend, iSndRetval, lowerMode[ifNo]);
			}
			else {
				print_data_byte(bufSend, iSndRetval, lowerMode[ifNo]);
			}
		}
		break;
	case 0:
		break;
	}
	//如果需要重传等机制，可能需要将buf或bufSend中的数据另外申请空间缓存起来
	if (bufSend != NULL) {
		//缓存bufSend数据，如果有必要的话

		//本例程中没有停等协议，bufSend的空间在用完以后需要释放
		free(bufSend);
	}
	else {
		//缓存buf里的数据，如果有必要的话

		//buf空间不需要释放
	}*/

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