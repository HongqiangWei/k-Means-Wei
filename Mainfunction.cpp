#include "stdafx.h"
#include "Point.h"
#include <iostream>
#include <time.h>


//------------------------------
//��������ɣ����������m��n֮��
//------------------------------
int Random_s(int m, int n) {
	srand((int)time(NULL));
	int pos, dis;
	if (m == n)
	{
		return m;
	}
	else if (m > n)
	{
		pos = n;
		dis = m - n + 1;
		return rand() % dis + pos;
	}
	else
	{
		pos = m;
		dis = n - m + 1;
		return rand() % dis + pos;
	}
}
//------------------------
//���뺯�������ݵ�λ��
//������filePath->�ļ�·��
//		other->���ݵ�
//------------------------
bool readTxt(char* filePath, std::vector<Point>& other) {
	FILE* fp = fopen(filePath, "r");
	if (fp == NULL) return false;
	long int filelength = fseek(fp, 0, SEEK_END);
	if (ftell(fp) == 0) {
		fclose(fp);
		std::cout << "file is empty!" << std::endl;
		return false;
		}
	fseek(fp, 0, SEEK_SET);
	Point temp;
	while (!feof(fp)) {
		fscanf(fp, "%f %f %f", &temp.x, &temp.y, &temp.z);
		other.push_back(temp);
	}
	long int  num = other.size();
	if (other[num - 1].x == other[num - 1].y == other[num - 1].z == 0.0f) 	other.pop_back();
	
	fclose(fp);
	return true;
}
//------------------------
//������������ݵ�λ��+RGB
//������filePath->�ļ�·��
//		other->���ݵ�
//------------------------
bool writeRgbTxt(char* filePath, std::vector<PointRgb>& other) {
	FILE* fp = fopen(filePath, "w+");
	if (fp == NULL) {
		std::cout << "file not existing!" << std::endl;
		return false;
	}
	long int filelength = fseek(fp, 0, SEEK_END);
	if (ftell(fp) != 0) {
		fclose(fp);
		std::cout << "file not empty!" << std::endl;
		return false;
	}
	fseek(fp, 0, SEEK_SET);
	char c = '\n';
	PointRgb temp;
	for (long int i = 0; i < other.size(); ++i) {
		temp = other[i];
		fprintf(fp, "%f %f %f %i %i %i%c", temp.x, temp.y, temp.z, temp.r, temp.g, temp.b, c);
	}
	fclose(fp);
	std::cout << "file write successfully!" << std::endl;
	return true;
}
//--------------------------------------
//�������ڽ�ԭ�����ݵ㻮�ֵ���ͬ�����
//������cluster->��������
//		other->���ݵ�
//--------------------------------------
bool separate(std::vector<Point>& cluster, std::vector<Point>& other) {
	for (size_t i = 0; i < other.size(); i++){
		float mindis = distance(cluster[0], other[i]);
		other[i].label = cluster[0].label;
		for (size_t j = 1; j < cluster.size(); j++){
			if (distance(cluster[j], other[i])<mindis){
				mindis = distance(cluster[j], other[i]);
				other[i].label = cluster[j].label;
			}
		}
	}
	return true;
}
//-------------------------
//ȷ���Ƿ��������
//�����µľ�������
//������cluster->��������
//		other->���ݵ�
//      gap->�¾����ľ����
//-------------------------
bool centroid(std::vector<Point>& cluster, std::vector<Point>& other, float gap) {
	std::vector<Point> centroid(cluster);  //�¾�������
	std::vector<size_t> num(cluster.size(), 0);  //ÿ����ĵ�����
	for (size_t i = 0; i < other.size(); i++){
		for (size_t j = 0; j < centroid.size(); j++){
			if (other[i].label == centroid[j].label){
				//���
				centroid[j].x += other[i].x;
				centroid[j].y += other[i].y;
				centroid[j].z += other[i].z;
				num[j]++;
				break;
			}
		}
	}

	for (size_t k = 0; k < centroid.size(); k++){
		//����������
		centroid[k].x = centroid[k].x / num[k];
		centroid[k].y = centroid[k].y / num[k];
		centroid[k].z = centroid[k].z / num[k];
	}

	bool stop = true;
	//�ж��Ƿ�Ӧ��ֹͣ����
	//�е�����
	for (size_t l = 0; l < cluster.size(); l++) {
		cluster[l].x = centroid[l].x;
		cluster[l].y = centroid[l].y;
		cluster[l].z = centroid[l].z;
		if (distance(cluster[l], centroid[l]) > gap) {
			//��������
			for (l = 0; l < cluster.size(); l++){
				cluster[l].x = centroid[l].x;
				cluster[l].y = centroid[l].y;
				cluster[l].z = centroid[l].z;
			}
			stop = false;
			break;
		}
	}
	return stop;
}
//-----------------------------------
//�������
//������argv[1]->�����ļ�·��
//      argv[2]->����ļ�·��,�������
//      argv[3]->����ļ�·��,��������
//------------------------------------
int main(int argc,char* argv) {
	
	if (argc < 4) {
		std::cout << "Insufficient parameters" << std::endl;
		return 0;
	}
	const int k = 500;
	const float gap = 0.0001;
	int iteNum;
	//1.��ȡ�ļ�
	//----------
	std::vector<Point> points;
	if (!readTxt(&argv[1], points)) return 0;
	else std::cout << "file read successfully!" << std::endl;

	//2.�����ʼ��������
	//------------------
	std::vector<Point> cluster;
	int length = points.size() / k;
	for (size_t i = 0; i < k; i++){
		points[i*length].label = i;
		cluster.push_back(points[i*length]);
	}
	std::cout << "Initial clustering center successfully��" << std::endl;
	
	//3.����
	//------
	bool iteration = true;
	while (iteration&&iteNum)
	{
		iteNum--;
		if (separate(cluster, points)) {
			iteration = !centroid(cluster, points, gap);
		}
	}
	std::cout << "end of clustering��"<< std::endl;

	//4.���
	//------
	std::vector<PointRgb> pointsout;
	std::vector<PointRgb> centersout;
	PointRgb tmp;
	//���������ɫ����
	int RGB[k][3];
	for (size_t i = 0; i <k; i++){
		RGB[i][0] = Random_s(i, 255);
		RGB[i][1] = Random_s(i + 25, 255);
		RGB[i][2] = Random_s(i + 75, 255);
	}
	//����������ɫ����
	for (size_t i = 0; i < points.size(); i++){
		tmp.x = points[i].x;
		tmp.y = points[i].y;
		tmp.z = points[i].z;
		tmp.r = RGB[points[i].label][0];
		tmp.g = RGB[points[i].label][1];
		tmp.b = RGB[points[i].label][2];
		pointsout.push_back(tmp);
	}
	//�������ģ���ɫ����
	for (size_t i = 0; i < cluster.size(); i++) {
		tmp.x = cluster[i].x;
		tmp.y = cluster[i].y;
		tmp.z = cluster[i].z;
		tmp.r = RGB[cluster[i].label][0];
		tmp.g = RGB[cluster[i].label][1];
		tmp.b = RGB[cluster[i].label][2];
		centersout.push_back(tmp);
	}

	//������ļ�
	bool m = writeRgbTxt(&argv[2], pointsout);  //������
	bool n = writeRgbTxt(&argv[3], centersout); //��������
	return 1;
}