#include <iostream>
#include <tuple>
#include <optional>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <ranges>
#include <algorithm>

using namespace std;

//Task 1 -------------------------------------------------------------------------------------------------

class Person
{
private:
	std::string last_name;
	std::string first_name;
	std::optional<std::string> patronimic;
public:
	Person();


	Person(std::string l_name, std::string f_name, std::string patr);

	std::string GetFirstName() const;

	std::optional<std::string> GetPatronimic() const;

	std::string GetLastName() const;

	static Person ReadFromStream(std::istringstream& iss);

	void SetPerson(std::istringstream& iss);

	friend std::ostream& operator << (std::ostream& out, const Person& person);

	friend bool operator< (const Person& p1, const Person& p2);

	friend bool operator== (const Person& p1, const Person& p2);

};
Person::Person() :last_name(""), first_name(""), patronimic(std::nullopt) {}


Person::Person(std::string l_name, std::string f_name, std::string patr) : last_name{ std::move(l_name) }, first_name{ std::move(f_name) }, patronimic{ std::move(patr) } {}

std::string Person::GetFirstName() const { return first_name; }

std::optional<std::string> Person::GetPatronimic() const { return patronimic; }

std::string Person::GetLastName() const { return last_name; }

Person Person::ReadFromStream(std::istringstream& iss)
{
	Person person;
	std::string temp;
	if (iss.peek() == ' ')
		iss.ignore(1);
	iss >> person.last_name >> person.first_name >> temp;
	if (temp != "-")
		person.patronimic.emplace(temp);
	return person;
}

void Person::SetPerson(std::istringstream& iss)
{
	std::string temp;
	iss >> last_name >> first_name >> temp;
	if (temp == "-")
		return;
	else
		patronimic.emplace(temp);
}


std::ostream& operator<< (std::ostream& out, const Person& person)
{
	out << person.last_name << ' ' << person.first_name;
	if (person.patronimic.has_value())
		out << ' ' << person.patronimic.value();
	return out;
}

bool operator< (const Person& p1, const Person& p2) {
	return std::tie(p1.last_name, p1.first_name, p1.patronimic) <
		std::tie(p2.last_name, p2.first_name, p2.patronimic);
}

bool operator== (const Person& p1, const Person& p2) {
	return std::tie(p1.last_name, p1.first_name, p1.patronimic) ==
		std::tie(p2.last_name, p2.first_name, p2.patronimic);
}

//Task 2 --------------------------------------------------------------------------------------------------------------

class PhoneNumber
{
private:
	int country_code;
	int city_code;
	std::string number;
	std::optional<int> add_number;
public:
	PhoneNumber();

	PhoneNumber(int country, int city, std::string num, std::optional<int> add_num);

	

	int GetCountryCode() const;

	int GetCityCode() const;

	std::string GetNumber() const;

	std::optional<int> GetAddNumber() const;

	static PhoneNumber ReadFromStream(std::istringstream& iss);

	void SetPhoneNumber(std::istringstream& iss);

	friend std::ostream& operator<< (std::ostream& out, const PhoneNumber& phone_number);

	friend bool operator< (const PhoneNumber& num1, const PhoneNumber& num2);

};

PhoneNumber::PhoneNumber() : country_code(0), city_code(0), number("0000000"), add_number(std::nullopt) {}

PhoneNumber::PhoneNumber(int country, int city, std::string num, std::optional<int> add_num) : country_code{ country }, city_code{ city }, number{ std::move(num) }, add_number{ add_num } {}



int PhoneNumber::GetCountryCode() const
{
	return country_code;
}

int PhoneNumber::GetCityCode() const
{
	return city_code;
}

std::string PhoneNumber::GetNumber() const
{
	return number;
}

std::optional<int> PhoneNumber::GetAddNumber()
const
{
	if (const int add_num = add_number.value())
		return add_num;
	else
		return add_number;
}

PhoneNumber PhoneNumber::ReadFromStream(std::istringstream& iss)
{
	PhoneNumber phonenumb;
	if (iss.peek() == ' ')
		iss.ignore(1);
	iss >> phonenumb.country_code >> phonenumb.city_code >>
		phonenumb.number;
	if (iss.peek() != '-') {
		iss >> phonenumb.add_number.emplace();
	}
	return phonenumb;
}

void PhoneNumber::SetPhoneNumber(std::istringstream& iss)
{
	std::string temp;
	iss >> country_code >> city_code >> number >> temp;

	if (temp == "-")
		return;
	else
		add_number.emplace(std::stoi(temp));
}

std::ostream& operator<< (std::ostream& out, const PhoneNumber& phone_number)
{
	out << '+' << phone_number.GetCountryCode() << " (" << phone_number.GetCityCode() << ") " << phone_number.GetNumber() << ' ';
	if (phone_number.add_number.has_value())
	{
		out << phone_number.add_number.value();
	}
	return out;
}

bool operator< (const PhoneNumber& num1, const PhoneNumber& num2)
{
	return std::tie(num1.country_code, num1.city_code, num1.number, num1.add_number) < std::tie(num2.country_code, num2.city_code, num2.number, num2.add_number);
}

//Task 3 -----------------------------------------------------------------------------------------------------------------

class PhoneBook
{
private:
	std::string string;
	std::vector<std::pair<Person, PhoneNumber>> phone_book;
public:
	PhoneBook(std::ifstream& file);

	~PhoneBook();

	void FileObj(std::ifstream& file);


	void SortByName();

	void SortByPhone();

	std::tuple<std::string, PhoneNumber> GetPhoneNumber(const std::string& surname);

	void ChangePhoneNumber(const Person& person, PhoneNumber number);

	friend std::ostream& operator<< (std::ostream& out, PhoneBook& book);
};

PhoneBook::PhoneBook(std::ifstream& file)
{
	FileObj(file);
	if (!file)
		std::cerr << "Cannot open the file\n";
}

PhoneBook::~PhoneBook() {}

void PhoneBook::FileObj(std::ifstream& file)
{
	while (!file.eof())
	{
		std::getline(file, string);
		std::istringstream istringstr(string);
		Person person;
		PhoneNumber number;
		person.SetPerson(istringstr);
		number.SetPhoneNumber(istringstr);
		phone_book.push_back(std::make_pair(person, number));
	}

}

void PhoneBook::SortByName()
{
	std::sort(phone_book.begin(), phone_book.end(), [](std::pair<Person, PhoneNumber> person1, std::pair<Person, PhoneNumber> person2)
		{
			return person1.first < person2.first;
		}
	);
}

void PhoneBook::SortByPhone()
{
	std::sort(phone_book.begin(), phone_book.end(), [](std::pair<Person, PhoneNumber> number1, std::pair<Person, PhoneNumber> number2)
		{
			return number1.second < number2.second;
		}
	);
}

std::tuple<std::string,PhoneNumber>
PhoneBook::GetPhoneNumber(const std::string& surname)
{
	size_t count = 0;
	const PhoneNumber* number{};
	auto lambda = [&](auto& entry)
	{
		if (entry.first.GetLastName() == surname)
		{
			count++;
			number = &entry.second;
		}
	};
	std::for_each(phone_book.begin(), phone_book.end(), lambda);
	if (count == 0)
	return { "none found", {} };
	else if (count > 1)
		return{ "found more than one", *number };
	return{ "", *number };
		
}

void PhoneBook::ChangePhoneNumber(const Person& person, PhoneNumber new_num)
{
	auto user = std::find_if(phone_book.begin(), phone_book.end(), [&person](std::pair<Person, PhoneNumber>& user)
		{
			return user.first == person;
		}
	);

	if (user != phone_book.end())
		user->second = new_num;
}

std::ostream& operator<< (std::ostream& out, PhoneBook& book)
{
	for (const auto& iter : book.phone_book)
	{
		out << iter.first << ' ' << iter.second << std::endl;
	}
	return out;
}

int main()
{
	std::ifstream file("PhoneBook.txt"); // path to the file PhoneBook.txt
	PhoneBook book(file);
	file.close();
	std::cout << book;

	std::cout << "\n----------SortByPhone-----------\n" << std::endl;
	book.SortByPhone();
	std::cout << book;

	std::cout << "\n----------SortByName------------\n" << std::endl;
	book.SortByName();
	std::cout << book;

	std::cout << "\n---------GetPhoneNumber---------\n" << std::endl;
	// lambda that gets a surname and returns the telephone number
	auto print_phone_number = [&book](const std::string& surname)
	{
		std::cout << surname << "\t";
		auto answer = book.GetPhoneNumber(surname);
		if (std::get<0>(answer).empty())
			std::cout << std::get<1>(answer);
		else
			std::cout << std::get<0>(answer);
		std::cout << std::endl;
	};

	// calls of lambdas
	print_phone_number("Nikitosik");
	print_phone_number("Viking");
	print_phone_number("Valdemar");
	print_phone_number("Zhora");
	print_phone_number("xDemore");
	print_phone_number("Lovers");

	std::cout << "\n--------ChangePhoneNumber--------\n" << std::endl;
	book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", std::nullopt });
	book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
	std::cout << book;

}
