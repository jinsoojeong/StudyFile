//#include <tuple>
//#include <string>
//#include <iostream>
//
//class Person
//{
//public:
//	Person(int age, std::string& name) : age_(age), name_(name) {}
//	~Person() {};
//	void print()
//	{
//		printf("Pserson Class - age : %d name : %s \n", age_, name_.c_str());
//	}
//private:
//	int age_;
//	std::string name_;
//
//};
//
//void print(std::tuple<int, bool> params)
//{
//	std::cout << "foward_as_tuple as Params : " << std::get<0>(params) << ", " << (std::get<1>(params) ? "true":"false") << std::endl;
//}
//
//int wmain()
//{
//	// make tuple
//	typedef std::tuple<int, std::string, bool, Person*> FuncTuple;
//	std::string name = "jjs";
//	Person* person = new Person(27, name);
//	FuncTuple tuple_instance = std::make_tuple(27, std::string("jjs"), true, person);
//
//	// get tuple size
//	size_t tuple_size = std::tuple_size<decltype(tuple_instance)>::value;
//	std::cout << "tuple instance size : " << tuple_size << std::endl;
//
//	// get element value 3type
//	std::tuple_element<0, decltype(tuple_instance)>::type get_age = std::get<0>(tuple_instance); // only constrant value
//	auto get_name = std::get<1>(tuple_instance);
//	bool get_torf = std::get<2>(tuple_instance);
//	Person* get_person = std::get<3>(tuple_instance);
//	std::cout << "tuple element 1 : " << get_age << " element 2 : " << get_name << "element 3 : " << (get_torf ? "true":"false") << std::endl;
//	get_person->print();
//
//	// tuple tie
//	int tie_age;
//	std::string tie_name;
//	bool tie_torf;
//	Person* tie_person;
//
//	std::tie(tie_age, tie_name, tie_torf, tie_person) = tuple_instance;
//
//	std::cout << "tuple tie element 1 : " << tie_age << " element 2 : " << tie_name << "element 3 : " << (tie_torf ? "true" : "false") << std::endl;
//	tie_person->print();
//
//	// tuple_cat
//	typedef std::tuple<int, bool> NumberBool;
//
//	NumberBool number_bool1;
//	NumberBool number_bool2;
//
//	auto number_bools = std::tuple_cat(number_bool1, number_bool2);
//
//	std::cout << "NumberBoo1 : " << std::get<0>(number_bools) << ", " << std::get<1>(number_bools) << std::endl;
//	std::cout << "NumberBoo2 : " << std::get<2>(number_bools) << ", " << std::get<3>(number_bools) << std::endl;
//
//	// foward_as_tuple
//	print(std::forward_as_tuple(20, true));
//
//	system("pause");
//	return 0;
//}
