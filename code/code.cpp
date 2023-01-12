#include<iostream>
#include<vector>
#include<cstring>
#include<cmath>
#include<time.h>
#include<cstdlib>
using namespace std;
int anstime = INT_MAX;//�����ܹ����ʱ
int ansfinal = INT_MAX;
int randm[51];//������飬����shuffle�еõ�һ�������ʼ����״̬
int ans_state[51];//��ǰ״̬
int ans_state_next[51];//��һ��״̬
double T0 = 100;//��ʼ�¶�
double Tmin = 0.001;//����¶�
double fac = 0.99;//�½�����
bool flagfinal = 0;//�ж��Ƿ�����ʵ����������Ž��־
double timefinal = 0;//��ʵ������е����Ž�
int ans_statef[51];
int m, n;
int list[501];
struct Information {
    int id;// �ڵ� id ̨�����ϼӹ�
    int cost;// ���� cost ʱ��
} a[51][51];// a[�ڼ�������][�ڼ���]
int machine[51][10001] = { 0 };// // machine[�������][ʱ��]
int step[51] = { 0 };// ÿ�������ӹ����˵ڼ���
int last_time[51] = { 0 };// ��¼ÿ��������һ�μӹ����ʱ�䣬��ÿ������las_time[�������] ʱ�ӹ����
int ans = 0;
void shuffle(int n) {//shuffle�㷨����0->n�������,���ڵõ�һ�������ʼ����״̬
    for (int i = 0; i < 30; i++) {
        randm[i] = 0;
    }
    vector<int>a;
    a.push_back(0);
    for (int i = 1; i <= n; i++)
        a.push_back(i);
    int t = n;
    for (int i = 1; i <= n; i++)
    {
        int x = 1 + rand() % t;
        randm[i - 1] = a[x];
        swap(a[x], a[t]);
        t--;
    }
}
void swap(int i, int j) {//���������е�����λ���㷨������Ѱ��ĳ��״̬������״̬
    for (int k = 0; k < n; k++) {
        ans_state_next[i] = ans_state[i];
    }
    int t = ans_state_next[i];
    ans_state_next[i] = ans_state_next[j];
    ans_state_next[j] = t;
}
void solve(double T) {
    memset(machine, 0, sizeof(machine));
    memset(step, 0, sizeof(step));
    memset(last_time, 0, sizeof(last_time));
    ans = 0;
    //�˴�ʹ��ģ���˻�
    for (int i = 1; i <= n; i++) {//�ӹ�˳�� �˴�ѡȡ����ʹ��ģ���˻��㷨
        for (int j = 1; j <= m; j++) {
            list[(i - 1) * m + j] = ans_state_next[i - 1];
        }
    }
    for (int i = 1; i <= n; i++) {//��ֵÿ������ÿ��������ʹ�û����ţ��ڱ������У�ÿ������Ļ����ż�Ϊ0->m-1
        for (int j = 1; j <= m; j++) {//������� 1->n 
            a[i][j].id = j;
        }
    }
    for (int i = 1; i <= m * n; i++) {
        int now = list[i];
        step[now]++;//step������е��Ĺ�����
        int id = a[now][step[now]].id, cost = a[now][step[now]].cost;
        int s = 0;
        for (int j = last_time[now] + 1; ; j++) {
            if (machine[id][j] == 0) {//����˻����Ѿ��������
                s++;
            }
            else {//�˻���û�д����
                s = 0;
            }
            if (s == cost) {
                for (int k = j - cost + 1; k <= j; k++) {
                    machine[id][k] = 1;
                }
                if (j > ans) ans = j;
                last_time[now] = j;
                break;
            }
        }
    }
    double b = rand() / (RAND_MAX + 1.0);
    if (ans < anstime) {//����������ȵ�ǰ�ⷨҪ����(ans<anstime)����ô���ܴ˽⡣
        anstime = ans;
        if (ansfinal > ans) {//���������������������Ž⣬��ô���䷨�Ƹ����Ž�(ans_state_next->ans_statef)
            ansfinal = ans;
            flagfinal = true;
            for (int i = 0; i < n; i++) {
                ans_statef[i] = ans_state_next[i];
            }
        }
        for (int i = 0; i < n; i++) {//��������⸳ֵ����ǰ��(ans_state_next->ans_state)��
            ans_state[i] = ans_state_next[i];
        }
    }
    else {//����������ȵ�ǰ�ⷨҪ����(ans<anstime)����ô��exp(-dE/T)�ĸ��ʽ��ܴ˽⡣
        double delta = (anstime - ans) / T;
        if (b < exp(delta)) {
            anstime = ans;
            for (int i = 0; i < n; i++) {
                ans_state[i] = ans_state_next[i];
            }
        }
    }
    int i = rand() % n, j = rand() % n;//i,jΪ���������(0,n)�������
    swap(i, j);//ans_state����i,j��Ӧ������������õ������=
}
int cnt = 0;
void sa(int n, double T) {//ģ���˻��㷨
    shuffle(n);//����shuffle�㷨��������Ϊn��һ������ĳ�ʼ��
    for (int i = 0; i < n; i++) {//����������⸳���ʼ̬
        ans_state_next[i] = randm[i];
    }
    while (T > Tmin) {//��T�½���С�ڵ���Tminʱ��ģ���˻��㷨����
        cnt += 1;
        solve(T);
        T *= fac;//ÿ��T��˥������fac���ٶ��½�
    }
}
int main()
{
    cin >> n >> m;//n������,m������
    for (int i = 1; i <= n; i++) {//ÿ������ÿ������ļӹ�ʱ��
        for (int j = 1; j <= m; j++) {
            int uus;
            cin >> uus >> a[i][j].cost;//uus���������,a[i][j].cost�����j�������ڵ�i�������ϼӹ���ʱ��
        }
    }
    for (int i = 0; i < 100; i++) {//����ظ�ʵ��(�ظ�ʵ��100��)
        double T = T0;//ÿ��ʵ�齫�¶ȳ�ʼ��ΪT0
        clock_t start, end;//ÿ��ʵ�鶨����ʼʱ�����ֹʱ��
        start = clock();
        cnt = 0;//cnt������ʵ����˻����
        sa(n,T);
        cout << "Count:" << cnt << endl;
        printf("anstime: %d\n ansstate: ", anstime);
        for (int i = 0; i < n; i++) {
            cout << ans_state[i] - 1 << " ";
        }
        cout << endl;
        end = clock();
        double t = (double)(end - start) / CLK_TCK;//���㵥��ʵ����ʱ
        printf("time=%lf\n\n", t);
        if (flagfinal ==true) {//����˽��֮ǰ��õ����Ž�Ҫ�ã���solve�����лὫflagfinal���Ϊtrue����flagfinalΪtrue����ʾ�£����˽Ᵽ�������Ž�
            timefinal = t;
            flagfinal = false;
        }
    }
    cout << "anstimefinal=" << ansfinal << endl;
    for (int i = 0; i < n; i++) {
        cout << ans_statef[i] - 1 << " ";
    }
    cout << "timefinal" << timefinal << endl;
    cout << endl;
    return 0;
}