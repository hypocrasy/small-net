//Nettester �Ĺ����ļ�
#include <iostream>
#include <conio.h>
#include "winsock.h"
#include "stdio.h"
#include "CfgFileParms.h"
#include "function.h"
using namespace std;

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
U8* cache=(char*)malloc(4000);
int cache_len = 0;
int lastsend = 0x3f3f3f3f;
int frame_num = 0;
bool cache_sta = 0;
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

	return { bufSend2,iSndRetval +16 };
}


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
U8* add_head(char* a,int len,)
{
	U8* buf1 = (char*)malloc(len + 5);
	*(buf1)=
	*(buf1) = 0;//ackλ����1����ACK��Ϣ��0��MESSAGE��Ϣ
	*(buf1 + 1) = num;//֡���λ
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
	cout << "�豸��"<< strDevID << "�����" << strLayer<< "\n";
	if (!ack)
	{
		cout << "last frame has'nt been accept";
		return;
	}
	
	int iSndRetval, len2 = 0, count = 0, len3 = 0;
	U8* bufSend2 = NULL;
	U8* bufSend3 = NULL;
	//�Ǹ߲����ݣ�ֻ�ӽӿ�0����ȥ,�߲�ӿ�Ĭ�϶����ֽ������ݸ�ʽ
	//����ײ�ӿ�ʱbit���飬��ô������֡��ֱ�ӷ��͵��²�

	  //len2���������֡����Ч֡�ĳ���
	cache_len = len;
	len--;
	//frame_num = len / 2 + (len%2);
	
	U8* buf1 = (char*)malloc(len + 5);
	*(buf1) = 0;//ackλ����1����ACK��Ϣ��0��MESSAGE��Ϣ
	*(buf1 + 1) = num;//֡���λ
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
		iSndRetval = SendtoLower(bufSend2, len2, 0); //��������Ϊ���ݻ��壬���ȣ��ӿں�
	}
	else {
		//����ײ�ӿ����ֽ����飬��bit����ת��Ϊ�ֽ����鲢��ŵ�bufSend3�У��ٷ���
		len3 = (len2 + 16) / 8 + 1;
		bufSend3 = (char*)malloc(len3);
		iSndRetval = BitArrayToByteArray(bufSend2, len2 + 16, bufSend3, len3);
		iSndRetval = SendtoLower(bufSend3, len3, 0);
		iSndRetval = iSndRetval * 8;//�����λ
	}
	ack = 0;
	lastsend = printCount;
	//ͳ��
	if (iSndRetval <= 0) {
		iSndErrorCount++;
	}
	else {
		iSndTotal += iSndRetval;
		iSndTotalCount++;
	}


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
		if(start==0||end==0) return { NULL,-1 };
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
	int iSndRetval, count = 0;
	U8* bufSend = NULL;
	U8* bufSend2 = NULL;
	U8* bufSend3 = NULL;
	if (ifNo == 0 && lowerNumber > 1) {
		//�ӽӿ�0�յ������ݣ�ֱ��ת�����ӿ�1
		if (lowerMode[0] == lowerMode[1]) {
			//�ӿ�0��1�����ݸ�ʽ��ͬ��ֱ��ת��
			iSndRetval = SendtoLower(buf, len, 1);
			if (lowerMode[0] == 1) {
				iSndRetval = iSndRetval * 8;//����ӿڸ�ʽΪbit���飬ͳһ�����λ�����ͳ��
			}
		}
		else {
			//�ӿ�0��ӿ�1�����ݸ�ʽ��ͬ����Ҫת�����ٷ���
			if (lowerMode[0] == 1) {
				//�ӽӿ�0���ӿ�1���ӿ�0���ֽ����飬�ӿ�1�Ǳ������飬��Ҫ����8��ת��
				bufSend = (U8*)malloc(len * 8);
				if (bufSend == NULL) {
					cout << "�ڴ�ռ䲻������������û�б�����" << endl;
					return;
				}
				//byte to bit
				iSndRetval = ByteArrayToBitArray(bufSend, len * 8, buf, len);
				iSndRetval = SendtoLower(bufSend, iSndRetval, 1);
			}
			else {
				//�ӽӿ�0���ӿ�1���ӿ�0�Ǳ������飬�ӿ�1���ֽ����飬��Ҫ��С�˷�֮һת��
				bufSend = (U8*)malloc(len / 8 + 1);
				if (bufSend == NULL) {
					cout << "�ڴ�ռ䲻������������û�б�����" << endl;
					return;
				}
				//bit to byte
				iSndRetval = BitArrayToByteArray(buf, len, bufSend, len / 8 + 1);
				iSndRetval = SendtoLower(bufSend, iSndRetval, 1);

				iSndRetval = iSndRetval * 8;//�����λ����ͳ��

			}
		}
		//ͳ��
		if (iSndRetval <= 0) {
			iSndErrorCount++;
		}
		else {
			iRcvForward += iSndRetval;
			iRcvForwardCount++;
		}
	}
	else {
		//�ǽӿ�0�����ݣ����ߵͲ�ֻ��1���ӿڵ����ݣ������ϵݽ�
			//����ӿ�0�Ǳ��������ʽ���߲�Ĭ�����ֽ����飬��ת�����ֽ����飬�����ϵݽ�
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
		if (*(bufSend2) == 0)//�����Ϣ��MESSAGE
		{
			
			iSndRetval = SendtoUpper(bufSend2, iSndRetval);
			//iSndRetval = iSndRetval * 8;//�����λ,����ͳ��
			U8* ACK1 = (char*)malloc(4);
			*(ACK1) = 1;//��ACKλ���1
			*(ACK1 + 1) = *(bufSend2 + 1);//�ڶ�λ���Ǳ��
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
		iSndRetval = iSndRetval * 8;//�����λ,����ͳ��


	/*else {
		//����ӿ�0���ֽ������ʽ������Ҫ�ѵײ㴫�����ֽ�ת����bit���飬������ȡ��Ч֡����תΪ�ֽ����飬���ϴ����߲�
		bufSend2 = (U8*)malloc(len * 8 - 16);  //�����Ч֡��bit����
		bufSend2 = change2(buf, len, ifNo);
		bufSend3 = (U8*)malloc(len2 / 8 + 1);  //��ŵײ��ֽ�תbit����ȡ����Ч֡��ת�ֽڵ��ֽ�����
		if (bufSend3 == NULL) {
			cout << "�ڴ�ռ䲻������������û�б�����" << endl;
			return;
		}
		iSndRetval = BitArrayToByteArray(bufSend2, len2, bufSend3, len2 / 8 + 1);
		iSndRetval = SendtoUpper(bufSend3, len2 / 8 + 1);
		iSndRetval = iSndRetval * 8;//�����λ������ͳ��
	}*/
	//ͳ��
		if (iSndRetval <= 0) {
			iSndErrorCount++;
		}
		else {
			iRcvToUpper += iSndRetval;
			iRcvToUpperCount++;
		}
	}

	//��ӡ
	/*switch (iWorkMode % 10) {
	case 1:
		cout << endl << "���սӿ� " << ifNo << " ���ݣ�" << endl;
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
		cout << endl << "���սӿ� " << ifNo << " ���ݣ�" << endl;
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
	//�����Ҫ�ش��Ȼ��ƣ�������Ҫ��buf��bufSend�е�������������ռ仺������
	if (bufSend != NULL) {
		//����bufSend���ݣ�����б�Ҫ�Ļ�

		//��������û��ͣ��Э�飬bufSend�Ŀռ��������Ժ���Ҫ�ͷ�
		free(bufSend);
	}
	else {
		//����buf������ݣ�����б�Ҫ�Ļ�

		//buf�ռ䲻��Ҫ�ͷ�
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