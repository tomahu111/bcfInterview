#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <array>
class Matrice
{
private:
	std::vector<int> dimensions;
	std::unique_ptr<int[]> numbers;
	int totalUnits;
public:
	Matrice(std::vector<int> dims, int data[]);
	Matrice(const Matrice& m);
	~Matrice();

	Matrice operator+=(const Matrice& m)
	{
		if (dimensions != m.dimensions)
		{
			throw "non adequate sizes of matrices";
		}
		else
		{
			/*std::thread t1([](const int& totalUnits, std::unique_ptr<int[]> numbers, std::unique_ptr<int[]> m.numbers) {
				for (int i = 0; i < totalUnits; i++)
				{
					numbers[i] += m.numbers[i];
				}
				});*/
			for (int i = 0; i < totalUnits; i++)
			{
				numbers[i] += m.numbers[i];
			}
		}
	}
	Matrice operator+(const Matrice& m2)
	{
		Matrice m3 = m2;
		if (dimensions != m2.dimensions)
		{
			throw "non adequate sizes of matrices";
		}
		else
		{
			
			for (int i = 0; i < m2.totalUnits; i++)
			{
				m3.numbers[i] += numbers[i];
			}
		}
		return m3;
	}
	
	Matrice operator-=(const Matrice& m)
	{
		if (dimensions != m.dimensions)
		{
			throw "non adequate sizes of matrices";
		}
		else
		{
			/*std::thread t1([](const int& totalUnits, std::unique_ptr<int[]> numbers, std::unique_ptr<int[]> m.numbers) {
				for (int i = 0; i < totalUnits; i++)
				{
					numbers[i] += m.numbers[i];
				}
				});*/
			for (int i = 0; i < totalUnits; i++)
			{
				numbers[i] -= m.numbers[i];
			}
		}
	}
	Matrice operator-(const Matrice& m2)
	{
		Matrice m3 = m2;
		if (dimensions != m2.dimensions)
		{
			throw "non adequate sizes of matrices";
		}
		else
		{

			for (int i = 0; i < m2.totalUnits; i++)
			{
				m3.numbers[i] -= numbers[i];
			}
		}
		return m3;
	}

	Matrice operator=(const Matrice& m)
	{
		if (dimensions != m.dimensions)
		{
			throw "non adequate sizes of matrices";
		}
		else
		{
			for (int i = 0; i < totalUnits; i++)
			{
				numbers[i] = m.numbers[i];
			}
		}
	}
	Matrice operator=(int* numbers)
	{
		for (int i = 0; i < totalUnits; i++)
		{

		}
	}
};

Matrice::Matrice(std::vector<int> dims, int data[])
{
	this->dimensions = dims;
	this->totalUnits = 1;
	for (int currDimension = 0; currDimension < dimensions.size(); currDimension++)
	{
		totalUnits = totalUnits * dimensions[currDimension];
	}
	this->numbers = std::make_unique<int[]>(totalUnits);

	for (int i = 0; i < totalUnits; i++)
	{
		numbers[i] = data[i];
		/*try {

		}
		catch () {
			std::cout << "Wrong data give, matrice cannot be created";
			this->~Matrice();
			break;
		}*/
	}
}
Matrice::Matrice(const Matrice& m) 
{
	this->dimensions = m.dimensions;
	this->totalUnits = m.totalUnits;
	this->numbers = std::make_unique<int[]>(totalUnits);

	for (int i = 0; i < totalUnits; i++)
	{
		numbers[i] = m.numbers[i];
	}
}
Matrice::~Matrice()
{
}

int main()
{
    std::cout << "Hello World!\n";
}
