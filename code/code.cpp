#include<iostream>
#include<vector>
#include<cstring>
#include<cmath>
#include<time.h>
#include<cstdlib>
using namespace std;
int anstime = INT_MAX;//代表总工序耗时
int ansfinal = INT_MAX;
int randm[51];//随机数组，用于shuffle中得到一个随机初始化的状态
int ans_state[51];//当前状态
int ans_state_next[51];//下一个状态
double T0 = 100;//初始温度
double Tmin = 0.001;//最低温度
double fac = 0.99;//下降因子
bool flagfinal = 0;//判断是否由于实验过程中最优解标志
double timefinal = 0;//在实验过程中的最优解
int ans_statef[51];
int m, n;
int list[501];
struct Information {
    int id;// 在第 id 台机器上加工
    int cost;// 花费 cost 时间
} a[51][51];// a[第几个工件][第几步]
int machine[51][10001] = { 0 };// // machine[机器编号][时间]
int step[51] = { 0 };// 每个工件加工到了第几步
int last_time[51] = { 0 };// 记录每个工件上一次加工完的时间，即每个工件las_time[工件编号] 时加工完的
int ans = 0;
void shuffle(int n) {//shuffle算法，将0->n随机打乱,用于得到一个随机初始化的状态
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
void swap(int i, int j) {//交换数组中的两个位的算法，用于寻找某个状态的领域状态
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
    //此处使用模拟退火
    for (int i = 1; i <= n; i++) {//加工顺序 此处选取可以使用模拟退火算法
        for (int j = 1; j <= m; j++) {
            list[(i - 1) * m + j] = ans_state_next[i - 1];
        }
    }
    for (int i = 1; i <= n; i++) {//赋值每个工件每个工序所使用机器号，在本例子中，每个工序的机器号即为0->m-1
        for (int j = 1; j <= m; j++) {//工件序号 1->n 
            a[i][j].id = j;
        }
    }
    for (int i = 1; i <= m * n; i++) {
        int now = list[i];
        step[now]++;//step代表进行到的工序数
        int id = a[now][step[now]].id, cost = a[now][step[now]].cost;
        int s = 0;
        for (int j = last_time[now] + 1; ; j++) {
            if (machine[id][j] == 0) {//代表此机器已经被处理过
                s++;
            }
            else {//此机器没有处理过
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
    if (ans < anstime) {//如果此领域解比当前解法要优秀(ans<anstime)，那么接受此解。
        anstime = ans;
        if (ansfinal > ans) {//如果此领域解甚至由于最优解，那么将其法制给最优解(ans_state_next->ans_statef)
            ansfinal = ans;
            flagfinal = true;
            for (int i = 0; i < n; i++) {
                ans_statef[i] = ans_state_next[i];
            }
        }
        for (int i = 0; i < n; i++) {//将此领域解赋值给当前解(ans_state_next->ans_state)。
            ans_state[i] = ans_state_next[i];
        }
    }
    else {//如果此领域解比当前解法要优秀(ans<anstime)，那么以exp(-dE/T)的概率接受此解。
        double delta = (anstime - ans) / T;
        if (b < exp(delta)) {
            anstime = ans;
            for (int i = 0; i < n; i++) {
                ans_state[i] = ans_state_next[i];
            }
        }
    }
    int i = rand() % n, j = rand() % n;//i,j为随机产生的(0,n)的随机数
    swap(i, j);//ans_state交换i,j对应的坐标的数，得到领域解=
}
int cnt = 0;
void sa(int n, double T) {//模拟退火算法
    shuffle(n);//调用shuffle算法产生长度为n的一个随机的初始解
    for (int i = 0; i < n; i++) {//将产生随机解赋予初始态
        ans_state_next[i] = randm[i];
    }
    while (T > Tmin) {//当T下降到小于等于Tmin时，模拟退火算法结束
        cnt += 1;
        solve(T);
        T *= fac;//每次T以衰减因子fac的速度下降
    }
}
int main()
{
    cin >> n >> m;//n个工件,m个机器
    for (int i = 1; i <= n; i++) {//每个工件每个工序的加工时间
        for (int j = 1; j <= m; j++) {
            int uus;
            cin >> uus >> a[i][j].cost;//uus代表工件序号,a[i][j].cost代表第j个工件在第i个机器上加工的时间
        }
    }
    for (int i = 0; i < 100; i++) {//多次重复实验(重复实验100次)
        double T = T0;//每次实验将温度初始化为T0
        clock_t start, end;//每次实验定义起始时间和终止时间
        start = clock();
        cnt = 0;//cnt代表单次实验的退火次数
        sa(n,T);
        cout << "Count:" << cnt << endl;
        printf("anstime: %d\n ansstate: ", anstime);
        for (int i = 0; i < n; i++) {
            cout << ans_state[i] - 1 << " ";
        }
        cout << endl;
        end = clock();
        double t = (double)(end - start) / CLK_TCK;//计算单词实验用时
        printf("time=%lf\n\n", t);
        if (flagfinal ==true) {//如果此解比之前求得的最优解要好，在solve函数中会将flagfinal标记为true，在flagfinal为true的提示下，将此解保留到最优解
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