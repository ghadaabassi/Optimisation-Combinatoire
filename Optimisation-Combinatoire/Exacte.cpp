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

struct Job
{
    int id;
    int duration;
    int mold;
};
struct Machine
{
    int id;
    int date = 0;
    vector<Job> jobs;
};

int mold[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool compare_jobs(const Job &job1, const Job &job2)
{
    return job1.duration < job2.duration;
}

vector<Job> trie_Jobs(vector<Job> jobs)
{

    sort(jobs.begin(), jobs.end(), compare_jobs);
    return jobs;
}

vector<Job> trie_Jobs_Johnson(vector<Job> jobs)
{

    sort(jobs.begin(), jobs.end(), compare_jobs);
    vector<Job> final;
    vector<Job> d;
    vector<Job> g;
    int i = 0;
    int t1 = 0;
    int t2 = 0;

    for (const auto &job : jobs)
    {
        if (i % 2 == 0)
        {
            d.push_back(job);
        }
        else
        {
            g.push_back(job);
        }
    }
    for (const auto &job : g)
    {
        final.push_back(job);
    }

    for (int i = d.size() - 1; i >= 0; i--)
    {
        final.push_back(d[i]);
    }

    return final;
}

void Johnson(vector<Job> jobs, Machine m1, Machine m2)
{

    vector<Job> j = trie_Jobs_Johnson(jobs);
    for (const auto &job : jobs)
    {
        if (job.id == 1)
        {
            m1.jobs.push_back(job);
            m1.date += job.duration;
            mold[job.mold] = m1.date;
        }
        else
        {

            if (m1.date < m2.date)
            {
                m1.jobs.push_back(job);
                if (mold[job.mold] <= m1.date)
                {
                    m1.date += job.duration;
                }
                else
                {
                    m1.date = m2.date + job.duration;
                }
                mold[job.mold] = m1.date;
            }
            else
            {
                m2.jobs.push_back(job);
                if (mold[job.mold] <= m2.date)
                {
                    m2.date += job.duration;
                }
                else
                {
                    m2.date = m1.date + job.duration;
                }
                mold[job.mold] = m2.date;
            }
        }
    }

    cout << "Tâches de machine M1 : ";
    for (const auto &job : m1.jobs)
    {
        cout << job.id << " ";
    }
    cout << endl;

    cout << "Tâches de machine M2 : ";
    for (const auto &job : m2.jobs)
    {
        cout << job.id << " ";
    }

    cout << endl;

    cout << "Temps total : ";
    if (m1.date > m2.date)
    {
        cout << m1.date << " ";
    }
    else
    {
        cout << m2.date << " ";
    }
    cout << endl;
}

void lecture_Fichier()
{
    // ifstream fichier("instances-A.txt");
    ifstream fichier("test.txt");

    if (fichier.is_open())
    {
        string ligne;
        int i = 1;

        vector<Job> j1;
        while (getline(fichier, ligne))
        {

            if (i % 3 == 0)
            {
                string word;

                std::istringstream iss(ligne);
                int k = 1;
                while (iss >> word)
                {
                    int num = std::stoi(word);
                    Job j;
                    j.id = k;
                    k++;
                    j.duration = num;
                    j1.push_back(j);
                }
            }

            if (i % 4 == 0)
            {
                string word;
                std::istringstream iss(ligne);
                int k = 0;
                while (iss >> word)
                {
                    int num = std::stoi(word);
                    j1[k].mold = num;
                    k++;
                }
                i = 0;
                std::cout << "\n ";
                std::cout << std::endl;
                Machine m1 = {1, 0, {}};
                Machine m2 = {2, 0, {}};

                std::cout << "Johnson : \n ";
                auto start = high_resolution_clock::now();
                Johnson(j1, m1, m2);

                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                cout << "Temps d'execution : " << duration.count() << " nanosecondes" << endl;

                j1.clear();
            }
            i++;
        }

        fichier.close(); // Fermeture du fichier
    }
    else
    {
        std::cout << "Erreur lors de l'ouverture du fichier" << std::endl;
    }
}

int main()
{

    Machine m1 = {1, 0, {}};
    Machine m2 = {2, 0, {}};
    lecture_Fichier();

    return 0;
}
