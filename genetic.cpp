#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

class Individual
{
public:
    std::vector<int> chromosome;
    int fitness;

    Individual()
    {
        for (int i = 0; i < 5; i++)
        {
            chromosome.push_back(rand() % 2);
        }
        calculateFitness();
    }

    int calculateFitness()
    {
        fitness = 0;
        for (int& gene : chromosome)
        {
            fitness += gene;
        }
    }

    void mutate()
    {
        if (rand() % 3 != 0) return;
        int position = rand() % chromosome.size();
        chromosome[position] = rand() % 2;
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
        return this->fitness > other.fitness;
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
    // Initialize
    srand(time(NULL));
    std::vector<Individual> population;
    int POPULATION_SIZE = 10;
    for (int i = 0; i < POPULATION_SIZE; i++)
        population.push_back(Individual());
    std::sort(population.begin(), population.end());

    // Evolving
    // while (true)
    for (int c = 0; c < 10; c++)
    {
        for (Individual& i: population)
        {
            i.show();
        }
        std::cout << std::endl;
        for (int i = 0; i < POPULATION_SIZE * 2 / 3; i++)
        {
            Individual& mom = population[rand() % (population.size() / 2)];
            Individual& dad = population[rand() % (population.size() / 2)];
            if (mom == dad) continue;
            Individual child = mom + dad;
            population.push_back(child);
        }
        std::sort(population.begin(), population.end());
        while (population.size() > POPULATION_SIZE)
            population.pop_back();
    }
}