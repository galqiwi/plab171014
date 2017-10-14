#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

void update(string fin_, string fout_) {

	ifstream fin(fin_);
	ofstream fout(fout_);
	double trigger = 25;
	double floor = 20;
	vector<double> x;
	double tmp;
	int frame;
	while (fin >> frame) {
		fin >> tmp;
		x.push_back(tmp);
	}
	bool flag = false;
	int frame_0;
	double x_0, vmid = 0;
	vector<double> v;
	vector<double> vok;
	for (int i = 0; i < x.size(); i++) {
		if (x[i] < floor)
			continue;
		if (x[i] <= trigger) {
			flag = false;
			continue;
		}
		if (!flag && x[i] > trigger + 1)
			continue;
		if (!flag) {
			flag = true;
			frame_0 = i;
			//cout << frame_0 << " " << x[frame_0] << endl;
			x_0 = x[frame_0];
		} else {
			if (i == frame_0)
				continue;
			v.push_back((x[i] - x_0) / (i - frame_0));
			//cout << x_0 << endl;
			vmid += (x[i] - x_0) / (i - frame_0);
			//cout << (x[i] - x_0) / (i - frame_0) << " " << x[i] << " " << i - frame_0 << " " << vmid << endl;
		}
	}
	vmid /= v.size();

	//cout << vmid << endl;
	for(int i = 0; i < v.size(); i++) {
		tmp = v[i];
		if (abs(tmp - vmid) > 0.03)
			continue;
		vok.push_back(tmp);
	}
	vmid = 0;
	for (int i = 0; i < vok.size(); i++) {
		vmid += vok[i];
	}
	vmid /= vok.size();
	cout << fin_ << ":" << endl;
	cout << vmid * 30 << endl;
	double delsq = 0;
	for (int i = 0; i < vok.size(); i++) {
		delsq += (vok[i] - vmid) * (vok[i] - vmid);
	}
	delsq /= vok.size() * (vok.size() - 1);
	delsq = sqrt(delsq);
	cout << delsq * 30 << endl;
	//cout << vmid << endl;
	//cout << vok.size() << " " << v.size() << " " << x.size() << endl;
}

int main() {
	ifstream fin("upd.conf");
	int n;
	fin >> n;
	string fin_, fout_;
	for (int i = 0; i < n; i++) {
		fin >> fin_ >> fout_;
		//cout << fout_ << endl;
		update(fin_, fout_);
	}
}