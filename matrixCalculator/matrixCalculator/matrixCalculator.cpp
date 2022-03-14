//program made by Tomasz Hresiukiewicz for bcf recruitment, to cover 
// n-dimensional matrixes and written using only stl libs, as requested by handbook
#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <stdexcept>
//helper function for casting multi dimensional arrays position into single dimensional one
int calculatePlace(const std::vector<int>& dimensions, std::vector<int>& pos);



class Matrice
{
private:
	std::vector<int> dimensions;
	std::unique_ptr<int[]> numbers;
	int totalUnits;
	//helping function for matrix multiplication
	bool moveRows(const std::vector<int>& dimsArr, std::vector<int>& rowArr)
	{
		for (int i = dimsArr.size() - 1; i >= 0; i--)
		{
			if (i == 0 && rowArr[i] == dimsArr[i] - 1)
			{
				return true;
			}
			else if (rowArr[i] == dimsArr[i]-1)
			{
				rowArr[i] = 0;
			}
			else
			{
				rowArr[i]++;
				return false;
			}
			
		}
		return false;
	}
	//helping function for matrix multiplication
	bool moveCollumns(const std::vector<int>& dimsArr, std::vector<int>& colArr)
	{
		if (colArr[dimsArr.size()-2] == dimsArr[dimsArr.size() - 2]-1)
		{
			colArr[dimsArr.size() - 2] = 0;
		}
		else
		{
			colArr[dimsArr.size() - 2]++;
			return false;
		}
		if (colArr[dimsArr.size() - 1] == dimsArr[dimsArr.size() - 1]-1)
		{
			colArr[dimsArr.size() - 1] = 0;
		}
		else
		{
			colArr[dimsArr.size() - 1]++;
			return false;
		}
		for (int i = dimsArr.size() - 3; i >= 0; i--)
		{
			if (i == 0 && colArr[i] == dimsArr[i] - 1)
			{
				return true;
			}
			else if (colArr[i] == dimsArr[i]-1)
			{
				colArr[i] = 0;
			}
			else
			{
				colArr[i]++;
				break;
			}
			
		}
		return false;
	}
public:
	Matrice(std::vector<int>& dims, const int data[]);
	Matrice(std::vector<int>& dims, std::unique_ptr<int[]> data);
	Matrice(std::vector<int>& dims, std::vector<int> data);
	Matrice(const Matrice& m);
	~Matrice();
	int operator[](std::vector<int>& pos)
	{
		int res = 0;
		int localTotal = totalUnits;
		if (pos.size() != dimensions.size())
		{
			throw(std::out_of_range("Non apropriate many dimensions given!"));
		}
		for (int i = 0; i < pos.size(); i++)
		{
			if (pos[i] > dimensions[i])
			{
				throw(std::out_of_range("Given arguments are incompatible with matrixes dimensions!"));
			}
			localTotal = localTotal / dimensions[i];
			res += localTotal * pos[i];
		}
		return numbers[res];
	}


	Matrice operator+=(const Matrice& m)
	{
		if (dimensions != m.dimensions)
		{
			throw "non adequate sizes of matrices";
		}
		else
		{
			for (int i = 0; i < totalUnits; i++)
			{
				numbers[i] += m.numbers[i];
			}
		}
		return *this;
	}
	Matrice operator+=(int m)
	{
		for (int i = 0; i < totalUnits; i++)
		{
			numbers[i] += m;
		}
		return *this;
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
		return *this;
	}
	Matrice operator-=(int m)
	{
		for (int i = 0; i < totalUnits; i++)
		{
			numbers[i] -= m;
		}
		return *this;
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

	Matrice operator*(const Matrice& m2)
	{
		std::vector<int> resDimensions;
		std::vector<int> currRow(2, 0);
		std::vector<int> currCol(2, 0);
		int resTotal = 1;
		int temp;
		int currResElement;
		int rowSize = dimensions[dimensions.size() - 1];
		bool done=false;
		if (dimensions.size() != m2.dimensions.size() || dimensions[dimensions.size()-1] != m2.dimensions[m2.dimensions.size()-2])
		{
			throw "non adequate sizes of matrices";
		}
		for (int i = 0; i < dimensions.size() - 2; i++)
		{
			if (dimensions[i] != m2.dimensions[i])
			{
				throw "non adequate sizes of matrices";
			}
			else
			{
				resDimensions.push_back(dimensions[i]);
				resTotal *= dimensions[i];
			}
			currRow.push_back(0);
			currCol.push_back(0);
		}
		resDimensions.push_back(m2.dimensions[m2.dimensions.size() - 1]);
		resTotal *= m2.dimensions[m2.dimensions.size() - 1];
		resDimensions.push_back(dimensions[dimensions.size() - 2]);
		resTotal *= dimensions[dimensions.size() - 2];
		std::unique_ptr<int[]> resNumbers = std::make_unique<int[]>(resTotal);
		
		currResElement = 0;
		while (!done)
		{
			try
			{
				temp = 0;
				for (int i = 0; i < m2.dimensions[m2.dimensions.size() - 1]; i++)
				{
					temp += numbers[calculatePlace(dimensions, currRow)] * m2.numbers[calculatePlace(m2.dimensions, currCol)];
					done = moveCollumns(m2.dimensions, currCol);
				}
				done = moveRows(dimensions, currRow);
				resNumbers[currResElement] = temp;
				currResElement++;
			}
			catch (const std::exception& e) {
				std::cout << "Caught exception \"" << e.what() << "\"\n";
			}
		}
		return Matrice(resDimensions, std::move(resNumbers));

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
	

	void printMatrix()
	{
		for (int i = 0; i < totalUnits; i++)
		{
			std::cout << numbers[i] << " ";
		}
		std::cout << std::endl;
	}
	void printMatrixInfo()
	{
		std::cout << dimensions.size() << " dimensional matrix, dimensions sizes (descending dimension order):" << std::endl;
		for (int i = 0; i < dimensions.size(); i++)
		{
			std::cout << dimensions[i] << " ";
		}
		std::cout << std::endl;
	}
};

Matrice::Matrice(std::vector<int> &dims, const int data[])
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
Matrice::Matrice(std::vector<int>& dims, std::unique_ptr<int[]> data)
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
Matrice::Matrice(std::vector<int>& dims, std::vector<int> data)
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


void printMenu();

void createMatrix(std::vector<Matrice>& matrixList);
void addMatrixes(std::vector<Matrice>& matrixList);
void subtractMatrixes(std::vector<Matrice>& matrixList);
void multiplyMatrixes(std::vector<Matrice>& matrixList);
void printMatrixesData(std::vector<Matrice>& matrixList);
void printMatrix(std::vector<Matrice>& matrixList);

int main()
{
	int userInpt;
	std::vector<Matrice> matrices;
	do
	{
		printMenu();
		std::cin >> userInpt;
		if (userInpt == 0)
		{
			std::cout << "Thanks for using this program" << std::endl;
		}
		else if (userInpt == 1)
		{
			createMatrix(matrices);
		}
		else if (userInpt == 2)
		{
			addMatrixes(matrices);
		}
		else if (userInpt == 3)
		{
			subtractMatrixes(matrices);
		}
		else if (userInpt == 4)
		{
			multiplyMatrixes(matrices);
		}
		else if (userInpt == 5)
		{
			printMatrixesData(matrices);
		}
		else if (userInpt == 6)
		{
			printMatrix(matrices);
		}
		else
		{
			std::cout << "Please provide proper input" << std::endl;
			continue;
		}
	} while (userInpt!=0);
}

int calculatePlace(const std::vector<int> &dimensions, std::vector<int> &pos)
{
	int res = 0;
	int localTotal = 1;
	for (int currDimension = 0; currDimension < dimensions.size(); currDimension++)
	{
		localTotal = localTotal * dimensions[currDimension];
	}
	if (pos.size() != dimensions.size())
	{
		throw(std::out_of_range("Non apropriately many dimensions given!"));
	}
	for (int i = 0; i < pos.size(); i++)
	{
		if (pos[i] > dimensions[i])
		{
			throw(std::out_of_range("Given arguments are incompatible with matrixes dimensions!"));
			return 0;
		}
		localTotal = localTotal / dimensions[i];
		res += localTotal * pos[i];
	}
	return res;
}

void printMenu()
{
	std::cout << "0 - leave the program\n"
		"1 - enter a matrix, it will be added to all matrixes list\n"
		"2 - add 2 matrixes, will require to type their indexes and them to be the same size\n"
		"3 - subtract 2 matrixes, will require to type their indexes and them to be the same size\n"
		"4 - multiply 2 matrixes, will require to type their indexes, them to have at least 2"
		"dimensions and for the second dimension from the back of the first matrix be the same"
		"size as the last dimension of the second one \n"
		"5 - print number of currently saved matrixes and their dimensions sizes\n"
		"6 - print whole matrix content, will require passing matrixes number\n";
}

void createMatrix(std::vector<Matrice>& matrixList)
{
	int userInpt;
	int temp;
	std::cout << "1 - input matrix from start (will require number of "
		"dimensions, their sizes and every element of the matrix)\n"
		"2 - just copy another matrix" << std::endl;
	std::cin >> userInpt;
	if (userInpt == 1)
	{
		int dimNum;
		int howManyNums=1;
		std::vector<int> dims;
		std::vector<int> nums;
		std::cout << "enter dimensions number" << std::endl;
		std::cin >> dimNum;
		for (int i = 0; i < dimNum; i++)
		{
			std::cout << "enter number of elements in " << i + 1 << "nth dimension ";
			std::cin >> temp;
			if (temp == 0)
			{
				std::cout << "dimension can't have size 0" << std::endl;
				return;
			}
			howManyNums = howManyNums * temp;
			dims.push_back(temp);
		}
		std::cout << "enter " << howManyNums << "numbers now" << std::endl;
		for (int i = 0; i < howManyNums; i++)
		{
			std::cin >> temp;
			nums.push_back(temp);
		}
		matrixList.push_back(Matrice(dims, nums));
		return;
	}
	else if (userInpt == 2)
	{
		int matrixNum;
		std::cout << "enter matrixes number" << std::endl;
		std::cin >> matrixNum;
		if (matrixNum >= 0 && matrixNum < matrixList.size())
		{
			matrixList.push_back(Matrice(matrixList[matrixNum]));
			return;
		}
	}
	std::cout << "wrong input" << std::endl;
	
}

void addMatrixes(std::vector<Matrice>& matrixList)
{
	int userInpt;
	int firstMatrix;
	int secondMatrix;
	std::cout << "1 - increment the first matrix as a result\n"
		"2 - save the result as a new matrix\n"
		"3 - just print the result" << std::endl;
	std::cin >> userInpt;
	std::cout << "type in 2 matrix indexes" << std::endl;
	std::cin >> firstMatrix >> secondMatrix;
	if ((firstMatrix >= 0 && secondMatrix >= 0) && (firstMatrix < matrixList.size() && secondMatrix < matrixList.size()))
	{
		if (userInpt == 1)
		{
			matrixList[firstMatrix] += matrixList[secondMatrix];
			return;
		}
		else if (userInpt == 2)
		{
			matrixList.push_back(matrixList[firstMatrix] + matrixList[secondMatrix]);
			return;
		}
		else if(userInpt == 3)
		{
			Matrice(matrixList[firstMatrix] + matrixList[secondMatrix]).printMatrix();
			return;
		}
	}
	std::cout << "wrong input" << std::endl;
	
}

void subtractMatrixes(std::vector<Matrice>& matrixList)
{
	int userInpt;
	int firstMatrix;
	int secondMatrix;
	std::cout << "1 - decrement the first matrix as a result\n"
		"2 - save the result as a new matrix\n"
		"3 - just print the result" << std::endl;
	std::cin >> userInpt;
	std::cout << "type in 2 matrix indexes" << std::endl;
	std::cin >> firstMatrix >> secondMatrix;
	if ((firstMatrix >= 0 && secondMatrix >= 0) && (firstMatrix < matrixList.size() && secondMatrix < matrixList.size()))
	{
		if (userInpt == 1)
		{
			matrixList[firstMatrix] -= matrixList[secondMatrix];
			return;
		}
		else if (userInpt == 2)
		{
			matrixList.push_back(matrixList[firstMatrix] - matrixList[secondMatrix]);
			return;
		}
		else if (userInpt == 3)
		{
			Matrice(matrixList[firstMatrix] - matrixList[secondMatrix]).printMatrix();
			return;
		}
	}
	std::cout << "wrong input" << std::endl;
}

void multiplyMatrixes(std::vector<Matrice>& matrixList)
{
	int userInpt;
	int firstMatrix;
	int secondMatrix;
	std::cout << "1 - save the result as a new matrix\n"
		"2 - just print the result" << std::endl;
	std::cin >> userInpt;
	std::cout << "type in 2 matrix indexes" << std::endl;
	std::cin >> firstMatrix >> secondMatrix;
	if ((firstMatrix >= 0 && secondMatrix >= 0) && (firstMatrix < matrixList.size() && secondMatrix < matrixList.size()))
	{
		if (userInpt == 1)
		{
			matrixList.push_back(matrixList[firstMatrix] * matrixList[secondMatrix]);
			return;
		}
		else if (userInpt == 2)
		{
			Matrice(matrixList[firstMatrix] * matrixList[secondMatrix]).printMatrix();
			return;
		}
	}
	std::cout << "wrong input" << std::endl;
}

void printMatrixesData(std::vector<Matrice>& matrixList)
{
	std::cout << "there are " << matrixList.size() << " matrices currently" << std::endl << std::endl;
	for (int i = 0; i < matrixList.size(); i++)
	{
		std::cout << "matrix number " << i << " is ";
		matrixList[i].printMatrixInfo();
		std::cout << "--------------------------------------------------------" <<
			std::endl;
	}
}

void printMatrix(std::vector<Matrice>& matrixList)
{
	int userInpt;
	std::cout << "enter number of matrix you want to see" << std::endl;
	std::cin >> userInpt;
	if (userInpt >= 0 && userInpt < matrixList.size())
	{
		matrixList[userInpt].printMatrixInfo();
		std::cout << "matrix contents:" << std::endl;
		matrixList[userInpt].printMatrix();
		return;
	}
	std::cout << "wrong input" << std::endl;
}
