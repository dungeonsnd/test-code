#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// 输入ip是网络序.
unsigned IPHash(unsigned peerCnt, unsigned ip)
{
    if(0==peerCnt)
        return 0;
    unsigned hash = 89;
    unsigned mask[3] ={0xFF000000,0x00FF0000,0x0000FF00};
    unsigned t =0;
    for(unsigned i = 0; i < 3; i++) {
      t =ip&mask[i];
      t =t>>(8*(3-i));
      hash = ( hash * 113 + t ) % 6271; //hash运算
    }
    return hash % peerCnt;
}

const double per = 0.5; //阈值，后端server命中个数与平均值偏离超过该比例则输出相关信息

int main() {
  srand((unsigned)time(NULL));
  int peer_number= 8; //后端server节点数
  int* result = new int[peer_number]; //存放每个后端server命中次数
  for(int i = 0; i < peer_number; i++) { //初始化
    result[i] = 0;
  }

  int total_num = 1000*10000;  //进行hash的总次数
  int total_num_temp = total_num;
  while(total_num_temp-->0) {
    int rand_num[3];
    unsigned ip =0;
    for(int i = 0; i < 3; i++) {
      rand_num[i]= rand()%255;  //随机生成三个数作为ip地址前三段
      ip +=rand_num[i]*256*i;
    }
    
    int hash =IPHash(peer_number, ip);
    result[hash]++; //统计hash值命中
  }

  // 设定一个阈值per，当每个server命中次数与平均值偏差超过该比例时记录。
  int avg = total_num/peer_number; 
  int max =(int) ((double)avg * (1 + per));
  int min =(int) ((double)avg * (1 - per));
  
  int not_good_peer_cnt_gt =0;
  int not_good_peer_cnt_lt =0;
  for(int i = 0; i < peer_number; i++) {
    cout<<i<<": "<<result[i]<< endl;
    if (result[i] > max)
    {
      not_good_peer_cnt_gt++;
    }
    if (result[i] < min)
    {
      not_good_peer_cnt_lt++;
    }
  }

  cout << "peer_number: "<<peer_number << endl;
  cout << "not_good_peer_cnt_gt: "<<not_good_peer_cnt_gt<< endl;
  cout << "not_good_peer_cnt_lt: "<<not_good_peer_cnt_lt<< endl;
  cout << "avg: " << avg << ", max: " << max << ", min:  " <<min<<endl;
  delete []result;
  
  return 0;
}
