#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int N = 100;         // nombre d'itérations du recuit simulé
const int T = 100;         // température initiale
const double alpha = 0.99; // taux de refroidissement
const double T_min = 1e-8; // température minimale

int n;           // nombre de tâches
int K;           // nombre de moules
const int m = 2; // nombre de machines parallèles identiques

vector<int> d;    // durées des tâches
vector<int> mold; // moules associés aux tâches
vector<int> moldresv = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
vector<int> t; // ordre de traitement des tâches
int makespan;
int sum;

int random(int min, int max)
{
    return min + rand() % (5);
}

int calculate_makespan()
{
    vector<vector<int>> f(K, vector<int>(m, 0)); // tableau de fin des tâches sur chaque machine
    for (int i = 0; i < n; i++)
    {
        int machine = t[i] % m;
        int moule = mold[t[i]];
        int start_time = f[moule][machine];
        f[moule][machine] = start_time + d[t[i]];
    }
    int max_f = 0;
    for (int k = 0; k < K; k++)
    {
        for (int j = 0; j < m; j++)
        {
            max_f = max(max_f, f[k][j]);
        }
    }
    return max_f;
}

void random_permutation()
{
    for (int i = 0; i < n; i++)
    {
        t[i] = i;
    }
    for (int i = 0; i < n; i++)
    {
        int j = random(i, n - 1);
        swap(t[i], t[j]);
        swap(d[i], d[j]);
    }
}

void simulated_annealing()
{
    double T_cur = T;

    int makespan_cur = calculate_makespan();
    makespan = makespan_cur;
    while (T_cur > T_min)
    {
        for (int i = 0; i < N; i++)
        {
            int j = random(0, n - 1);
            int k = random(0, n - 1);
            swap(t[j], t[k]);
            int makespan_new = calculate_makespan();
            int delta = makespan_new - makespan_cur;
            if (delta < 0 || exp(-delta / T_cur) > (double)random(0, 1000) / 1000)
            {
                makespan_cur = makespan_new;
                if (makespan_cur < makespan)
                {
                    makespan = makespan_cur;
                }
            }
            else
            {
                swap(t[j], t[k]);
                swap(d[i], d[j]);
            }
        }
        T_cur *= alpha;
    }
}

void lecture_Fichier()
{

    ifstream fichier("test.txt");
    K = 2;
    if (fichier.is_open())
    {
        string ligne;
        int i = 1;
        int k;
        int n = 0;
        while (getline(fichier, ligne))
        {

            if (i % 3 == 0)
            {
                string word;
                n = 0;
                int h = 1;
                std::istringstream iss(ligne);

                while (iss >> word)
                {
                    int num = std::stoi(word);
                    n++;
                    t.push_back(h);
                    d.push_back(num);
                    h++;
                }
            }
            if (i % 4 == 0)
            {
                string word;
                std::istringstream iss(ligne);
                while (iss >> word)
                {
                    int num = std::stoi(word);
                    mold.push_back(num);
                }
                i = 0;
                t.resize(n);

                random_permutation();

                // Recherche de la solution optimale avec le recuit simulé

                auto start = high_resolution_clock::now();

                simulated_annealing();

                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                cout << "Temps d'execution : " << duration.count() << " nanosecondes" << endl;

                int j = 0, s = 0;
                vector<int> m2;
                vector<int> m1;
                int t1 = 0, t2 = 0;

                for (int i = 0; i < n; i++)
                {
                    {
                        if (i == 0)
                        {
                            m1.push_back(t[i]);
                            t1 += d[i];
                            moldresv[mold[i]] = t1;
                        }
                        else
                        {

                            if (t1 < t2)
                            {
                                m1.push_back(t[i]);
                                j++;
                                if (moldresv[mold[i]] <= t1)
                                {
                                    t1 += d[i];
                                }
                                else
                                {
                                    t1 += t2 + d[i];
                                }
                                moldresv[mold[i]] = t1;
                            }
                            else
                            {

                                m2.push_back(t[i]);
                                s++;
                                if (moldresv[mold[i]] <= t2)
                                {
                                    t2 += d[i];
                                }
                                else
                                {
                                    t2 += t1 + d[i];
                                }
                                moldresv[mold[i]] = t1;
                            }
                        }
                    }
                }

                cout << "Machine M1 : ";
                for (int i = 0; i < j; i++)
                {
                    cout << m1[i] << " ";
                }

                cout << "\nMachine M2 : ";
                for (int i = 0; i < s; i++)
                {
                    cout << m2[i] << " ";
                }

                cout << "\nMakeSpan est : " << max(t1, t2) << "\n";
                cout << endl;

                d.clear();
                mold.clear();
            }
            i++;
        }

        fichier.close();
    }
    else
    {
        std::cout << "Erreur lors de l'ouverture du fichier" << std::endl;
    }
}

int main()
{

    lecture_Fichier();

    return 0;
}
