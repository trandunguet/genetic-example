#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

int POPULATION_SIZE = 500;
std::vector<std::pair<int, int>> dataset;

class Individual
{
public:
    std::vector<int> chromosome;
    int fitness;

    Individual(int chromosomeLength)
    {
        for (int i = 0; i < chromosomeLength; i++)
        {
            chromosome.push_back(rand() % 60 - 30);
        }
        calculateFitness();
    }

    int calculateFitness()
    {
        // Fitness = total error on the whole dataset. The lower the better.
        fitness = 0;
        for (std::pair<int, int>& data : dataset)
        {
            int y = 0;
            for (int i = 0; i < chromosome.size(); i++)
            {
                y += chromosome[i] * pow(data.first, i);
            }
            fitness += abs(data.second - y);
        }
    }

    void mutate()
    {
        int position = rand() % chromosome.size();
        chromosome[position] += rand() % 3 - 1;
        calculateFitness();
    }

    // Crossover: child = mommy + daddy
    Individual operator+(const Individual& partner) const
    {
        Individual child1(*this), child2(partner);
        int crossOverPoint = rand() % (chromosome.size() + 1);
        for (int i = 0; i < crossOverPoint; i++)
        {
            child1.chromosome[i] = partner.chromosome[i];
            child2.chromosome[i] = this->chromosome[i];
        }
        child1.calculateFitness();
        child2.calculateFitness();
        child1.mutate();
        child2.mutate();
        if (child1.fitness > child2.fitness)
            return child1;
        else
            return child2;
    }

    // For sorting
    bool operator<(const Individual& other) const
    {
        return this->fitness < other.fitness;
    }

    bool operator==(const Individual& other) const
    {
        return this->chromosome == other.chromosome;
    }

    void show()
    {
        for (int& gene: chromosome)
        {
            std::cout << gene << " ";
        }
        std::cout << "fitness: " << fitness << std::endl;
    }
};

int main()
{
    // Input
    std::ifstream input("genetic.inp");
    if (!input.is_open())
    {
        std::cout << "genetic.inp not found. Please create one" << std::endl;
        return 0;
    }
    int CHROMOSOME_LENGTH;
    input >> CHROMOSOME_LENGTH;
    while (!input.eof())
    {
        int x, y;
        input >> x >> y;
        dataset.push_back(std::pair<int, int>(x, y));
    }

    // Initialize
    srand(time(NULL));
    std::vector<Individual> population;
    for (int i = 0; i < POPULATION_SIZE; i++)
        population.push_back(Individual(CHROMOSOME_LENGTH + 1));

    // Evolving
    while (true)
    {
        if (population[0].fitness == 0) break;
        for (int i = 0; i < POPULATION_SIZE; i++)
        {
            Individual& mom = population[rand() % (population.size())];
            Individual& dad = population[rand() % (population.size())];
            if (mom == dad) continue;
            Individual child = mom + dad;
            population.push_back(child);
        }
        std::sort(population.begin(), population.end());
        population[0].show();
        while (population.size() > POPULATION_SIZE)
            population.pop_back();
    }
}